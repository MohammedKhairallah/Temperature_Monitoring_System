/* 
 * File:   ecu_led.h
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on August 20, 2023, 1:44 PM
 */

#ifndef ECU_LED_H
#define	ECU_LED_H

/* Section : Includes */
#include"../../MCAL_layer/GPIO/hal_gpio.h"
#include"ecu_led_cfg.h"

/* Section : Macro Declarations */


/* Section : Macro Functions Declarations */


/* Section : Data type Declarations */
typedef enum{
    LED_OFF = 0,
    LED_ON
}led_status;
typedef struct{
    uint8_t port_name      :4;
    uint8_t pin            :3;  
    uint8_t led_status     :1;
}led_t;
/* Section : Functions Declaration */
Std_ReturnType led_initialize(const led_t *led);
Std_ReturnType led_turn_on(const led_t *led);
Std_ReturnType led_turn_off(const led_t *led);
Std_ReturnType led_turn_toggle(const led_t *led);
#endif	/* ECU_LED_H */

