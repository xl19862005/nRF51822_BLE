#include "app_watch_lock.h"

/*process the watch lock status*/
extern app_uart_buffer_t uart_rx;
uint8_t test= 0;

void watch_lock_status_process(int len)
{
	app_uart_buffer_t* pbf = &uart_rx;
	own_manuf_data_t manuf_data={0};

	ble_lock_watch_status_t code = uart_buffer_pull_data(pbf->iget,NO_CRC);

	printf("watch_lock_status_process: code=0x%x,iget=%d,iput=%d\n",code,pbf->iget,pbf->iput);

	if((code == BLE_WATCH_UNLOCK)|| (code == BLE_WATCH_LOCKED))
	{
		sd_ble_gap_adv_stop();
		manuf_data.watch_lock_status = test++;
		app_advertising_restart(&manuf_data);
	}
}
