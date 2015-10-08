#ifndef _APP_LINK_CHECK_H
#define _APP_LINK_CHECK_H
#include "app_board.h"

//binding service
#define BINDING_SERVECE_UUID 					0xAA02
#define SERVER_KEY_CHARACTERISTIC_UUID		0xBB04
#define WATCH_KEY_CHARACTERISTIC_UUID		0xBB05

typedef enum{
	BLE_LINK_CHECK_ASK = 0x00,
	BLE_LINK_CHECK_PASS	
}ble_link_check_status_t;

typedef struct ble_binding_service ble_binding_service_t;

typedef void(*ble_binding_handler_t)(ble_binding_service_t * p_bind, uint8_t * p_data, uint16_t length);

/**@brief Xandy add own(action) service structure.
 *
 * @details This structure contains status information related to the service.
 */
struct ble_binding_service
{
    uint8_t                  uuid_type;               /**< UUID type for own(binding) Service Base UUID. */
    uint16_t                 service_handle;          /**< Handle of own(binding) Service . */
    ble_gatts_char_handles_t server_key_handles;              /**< Handles related to the server key numbers get. */
    ble_gatts_char_handles_t watch_key_handles;              /**< Handles related to the watch decipher the key. */
	uint16_t                 conn_handle;             /**< Handle of the current connection (as provided by the S110 SoftDevice). BLE_CONN_HANDLE_INVALID if not in a connection. */
    bool                     is_notification_enabled; /**< Variable to indicate if the peer has enabled notification of the RX characteristic.*/
    ble_binding_handler_t   data_handler;            /**< Event handler to be called for handling received data. */
};

void ble_link_check_process(int len);
#endif
