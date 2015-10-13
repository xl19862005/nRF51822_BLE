#include "app_settings.h"

extern app_uart_buffer_t uart_rx;

void ble_settings_process(int len)
{
	app_uart_buffer_t* pbf = &uart_rx;

	ble_settings_t code = uart_buffer_pull_data(pbf->iget,NO_CRC);

	printf("code=0x%x,iget=%d,iput=%d\n",code,pbf->iget,pbf->iput);
	switch(code)
	{
		case BLE_RF_DB_SET:
			
		break;

		case BLE_INTERVAL_SET:

		break;

		case BLE_DISCOVERY_SET:

		break;

		case BLE_HOST_GUEST_SET:

		break;	

		default:
			LOG_DEBUG("The ble settings status code is unknow!");
		break;	
	}
}

