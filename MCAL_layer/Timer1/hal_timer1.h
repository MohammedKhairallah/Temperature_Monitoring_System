/* 
 * File:   hal_timer1.h
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on October 24, 2023, 5:44 PM
 */

#ifndef HAL_TIMER1_H
#define	HAL_TIMER1_H

/* Section : Includes */
#include"../proc/pic18f4620.h"
#include"../mcal_std_types.h"
#include"../GPIO/hal_gpio.h"
#include"../Interrupt/mcal_internal_interrupt.h"


/* Section : Macro Declarations */

/*
 * This Two macros define the value of "Timer1_Mode" in "Timer1_t" Structure.
 */
#define TIMER1_TIMER_MODE       0
#define TIMER1_COUNTER_MODE     1
/**
 * @brief  This Four macros define the value of "Timer1_Prescaler_Value" in "Timer1_t" Structure.
 * @brief  Timer1 Prescaler Select bits
 */
#define TIMER1_PRESCALER_VALUE_DIV_BY_1    0
#define TIMER1_PRESCALER_VALUE_DIV_BY_2    1
#define TIMER1_PRESCALER_VALUE_DIV_BY_4    2
#define TIMER1_PRESCALER_VALUE_DIV_BY_8    3
/*
 * This Two macros define the value of "Timer1_WR_Reg_Mode" in "Timer1_t" Structure.
 */
#define TIMER1_RW_REG_8BIT_MODE     0
#define TIMER1_RW_REG_16BIT_MODE    1
/*
 * This Two macros define the value of "Timer1_Oscillator" in "Timer1_t" Structure.
 */
#define TIMER1_OSCILLATOR_ENABLE_CFG       1
#define TIMER1_OSCILLATOR_DISABLE_CFG      0
/*
 * This Two macros define the value of "Timer1_Counter_Mode" in "Timer1_t" Structure.
 */
#define TIMER1_ASYNC_COUNTER_ENABLE_CFG    1
#define TIMER1_SYNC_COUNTER_ENABLE_CFG     0



/* Section : Macro Functions Declarations */
/**
 * @brief  Timer1 On bit 
 * @note   TIMER1_MODULE_ENABLE()  : Enable the Timer1 Module
 *         TIMER1_MODULE_DISABLE() : Disable the Timer1 Module
 */
#define TIMER1_MODULE_ENABLE()          (T1CONbits.TMR1ON = 1)
#define TIMER1_MODULE_DISABLE()         (T1CONbits.TMR1ON = 0)
/**
 * @brief   Timer1 Clock Source Select bit 
 * @note   TIMER1_TIMER_MODE_ENABLE()   :  Enable the Timer1 Module as Timer
 *         TIMER1_COUNTER_MODE_ENABLE() :  Enable the Timer0 Module as Counter
 */
#define TIMER1_TIMER_MODE_ENABLE()      (T1CONbits.TMR1CS = 0)
#define TIMER1_COUNTER_MODE_ENABLE()    (T1CONbits.TMR1CS = 1)
/**
 * @brief  Timer1 oscillator Assignment bit
 * @note   TIMER1_OSCILLATOR_ENABLE()   : Timer1 oscillator is enabled.
 *         TIMER1_OSCILLATOR_DISABLE()  : Timer1 oscillator is shut off.
 */
#define TIMER1_OSCILLATOR_ENABLE()       (T1CONbits.T1OSCEN = 1)
#define TIMER1_OSCILLATOR_DISABLE()      (T1CONbits.T1OSCEN = 0)
/**
 * @brief  Timer1 External Clock Input Synchronization Select bit
 * @note   TIMER1_ASYNC_COUNTER_ENABLE()   : Do not synchronize external clock input.
 *         TIMER1_SYNC_COUNTER_DISABLE()   : Synchronize external clock input.
 * @Preconditions TIMER1_COUNTER_MODE_ENABLE() Function should have been called before using this.
 */
#define TIMER1_ASYNC_COUNTER_ENABLE()       (T1CONbits.T1SYNC = 1)
#define TIMER1_SYNC_COUNTER_ENABLE()        (T1CONbits.T1SYNC = 0)
/**
 * @brief  16-Bit Read/Write Mode Enable bit
 * @note   TIMER1_RW_REG_8BIT_MODE_ENABLE()  : Enables register read/write of TImer1 in one 16-bit operation.
 *         TIMER1_RW_REG_16BIT_MODE_ENABLE() : Enables register read/write of Timer1 in two 8-bit operations.
 */
#define TIMER1_RW_REG_8BIT_MODE_ENABLE()       (T1CONbits.RD16 = 0)
#define TIMER1_RW_REG_16BIT_MODE_ENABLE()      (T1CONbits.RD16 = 1)
/**
 * @brief  Timer1 System Clock Status bit
 * @note   TIMER1_SYSTEM_CLK_STATUS()  : if output = 1 -->  Device clock is derived from Timer1 oscillator
                                                   = 0 -->  Device clock is derived from another source
 */
#define TIMER1_SYSTEM_CLK_STATUS()       (T1CONbits.T1RUN)
/**
 * @brief  Timer1 Input Clock Prescale Select bits
 */
#define TIMER1_PRESCALER_SELECT(_PRESCALER_)    (T1CONbits.T1CKPS = _PRESCALER_)

/* Section : Data type Declarations */

typedef struct{
    #if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void (* Timer1_InterruptHandler)(void);
    Interrupt_Priority_cfg priority;   
    #endif
    uint16_t Timer1_Preload_Value;        /* Preload value configuration */
    uint8_t Timer1_Prescaler_Value :2;    /* Prescale value configuration */
    uint8_t Timer1_Mode            :1;    /* Timer Mode / Counter Mode */
    uint8_t Timer1_Oscillator      :1;    /* Timer1 Oscillator or Another source*/
    uint8_t Timer1_Counter_Mode    :1;    /* Synchronous or ASynchronous Counter */
    uint8_t Timer1_WR_Reg_Mode     :1;    /* One 16-bit operation or Two 8-bit operations */
    uint8_t Timer1_Reserved        : 2;
}Timer1_t;

/* Section : Functions Declaration */
Std_ReturnType Timer1_Initialize(const Timer1_t *timer1_obj);
Std_ReturnType Timer1_Deinitialize(const Timer1_t *timer1_obj);
Std_ReturnType Timer1_Write_value(const Timer1_t *timer1_obj, uint16_t value);
Std_ReturnType Timer1_Read_value(const Timer1_t *timer1_obj, uint16_t *value);

#endif	/* HAL_TIMER1_H */

