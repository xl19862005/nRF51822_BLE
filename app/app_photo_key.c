#include "app_photo_key.h"

extern app_uart_buffer_t uart_rx;

void ble_photo_key_status_process(int len)
{
	app_uart_buffer_t* pbf = &uart_rx;

	ble_photo_key_status_t code = uart_buffer_pull_data(pbf->iget,NO_CRC);

	//printf("watch_lock_status_process: code=0x%x\n",code);
	printf("code=0x%x,iget=%d,iput=%d\n",code,pbf->iget,pbf->iput);
	switch(code)
	{
		case BLE_PHOTO_KEY_DISABLE:
			
		break;

		case BLE_PHOTO_KEY_ENABLE:

		break;

		default:
			LOG_DEBUG("The photo key status code is unknow!");
		break;	
	}
}

