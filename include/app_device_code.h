#ifndef _APP_DEVICE_CODE_H
#define _APP_DEVICE_CODE_H
//all fun. and devices defines 
typedef enum{
	BLE_WATCH_LOCK_STATUS = 0x01,
	BLE_POSITION_STATUS,				
	BLE_PHOTO_KEY_STATUS,
	BLE_FINDER_STATUS,
	BLE_BOND_ACT_STATUS,
	BLE_LINK_CHECK_STATUS,
	BLE_PROXIMITY,
	BLE_ACCEL
}ble_device_t;

//ble lock watch codes
typedef enum{
	BLE_WATCH_UNLOCK = 0x00,
	BLE_WATCH_LOCKED
}ble_lock_watch_status_t;

//ble local position codes
typedef enum{
	BLE_POSITION_DISABLE = 0x00,
	BLE_POSITION_ENABLE	
}ble_position_status_t;

//ble photo key codes
typedef enum{
	BLE_PHOTO_KEY_DISABLE = 0x00,
	BLE_PHOTO_KEY_ENABLE
}ble_photo_key_status_t;

//ble finder codes
typedef enum{
	BLE_FINDER_DISABLE = 0x00,
	BLE_FINDER_ENABLE
}ble_finder_status_t;

//ble bond act. codes
typedef enum{
	BLE_BOND_ACT_EXIT = 0x00,
	BLE_BOND_ACT_ENTER
}ble_bond_act_status_t;

typedef enum{
	BLE_LINK_CHECK_ASK = 0x00,
	BLE_LINK_CHECK_PASS	
}ble_link_check_status_t;
#endif
