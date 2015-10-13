#ifndef _APP_UART_RTX_H
#define _APP_UART_RTX_H
#include "app_board.h"
#include "app_device_code.h"
/*UART receive and send file
*In this file,you can get funs to analyse cmd from uart
*and send cmd to host*/

//ring buffer max size
#define MAX_RING_BUFFER_SIZE	 UART_BUFFER_SIZE

//send data buffer max size
#define MAX_SEND_DATA_SIZE (MAX_RING_BUFFER_SIZE - 9)// 9 is for 2heads + 1cmdcnt + 1Len + 1cmdlen + 1device + 1code +2crc

//a package of the cmd head
#define CMD_HEAD0		0x5A
#define CMD_HEAD1		0xA5
#define CMD_HEAD 		0x5AA5

typedef enum{
	CRC_WORD,
	CRC_GEN,
	CRC_SEND_GEN,
	NO_CRC
}crc_type;

typedef struct{
	int iput;
	int iget;
	uint8_t buffer[MAX_RING_BUFFER_SIZE];
}app_uart_buffer_t;

typedef struct{
	bool isBusy;
	bool isReady;
	int iput;
	int cmdCnt;
	uint8_t buffer[MAX_RING_BUFFER_SIZE];
}app_uart_send_buffer_t;

typedef void(* uart_rx_cb)(int len);

typedef struct{
	uart_rx_cb cb;
}app_uart_rx_cb_t;

//uart send data
void app_uart_evt_send(void);
void app_uart_tx_buffer_push(ble_device_t device, int code, const uint8_t* data, int len);

//uart receive data
void app_uart_rtx_init(void);
void app_uart_evt_analyse(void);
void uart_buffer_push_data(uint8_t data);
uint8_t uart_buffer_pull_data(int iget, crc_type type);
void get_uart_data(app_uart_buffer_t* rx);
#endif
