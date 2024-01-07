/* 
 * File:   ecu_relay.h
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on September 15, 2023, 4:37 PM
 */

#ifndef ECU_RELAY_H
#define	ECU_RELAY_H

/* Section : Includes */
#include"../../MCAL_layer/GPIO/hal_gpio.h"
#include"ecu_relay_cfg.h"

/* Section : Macro Declarations */
#define RELAY_ON_STATUS    0x01U
#define RELAY_OFF_STATUS   0x00U

/* Section : Macro Functions Declarations */


/* Section : Data type Declarations */
typedef struct{
    uint8_t relay_port    :4;
    uint8_t relay_pin     :3;
    uint8_t relay_status  :1;
}relay_t;

/* Section : Functions Declaration */
Std_ReturnType relay_initialize(const relay_t *relay);
Std_ReturnType relay_turn_on(const relay_t *relay);
Std_ReturnType relay_turn_off(const relay_t *relay);
#endif	/* ECU_RELAY_H */

