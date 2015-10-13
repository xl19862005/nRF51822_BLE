#ifndef _APP_DEVICE_CONN_H
#define _APP_DEVICE_CONN_H
#include "app_board.h"

//device link status define
typedef enum{
	DEVICE_DISCONNECTED,
	DEVICE_CONNECTED	
}device_conn_status_t;

void app_device_connected_status_handler(ble_evt_t * p_ble_evt, device_conn_status_t conn_status);
#endif
