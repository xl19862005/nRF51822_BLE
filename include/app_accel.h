#ifndef _APP_ACCEL_H
#define _APP_ACCEL_H
#include "app_board.h"

typedef enum{
	BLE_ACCEL_OFF = 0x00,
	BLE_ACCEL_ON
}ble_accel_t;

void ble_accel_process(int len);
#endif
