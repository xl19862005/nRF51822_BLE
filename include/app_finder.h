#ifndef _APP_FINDER_H
#define _APP_FINDER_H
#include "app_board.h"

//ble finder codes
typedef enum{
	BLE_FINDER_DISABLE = 0x00,
	BLE_FINDER_ENABLE
}ble_finder_status_t;

void ble_finder_status_process(int len);
#endif
