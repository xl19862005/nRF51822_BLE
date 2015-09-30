#ifndef _APP_BOND_ACT_H
#define _APP_BOND_ACT_H
#include "app_board.h"

//ble bond act. codes
typedef enum{
	BLE_BOND_ACT_EXIT = 0x00,
	BLE_BOND_ACT_ENTER
}ble_bond_act_status_t;

typedef struct ble_action_service ble_action_service_t;

typedef void(*ble_bond_action_handler_t)(ble_action_service_t * p_action, uint8_t * p_data, uint16_t length);

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
	uint16_t                 conn_handle;             /**< Handle of the current connection (as provided by the S110 SoftDevice). BLE_CONN_HANDLE_INVALID if not in a connection. */
    bool                     is_notification_enabled; /**< Variable to indicate if the peer has enabled notification of the RX characteristic.*/
    ble_bond_action_handler_t   data_handler;            /**< Event handler to be called for handling received data. */
};

void ble_bond_action_process(int len);
#endif
