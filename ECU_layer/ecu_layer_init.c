/* 
 * File:   ecu_layer_init.c
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on September 27, 2023, 12:16 PM
 */
#include"ecu_layer_init.h"

chr_lcd_4bit_t LCD1 = {
    .lcd_rs_pin.port = PORTC_INDEX,
    .lcd_rs_pin.pin = GPIO_PIN0,
    .lcd_rs_pin.logic = GPIO_LOW,
    .lcd_rs_pin.direction = GPIO_direction_OUTPUT,
    
    .lcd_enable_pin.port = PORTC_INDEX,
    .lcd_enable_pin.pin = GPIO_PIN1,
    .lcd_enable_pin.logic = GPIO_LOW,
    .lcd_enable_pin.direction = GPIO_direction_OUTPUT,
    
    .lcd_data_pins[0].port = PORTC_INDEX,
    .lcd_data_pins[0].pin = GPIO_PIN2,
    .lcd_data_pins[0].logic = GPIO_LOW,
    .lcd_data_pins[0].direction = GPIO_direction_OUTPUT,
    
    .lcd_data_pins[1].port = PORTC_INDEX,
    .lcd_data_pins[1].pin = GPIO_PIN3,
    .lcd_data_pins[1].logic = GPIO_LOW,
    .lcd_data_pins[1].direction = GPIO_direction_OUTPUT,
    
    .lcd_data_pins[2].port = PORTC_INDEX,
    .lcd_data_pins[2].pin = GPIO_PIN4,
    .lcd_data_pins[2].logic = GPIO_LOW,
    .lcd_data_pins[2].direction = GPIO_direction_OUTPUT,
    
    .lcd_data_pins[3].port = PORTC_INDEX,
    .lcd_data_pins[3].pin = GPIO_PIN5,
    .lcd_data_pins[3].logic = GPIO_LOW,
    .lcd_data_pins[3].direction = GPIO_direction_OUTPUT
};

dc_motor_t dc_motor_01 = {
    .dc_motor_pin[0].port      = PORTB_INDEX,
    .dc_motor_pin[0].pin       = GPIO_PIN0,
    .dc_motor_pin[0].logic     = DC_MOTOR_OFF_STATUS,
    .dc_motor_pin[0].direction = GPIO_direction_OUTPUT,
    .dc_motor_pin[1].port      = PORTB_INDEX,
    .dc_motor_pin[1].pin       = GPIO_PIN1,
    .dc_motor_pin[1].logic     = DC_MOTOR_OFF_STATUS,
    .dc_motor_pin[1].direction = GPIO_direction_OUTPUT
};

dc_motor_t dc_motor_02 = {
    .dc_motor_pin[0].port      = PORTB_INDEX,
    .dc_motor_pin[0].pin       = GPIO_PIN2,
    .dc_motor_pin[0].logic     = DC_MOTOR_OFF_STATUS,
    .dc_motor_pin[0].direction = GPIO_direction_OUTPUT,
    .dc_motor_pin[1].port      = PORTB_INDEX,
    .dc_motor_pin[1].pin       = GPIO_PIN3,
    .dc_motor_pin[1].logic     = DC_MOTOR_OFF_STATUS,
    .dc_motor_pin[1].direction = GPIO_direction_OUTPUT
};

void ecu_layer_intialize(void)
{
    Std_ReturnType ret = E_OK;
     
    /* Initialize LCD and DC motors */
    ret = lcd_4bit_initialize(&LCD1);
    ret = dc_motor_initialize(&dc_motor_01);
    ret = dc_motor_initialize(&dc_motor_02);
}
