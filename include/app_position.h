#ifndef _APP_POSITION_H
#define  _APP_POSITION_H
#include "app_board.h"

//ble local position codes
typedef enum{
	BLE_POSITION_DISABLE = 0x00,
	BLE_POSITION_ENABLE	
}ble_position_status_t;

void ble_position_status_process(int len);
#endif
