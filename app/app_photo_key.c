#include "app_photo_key.h"

extern app_uart_buffer_t uart_rx;
extern own_manuf_data_t manuf_data;

void ble_photo_key_status_process(int len)
{
	app_uart_buffer_t* pbf = &uart_rx;

	ble_photo_key_status_t code = uart_buffer_pull_data(pbf->iget,NO_CRC);

	if((code == BLE_PHOTO_KEY_DISABLE)|| (code == BLE_PHOTO_KEY_ENABLE))
	{
		app_advertising_stop();
		manuf_data.photo_key_press = code;
		app_advertising_restart(100, 0, BLE_GAP_ADV_TYPE_ADV_NONCONN_IND, &manuf_data);
	}
}

