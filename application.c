/* 
 * File:   application.c
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on August 20, 2023, 1:35 PM
 */

#include "application.h"

Std_ReturnType ret = E_OK;
uint16_t lm35_res_1, lm35_res_2, lm35_res_1_Celsius, lm35_res_2_Celsius;
volatile uint8_t  Resultstr1[7], Resultstr2[7];

int main(){
    application_intialize();  /* Make initialization for all hardware */
    
    /* Display initial messages on LCD */
    ret = lcd_4bit_send_string_pos(&LCD1, 1, 5, "Temperature");
    ret = lcd_4bit_send_string_pos(&LCD1, 2, 6, "Monitoring");
    ret = lcd_4bit_send_string_pos(&LCD1, 3, 8, "System");
    __delay_ms(2000); // Delay for 2 seconds
    
    ret = lcd_4bit_send_command(&LCD1, LCD_CLEAR); // Clear LCD
    ret = lcd_4bit_send_command(&LCD1, LCD_RETURN_HOME); // Move cursor to home position
    
     /* Display initial sensor labels on LCD */
    ret = lcd_4bit_send_string_pos(&LCD1, 2, 1, "Sensor 1: ");
    ret = lcd_4bit_send_string_pos(&LCD1, 3, 1, "Sensor 1: ");
    
    ret = lcd_4bit_send_string_pos(&LCD1, 2, 14, "Celsius");
    ret = lcd_4bit_send_string_pos(&LCD1, 3, 14, "Celsius");
    
    /* Main loop for reading sensors and controlling DC motors based on temperature */
    while(1){
        // Read sensor values
        ret = ADC_Get_Conversion_Blocking(&ADC1, ADC_CHANNEL_AN0, &lm35_res_1);
        ret = ADC_Get_Conversion_Blocking(&ADC1, ADC_CHANNEL_AN1, &lm35_res_2);
        
        lm35_res_1_Celsius = lm35_res_1 *  4.88f;  /* 5000(mv) / 1024(10-Bit) => 4.88 */
        lm35_res_2_Celsius = lm35_res_2 *  4.88f;  /* 5000(mv) / 1024(10-Bit) => 4.88 */
        
        lm35_res_1_Celsius /= 10; /* The sensitivity of LM35 is 10 mV/degree Celsius */
        lm35_res_2_Celsius /= 10; /* The sensitivity of LM35 is 10 mV/degree Celsius */
        
        // Convert Celsius values to strings for display
        ret = convert_uint8_to_string(lm35_res_1_Celsius, Resultstr1);
        ret = convert_uint8_to_string(lm35_res_2_Celsius, Resultstr2);
        
        // Display sensor readings on LCD
        ret = lcd_4bit_send_string_pos(&LCD1, 2, 11, Resultstr1);
        ret = lcd_4bit_send_string_pos(&LCD1, 3, 11, Resultstr2);
        
        // Control DC motors based on temperature thresholds
        if(lm35_res_1_Celsius > 25){
            dc_motor_move_left(&dc_motor_01);
        }
        else{
            dc_motor_stop(&dc_motor_01);
        }
        
        if(lm35_res_2_Celsius > 25){
            dc_motor_move_left(&dc_motor_02);
        }
        else{
            dc_motor_stop(&dc_motor_02);
        }
    }
    return (EXIT_SUCCESS);
}
void application_intialize(void)
{
    ecu_layer_intialize(); // Initialize ECU layer
    mcal_layer_intialize(); // Initialize MCAL layer
}
