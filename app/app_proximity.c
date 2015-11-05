#include "app_proximity.h"

extern app_uart_buffer_t uart_rx;

static bool m_new_ps = false;
static ble_proximity_t m_proximity_status = BLE_PROXIMITY_OFF;

static void ble_ps_status_send(const uint8_t* ps_data, uint8_t len)
{
	app_uart_tx_buffer_push(BLE_PROXIMITY, BLE_PROXIMITY_ON, ps_data, len);
	m_new_ps = false;
}

static void proximity_evt_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
	uint8_t int_status;
	uint8_t ps_data[2]={0};

    if (!nrf_drv_gpiote_in_is_set(pin))
	{
		hs3000a_read(INTERRUPT_STATUS_REG, &int_status, 1);
		
		if((int_status == 0x11) || (int_status == 0x12))
		{
			hs3000a_read(PS_DATA_MSB_REG, &ps_data[0], 1);
			hs3000a_read(PS_DATA_LSB_REG, &ps_data[1], 1);
			
			m_new_ps = true;
			ble_ps_status_send(ps_data, 2);
		}
	}
}

static uint32_t proximity_interrupt_init(void)
{
    uint32_t err_code;
	nrf_drv_gpiote_in_config_t config = GPIOTE_CONFIG_IN_SENSE_HITOLO(false);

    if (!nrf_drv_gpiote_is_init())
    {
        err_code = nrf_drv_gpiote_init();
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

	config.pull = NRF_GPIO_PIN_PULLUP;

    err_code = nrf_drv_gpiote_in_init(PS_IRQ_PIN, &config, proximity_evt_handler);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
}

static void proximity_on_off(ble_proximity_t on_off)
{
    uint32_t err_code;

	if(m_proximity_status == on_off)
		return;

	m_proximity_status = on_off;
	
	if(on_off == BLE_PROXIMITY_ON)
	{
		if(twi_master_init()) 
		{
			hs3000a_reset();
			if(hs3000a_init())
			{ 
				err_code = proximity_interrupt_init();
				APP_ERROR_CHECK(err_code);
			}
		}
	}
	else
	{
		//power off func.
		//hs3000a_reset();
	}
}

void ble_proximity_process(int len)
{
	app_uart_buffer_t* pbf = &uart_rx;

	ble_proximity_t code = uart_buffer_pull_data(pbf->iget,NO_CRC);

	proximity_on_off(code);
}
