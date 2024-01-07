/* 
 * File:   hal_timer0.h
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on October 22, 2023, 5:35 PM
 */

#ifndef HAL_TIMER0_H
#define	HAL_TIMER0_H

/* Section : Includes */
#include"../proc/pic18f4620.h"
#include"../mcal_std_types.h"
#include"../GPIO/hal_gpio.h"
#include"../Interrupt/mcal_internal_interrupt.h"
#include"hal_timer_cfg.h"

/* Section : Macro Declarations */

/*
 * @brief This Two macros define the value of "Prescaler_Enable" in "Timer0_t" Structure.
 */
#define TIMER0_PRESCALER_ENABLE_CFG     1 
#define TIMER0_PRESCALER_DISABLE_CFG    0
/*
 * @brief This Two macros define the value of "Timer0_Counter_Edge" in "Timer0_t" Structure.
 */
#define TIMER0_RISING_EDGE_CFG     1
#define TIMER0_FALLING_EDGE_CFG    0
/*
 * This Two macros define the value of "Timer0_Mode" in "Timer0_t" Structure.
 */
#define TIMER0_TIMER_MODE       1
#define TIMER0_COUNTER_MODE     0
/*
 * This Two macros define the value of "Timer0_Register_Size" in "Timer0_t" Structure.
 */
#define TIMER0_8BIT_REGISTER_MODE       1
#define TIMER0_16BIT_REGISTER_MODE      0

/* Section : Macro Functions Declarations */

/**
 * @brief  Timer0 On/Off Control bit
 * @note   TIMER0_MODULE_ENABLE()  : Enable the Timer0 Module
 *         TIMER0_MODULE_DISABLE() : Disable the Timer0 Module
 */
#define TIMER0_MODULE_ENABLE()          (T0CONbits.TMR0ON = 1)
#define TIMER0_MODULE_DISABLE()         (T0CONbits.TMR0ON = 0)
/**
 * @brief  Timer0 8-Bit/16-Bit Control bit
 * @note   TIMER0_8BIT_MODE_ENABLE()  : Configured the Timer0 Module as an 8-bit timer/counter
 *         TIMER0_16BIT_MODE_ENABLE() : Configured the Timer0 Module as a 16-bit timer/counter
 */
#define TIMER0_8BIT_MODE_ENABLE()       (T0CONbits.T08BIT = 1)
#define TIMER0_16BIT_MODE_ENABLE()      (T0CONbits.T08BIT = 0)
/**
 * @brief  Timer0 Clock Source Select bit
 * @note   TIMER0_TIMER_MODE_ENABLE()   : Enable the Timer0 Module as Timer
 *         TIMER0_COUNTER_MODE_ENABLE() : Enable the Timer0 Module as Counter
 */
#define TIMER0_TIMER_MODE_ENABLE()      (T0CONbits.T0CS = 0)
#define TIMER0_COUNTER_MODE_ENABLE()    (T0CONbits.T0CS = 1)
/**
 * @brief  Timer0 Source Edge Select bit
 * @Preconditions TIMER0_COUNTER_MODE_ENABLE() Function should have been called before calling this function.
 * @note   TIMER0_FALLING_EDGE_ENABLE()  : Increment The counter on Falling Edge Mode "high-to-low" 
 *         TIMER0_RISING_EDGE_ENABLE()   : Increment The counter on Falling Edge Mode "high-to-low" 
 */
#define TIMER0_FALLING_EDGE_ENABLE()    (T0CONbits.T0SE = 1)
#define TIMER0_RISING_EDGE_ENABLE()     (T0CONbits.T0SE = 0)
/**
 * @brief  Timer0 Prescaler Assignment bit
 * @note   TIMER0_PRESCALER_ENABLE()   : Timer0 prescaler is assigned. Timer0 clock input comes from prescaler output. 
 *         TIMER0_PRESCALER_DISABLE()  : TImer0 prescaler is not assigned. Timer0 clock input bypasses prescaler.
 */
#define TIMER0_PRESCALER_ENABLE()       (T0CONbits.PSA = 0)
#define TIMER0_PRESCALER_DISABLE()      (T0CONbits.PSA = 1)


/* Section : Data type Declarations */

/**
 * @brief  Timer0 Prescaler Select bits "Prescaler value configuration"
 * @Preconditions TIMER0_PRESCALER_ENABLE() Function should have been called before using this.
 */
typedef enum{
    PRESCALER_VALUE_DIV_BY_2 = 0,
    PRESCALER_VALUE_DIV_BY_4,
    PRESCALER_VALUE_DIV_BY_8,
    PRESCALER_VALUE_DIV_BY_16,
    PRESCALER_VALUE_DIV_BY_32,
    PRESCALER_VALUE_DIV_BY_64,
    PRESCALER_VALUE_DIV_BY_128,
    PRESCALER_VALUE_DIV_BY_256
}Timer0_Prescaler_Selec_t;
/**
 * @brief  The Timer0_t structure represents a configuration structure for Timer0 module. 
 * @Structure_Members : It includes various fields that define the settings for Timer0, such as 
 * (1) Timer0_InterruptHandler : This member stores the pointer to the interrupt handler function for Timer0.
                                 It is only defined if the TIMER0_INTERRUPT_FEATURE_ENABLE macro is defined as INTERRUPT_FEATURE_ENABLE.
 * (2) priority : This member represents the interrupt priority configuration for Timer0. 
                  It is only defined if the TIMER0_INTERRUPT_FEATURE_ENABLE macro is defined as INTERRUPT_FEATURE_ENABLE.
 * 
 * (3) Prescaler_Value : This member stores the prescaler value for Timer0. 
                         The specific data type Timer0_Prescaler_Selec_t represents the prescaler selection type, which is not provided in the given code snippet.
 * (4) Timer0_Preload_value : This member holds the preload value for Timer0.
 * (5) Prescaler_Enable : This member is a 1-bit field indicating whether the prescaler for Timer0 is enabled (1) or disabled (0).
 * (6) Timer0_Counter_Edge : This member is a 1-bit field representing the counter edge configuration for Timer0.
 * (7) Timer0_Mode : This member is a 1-bit field indicating the mode of Timer0 (timer mode or counter mode).
 * (8) Timer0_Register_Size : This member is a 1-bit field specifying the register size of Timer0 (8-bit or 16-bit).
 * (9) Timer0_Reserved : This member represents 4 reserved bits for future use or alignment purposes.
 */
typedef struct{
    #if TIMER0_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void (* Timer0_InterruptHandler)(void);
    Interrupt_Priority_cfg priority;   
    #endif
    Timer0_Prescaler_Selec_t Prescaler_Value;
    uint16_t Timer0_Preload_value;        
    uint8_t Prescaler_Enable       :1;
    uint8_t Timer0_Counter_Edge    :1;
    uint8_t Timer0_Mode            :1;
    uint8_t Timer0_Register_Size   :1;
    uint8_t Timer0_Reserved        :4;
}Timer0_t;

/* Section : Functions Declaration */
Std_ReturnType Timer0_Initialize(const Timer0_t *timer0_obj);
Std_ReturnType Timer0_Deinitialize(const Timer0_t *timer0_obj);
Std_ReturnType Timer0_Write_value(const Timer0_t *timer0_obj, uint16_t value);
Std_ReturnType Timer0_Read_value(const Timer0_t *timer0_obj, uint16_t *value);
#endif	/* HAL_TIMER0_H */

