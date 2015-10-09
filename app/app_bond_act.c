#include "app_bond_act.h"

extern app_uart_buffer_t uart_rx;
extern own_manuf_data_t manuf_data;

static void link_check_service_init(int len)
{
	//ACTION_SERVECE_UUID
	
	//RANDOM_CHARACTERISTIC_UUID //watch write
	//VERIFY_CHARACTERISTIC_UUID	//phone write
}


void ble_bond_action_process(int len)
{
	app_uart_buffer_t* pbf = &uart_rx;

	ble_bond_act_status_t code = uart_buffer_pull_data(pbf->iget,NO_CRC);

	printf("code=0x%x,iget=%d,iput=%d\n",code,pbf->iget,pbf->iput);
	if((code == BLE_BOND_ACT_EXIT)|| (code == BLE_BOND_ACT_ENTER))
	{
		//BLE_BOND_ACT_ENTER set the adv as a connect adv
		app_advertising_stop();
		manuf_data.binding_status = code;
		if(BLE_BOND_ACT_ENTER)
		{
			app_advertising_restart(100, 0, BLE_GAP_ADV_TYPE_ADV_IND, &manuf_data);
		}
		else
		{
			app_advertising_restart(100, 0, BLE_GAP_ADV_TYPE_ADV_NONCONN_IND, &manuf_data);
		}
	}
}

