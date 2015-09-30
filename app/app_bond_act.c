#include "app_bond_act.h"

extern app_uart_buffer_t uart_rx;

void ble_bond_action_process(int len)
{
	app_uart_buffer_t* pbf = &uart_rx;

	ble_bond_act_status_t code = uart_buffer_pull_data(pbf->iget,NO_CRC);

	//printf("watch_lock_status_process: code=0x%x\n",code);
	printf("code=0x%x,iget=%d,iput=%d\n",code,pbf->iget,pbf->iput);
	switch(code)
	{
		case BLE_BOND_ACT_EXIT:
			
		break;

		case BLE_BOND_ACT_ENTER:

		break;

		default:
			LOG_DEBUG("The ble bond action status code is unknow!");
		break;	
	}
}

