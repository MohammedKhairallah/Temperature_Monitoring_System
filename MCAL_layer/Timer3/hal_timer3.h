/* 
 * File:   hal_timer3.h
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on October 26, 2023, 10:41 AM
 */

#ifndef HAL_TIMER3_H
#define	HAL_TIMER3_H

/* Section : Includes */
#include"../proc/pic18f4620.h"
#include"../mcal_std_types.h"
#include"../GPIO/hal_gpio.h"
#include"../Interrupt/mcal_internal_interrupt.h"

/* Section : Macro Declarations */

/*
 * This Two macros define the value of "Timer3_Mode" in "Timer3_t" Structure.
 */
#define TIMER3_TIMER_MODE       0
#define TIMER3_COUNTER_MODE     1
/*
 * This Two macros define the value of "Timer1_WR_Reg_Mode" in "Timer3_t" Structure.
 */
#define TIMER3_RW_REG_8BIT_MODE     0
#define TIMER3_RW_REG_16BIT_MODE    1
/**
 * @brief  This three macros define the value of "Timer3_Prescaler_Value" in "Timer3_t" Structure.
 * @brief  Timer3 Input Clock Prescale Select bits
 */
#define TIMER3_PRESCALER_VALUE_DIV_BY_1     0
#define TIMER3_PRESCALER_VALUE_DIV_BY_2     1
#define TIMER3_PRESCALER_VALUE_DIV_BY_4     2
#define TIMER3_PRESCALER_VALUE_DIV_BY_8     3
/*
 * This Two macros define the value of "Timer3_Counter_Mode" in "Timer3_t" Structure.
 */
#define TIMER3_ASYNC_COUNTER_ENABLE_CFG    1
#define TIMER3_SYNC_COUNTER_ENABLE_CFG     0


/* Section : Macro Functions Declarations */

/**
 * @brief  Timer3 On bit 
 * @note   TIMER3_MODULE_ENABLE()  : Enable the Timer3 Module
 *         TIMER3_MODULE_DISABLE() : Disable the Timer3 Module
 */
#define TIMER3_MODULE_ENABLE()          (T3CONbits.TMR3ON = 1)
#define TIMER3_MODULE_DISABLE()         (T3CONbits.TMR3ON = 0)
/**
 * @brief  16-Bit Read/Write Mode Enable bit
 * @note   TIMER1_RW_REG_8BIT_MODE_ENABLE()  : Enables register read/write of TImer3 in one 16-bit operation.
 *         TIMER1_RW_REG_16BIT_MODE_ENABLE() : Enables register read/write of Timer3 in two 8-bit operations.
 */
#define TIMER3_RW_REG_8BIT_MODE_ENABLE()       (T3CONbits.RD16 = 0)
#define TIMER3_RW_REG_16BIT_MODE_ENABLE()      (T3CONbits.RD16 = 1)
/**
 * @brief   Timer3 Clock Source Select bit 
 * @note   TIMER3_TIMER_MODE_ENABLE()   :  Enable the Timer1 Module as Timer
 *         TIMER3_COUNTER_MODE_ENABLE() :  Enable the Timer0 Module as Counter
 */
#define TIMER3_TIMER_MODE_ENABLE()      (T3CONbits.TMR3CS = 0)
#define TIMER3_COUNTER_MODE_ENABLE()    (T3CONbits.TMR3CS = 1)
/**
 * @brief   Timer3 External Clock Input Synchronization Control bit
 * @note   TIMER3_ASYNC_COUNTER_ENABLE()   : Do not synchronize external clock input.
 *         TIMER3_SYNC_COUNTER_ENABLE()    : Synchronize external clock input.
 * @Preconditions TIMER3_COUNTER_MODE_ENABLE() Function should have been called before using this.
 */
#define TIMER3_ASYNC_COUNTER_ENABLE()       (T3CONbits.T3SYNC = 1)
#define TIMER3_SYNC_COUNTER_ENABLE()        (T3CONbits.T3SYNC = 0)
/**
 * @brief   Timer2 Clock Prescale Select bits
 */
#define TIMER3_PRESCALER_SELECT(_PRESCALER_)    (T3CONbits.T3CKPS = _PRESCALER_)


/* Section : Data type Declarations */
/**
 * @brief  The Timer3_t structure represents a configuration structure for Timer3 module. 
 */
typedef struct{
    #if TIMER3_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void (* Timer3_InterruptHandler)(void);
    Interrupt_Priority_cfg priority;   
    #endif
    uint16_t Timer3_Preload_Value;        /* Preload value configuration */
    uint8_t Timer3_Prescaler_Value :2;    /* Prescale value configuration */
    uint8_t Timer3_Mode            :1;    /* Timer Mode / Counter Mode */
    uint8_t Timer3_Counter_Mode    :1;    /* Synchronous or ASynchronous Counter */
    uint8_t Timer3_WR_Reg_Mode     :1;    /* One 16-bit operation or Two 8-bit operations */
    uint8_t Timer3_Reserved        :3;
}Timer3_t;

/* Section : Functions Declaration */
Std_ReturnType Timer3_Initialize(const Timer3_t *timer3_obj);
Std_ReturnType Timer3_Deinitialize(const Timer3_t *timer3_obj);
Std_ReturnType Timer3_Write_value(const Timer3_t *timer3_obj, uint16_t value);
Std_ReturnType Timer3_Read_value(const Timer3_t *timer3_obj, uint16_t *value);

#endif	/* HAL_TIMER3_H */

