#include "app_watch_lock.h"

/*process the watch lock status*/
extern app_uart_buffer_t uart_rx;
extern own_manuf_data_t manuf_data;

void watch_lock_status_process(int len)
{
	app_uart_buffer_t* pbf = &uart_rx;

	ble_lock_watch_status_t code = uart_buffer_pull_data(pbf->iget,NO_CRC);

	if((code == BLE_WATCH_UNLOCK)|| (code == BLE_WATCH_LOCKED))
	{
		app_advertising_stop();
		manuf_data.watch_lock_status = code;
		app_advertising_restart(100, 0, BLE_GAP_ADV_TYPE_ADV_NONCONN_IND, &manuf_data);
	}
}
