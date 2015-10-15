#ifndef _APP_BOND_ACT_H
#define _APP_BOND_ACT_H
//#include "app_board.h"
#include "app_uart_rtx.h"

//the length of watch random data(in bytes) 
#define WATCH_RANDOM_DATA_LEN		6

#define BLE_ACT_MAX_DATA_LEN (GATT_MTU_SIZE_DEFAULT - 3) /**< Maximum length of data (in bytes) that can be transmitted to the peer by the Nordic UART service module. */

/**@brief 128-bit UUID base List. */
#define  OWN_BASE_128UUID {{0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15,0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}};

//action service
#define ACTION_SERVICE_UUID				0xAA01
#define RANDOM_CHARACTERISTIC_UUID	0xBB01
#define VERIFY_CHARACTERISTIC_UUID    0xBB02
#define ANTILOST_CHARACTERISTIC_UUID	0XBB03 	

//binding service
#define BINDING_SERVICE_UUID							0xAA02
#define SERVER_KEY_CHARACTERISTIC_UUID			0xBB04
#define WATCH_KEY_CHARACTERISTIC_UUID			0xBB05

//ble bond act. codes
typedef enum{
	BLE_BOND_ACT_EXIT = 0x00,
	BLE_BOND_ACT_ENTER,
	BLE_BOND_ACT_SUCESS
}ble_bond_act_status_t;

typedef struct ble_action_service ble_action_service_t;
typedef struct ble_binding_service ble_binding_service_t;


typedef void(*ble_bond_action_handler_t)(ble_action_service_t * p_action, uint8_t * p_data, uint16_t length);
typedef void(*ble_binding_handler_t)(ble_binding_service_t * p_binding, uint8_t * p_data, uint16_t length);

typedef struct
{
    ble_bond_action_handler_t data_handler; /**< Event handler to be called for handling received data. */
} ble_bond_action_init_t;

typedef struct
{
    ble_binding_handler_t data_handler; /**< Event handler to be called for handling received data. */
} ble_binding_init_t;


/**@brief Xandy add own(action) service structure.
 *
 * @details This structure contains status information related to the service.
 */
struct ble_action_service
{
    uint8_t                  uuid_type;               /**< UUID type for own(action) Service Base UUID. */
    uint16_t                 service_handle;          /**< Handle of own(action) Service (as provided by the S110 SoftDevice). */
    ble_gatts_char_handles_t random_handles;              /**< Handles related to the random numbers get. */
    ble_gatts_char_handles_t verify_handles;              /**< Handles related to the verify. */
    ble_gatts_char_handles_t antlost_handles;              /**< Handles related to the ANT lost. */
    ble_bond_action_handler_t   data_handler;            /**< Event handler to be called for handling received data. */
	uint16_t  						conn_handle;             /**< Handle of the current connection (as provided by the S110 SoftDevice). BLE_CONN_HANDLE_INVALID if not in a connection. */
	bool                     is_notification_enabled; /**< Variable to indicate if the peer has enabled notification of the RX characteristic.*/
};

/**@brief Xandy add own(binding) service structure.
 *
 * @details This structure contains status information related to the service.
 */
struct ble_binding_service
{
    uint8_t                  uuid_type;               /**< UUID type for own(binding) Service Base UUID. */
    uint16_t                 service_handle;          /**< Handle of own(binding) Service (as provided by the S110 SoftDevice). */
    ble_gatts_char_handles_t server_key_handles;              /**< Handles related to the random numbers get. */
    ble_gatts_char_handles_t watch_key_handles;              /**< Handles related to the verify. */
    ble_binding_handler_t   data_handler;            /**< Event handler to be called for handling received data. */
	uint16_t  						conn_handle;             /**< Handle of the current connection (as provided by the S110 SoftDevice). BLE_CONN_HANDLE_INVALID if not in a connection. */
	bool                     is_notification_enabled; /**< Variable to indicate if the peer has enabled notification of the RX characteristic.*/
};

typedef struct watch_action
{
	ble_action_service_t action;
	ble_binding_service_t	binding;
}watch_action_t;

void watch_binding_data_handler(ble_binding_service_t* const p_binding, const uint8_t * p_data, uint16_t length);
void watch_action_data_handler(ble_action_service_t* const p_action, const uint8_t* p_data, uint16_t length);
uint32_t watch_action_service_init(ble_action_service_t * p_action, const ble_bond_action_init_t *p_action_init);
uint32_t binding_service_init(ble_binding_service_t * p_binding, const ble_binding_init_t *p_binding_init);
void ble_bond_action_process(int len);
#endif
