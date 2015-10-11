#include "app_bond_act.h"

extern app_uart_buffer_t uart_rx;
extern own_manuf_data_t manuf_data;
extern ble_action_service_t m_action;
extern watch_action_t m_watch;

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

static uint32_t add_char(uint16_t uuid, 
								  uint8_t uuid_type, 
								  const ble_gatts_char_md_t *p_char_md, 
								  uint16_t service_handle, 
								  ble_gatts_char_handles_t *p_char_handle)
{
	ble_gatts_attr_md_t cccd_md;
	ble_gatts_attr_t	attr_char_value;
	ble_uuid_t			ble_uuid;
	ble_gatts_attr_md_t attr_md;

	memset(&cccd_md, 0, sizeof(cccd_md));
	
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
	cccd_md.vloc = BLE_GATTS_VLOC_STACK;

	ble_uuid.type = uuid_type;
	ble_uuid.uuid = uuid;

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
	attr_char_value.init_len  = sizeof(uint8_t);
	attr_char_value.init_offs = 0;
	attr_char_value.max_len   = BLE_ACT_MAX_DATA_LEN;

	return sd_ble_gatts_characteristic_add(service_handle,p_char_md,&attr_char_value,p_char_handle);
}



static uint32_t random_char_add(ble_action_service_t * p_action)
{
	ble_gatts_char_md_t char_md;

	int i;

	memset(&char_md, 0, sizeof(ble_gatts_char_md_t));

	char_md.char_props.read = 1;
	char_md.p_char_user_desc  = NULL;
	char_md.p_char_pf		  = NULL;
	char_md.p_user_desc_md	  = NULL;
	char_md.p_cccd_md		  = NULL;
	char_md.p_sccd_md		  = NULL;
	
	add_char(RANDOM_CHARACTERISTIC_UUID, 
				 p_action->uuid_type, 
				 &char_md, 
				 p_action->service_handle, 
				 &p_action->random_handles);
}



static uint32_t verify_char_add(ble_action_service_t * p_action)
{
	ble_gatts_char_md_t char_md;

	memset(&char_md, 0, sizeof(ble_gatts_char_md_t));

	char_md.char_props.read = 1;
	char_md.char_props.write = 1;
	char_md.char_props.write_wo_resp = 1;
	char_md.p_char_user_desc  = NULL;
	char_md.p_char_pf		  = NULL;
	char_md.p_user_desc_md	  = NULL;
	char_md.p_cccd_md		  = NULL;
	char_md.p_sccd_md		  = NULL;
	
	add_char(VERIFY_CHARACTERISTIC_UUID, 
				 p_action->uuid_type, 
				 &char_md, 
				 p_action->service_handle, 
				 &p_action->random_handles);
}

uint32_t watch_action_service_init(ble_action_service_t * p_action, const ble_bond_action_init_t *p_action_init)
{
    uint32_t      err_code;
    ble_uuid_t    ble_uuid;
    ble_uuid128_t nus_base_uuid = OWN_BASE_128UUID;

	//p_action->conn_handle = p_action_init->conn_handler;
	p_action->conn_handle = BLE_CONN_HANDLE_INVALID;
	p_action->data_handler = p_action_init->data_handler;
	p_action->is_notification_enabled = false;

	//action service
	err_code = sd_ble_uuid_vs_add(&nus_base_uuid, &p_action->uuid_type);
    if(err_code != NRF_SUCCESS)
    {
		return err_code;
	}

    ble_uuid.type = p_action->uuid_type;
    ble_uuid.uuid = ACTION_SERVICE_UUID;


    // Add the service.
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &ble_uuid,
                                        &p_action->service_handle);
    if(err_code != NRF_SUCCESS)
    {
		return err_code;
	}
	
	//RANDOM_CHARACTERISTIC_UUID //watch write
	err_code = random_char_add(p_action); 
    if(err_code != NRF_SUCCESS)
    {
		return err_code;
	}
	
	//VERIFY_CHARACTERISTIC_UUID	//phone write
	err_code = verify_char_add(p_action);
    
	return err_code;
}

uint32_t server_key_char_add(ble_binding_t * p_binding)
{
	ble_gatts_char_md_t char_md;

	memset(&char_md, 0, sizeof(ble_gatts_char_md_t));

	char_md.char_props.read = 1;
	char_md.char_props.write = 1;
	char_md.char_props.write_wo_resp = 1;
	char_md.p_char_user_desc  = NULL;
	char_md.p_char_pf		  = NULL;
	char_md.p_user_desc_md	  = NULL;
	char_md.p_cccd_md		  = NULL;
	char_md.p_sccd_md		  = NULL;
	
	add_char(SERVER_KEY_CHARACTERISTIC_UUID, 
				 p_binding->uuid_type, 
				 &char_md, 
				 p_binding->service_handle, 
				 &p_binding->server_key_handles);
}

uint32_t watch_key_char_add(ble_binding_t * p_binding)
{
	ble_gatts_char_md_t char_md;

	memset(&char_md, 0, sizeof(ble_gatts_char_md_t));

	char_md.char_props.read = 1;
	char_md.p_char_user_desc  = NULL;
	char_md.p_char_pf		  = NULL;
	char_md.p_user_desc_md	  = NULL;
	char_md.p_cccd_md		  = NULL;
	char_md.p_sccd_md		  = NULL;
	
	add_char(WATCH_KEY_CHARACTERISTIC_UUID, 
				 p_binding->uuid_type, 
				 &char_md, 
				 p_binding->service_handle, 
				 &p_binding->server_key_handles);
}

uint32_t binding_service_init(ble_binding_t * p_binding, const ble_binding_init_t *p_binding_init)
{
    uint32_t      err_code;
    ble_uuid_t    ble_uuid;
    ble_uuid128_t nus_base_uuid = OWN_BASE_128UUID;

	//p_binding->conn_handle = p_action_init->conn_handler;
	p_binding->conn_handle = BLE_CONN_HANDLE_INVALID;
	p_binding->data_handler = p_binding_init->data_handler;
	p_binding->is_notification_enabled = false;

	//binding service
	err_code = sd_ble_uuid_vs_add(&nus_base_uuid, &p_binding->uuid_type);
    if(err_code != NRF_SUCCESS)
    {
		return err_code;
	}

    ble_uuid.type = p_binding->uuid_type;
    ble_uuid.uuid = BINDING_SERVICE_UUID;


    // Add the service.
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &ble_uuid,
                                        &p_binding->service_handle);
    if(err_code != NRF_SUCCESS)
    {
		return err_code;
	}
	
	//SERVER_KEY_CHARACTERISTIC_UUID //phone write
	err_code = server_key_char_add(p_binding); 
    if(err_code != NRF_SUCCESS)
    {
		return err_code;
	}
	
	//WATCH_KEY_CHARACTERISTIC_UUID	//watch write
	err_code = watch_key_char_add(p_binding);
    
	return err_code;
}


void watch_action_data_handler(ble_action_service_t * p_action, uint8_t * p_data, uint16_t length)
{
	app_uart_buffer_t* pbf = &uart_rx;
	uint8_t random_data[6];
	int i;

	for(i=0;i<6;i++)
	{
		random_data[i] = uart_buffer_pull_data(pbf->iget,NO_CRC);
	}

	p_data = random_data;
} 

void watch_binding_data_handler(ble_binding_t * p_binding, uint8_t * p_data, uint16_t length)
{

}

void ble_bond_action_process(int len)
{
    uint32_t      err_code;
	app_uart_buffer_t* pbf = &uart_rx;
	//ble_nus_init_t nus_init;
	ble_bond_act_status_t code = uart_buffer_pull_data(pbf->iget,NO_CRC);

	printf("ble_bond_action_process: code=0x%x,iget=%d,iput=%d\n",code,pbf->iget,pbf->iput);
	if((code == BLE_BOND_ACT_EXIT)|| (code == BLE_BOND_ACT_ENTER))
	{
		//BLE_BOND_ACT_ENTER set the adv as a connect adv
		app_advertising_stop();
		manuf_data.binding_status = code;
		if(BLE_BOND_ACT_ENTER)
		{
			m_watch.action.data_len = len - 2; //device + code 2 bytes
			app_advertising_restart(100, 0, BLE_GAP_ADV_TYPE_ADV_IND, &manuf_data);
		}
		else
		{
			app_advertising_restart(100, 0, BLE_GAP_ADV_TYPE_ADV_NONCONN_IND, &manuf_data);
		}
	}
}

