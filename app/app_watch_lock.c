#include "app_watch_lock.h"
/*process the watch lock status*/

void watch_lock_status_process(const uint8_t* pdata, int len)
{
	ble_lock_watch_status_t code = *pdata++;

	switch(code)
	{
		case BLE_WATCH_UNLOCK:

		break;

		case BLE_WATCH_LOCKED:

		break;

		default:
			LOG_DEBUG("The watch lock status code is unknow!");
		break;	
	}
}
