#ifndef _APP_CENTRAL_H
#define _APP_CENTRAL_H
#include "app_board.h"

#define NUMBER_OF_PERIPHERALS            6 //the max is 8
#define DATA_BUFFER_SIZE                   16 /* Size of bufer that collects HBR data from one peripheral. */
#define INVALID_DESCRIPTOR_HANDLE      0

typedef struct{
	uint16_t conn_handle;
	uint16_t descriptor_handle;
	ble_gap_addr_t peer_addr;
	uint8_t data[DATA_BUFFER_SIZE];
}peripheral_t;
#endif
