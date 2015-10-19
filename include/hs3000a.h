#ifndef _HS_3000A_H
#define _HS_3000A_H
#include "twi_master.h"

//device address
#define HS3000A_ADDR		0x6E

/*data registers*/

//PS
#define RESET_REG					0x01
#define INT_CFG_REG				0x02
#define PS_FREQUENCY_REG		0x0D
#define PS_SAMP_DELAY_REG		0x0E
#define PS_LED_CURRENT_REG	0x0F
#define PS_TH_UP_MSB_REG		0x10
#define PS_TH_UP_LSB_REG		0x11
#define PS_TH_LO_MSB_REG		0x12
#define PS_TH_LO_LSB_REG		0x13
#define PS_FILTER_CFG_REG		0x14
#define PS_CONFIG_REG			0x15
#define PS_INTERVAL_REG		0x16
#define PS_CONTROL_REG			0x17
#define PS_DATA_MSB_REG		0x41
#define PS_DATA_LSB_REG		0x42

//ALS
#define ALS_TH_UP_MSB_REG		0x20
#define ALS_TH_UP_LSB_REG		0x21
#define ALS_TH_LO_MSB_REG		0x22
#define ALS_TH_LO_LSB_REG		0x23
//0x24 reserved
#define ALS_CONFIG_REG			0x25
#define ALS_INTERVAL_REG		0x26
#define ALS_CONTROL_REG		0x27
#define ALS1_DATA_MSB_REG		0x43
#define ALS1_DATA_LSB_REG		0x44
#define ALS2_DATA_MSB_REG		0x45
#define ALS2_DATA_LSB_REG		0x46

#define INTERRUPT_STATUS_REG	0x40

typedef struct ps_data{
	uint8_t addr;
	uint8_t data;
}ps_data_t;

bool hs3000a_init(void);
bool hs3000a_reset(void);
bool hs3000a_read(uint8_t register_address, uint8_t * destination, uint8_t number_of_bytes);
#endif
