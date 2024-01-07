/* 
 * File:   ecu_button.c
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on September 12, 2023, 2:34 PM
 */

#include"ecu_button.h"
/**
 * @brief Initialize the assigned pin to be input
 * @param btn : pointer to the button configurations  @ref : button_t
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType button_initialize(const button_t *btn){
    Std_ReturnType ret = E_OK;
    if(NULL == btn)
    {
        ret = E_NOT_OK;
    }else
    {
        ret = gpio_pin_direction_intialize(&(btn->button_pin));
    }
    return ret;
}
/**
 * @brief Read the state of the button
 * @param btn : pointer to the button configurations @ref : button_t
 * @param btn_state : pointer to points to the button state @ref : button_state_t 
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType button_read_state(const button_t *btn, button_state_t *btn_state){
    Std_ReturnType ret = E_OK;
    logic_t pin_logic_status = GPIO_LOW;
    if((NULL == btn)||(NULL == btn_state))
    {
        ret = E_NOT_OK;
    }else
    {
        ret = gpio_pin_read_logic(&(btn->button_pin), &pin_logic_status);
        if(BUTTON_ACTIVE_HIGH==(btn->button_connection))
        {
            if(GPIO_HIGH == pin_logic_status)
            {
                *btn_state = BUTTON_PRESSED;
            }else
            {
                *btn_state = BUTTON_RELEASED;
            }
        }else if(BUTTON_ACTIVE_LOW==(btn->button_connection))
        {
            if(GPIO_LOW == pin_logic_status)
            {
                *btn_state = BUTTON_PRESSED;
            }else
            {
                *btn_state = BUTTON_RELEASED;
            }
            ret = E_OK;
        }else{ /* Nothing */ } 
    }
    return ret;
}