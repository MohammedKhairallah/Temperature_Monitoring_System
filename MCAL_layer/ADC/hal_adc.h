/* 
 * File:   hal_adc.h
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on October 19, 2023, 10:31 AM
 */

#ifndef HAL_ADC_H
#define	HAL_ADC_H

/* Section : Includes */
#include"../proc/pic18f4620.h"
#include"../mcal_std_types.h"
#include"../GPIO/hal_gpio.h"
#include"../Interrupt/mcal_internal_interrupt.h"
#include"hal_adc_cfg.h"

/* Section : Macro Declarations */

/**
 * @brief Analog-To-Digital Port Configuration Control
 * @note  When ADC_AN4_ANALOG_FUNCTIONALITY is configured, this means
 *        AN4,AN3,AN2,AN1,AN0 are Analog functionality.
 */
#define ADC_AN0_ANALOG_FUNCTIONALITY     0x0E
#define ADC_AN1_ANALOG_FUNCTIONALITY     0x0D
#define ADC_AN2_ANALOG_FUNCTIONALITY     0x0C
#define ADC_AN3_ANALOG_FUNCTIONALITY     0x0B
#define ADC_AN4_ANALOG_FUNCTIONALITY     0x0A
#define ADC_AN5_ANALOG_FUNCTIONALITY     0x09
#define ADC_AN6_ANALOG_FUNCTIONALITY     0x08
#define ADC_AN7_ANALOG_FUNCTIONALITY     0x07
#define ADC_AN8_ANALOG_FUNCTIONALITY     0x06
#define ADC_AN9_ANALOG_FUNCTIONALITY     0x05
#define ADC_AN10_ANALOG_FUNCTIONALITY    0x04
#define ADC_AN11_ANALOG_FUNCTIONALITY    0x03
#define ADC_AN12_ANALOG_FUNCTIONALITY    0x02
#define ADC_ALL_ANALOG_FUNCTIONALITY     0x00
#define ADC_ALL_DIGITAL_FUNCTIONALITY    0x0F

#define ADC_RESULT_RIGHT    0x01U
#define ADC_RESULT_LEFT     0x00U

#define ADC_VOLTAGE_REFERENCE_ENABLED       0x01U
#define ADC_VOLTAGE_REFERENCE_DISABLED      0x00U

#define ADC_CONVERSION_COMPLETED  0x01U
#define ADC_CONVERSION_INPROGRESS 0x00U

/* Section : Macro Functions Declarations */

/* A/D Conversion Status : A/D conversion in progress / A/D Idle */
#define ADC_CONVERSION_STATUS()    (ADCON0bits.GO_nDONE)

/**
 * @brief Start the Analog-To-Digital Conversion
 */
#define ADC_START_CONVERSION()    (ADCON0bits.GODONE = 1)

/**
 * @brief  Analog-To-Digital Control
 * @note   ADC_CONVERTER_ENABLE()  : Enable the Analog-To-Digital
 *         ADC_CONVERTER_DISABLE() : Disable the Analog-To-Digital
 */
#define ADC_CONVERTER_ENABLE()     (ADCON0bits.ADON = 1)
#define ADC_CONVERTER_DISABLE()    (ADCON0bits.ADON = 0)

/**
 * @brief  Voltage Reference Configuration
 * @note   ADC_ENABLE_VOLTAGE_REFERENCE()  : Voltage Reference is VREF- & VREF+
 *         ADC_DISABLE_VOLTAGE_REFERENCE() : Voltage Reference is VSS & VDD
 */
#define ADC_ENABLE_VOLTAGE_REFERENCE()     do{ \
                                                ADCON1bits.VCFG1 = 1; \
                                                ADCON1bits.VCFG0 = 1; \
                                            }while(0)
#define ADC_DISABLE_VOLTAGE_REFERENCE()    do{ \
                                                 ADCON1bits.VCFG1 = 0; \
                                                 ADCON1bits.VCFG0 = 0; \
                                            }while(0)

/**
 * @brief  Analog-To-Digital Port Configuration Control
 * @note   Example : ADC_ANALOG_DIGITAL_PORT_CONFIG(ADC_AN4_ANALOG_FUNCTIONALITY);
		   When ADC_AN4_ANALOG_FUNCTIONALITY is configured, this means
 *         AN4,AN3,AN2,AN1,AN0 are Analog functionality.
 *         @ref Analog-To-Digital Port Configuration Control
 */
#define ADC_ANALOG_DIGITAL_PORT_CONFIG(_CONFIG)    (ADCON1bits.PCFG = _CONFIG)

/**
 * @brief  A/D Result Format Select
 */
#define ADC_RIGHT_JUSTIFIED()    (ADCON2bits.ADFM = 1)
#define ADC_LEFT_JUSTIFIED()     (ADCON2bits.ADFM = 0)

//#define ADC_ACQUISITION_TIME_SELECT(_CONFIG)    (ADCON2bits_t.ACQT = _CONFIG)


/* Section : Data type Declarations */

/**
 * @brief Analog Channel Select
 */
typedef enum{
    ADC_CHANNEL_AN0 = 0,
    ADC_CHANNEL_AN1, 
    ADC_CHANNEL_AN2,
    ADC_CHANNEL_AN3,
    ADC_CHANNEL_AN4,
    ADC_CHANNEL_AN5,
    ADC_CHANNEL_AN6,
    ADC_CHANNEL_AN7,
    ADC_CHANNEL_AN8,
    ADC_CHANNEL_AN9,
    ADC_CHANNEL_AN10,
    ADC_CHANNEL_AN11,
    ADC_CHANNEL_AN12
}adc_Channel_Select_t;
/**
 * @brief  A/D Acquisition Time Select 
 * @note   Acquisition time (sampling time) is the time required for the (ADC) to 
 *         capture the input voltage during sampling.
 * @note   Acquisition time of a Successive Approximation Register (SAR) ADC is the amount of time 
 *         required to charge the holding capacitor (CHOLD) on the front end of an ADC
 */
typedef enum{
    ADC_0_TAD = 0,
    ADC_2_TAD,
    ADC_4_TAD,
    ADC_6_TAD,
    ADC_8_TAD,
    ADC_12_TAD,
    ADC_16_TAD,
    ADC_20_TAD,
}adc_acquisition_time_t;
/**
 * @brief  A/D Conversion Clock Select
 * @note   If the A/D FRC clock source is selected, a delay of one TCY (instruction cycle) 
 *         is added before the A/D clock starts.
 * @note   This allows the SLEEP instruction to be executed before starting a conversion.
 */
typedef enum{
    ADC_CONVERSION_CLOCK_FOSC_DIV_2 = 0,
    ADC_CONVERSION_CLOCK_FOSC_DIV_8,
    ADC_CONVERSION_CLOCK_FOSC_DIV_32,
    ADC_CONVERSION_CLOCK_FOSC_DIV_FRC,
    ADC_CONVERSION_CLOCK_FOSC_DIV_4,
    ADC_CONVERSION_CLOCK_FOSC_DIV_16,
    ADC_CONVERSION_CLOCK_FOSC_DIV_64
}adc_conversion_clock_t;
/**
 * ADC Configurations
 */
typedef struct{
    #if ADC_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void (* ADC_InterruptHandler)(void);
    Interrupt_Priority_cfg priority;
    #endif   
    adc_acquisition_time_t Acquisition_Time;  /* @ref adc_acquisition_time_t */
    adc_conversion_clock_t Conversion_Clock;  /* @ref adc_conversion_clock_t */
    adc_Channel_Select_t ADC_Channel;         /* @ref adc_channel_select_t */
    uint8_t Voltage_Reference : 1;              /* Voltage Reference Configuration */
    uint8_t Result_Format : 1;                  /* A/D Result Format Select */
    uint8_t ADC_Reserved : 6;
}adc_conf_t;;

/* Section : Functions Declaration */
Std_ReturnType ADC_initialize(const adc_conf_t *ADC_obj);
Std_ReturnType ADC_deinitialize(const adc_conf_t *ADC_obj);
Std_ReturnType ADC_Selsect_Channel(const adc_conf_t *ADC_obj, adc_Channel_Select_t channel);
Std_ReturnType ADC_Start_Conversion(const adc_conf_t *ADC_obj);
Std_ReturnType ADC_IS_Conversion_Done(const adc_conf_t *ADC_obj, uint8_t *Conversion_Status);
Std_ReturnType ADC_Get_Conversion_Result(const adc_conf_t *ADC_obj, uint16_t *Conversion_Result);
Std_ReturnType ADC_Get_Conversion_Blocking(const adc_conf_t *ADC_obj, adc_Channel_Select_t channel, uint16_t *Conversion_Result);

#if ADC_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
Std_ReturnType ADC_Start_Conversion_Interrupt(const adc_conf_t *ADC_obj, adc_Channel_Select_t channel);
#endif

#endif	/* HAL_ADC_H */

