#ifndef _APP_BOND_ACT_H
#define _APP_BOND_ACT_H
#include "app_board.h"

//ble bond act. codes
typedef enum{
	BLE_BOND_ACT_EXIT = 0x00,
	BLE_BOND_ACT_ENTER
}ble_bond_act_status_t;

void ble_bond_action_process(int len);
#endif
