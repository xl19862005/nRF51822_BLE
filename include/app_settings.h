#ifndef _APP_SETTINGS_H
#define _APP_SETTINGS_H
#include "app_board.h"

typedef enum{
	BLE_RF_DB_SET = 0x00,
	BLE_INTERVAL_SET,
	BLE_DISCOVERY_SET,
	BLE_HOST_GUEST_SET
}ble_settings_t;

void ble_settings_process(int len);
#endif
