/* 
 * File:   ecu_seven_segment.c
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on September 20, 2023, 10:58 AM
 */
#include"ecu_seven_segment.h"
/**
 * @brief make seven segment initialization
 * @param seven_segment : pointer to the seven segment configurations  @ref : seven_seg_t
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType seven_seg_initialize(const seven_seg_t *seven_segment){
    Std_ReturnType ret = E_OK;
    if(NULL == seven_segment)
    {
        ret = E_NOT_OK;
    }else
    {
        ret = gpio_pin_intialize(&(seven_segment->seven_seg_pin[SEVEN_SEGMENT_PIN0]));
        ret = gpio_pin_intialize(&(seven_segment->seven_seg_pin[SEVEN_SEGMENT_PIN1]));
        ret = gpio_pin_intialize(&(seven_segment->seven_seg_pin[SEVEN_SEGMENT_PIN2]));
        ret = gpio_pin_intialize(&(seven_segment->seven_seg_pin[SEVEN_SEGMENT_PIN3]));
    }
    return ret;
}
/**
 * @brief Write a number to the seven segment (number from 0 to 9)
 * @param seven_segment : pointer to the seven segment configurations  @ref : seven_seg_t
 * @param number : the required number to be show
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType seven_seg_write_number(const seven_seg_t *seven_segment, uint8_t number){
    Std_ReturnType ret = E_OK;
    if((NULL == seven_segment)||(number > 9))
    {
        ret = E_NOT_OK;
    }else
    {
        ret = gpio_pin_write_logic(&((seven_segment->seven_seg_pin[SEVEN_SEGMENT_PIN0])), number & 0x01);
        ret = gpio_pin_write_logic(&((seven_segment->seven_seg_pin[SEVEN_SEGMENT_PIN1])), (number>>1) & 0x01);
        ret = gpio_pin_write_logic(&((seven_segment->seven_seg_pin[SEVEN_SEGMENT_PIN2])), (number>>2) & 0x01);
        ret = gpio_pin_write_logic(&((seven_segment->seven_seg_pin[SEVEN_SEGMENT_PIN3])), (number>>3) & 0x01);
    }
    return ret;
}