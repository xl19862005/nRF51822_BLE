#include "app_uart_rtx.h"

app_ring_buffer_t uart_rx;

bool is_cmd_head(uint8_t data)
{
	return (data == CMD_HEAD);
}

/*dispatch uart event to the process*/
void app_uart_evt_dispatch(void)
{
	int i;

	for(i=uart_rx.iget;i<uart_rx.iput;i++,uart_rx.iget++)
	{
		if(is_cmd_head(uart_rx.buffer[uart_rx.iget]))
		{
			
		}
	}
}