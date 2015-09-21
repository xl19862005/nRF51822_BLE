#include "app_uart_rtx.h"

app_uart_buffer_t uart_rx;

static bool is_cmd_head(const uint8_t* pdata)
{
	return ((*pdata++ == CMD_HEAD0) && (*pdata == CMD_HEAD1));
}


static uint16_t crc16_check(const uint8_t *data_ptr, uint8_t data_length)
{
	/*polynomial*/
	uint16_t crc_gen = 0x1021;
	uint16_t crc;
	uint8_t i, j;

	/*init value of crc*/
	crc = 0xFFFF;
	
	if (data_length != 0)
	{
		for (i = 0; i < data_length; i++)
		{
			crc ^= (uint16_t)(data_ptr[i]);
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
	return crc;
}

static void device_cmd_dispatch(const uint8_t* pdata)
{
	int len = *pdata++;
	ble_device_t device = *pdata++;

	switch(device)
	{
		case BLE_WATCH_LOCK_STATUS:

		break;

		case BLE_POSITION_STATUS:

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

static void cmd_process(const uint8_t* pdata, int cmdCnt, int length)
{
	int i,j=0;

	for(i=0;i<cmdCnt;i++)
	{
		device_cmd_dispatch(&pdata[j]);
		j += pdata[j];
	}
}

/*dispatch uart event to the process*/
void app_uart_evt_dispatch(void)
{
	int i,iput,iget,cmdCnt,l;
	app_uart_buffer_t* pbf = &uart_rx;

	iput = pbf->iput;
	iget = pbf->iget;

	for(i=iget;i<pbf->iput;i++,iget++)
	{
		if(is_cmd_head(&pbf->buffer[i]))
		{
			cmdCnt = pbf->buffer[i+2];
			l = pbf->buffer[i+3];

			//have received one full package cmd
			if(iput >= l+6) //head2+cmdnum1+len1+l+crc2
			{	
				if(crc16_check(pbf->buffer, iput-2) == (uint16_t)(pbf->buffer[iput-1] << 8 | pbf->buffer[iput-2]))
				{
					cmd_process(&pbf->buffer[i+4], cmdCnt, l);
				}
				else
				{
					LOG_DEBUG("crc check error!");
				}
			}
		}
		else//one package cmd is not full
			return;
	}
}
