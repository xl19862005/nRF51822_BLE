#include "app_board.h"

uint16_t                         m_conn_handle = BLE_CONN_HANDLE_INVALID;    /**< Handle of the current connection. */
ble_nus_t                        m_nus;                                      /**< Structure to identify the Nordic UART Service. */

extern app_uart_buffer_t uart_rx;

/*timers init*/
static void timers_init(void)
{
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS, APP_TIMER_OP_QUEUE_SIZE, false);
}

/**@brief   Function for handling app_uart events.
 *
 * @details This function will receive a single character from the app_uart module and append it to 
 *          a string. The string will be be sent over BLE when the last character received was a 
 *          'new line' i.e '\n' (hex 0x0D) or if the string has reached a length of 
 *          @ref NUS_MAX_DATA_LENGTH.
 */
/**@snippet [Handling the data received over UART] */
void uart_event_handle(app_uart_evt_t * p_event)
{
    uint32_t       err_code;

    switch (p_event->evt_type)
    {
		case APP_UART_DATA_READY:
			UNUSED_VARIABLE(app_uart_get(&uart_rx.buffer[uart_rx.iput++]));

			//send uart message to BLE
			if ((uart_rx.buffer[uart_rx.iput-1] == '\n') || (uart_rx.iput >= (MAX_RING_BUFFER_SIZE)))
			{
				err_code = ble_nus_string_send(&m_nus, uart_rx.buffer, uart_rx.iput);
				if (err_code != NRF_ERROR_INVALID_STATE)
				{
					APP_ERROR_CHECK(err_code);
				}
				
				//uart_rx.iput = 0;
			}

			if(uart_rx.iput >= MAX_RING_BUFFER_SIZE)
			{
				uart_rx.iput = 0;
				uart_rx.iget = 0;
				LOG_DEBUG("receive cmd lengther then 512 bytes!");
			}
			break;

        case APP_UART_COMMUNICATION_ERROR:
            APP_ERROR_HANDLER(p_event->data.error_communication);
            break;

        case APP_UART_FIFO_ERROR:
            APP_ERROR_HANDLER(p_event->data.error_code);
            break;

        default:
            break;
    }
}
/**@snippet [Handling the data received over UART] */

/**@brief  Function for initializing the UART module.
 */
/**@snippet [UART Initialization] */
static void uart_init(void)
{
    uint32_t                     err_code;
    const app_uart_comm_params_t comm_params =
    {
        RX_PIN_NUMBER,
        TX_PIN_NUMBER,
        RTS_PIN_NUMBER,
        CTS_PIN_NUMBER,
        APP_UART_FLOW_CONTROL_DISABLED,
        false,
        UART_BAUDRATE_BAUDRATE_Baud115200
    };

    APP_UART_FIFO_INIT( &comm_params,
                       UART_RX_BUF_SIZE,
                       UART_TX_BUF_SIZE,
                       uart_event_handle,
                       APP_IRQ_PRIORITY_LOW,
                       err_code);
    APP_ERROR_CHECK(err_code);
}
/**@snippet [UART Initialization] */

void app_board_init(void)
{
	timers_init();
	uart_init();
}
