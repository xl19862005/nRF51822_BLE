#ifndef _APP_UART_RTX_H
#define _APP_UART_RTX_H
#include "app_board.h"
/*UART receive and send file
*In this file,you can get funs to analyse cmd from uart
*and send cmd to host*/

//ring buffer max size
#define MAX_RING_BUFFER_SIZE	 512
//a package for cmd head
#define CMD_HEAD	0xF5


typedef struct{
	uint32_t iput;
	uint32_t iget;
	uint8_t buffer[MAX_RING_BUFFER_SIZE];
}app_ring_buffer_t;

void app_uart_evt_dispatch(void);
#endif
