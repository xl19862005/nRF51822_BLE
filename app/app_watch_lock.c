#include "app_watch_lock.h"
/*process the watch lock status*/
extern app_uart_buffer_t uart_rx;

void watch_lock_status_process(int len)
{
	app_uart_buffer_t* pbf = &uart_rx;

	ble_lock_watch_status_t code = uart_buffer_pull_data(pbf->iget,NO_CRC);

	//printf("watch_lock_status_process: code=0x%x\n",code);
	printf("code=0x%x,iget=%d,iput=%d\n",code,pbf->iget,pbf->iput);
	switch(code)
	{
		case BLE_WATCH_UNLOCK:
			
		break;

		case BLE_WATCH_LOCKED:

		break;

		default:
			LOG_DEBUG("The watch lock status code is unknow!");
		break;	
	}
}
