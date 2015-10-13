#include "app_ble.h"

extern uint16_t                         m_conn_handle;
extern watch_action_t m_watch;

static ble_gap_adv_params_t     	   m_adv_params;  
own_manuf_data_t manuf_data;

static bool is_advertising_start = false;

/**@brief Function for starting advertising.
 */
static void advertising_start(void)
{
    uint32_t err_code;
    
    err_code = sd_ble_gap_adv_start(&m_adv_params);
    APP_ERROR_CHECK(err_code);
	is_advertising_start = true;
}

uint32_t connected_evt_handle(ble_evt_t * p_ble_evt)
{
    uint32_t                         err_code;
	
	err_code = bsp_indication_set(BSP_INDICATE_CONNECTED);
	APP_ERROR_CHECK(err_code);
	
	m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
	is_advertising_start = false;
	app_device_connected_status_handler(p_ble_evt, DEVICE_CONNECTED);
	return err_code;
}

uint32_t disconnected_evt_handle(ble_evt_t * p_ble_evt)
{
    uint32_t                         err_code;

	err_code = bsp_indication_set(BSP_INDICATE_IDLE);
	APP_ERROR_CHECK(err_code);
	m_conn_handle = BLE_CONN_HANDLE_INVALID;

	app_device_connected_status_handler(p_ble_evt, DEVICE_DISCONNECTED);
	app_advertising_restart(100, 0, BLE_GAP_ADV_TYPE_ADV_NONCONN_IND, &manuf_data);
	
	return err_code;
}

/**@brief Function for the Application's S110 SoftDevice event handler.
 *
 * @param[in] p_ble_evt S110 SoftDevice event.
 */
static void on_ble_evt(ble_evt_t * p_ble_evt)
{
    uint32_t                         err_code;
    
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
			connected_evt_handle(p_ble_evt);
            break;
            
        case BLE_GAP_EVT_DISCONNECTED:
			disconnected_evt_handle(p_ble_evt);
            break;

        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            // Pairing not supported
            err_code = sd_ble_gap_sec_params_reply(m_conn_handle, BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL);
            APP_ERROR_CHECK(err_code);
			LOG_INFO("Some one request to provide security param!");
            break;

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            // No system attributes have been stored.
            err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0, 0);
            APP_ERROR_CHECK(err_code);
			LOG_INFO("BLE_GATTS_EVT_SYS_ATTR_MISSING!");
            break;

		case BLE_GATTS_EVT_TIMEOUT:
			LOG_INFO("BLE_GATTS_EVT_TIMEOUT");
			break;

        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for dispatching a S110 SoftDevice event to all modules with a S110 SoftDevice 
 *        event handler.
 *
 * @details This function is called from the S110 SoftDevice event interrupt handler after a S110 
 *          SoftDevice event has been received.
 *
 * @param[in] p_ble_evt  S110 SoftDevice event.
 */
static void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{
    on_ble_evt(p_ble_evt);
	ble_watch_action_on_ble_evt(&m_watch, p_ble_evt);
}

/**@brief Function for dispatching a system event to interested modules.
 *
 * @details This function is called from the System event interrupt handler after a system
 *          event has been received.
 *
 * @param[in]   sys_evt   System stack event.
 */
static void sys_evt_dispatch(uint32_t sys_evt)
{

}

/**@brief Function for the S110 SoftDevice initialization.
 *
 * @details This function initializes the S110 SoftDevice and the BLE event interrupt.
 */
static void ble_stack_init(void)
{
    uint32_t err_code;
    
    // Initialize SoftDevice.
    SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_XTAL_20_PPM, NULL);

    // Enable BLE stack.
    ble_enable_params_t ble_enable_params;
    memset(&ble_enable_params, 0, sizeof(ble_enable_params_t));
#ifdef S130
    ble_enable_params.gatts_enable_params.attr_tab_size   = BLE_GATTS_ATTR_TAB_SIZE_DEFAULT;
#endif
    ble_enable_params.gatts_enable_params.service_changed = IS_SRVC_CHANGED_CHARACT_PRESENT;
    err_code = sd_ble_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);
    
    // Subscribe for BLE events.
    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    APP_ERROR_CHECK(err_code);

    // Register with the SoftDevice handler module for BLE events.
    err_code = softdevice_sys_evt_handler_set(sys_evt_dispatch);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for the GAP initialization.
 *
 * @details This function will set up all the necessary GAP (Generic Access Profile) parameters of 
 *          the device. It also sets the permissions and appearance.
 */
static void gap_params_init(void)
{
    uint32_t                err_code;
	int i; 
	char device_name[50],tmp[2];
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;
	ble_gap_addr_t gap_addr;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

	err_code = sd_ble_gap_address_get(&gap_addr);
    APP_ERROR_CHECK(err_code);

	//Xandy@ add mac address to device name
	sprintf(device_name,"%s",DEVICE_NAME);
	for(i=BLE_GAP_ADDR_LEN-1;i>=0;i--)
	{
		sprintf(tmp,"%2x",gap_addr.addr[i]);
		strcat(device_name,tmp);
	}
    
    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *) device_name,
                                          strlen(device_name));
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(ble_gap_conn_params_t));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for putting the chip into sleep mode.
 *
 * @note This function will not return.
 */
void sleep_mode_enter(void)
{
    uint32_t err_code = bsp_indication_set(BSP_INDICATE_IDLE);
    APP_ERROR_CHECK(err_code);

    // Prepare wakeup buttons.
    err_code = bsp_btn_ble_sleep_mode_prepare();
    APP_ERROR_CHECK(err_code);

    // Go to system-off mode (this function will not return; wakeup will cause a reset).
    err_code = sd_power_system_off();
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing the Advertisement packet.
 *
 * @details This function initializes the data that is to be placed in an advertisement packet in 
 *          both connectable and non-connectable modes.
 *
 */
static void advertising_data_init(own_manuf_data_t* p_manuf_data)
{
    uint32_t                   err_code;
    ble_advdata_t              advdata;
    ble_advdata_manuf_data_t   manuf_data;

    // Build and set advertising data
    memset(&advdata, 0, sizeof(ble_advdata_t));
	memset(&manuf_data, 0, sizeof(ble_advdata_manuf_data_t));

    advdata.flags                 = BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED;
    advdata.name_type          = BLE_ADVDATA_FULL_NAME;

	//set the manuf. specific data to the scan data
	manuf_data.company_identifier = APP_COMPANY_IDENTIFIER;
	manuf_data.data.size = sizeof(own_manuf_data_t);
	manuf_data.data.p_data = (uint8_t*)p_manuf_data;
	advdata.p_manuf_specific_data = &manuf_data;

    err_code = ble_advdata_set(&advdata, NULL);
    APP_ERROR_CHECK(err_code);
}


static void manufacture_data_init(void)
{
	memset(&manuf_data, 0, sizeof(own_manuf_data_t));
}

/**@brief Function for initializing the connectable advertisement parameters.
 *
 * @details This function initializes the advertisement parameters to values that will put 
 *          the application in connectable mode.
 *
 */
static void connectable_adv_init(uint32_t adv_interval_ms, uint32_t adv_timeout_sec)
{
    // Initialize advertising parameters (used when starting advertising).
    memset(&m_adv_params, 0, sizeof(ble_gap_adv_params_t));
    
    m_adv_params.type        = BLE_GAP_ADV_TYPE_ADV_IND ;
    m_adv_params.p_peer_addr = NULL;                               // Undirected advertisement
    m_adv_params.fp          = BLE_GAP_ADV_FP_ANY;
    m_adv_params.interval    = MSEC_TO_UNITS(adv_interval_ms, UNIT_0_625_MS);
    m_adv_params.timeout     = adv_timeout_sec;
}


/**@brief Function for initializing the non-connectable advertisement parameters.
 *
 * @details This function initializes the advertisement parameters to values that will put 
 *          the application in non-connectable mode.
 *
 */
static void non_connectable_adv_init(uint32_t adv_interval_ms, uint32_t adv_timeout_sec)
{
    // Initialize advertising parameters (used when starting advertising).
    memset(&m_adv_params, 0, sizeof(ble_gap_adv_params_t));

    m_adv_params.type        = BLE_GAP_ADV_TYPE_ADV_NONCONN_IND;
    m_adv_params.p_peer_addr = NULL;                               // Undirected advertisement
    m_adv_params.fp          = BLE_GAP_ADV_FP_ANY;
    m_adv_params.interval    = MSEC_TO_UNITS(adv_interval_ms, UNIT_0_625_MS);
    m_adv_params.timeout     = adv_timeout_sec;
}

void app_advertising_stop()
{
    uint32_t      err_code;

	if(m_conn_handle != BLE_CONN_HANDLE_INVALID)
	{
		err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
		if (err_code != NRF_ERROR_INVALID_STATE)
		{
			APP_ERROR_CHECK(err_code);
		}
		
	}else if(is_advertising_start)
	{
		err_code = sd_ble_gap_adv_stop();
		APP_ERROR_CHECK(err_code);
		is_advertising_start = false;
	}
}

void app_advertising_restart(uint32_t adv_interval_ms, uint32_t adv_timeout_sec, uint8_t adv_type, own_manuf_data_t* p_manuf_data)
{
	uint32_t err_code;


	if(adv_type == BLE_GAP_ADV_TYPE_ADV_NONCONN_IND)
	{
		//non_connectable adv,must set the binding status to 0
		p_manuf_data->binding_status = 0;
		non_connectable_adv_init(adv_interval_ms, adv_timeout_sec);
	}else if(adv_type == BLE_GAP_ADV_TYPE_ADV_IND)
	{		
		connectable_adv_init(adv_interval_ms, adv_timeout_sec);
	} 

	advertising_data_init(p_manuf_data);

	if(m_conn_handle == BLE_CONN_HANDLE_INVALID)
	{
		advertising_start();
	}
}

//sd_ble_gap_rssi_start

void app_ble_init(void)
{
    uint32_t      err_code; 

	ble_stack_init();
	gap_params_init();
	manufacture_data_init();
	err_code = app_own_service_init();
	APP_ERROR_CHECK(err_code);

	app_advertising_restart(100, 0, BLE_GAP_ADV_TYPE_ADV_NONCONN_IND, &manuf_data);
}
