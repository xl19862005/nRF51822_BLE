#include "app_uart_rtx.h"
#include "app_watch_lock.h"
#include "app_position.h"
#include "app_photo_key.h"
#include "app_finder.h"
#include "app_bond_act.h"
#include "app_settings.h"
#include "app_proximity.h"
#include "app_accel.h"

app_uart_buffer_t uart_rx;
app_uart_send_buffer_t uart_tx;
const app_uart_rx_cb_t rx_cb[BLE_ENUM_END] = {
	watch_lock_status_process,
	ble_position_status_process,
	ble_photo_key_status_process,
	ble_finder_status_process,
	ble_bond_action_process,
	ble_settings_process,
	ble_proximity_process,
	ble_accel_process
	//other funs.
	//.
	//.
	//.
};


void get_uart_data(app_uart_buffer_t* rx)
{
	rx = & uart_rx;
}

void uart_buffer_push_data(uint8_t data)
{
	app_uart_buffer_t* pbf = &uart_rx;

	pbf->buffer[pbf->iput] = data;
	pbf->iput = (pbf->iput+1) % MAX_RING_BUFFER_SIZE;
}

uint8_t uart_buffer_pull_data(int iget,crc_type  type)
{
	app_uart_buffer_t* pbf = &uart_rx;
	uint8_t data;

	if(type == CRC_WORD)
	{
		if(iget >= MAX_RING_BUFFER_SIZE)
		{
			iget -=  MAX_RING_BUFFER_SIZE;
		}
	}
	else if(type == CRC_GEN)
	{
		iget = iget % MAX_RING_BUFFER_SIZE;
	}
	else
	{
		iget = pbf->iget;
		pbf->iget = (iget+1) % MAX_RING_BUFFER_SIZE;
	}

	data = pbf->buffer[iget];

	return data;
}

static bool is_cmd_head(uint16_t head)
{
	return (head == CMD_HEAD);
}

static uint16_t crc16_check(int iget, uint16_t data_length, crc_type type)
{
	/*polynomial*/
	uint16_t crc_gen = 0x1021;
	uint16_t crc;
	int i, j;
	app_uart_send_buffer_t* psb = &uart_tx;


	/*init value of crc*/
	crc = 0xFFFF;
	
	if (data_length != 0)
	{
		for (i = 0; i < data_length; i++)
		{
			if(type == CRC_GEN)
				crc ^= (uint16_t)(uart_buffer_pull_data(iget+i, CRC_GEN));
			else
				crc ^= (uint16_t)(psb->buffer[i]);
			for (j = 0; j < 8; j++)
			{
				if ((crc & 0x01)  == 0x01)   
				{
					crc >>= 1;    
					crc ^= crc_gen;
				}
				else
				{
					crc >>= 1;
				}
			}/*end for*/
		}/*end for*/
	}/*end if*/

	//printf("get crc = 0x%x\n",crc);
	return crc;
}

static void device_cmd_dispatch()
{
	app_uart_buffer_t* pbf = &uart_rx;

	int len = uart_buffer_pull_data(pbf->iget,NO_CRC);
	ble_device_t device = uart_buffer_pull_data(pbf->iget,NO_CRC);

	printf("device_cmd_dispatch: len=%d,device=0x%x\n",len,device);
	if((device >= BLE_WATCH_LOCK_STATUS) && (device < BLE_ENUM_END))
	{
		rx_cb[device].cb(len);
	}
}

void app_uart_rtx_init(void)
{
	memset(&uart_rx, 0, sizeof(app_uart_buffer_t));
	memset(&uart_tx, 0, sizeof(app_uart_send_buffer_t));
}

/*package the send event*/
static void send_evt_package(app_uart_send_buffer_t *psb)
{
	int i;
	uint16_t crc;

	psb->isBusy = true;

	//add head
	psb->buffer[0] = CMD_HEAD0;
	psb->buffer[1] = CMD_HEAD1;

	//add cmds count
	psb->buffer[2] = psb->cmdCnt;

	//add cmds length
	psb->buffer[3] = psb->iput - 4;

	//add crc 2 bytes
	crc = crc16_check(0, psb->iput, CRC_SEND_GEN);
	psb->buffer[psb->iput++] = (crc | 0xFF00) >> 8;
	psb->buffer[psb->iput++] = crc | 0x00FF;
}

void app_uart_tx_buffer_push(int device, int code, const uint8_t* data, int len)
{
	app_uart_send_buffer_t	 *psb = &uart_tx;

	psb->buffer[4] = 2 + len;
	psb->buffer[5] = device;
	psb->buffer[6] = code;

	memcpy(&psb->buffer[7],data,len);
	
	psb->iput = 7 + len;
	psb->isReady = true;
	psb->isBusy = true;
}

/*uart send cmd package*/
void app_uart_evt_send(void)
{
	int i, timeout = 5000;//timeout
	app_uart_send_buffer_t	 *psb = &uart_tx;

	if(psb->isReady)
	{
		send_evt_package(psb);

		for(int i=0;i<psb->iput;i++)
		{
			timeout = 5000;
			while(app_uart_put(psb->buffer[i]) != NRF_SUCCESS)
			{
				if(timeout-- <= 0)
				{
					LOG_DEBUG("app_uart_evt_send timeout!");
				}
			}
			
		}

		memset(&uart_tx, 0, sizeof(app_uart_send_buffer_t));
	}
}

/*dispatch uart event to the process*/
void app_uart_evt_analyse(void)
{
	int i,iput,iget,L,cmdCnt,l;
	uint8_t head0,head1;
	uint16_t crc;
	app_uart_buffer_t* pbf = &uart_rx;

	iput = pbf->iput;
	iget = pbf->iget;

	//in same condition iput or iget will biger than MAX value?
	if((iput > MAX_RING_BUFFER_SIZE) || (iget > MAX_RING_BUFFER_SIZE))
	{
		app_uart_rtx_init();
		return;
	}

	//the length of the uart data
	if(iput >= iget)
		L = iput - iget;
	else
	{
		L = MAX_RING_BUFFER_SIZE - iget + iput;
	}

	//head2+cmdcnt1+Len1
	if(L > 4)
	{
		head0 = uart_buffer_pull_data(pbf->iget,NO_CRC);
		head1 = uart_buffer_pull_data(pbf->iget,NO_CRC);
		if(is_cmd_head((head0 << 8) | head1))
		{
			cmdCnt = uart_buffer_pull_data(pbf->iget,NO_CRC);
			l = uart_buffer_pull_data(pbf->iget,NO_CRC);

			if(L >= (l+6))//head2+cmdnum1+len1+l+crc2
			{
				crc = uart_buffer_pull_data(pbf->iget+l,CRC_WORD) << 8 | uart_buffer_pull_data(pbf->iget+l+1,CRC_WORD);
				if(crc16_check(iget, L-2, CRC_GEN) == crc)
				{
					for(i=0;i<cmdCnt;i++)
					{
						device_cmd_dispatch();
					}
						pbf->iget += 2;//pass the 2 crc bytes
				}
				else
				{
					app_uart_rtx_init();
					printf("crc check error(crc=0x%x)!\n",crc);
				}
			}
			else//one package cmd is not full
			{
				pbf->iget = iget;
			}
		}
	}
}
