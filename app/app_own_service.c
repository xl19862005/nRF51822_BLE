#include "app_own_service.h"

watch_action_t m_watch;

uint32_t app_own_service_init()
{
	uint32_t err_code;
	ble_bond_action_init_t action_init;
	ble_binding_init_t binding_init;

	action_init.data_handler  = watch_action_data_handler;
	binding_init.data_handler = watch_binding_data_handler;

	err_code = watch_action_service_init(&m_watch.action, &action_init);
    if(err_code != NRF_SUCCESS)
    {
		return err_code;
	}
	err_code = binding_service_init(&m_watch.binding,&binding_init);

	return err_code;
}
