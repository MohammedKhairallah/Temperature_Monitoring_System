/* 
 * File:   ecu_seven_segment.h
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on September 20, 2023, 10:58 AM
 */

#ifndef ECU_SEVEN_SEGMENT_H
#define	ECU_SEVEN_SEGMENT_H

/* Section : Includes */
#include"../../MCAL_layer/GPIO/hal_gpio.h"
#include"ecu_seven_segment_cfg.h"

/* Section : Macro Declarations */
#define SEVEN_SEGMENT_PIN0   0
#define SEVEN_SEGMENT_PIN1   1
#define SEVEN_SEGMENT_PIN2   2
#define SEVEN_SEGMENT_PIN3   3

/* Section : Macro Functions Declarations */


/* Section : Data type Declarations */
typedef enum {
    SEVEN_SEGMENT_COMMMON_ANODE,
    SEVEN_SEGMENT_COMMMON_CATHODE
}seven_seg_type_t;

typedef struct {
    pin_config_t seven_seg_pin[4];
    seven_seg_type_t seven_seg_type;
}seven_seg_t;

/* Section : Functions Declaration */
Std_ReturnType seven_seg_initialize(const seven_seg_t *seven_segment);
Std_ReturnType seven_seg_write_number(const seven_seg_t *seven_segment, uint8_t number);

#endif	/* ECU_SEVEN_SEGMENT_H */

