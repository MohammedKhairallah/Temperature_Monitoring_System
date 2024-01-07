/* 
 * File:   ecu_keypad.h
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on September 21, 2023, 7:16 PM
 */

#ifndef ECU_KEYPAD_H
#define	ECU_KEYPAD_H

/* Section : Includes */
#include"../../MCAL_layer/GPIO/hal_gpio.h"
#include"ecu_keypad_cfg.h"

/* Section : Macro Declarations */
#define KEYPAD_ROWS       4
#define KEYPAD_COLUMNS    4

/* Section : Macro Functions Declarations */


/* Section : Data type Declarations */
typedef struct {
    pin_config_t keypad_row_pins[KEYPAD_ROWS];
    pin_config_t keypad_columns_pins[KEYPAD_COLUMNS];
}keypad_t;

/* Section : Functions Declaration */
Std_ReturnType keypad_initialize(const keypad_t *keypad);
Std_ReturnType keypad_get_value(const keypad_t *keypad, uint8_t *value);

#endif	/* ECU_KEYPAD_H */

