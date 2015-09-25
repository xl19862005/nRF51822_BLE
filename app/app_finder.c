#include "app_finder.h"

extern app_uart_buffer_t uart_rx;

void ble_finder_status_process(int len)
{
	app_uart_buffer_t* pbf = &uart_rx;

	ble_finder_status_t code = uart_buffer_pull_data(pbf->iget,NO_CRC);

	//printf("watch_lock_status_process: code=0x%x\n",code);
	printf("code=0x%x,iget=%d,iput=%d\n",code,pbf->iget,pbf->iput);
	switch(code)
	{
		case BLE_FINDER_DISABLE:
			
		break;

		case BLE_FINDER_ENABLE:

		break;

		default:
			LOG_DEBUG("The ble finder status code is unknow!");
		break;	
	}
}

