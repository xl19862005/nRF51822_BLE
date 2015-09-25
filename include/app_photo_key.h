#ifndef _APP_PHOTO_KEY_H
#define _APP_PHOTO_KEY_H
#include "app_board.h"

//ble photo key codes
typedef enum{
	BLE_PHOTO_KEY_DISABLE = 0x00,
	BLE_PHOTO_KEY_ENABLE
}ble_photo_key_status_t;

void ble_photo_key_status_process(int len);
#endif
