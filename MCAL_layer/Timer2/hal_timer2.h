/* 
 * File:   hal_timer2.h
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on October 26, 2023, 8:23 AM
 */

#ifndef HAL_TIMER2_H
#define	HAL_TIMER2_H

/* Section : Includes */
#include"../proc/pic18f4620.h"
#include"../mcal_std_types.h"
#include"../GPIO/hal_gpio.h"
#include"../Interrupt/mcal_internal_interrupt.h"


/* Section : Macro Declarations */
/**
 * @brief  This three macros define the value of "Timer2_Prescaler_Value" in "Timer2_t" Structure.
 * @brief  Timer2 Clock Prescale Select bits
 */
#define TIMER2_PRESCALER_VALUE_DIV_BY_1     0
#define TIMER2_PRESCALER_VALUE_DIV_BY_2     1
#define TIMER2_PRESCALER_VALUE_DIV_BY_16    2
/**
 * @brief  This macros define the value of "Timer2_Postscaler_Value" in "Timer2_t" Structure.
 * @brief  Timer2 Output Postscale Select bits
 */
#define TIMER2_POSTSCALER_VALUE_DIV_BY_1       0
#define TIMER2_POSTSCALER_VALUE_DIV_BY_2       1
#define TIMER2_POSTSCALER_VALUE_DIV_BY_3       2
#define TIMER2_POSTSCALER_VALUE_DIV_BY_4       3
#define TIMER2_POSTSCALER_VALUE_DIV_BY_5       4
#define TIMER2_POSTSCALER_VALUE_DIV_BY_6       5
#define TIMER2_POSTSCALER_VALUE_DIV_BY_7       6
#define TIMER2_POSTSCALER_VALUE_DIV_BY_8       7
#define TIMER2_POSTSCALER_VALUE_DIV_BY_9       8
#define TIMER2_POSTSCALER_VALUE_DIV_BY_10      9
#define TIMER2_POSTSCALER_VALUE_DIV_BY_11      10
#define TIMER2_POSTSCALER_VALUE_DIV_BY_12      11
#define TIMER2_POSTSCALER_VALUE_DIV_BY_13      12
#define TIMER2_POSTSCALER_VALUE_DIV_BY_14      13
#define TIMER2_POSTSCALER_VALUE_DIV_BY_15      14
#define TIMER2_POSTSCALER_VALUE_DIV_BY_16      15



/* Section : Macro Functions Declarations */
/**
 * @brief  Timer2 On bit 
 * @note   TIMER2_MODULE_ENABLE()  : Enable the Timer2 Module
 *         TIMER2_MODULE_DISABLE() : Disable the Timer2 Module
 */
#define TIMER2_MODULE_ENABLE()          (T2CONbits.TMR2ON = 1)
#define TIMER2_MODULE_DISABLE()         (T2CONbits.TMR2ON = 0)
/**
 * @brief   Timer2 Clock Prescale Select bits
 */
#define TIMER2_PRESCALER_SELECT(_PRESCALER_)    (T2CONbits.T2CKPS = _PRESCALER_)
/**
 * @brief    Timer2 Output Postscale Select bits
 */
#define TIMER2_POSTSCALER_SELECT(_POSTSCALER_)    (T2CONbits.TOUTPS = _POSTSCALER_)

/* Section : Data type Declarations */
typedef struct{
    #if TIMER2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void (* Timer2_InterruptHandler)(void);
    Interrupt_Priority_cfg priority;   
    #endif
    uint8_t Timer2_Preload_Value;        /* Preload value configuration */
    uint8_t Timer2_Postscaler_Value     :4;    /* Prescale value configuration */
    uint8_t Timer2_Prescaler_Value      :2;    /* Timer Mode / Counter Mode */
    uint8_t Timer1_Reserved             : 2;
}Timer2_t;

/* Section : Functions Declaration */
Std_ReturnType Timer2_Initialize(const Timer2_t *timer2_obj);
Std_ReturnType Timer2_Deinitialize(const Timer2_t *timer2_obj);
Std_ReturnType Timer2_Write_value(const Timer2_t *timer2_obj, uint8_t value);
Std_ReturnType Timer2_Read_value(const Timer2_t *timer2_obj, uint8_t *value);



#endif	/* HAL_TIMER2_H */

