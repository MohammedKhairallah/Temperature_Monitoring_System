/* 
 * File:   ecu_dc_motor.c
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on September 16, 2023, 2:59 PM
 */

#include"ecu_dc_motor.h"

/**
 * @brief Initialize the assigned pin to be output turn the Motor OFF or ON
 * @param _dc_motor : pointer to the motor module configurations
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType dc_motor_initialize(const dc_motor_t *_dc_motor){
    Std_ReturnType ret = E_OK;
    if(NULL == _dc_motor)
    {
        ret = E_NOT_OK;
    }else
    {
        ret = gpio_pin_intialize(&(_dc_motor->dc_motor_pin[DC_MOTOR_PIN0]));
        ret = gpio_pin_intialize(&(_dc_motor->dc_motor_pin[DC_MOTOR_PIN1]));
    }
    return ret;
}
/**
 * @brief Move the motor to the right direction
 * @param _dc_motor : pointer to the motor module configurations
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType dc_motor_move_right(const dc_motor_t *_dc_motor){
    Std_ReturnType ret = E_OK;
    if(NULL == _dc_motor)
    {
        ret = E_NOT_OK;
    }else
    {
        ret = gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[DC_MOTOR_PIN0]), GPIO_HIGH);
        ret = gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[DC_MOTOR_PIN1]), GPIO_LOW);
    }
    return ret;
}
/**
 * @brief Move the motor to the left direction
 * @param _dc_motor : pointer to the motor module configurations
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType dc_motor_move_left(const dc_motor_t *_dc_motor){
    Std_ReturnType ret = E_OK;
    if(NULL == _dc_motor)
    {
        ret = E_NOT_OK;
    }else
    {
        ret = gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[DC_MOTOR_PIN0]), GPIO_LOW);
        ret = gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[DC_MOTOR_PIN1]), GPIO_HIGH);
    }
    return ret;
}
/**
 * @brief Stop the motor movement
 * @param _dc_motor : pointer to the motor module configurations
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType dc_motor_stop(const dc_motor_t *_dc_motor){
    Std_ReturnType ret = E_OK;
    if(NULL == _dc_motor)
    {
        ret = E_NOT_OK;
    }else
    {
        ret = gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[DC_MOTOR_PIN0]), GPIO_LOW);
        ret = gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[DC_MOTOR_PIN1]), GPIO_LOW);
    }
    return ret;
}
