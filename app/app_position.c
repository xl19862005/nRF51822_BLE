#include "app_position.h"

extern app_uart_buffer_t uart_rx;
extern own_manuf_data_t manuf_data;

void ble_position_status_process(int len)
{
	app_uart_buffer_t* pbf = &uart_rx;

	ble_position_status_t code = uart_buffer_pull_data(pbf->iget,NO_CRC);

	printf("ble_position_status_process: code=0x%x,iget=%d,iput=%d\n",code,pbf->iget,pbf->iput);
	if((code == BLE_POSITION_DISABLE)|| (code == BLE_POSITION_ENABLE))
	{
		app_advertising_stop();
		manuf_data.position_enable = code;
		app_advertising_restart(100, 0, BLE_GAP_ADV_TYPE_ADV_NONCONN_IND, &manuf_data);
	}
}

