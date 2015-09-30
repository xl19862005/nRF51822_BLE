#ifndef _APP_LINK_CHECK_H
#define _APP_LINK_CHECK_H
#include "app_board.h"

typedef enum{
	BLE_LINK_CHECK_ASK = 0x00,
	BLE_LINK_CHECK_PASS	
}ble_link_check_status_t;

void ble_link_check_process(int len);
#endif
