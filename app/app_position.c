#include "app_position.h"

extern app_uart_buffer_t uart_rx;

void ble_positon_status_process(int len)
{
	app_uart_buffer_t* pbf = &uart_rx;

	ble_position_status_t code = uart_buffer_pull_data(pbf->iget,NO_CRC);

	//printf("watch_lock_status_process: code=0x%x\n",code);
	printf("code=0x%x,iget=%d,iput=%d\n",code,pbf->iget,pbf->iput);
	switch(code)
	{
		case BLE_POSITION_DISABLE:
			
		break;

		case BLE_POSITION_ENABLE:

		break;

		default:
			LOG_DEBUG("The ble positon status code is unknow!");
		break;	
	}
}

