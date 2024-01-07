/* 
 * File:   mcal_layer_init.c
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on October 21, 2023, 9:26 AM
 */
#include"mcal_layer_init.h"

adc_conf_t ADC1 = {
    .ADC_Channel = ADC_CHANNEL_AN0,
    .Acquisition_Time = ADC_12_TAD,
    .Conversion_Clock = ADC_CONVERSION_CLOCK_FOSC_DIV_16,
    .Result_Format = ADC_RESULT_RIGHT,
    .Voltage_Reference = ADC_VOLTAGE_REFERENCE_DISABLED
};

void mcal_layer_intialize(void)
{
    Std_ReturnType ret = E_OK;
    ret = ADC_initialize(&ADC1);
}
