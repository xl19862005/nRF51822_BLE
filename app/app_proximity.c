#include "app_proximity.h"

extern app_uart_buffer_t uart_rx;

void ble_proximity_process(int len)
{
	app_uart_buffer_t* pbf = &uart_rx;

	ble_proximity_t code = uart_buffer_pull_data(pbf->iget,NO_CRC);

	//printf("watch_lock_status_process: code=0x%x\n",code);
	printf("code=0x%x,iget=%d,iput=%d\n",code,pbf->iget,pbf->iput);
	switch(code)
	{
		case BLE_PROXIMITY_OFF:
			
		break;

		case BLE_PROXIMITY_ON:

		break;

		default:
			LOG_DEBUG("The ble proximity status code is unknow!");
		break;	
	}
}
