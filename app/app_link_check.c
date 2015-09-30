#include "app_link_check.h"

extern app_uart_buffer_t uart_rx;

void ble_link_check_process(int len)
{
	app_uart_buffer_t* pbf = &uart_rx;

	ble_link_check_status_t code = uart_buffer_pull_data(pbf->iget,NO_CRC);

	//printf("watch_lock_status_process: code=0x%x\n",code);
	printf("code=0x%x,iget=%d,iput=%d\n",code,pbf->iget,pbf->iput);
	switch(code)
	{
		case BLE_LINK_CHECK_ASK:
			
		break;

		case BLE_LINK_CHECK_PASS:

		break;

		default:
			LOG_DEBUG("The ble link check status code is unknow!");
		break;	
	}
}

