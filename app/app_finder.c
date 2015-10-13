#include "app_finder.h"

extern app_uart_buffer_t uart_rx;
extern own_manuf_data_t manuf_data;

void ble_finder_status_process(int len)
{
	app_uart_buffer_t* pbf = &uart_rx;

	ble_finder_status_t code = uart_buffer_pull_data(pbf->iget,NO_CRC);

	//printf("code=0x%x,iget=%d,iput=%d\n",code,pbf->iget,pbf->iput);
	if((code == BLE_FINDER_DISABLE)|| (code == BLE_FINDER_ENABLE))
	{
		app_advertising_stop();
		manuf_data.finder_status = code;
		app_advertising_restart(100, 0, BLE_GAP_ADV_TYPE_ADV_NONCONN_IND, &manuf_data);
	}
}

