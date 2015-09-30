#ifndef _APP_PROXIMITY_H
#define _APP_PROXIMITY_H
#include "app_board.h"

typedef enum{
	BLE_PROXIMITY_OFF = 0x00,
	BLE_PROXIMITY_ON	
}ble_proximity_t;

void ble_proximity_process(int len);
#endif
