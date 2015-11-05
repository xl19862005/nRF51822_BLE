#include "hs3000a.h"

const ps_data_t ps_init_data[] = {
	{INT_CFG_REG, 				0x02},// INT_CONFIG assert interrupt until cleared
	{PS_LED_CURRENT_REG, 	0x09},// PS_LED_CURRENT 50mA
	{PS_TH_UP_MSB_REG, 		0x8F},// PS_TH_UP_MSB
	{PS_TH_UP_LSB_REG, 		0xFF},// PS_TH_UP_LSB
	{PS_TH_LO_MSB_REG, 		0x70},// PS_TH_LO_MSB
	{PS_TH_LO_LSB_REG, 		0x00},// PS_TH_LO_LSB
	{PS_FILTER_CFG_REG, 	0x11},// PS_FILTER_CONFIG turn off filtering
	{PS_CONFIG_REG, 			0x09},// PS_CONFIG ALS blanking enabled, 300us integration time
	{PS_INTERVAL_REG, 		0x0A},// PS_INTERVAL 50ms wait
	{PS_CONTROL_REG, 			0x02},// PS_CONTROL enable continuous PS measurements
	{ALS_TH_UP_MSB_REG, 	0xFF},// ALS_TH_UP_MSB
	{ALS_TH_UP_LSB_REG, 	0xFF},// ALS_TH_UP_LSB
	{ALS_TH_LO_MSB_REG, 	0x00},// ALS_TH_LO_MSB
	{ALS_TH_LO_LSB_REG, 	0x00},// ALS_TH_LO_LSB
	{ALS_CONFIG_REG, 			0x44},// ALS_CONFIG ALS2 disabled, ALS1 enabled, 100ms integration time
	{ALS_INTERVAL_REG, 		0x00},// ALS_INTERVAL continuous measurement mode
	{ALS_CONTROL_REG, 		0x02} // ALS_CONTROL enable continuous ALS measurements
};

static bool hs3000a_write(uint8_t register_address, uint8_t value)
{
    uint8_t w2_data[2];

    w2_data[0] = register_address;
    w2_data[1] = value;
    return twi_master_transfer(HS3000A_ADDR, w2_data, 2, TWI_ISSUE_STOP);
}

bool hs3000a_read(uint8_t register_address, uint8_t * destination, uint8_t number_of_bytes)
{
    bool transfer_succeeded;
	
    transfer_succeeded  = twi_master_transfer(HS3000A_ADDR, &register_address, 1, TWI_DONT_ISSUE_STOP);
    transfer_succeeded &= twi_master_transfer(HS3000A_ADDR|TWI_READ_BIT, destination, number_of_bytes, TWI_ISSUE_STOP);
    return transfer_succeeded;
}

static bool hs3000a_write_block(uint8_t* data, uint8_t length)
{
    return twi_master_transfer(HS3000A_ADDR, data, length, TWI_ISSUE_STOP);
}

bool hs3000a_init(void)
{
	int i;
	bool err;
	//return hs3000a_write_block((uint8_t*)ps_init_data, sizeof(ps_init_data));

	for(i=0;i<sizeof(ps_init_data);i++)
	{
		err &= hs3000a_write(ps_init_data[i].addr, ps_init_data[i].data);
	}

	return err;
}

bool hs3000a_reset(void)
{
	return hs3000a_write(RESET_REG, 0x00);
}
