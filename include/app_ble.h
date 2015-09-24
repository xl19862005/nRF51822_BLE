#ifndef _APP_BLE_H
#define _APP_BLE_H
#include "app_board.h"

#define IS_SRVC_CHANGED_CHARACT_PRESENT 0                                           /**< Include the service_changed characteristic. If not enabled, the server's database cannot be changed for the lifetime of the device. */

#define NUS_SERVICE_UUID_TYPE           BLE_UUID_TYPE_VENDOR_BEGIN                  /**< UUID type for the Nordic UART Service (vendor specific). */

//owner service 
#define SERVICE_UUID128                         0x1b, 0xc5, 0xd5, 0xa5, 0x02, 0x00, 0xbe, 0xa1, 0xe3, 0x11, 0x6b, 0xed, 0x40, 0xfe, 0x0b, 0x31 /* UUID 128: 310bfe40-ed6b-11e3-a1be-0002a5d5c51b */
#define SERVICE_CHARACTERISTIC_UUID             0x9001
#define SERVICE_CHARACTERISTIC_VALUE            {0,0,0}
#define SERVICE_CHARACTERISTIC_DESCRIPTOR_UUID  0x9002

//action service
#define ACTION_SERVECE_UUID				0xAA01
#define RANDOM_CHARACTERISTIC_UUID	0xBB01
#define VERIFY_CHARACTERISTIC_UUID    0xBB02
#define ANTILOST_CHARACTERISTIC_UUID	0XBB03 	

//binding service
#define BINDING_SERVECE_UUID 					0xAA02
#define SERVER_KEY_CHARACTERISTIC_UUID		0xBB04
#define WATCH_KEY_CHARACTERISTIC_UUID		0xBB05

#define APP_ADV_INTERVAL                64                                          /**< The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */
#define APP_ADV_TIMEOUT_IN_SECONDS      180                                         /**< The advertising timeout (in units of seconds). */

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(20, UNIT_1_25_MS)             /**< Minimum acceptable connection interval (20 ms), Connection interval uses 1.25 ms units. */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(75, UNIT_1_25_MS)             /**< Maximum acceptable connection interval (75 ms), Connection interval uses 1.25 ms units. */
#define SLAVE_LATENCY                   0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)             /**< Connection supervisory timeout (4 seconds), Supervision Timeout uses 10 ms units. */
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER)  /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER) /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                           /**< Number of attempts before giving up the connection parameter negotiation. */

void sleep_mode_enter(void);
void app_ble_init(void);
#endif
