/* 
 * File:   ecu_keypad.c
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on September 21, 2023, 7:16 PM
 */
#include"ecu_keypad.h"


static const uint8_t btn_values[KEYPAD_ROWS][KEYPAD_COLUMNS] = {
                                                                {'7','8','9','/'},
                                                                {'4','5','6','*'},
                                                                {'1','2','3','-'},
                                                                {'#','0','=','+'}
                                                                };

/**
 * @brief Initialize the keypad assigned pins 
 * @param keypad : pointer to the keypad module configurations  @ref : keypad_t
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType keypad_initialize(const keypad_t *keypad){
    Std_ReturnType ret =E_OK;
    uint8_t count = 0;
    if(NULL == keypad)
    {
        ret = E_NOT_OK;
    }else
    {
        for(count=0;count<KEYPAD_ROWS;count++)
        {
            ret = gpio_pin_intialize(&(keypad->keypad_row_pins[count]));
        }
        for(count=0;count<KEYPAD_COLUMNS;count++)
        {
            ret = gpio_pin_intialize(&(keypad->keypad_columns_pins[count]));
        }
    }
    return ret;
}
/**
 * @brief Get the value of the button pressed by the user and performing the scanning algorithm
 * @param keypad : pointer to the keypad module configurations @ref : keypad_t
 * @param value  : value of the button pressed by the user
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType keypad_get_value(const keypad_t *keypad, uint8_t *value){
    Std_ReturnType ret =E_OK;
    uint8_t row_count = 0,count1 = 0,column_count = 0;
    logic_t column_status = GPIO_LOW;
    
    if((NULL == keypad)||(NULL == value))
    {
        ret = E_NOT_OK;
    }else
    {
        for(row_count=0;row_count<KEYPAD_ROWS;row_count++)
        {
            for(count1=0;count1<KEYPAD_ROWS;count1++)
            {
                ret = gpio_pin_write_logic(&(keypad->keypad_row_pins[count1]), GPIO_LOW);
            }
            ret = gpio_pin_write_logic(&(keypad->keypad_row_pins[row_count]), GPIO_HIGH);
            for(column_count=0;column_count<KEYPAD_COLUMNS;column_count++)
            {
                ret = gpio_pin_read_logic(&(keypad->keypad_columns_pins[column_count]), &column_status); 
                if(GPIO_HIGH==column_status){
                    *value = btn_values[row_count][column_count];
                    break;
                }
            }
        }
    }
    return ret;
}