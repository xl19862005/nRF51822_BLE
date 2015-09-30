#include "app_accel.h"

extern app_uart_buffer_t uart_rx;

void ble_accel_process(int len)
{
	app_uart_buffer_t* pbf = &uart_rx;

	ble_accel_t code = uart_buffer_pull_data(pbf->iget,NO_CRC);

	//printf("watch_lock_status_process: code=0x%x\n",code);
	printf("code=0x%x,iget=%d,iput=%d\n",code,pbf->iget,pbf->iput);
	switch(code)
	{
		case BLE_ACCEL_OFF:
			
		break;

		case BLE_ACCEL_ON:

		break;

		default:
			LOG_DEBUG("The ble accel status code is unknow!");
		break;	
	}
}

