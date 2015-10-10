#include "app_bond_act.h"

extern app_uart_buffer_t uart_rx;
extern own_manuf_data_t manuf_data;
extern ble_action_service_t m_action;

static void watch_on_connect(ble_action_service_t * p_action, ble_evt_t * p_ble_evt)
{
    p_action->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}


static void watch_on_disconnect(ble_action_service_t * p_action, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_action->conn_handle = BLE_CONN_HANDLE_INVALID;
}


static void watch_on_write(ble_action_service_t * p_action, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    if (
        (p_evt_write->handle == p_action->verify_handles.cccd_handle)
        &&
        (p_evt_write->len == 2)
       )
    {
        if (ble_srv_is_notification_enabled(p_evt_write->data))
        {
            p_action->is_notification_enabled = true;
        }
        else
        {
            p_action->is_notification_enabled = false;
        }
    }
    else if (
             (p_evt_write->handle == p_action->random_handles.value_handle)
             &&
             (p_action->data_handler != NULL)
            )
    {
        p_action->data_handler(p_action, p_evt_write->data, p_evt_write->len);
    }
    else
    {
        // Do Nothing. This event is not relevant for this service.
    }
}

void ble_watch_action_on_ble_evt(ble_action_service_t * p_action, ble_evt_t * p_ble_evt)
{
    if ((p_action == NULL) || (p_ble_evt == NULL))
    {
        return;
    }

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            watch_on_connect(p_action, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            watch_on_disconnect(p_action, p_ble_evt);
            break;

        case BLE_GATTS_EVT_WRITE:
            watch_on_write(p_action, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}

static uint32_t random_char_add(ble_action_service_t * p_action)
{
	ble_gatts_char_md_t char_md;
	ble_gatts_attr_t	attr_char_value;
	ble_uuid_t			ble_uuid;
	ble_gatts_attr_md_t attr_md;

	memset(&char_md, 0, sizeof(char_md));

	char_md.char_props.read		 = 1;
	char_md.p_char_user_desc		 = NULL;
	char_md.p_char_pf				 = NULL;
	char_md.p_user_desc_md			 = NULL;
	char_md.p_cccd_md				 = NULL;
	char_md.p_sccd_md				 = NULL;

	ble_uuid.type = p_action->uuid_type;
	ble_uuid.uuid = RANDOM_CHARACTERISTIC_UUID;

	memset(&attr_md, 0, sizeof(attr_md));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

	attr_md.vloc	= BLE_GATTS_VLOC_STACK;
	attr_md.rd_auth = 0;
	attr_md.wr_auth = 0;
	attr_md.vlen	= 0;

	memset(&attr_char_value, 0, sizeof(attr_char_value));

	attr_char_value.p_uuid	  = &ble_uuid;
	attr_char_value.p_attr_md = &attr_md;
	attr_char_value.init_len  = 1;
	attr_char_value.init_offs = 0;
	attr_char_value.max_len   = BLE_ACT_MAX_DATA_LEN;

	return sd_ble_gatts_characteristic_add(p_action->service_handle,
										   &char_md,
										   &attr_char_value,
										   &p_action->random_handles);
}



static uint32_t verify_char_add(ble_action_service_t * p_action)
{
	ble_gatts_char_md_t char_md;
	ble_gatts_attr_md_t cccd_md;
	ble_gatts_attr_t	attr_char_value;
	ble_uuid_t			ble_uuid;
	ble_gatts_attr_md_t attr_md;

	memset(&cccd_md, 0, sizeof(cccd_md));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

	cccd_md.vloc = BLE_GATTS_VLOC_STACK;

	memset(&char_md, 0, sizeof(char_md));

	char_md.char_props.notify = 1;
	char_md.p_char_user_desc  = NULL;
	char_md.p_char_pf		  = NULL;
	char_md.p_user_desc_md	  = NULL;
	char_md.p_cccd_md		  = &cccd_md;
	char_md.p_sccd_md		  = NULL;

	ble_uuid.type = p_action->uuid_type;
	ble_uuid.uuid = VERIFY_CHARACTERISTIC_UUID;

	memset(&attr_md, 0, sizeof(attr_md));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

	attr_md.vloc	= BLE_GATTS_VLOC_STACK;
	attr_md.rd_auth = 0;
	attr_md.wr_auth = 0;
	attr_md.vlen	= 1;

	memset(&attr_char_value, 0, sizeof(attr_char_value));

	attr_char_value.p_uuid	  = &ble_uuid;
	attr_char_value.p_attr_md = &attr_md;
	attr_char_value.init_len  = sizeof(uint8_t);
	attr_char_value.init_offs = 0;
	attr_char_value.max_len   = BLE_ACT_MAX_DATA_LEN;

	return sd_ble_gatts_characteristic_add(p_action->service_handle,
										   &char_md,
										   &attr_char_value,
										   &p_action->verify_handles);
}


static uint32_t link_check_service_init(ble_action_service_t * p_action, const ble_bond_action_init_t *p_action_init)
{
    uint32_t      err_code;
    ble_uuid_t    ble_uuid;
    ble_uuid128_t nus_base_uuid = OWN_BASE_128UUID;

	//p_action->conn_handle = p_action_init->conn_handler;
	p_action->conn_handle = BLE_CONN_HANDLE_INVALID;
	p_action->data_handler = p_action_init->data_handler;
	p_action->is_notification_enabled = false;

	//binding service
	ble_uuid.uuid = ACTION_SERVICE_UUID;
	err_code = sd_ble_uuid_vs_add(&nus_base_uuid, &ble_uuid.type);
    if(err_code != NRF_SUCCESS)
    {
		return err_code;
	}

    // Add the service.
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &ble_uuid,
                                        &p_action->service_handle);
    if(err_code != NRF_SUCCESS)
    {
		return err_code;
	}
	
	//RANDOM_CHARACTERISTIC_UUID //watch write
	//err_code = random_char_add(p_action); 
    if(err_code != NRF_SUCCESS)
    {
		return err_code;
	}
	
	//VERIFY_CHARACTERISTIC_UUID	//phone write
	//err_code = verify_char_add(p_action);
    
	return err_code;
}

static void action_data_handler(ble_action_service_t * p_action, uint8_t * p_data, uint16_t length)
{

}

void ble_bond_action_process(int len)
{
    uint32_t      err_code;
	app_uart_buffer_t* pbf = &uart_rx;
	ble_bond_action_init_t action_init;
	ble_nus_init_t nus_init;
	ble_bond_act_status_t code = uart_buffer_pull_data(pbf->iget,NO_CRC);

	printf("ble_bond_action_process: code=0x%x,iget=%d,iput=%d\n",code,pbf->iget,pbf->iput);
	if((code == BLE_BOND_ACT_EXIT)|| (code == BLE_BOND_ACT_ENTER))
	{
		//BLE_BOND_ACT_ENTER set the adv as a connect adv
		app_advertising_stop();
		manuf_data.binding_status = code;
		if(BLE_BOND_ACT_ENTER)
		{
			action_init.data_handler = action_data_handler;
			//err_code = link_check_service_init(&m_action, &action_init);
           // APP_ERROR_CHECK(err_code);
			app_advertising_restart(100, 0, BLE_GAP_ADV_TYPE_ADV_IND, &manuf_data);
		}
		else
		{
			app_advertising_restart(100, 0, BLE_GAP_ADV_TYPE_ADV_NONCONN_IND, &manuf_data);
		}
	}
}

