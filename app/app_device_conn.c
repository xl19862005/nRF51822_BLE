#include "app_device_conn.h"

void app_device_connected_status_handler(ble_evt_t * p_ble_evt, device_conn_status_t conn_status)
{
	int i;
	ble_gap_evt_connected_t* p_connected = &p_ble_evt->evt.gap_evt.params.connected;

	if(conn_status == DEVICE_CONNECTED)
	{
		app_uart_tx_buffer_push(BLE_DEVICE_LINK_STATUS, DEVICE_CONNECTED, p_connected->peer_addr.addr, BLE_GAP_ADDR_LEN);	
	}
	else if(conn_status == DEVICE_DISCONNECTED)
	{
		app_uart_tx_buffer_push(BLE_DEVICE_LINK_STATUS, DEVICE_DISCONNECTED, NULL, 0);	
	}
}
