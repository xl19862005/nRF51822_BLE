#ifndef _APP_WATCH_LOCK_H
#define _APP_WATCH_LOCK_H
#include "app_board.h"

//ble lock watch codes
typedef enum{
	BLE_WATCH_UNLOCK = 0x00,
	BLE_WATCH_LOCKED
}ble_lock_watch_status_t;

void watch_lock_status_process(const uint8_t* pdata, int len);
#endif
