#ifndef _APP_BOARD_H_
#define _APP_BOARD_H_
#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "nrf51_bitfields.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "softdevice_handler.h"
#include "app_timer.h"
#include "app_button.h"
#include "ble_nus.h"
#include "app_uart.h"
#include "app_util_platform.h"
#include "bsp.h"
#include "bsp_btn_ble.h"
#include "app_io.h"
#include "app_ble.h"
#include "debug.h"
#include "app_uart_rtx.h"
#include "app_device_code.h"
#include "app_watch_lock.h"

#define APP_TIMER_PRESCALER             0                                           /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_MAX_TIMERS            (2 + BSP_APP_TIMERS_NUMBER)                 /**< Maximum number of simultaneously created timers. */
#define APP_TIMER_OP_QUEUE_SIZE         4                                           /**< Size of timer operation queues. */

#define UART_TX_BUF_SIZE                UART_BUFFER_SIZE                                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE                UART_BUFFER_SIZE                                         /**< UART RX buffer size. */

void app_board_init(void);

#endif
