#include "app_own_service.h"

uint32_t app_own_service_init(watch_action_t *p_watch, const ble_bond_action_init_t *p_action_init, const ble_binding_init_t * p_binding_init)
{
	uint32_t err_code;

	err_code = watch_action_service_init(&p_watch->action, p_action_init);
    if(err_code != NRF_SUCCESS)
    {
		return err_code;
	}
	err_code = binding_service_init(&p_watch->binding,p_binding_init);

	return err_code;
}
