/* 
 * File:   ecu_layer_init.h
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on September 27, 2023, 12:16 PM
 */

#ifndef ECU_LAYER_INIT_H
#define	ECU_LAYER_INIT_H

/* Section : Includes */
#include"LED/ecu_led.h"
#include"button/ecu_button.h"
#include"Relay/ecu_relay.h"
#include"DC_Motor/ecu_dc_motor.h"
#include"7_segment/ecu_seven_segment.h"
#include"keypad/ecu_keypad.h"
#include"Character_LCD/ecu_chr_lcd.h"

/* Section : Macro Declarations */


/* Section : Macro Functions Declarations */

extern chr_lcd_4bit_t LCD1;

extern dc_motor_t dc_motor_01;
extern dc_motor_t dc_motor_02;

/* Section : Data type Declarations */

/* Section : Functions Declaration */
void ecu_layer_intialize(void);

#endif	/* ECU_LAYER_INIT_H */

