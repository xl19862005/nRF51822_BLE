#include "app_uart_rtx.h"

app_uart_buffer_t uart_rx;

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

static uint16_t crc16_check(int iget, uint16_t data_length)
{
	/*polynomial*/
	uint16_t crc_gen = 0x1021;
	uint16_t crc;
	int i, j;


	/*init value of crc*/
	crc = 0xFFFF;
	
	if (data_length != 0)
	{
		for (i = 0; i < data_length; i++)
		{
			
			crc ^= (uint16_t)(uart_buffer_pull_data(iget+i, CRC_GEN));
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

	switch(device)
	{
		case BLE_WATCH_LOCK_STATUS:
			watch_lock_status_process(len);
		break;

		case BLE_POSITION_STATUS:
			ble_positon_status_process(len);
		break;

		case BLE_PHOTO_KEY_STATUS:

		break;

		case BLE_FINDER_STATUS:

		break;

		case BLE_BOND_ACT_STATUS:

		break;

		case BLE_LINK_CHECK_STATUS:

		break;

		case BLE_PROXIMITY:

		break;

		case BLE_ACCEL:

		break;

		default:
			LOG_DEBUG("This's a unknow device!");
		break;
	}
}

void app_uart_rtx_init(void)
{
	memset(&uart_rx, 0, sizeof(app_uart_buffer_t));
}

/*dispatch uart event to the process*/
void app_uart_evt_dispatch(void)
{
	int i,iput,iget,L,cmdCnt,l;
	uint8_t head0,head1;
	uint16_t crc;
	app_uart_buffer_t* pbf = &uart_rx;

	iput = pbf->iput;
	iget = pbf->iget;

	//in same condition iput or iget will biger than MAX value?
	if((iput == iget) || (iput > MAX_RING_BUFFER_SIZE) || (iget > MAX_RING_BUFFER_SIZE))
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
				if(crc16_check(iget, L-2) == crc)
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
