/* 
 * File:   ecu_button.c
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on September 12, 2023, 2:34 PM
 */

#include"ecu_relay.h"

/**
 * @brief Initialize the assigned pin to be output turn the relay OFF or ON
 * @param relay : pointer to the relay module configurations
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType relay_initialize(const relay_t *relay){
    Std_ReturnType ret = E_OK;
    if(NULL == relay)
    {
        ret = E_NOT_OK;
    }else
    {
        pin_config_t relay_obj = {
            .port      = relay->relay_port,
            .pin       = relay->relay_pin,
            .logic     = relay->relay_status,
            .direction = GPIO_direction_OUTPUT
        };
        ret = gpio_pin_intialize(&relay_obj);
    }
    return ret;
}
/**
 * @brief turn the relay ON
 * @param relay : pointer to the relay module configurations
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType relay_turn_on(const relay_t *relay){
    Std_ReturnType ret = E_OK;
    if(NULL == relay)
    {
        ret = E_NOT_OK;
    }else
    {
        pin_config_t relay_obj = {
            .port      = relay->relay_port,
            .pin       = relay->relay_pin,
            .logic     = relay->relay_status,
            .direction = GPIO_direction_OUTPUT
        };
        ret = gpio_pin_write_logic(&relay_obj, GPIO_HIGH);
    }
    return ret;
}
/**
 * @brief turn the relay OFF
 * @param relay : pointer to the relay module configurations
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType relay_turn_off(const relay_t *relay){
    Std_ReturnType ret = E_OK;
    if(NULL == relay)
    {
        ret = E_NOT_OK;
    }else
    {
        pin_config_t relay_obj = {
            .port      = relay->relay_port,
            .pin       = relay->relay_pin,
            .logic     = relay->relay_status,
            .direction = GPIO_direction_OUTPUT
        };
        ret = gpio_pin_write_logic(&relay_obj, GPIO_LOW);
    }
    return ret;
}