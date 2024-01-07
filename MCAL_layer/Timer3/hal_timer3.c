/* 
 * File:   hal_timer3.c
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on October 26, 2023, 10:41 AM
 */
#include"hal_timer3.h"

/* Pointer to function to hold the call backs for Timer3 Module */
#if TIMER3_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static InterruptHandler Timer3_InterruptHandler = NULL;
#endif

 static inline void Timer3_Mode_Select(const Timer3_t *timer3_obj);
static inline void Timer3_WR_Register_Size_config(const Timer3_t *timer3_obj);

static uint16_t Timer3_preload = ZERO_INIT;
/**
 * @Summary Initializes the Timer3 Module.
 * @Description This routine initializes the Timer3 Module.
 *              This routine must be called before any other Timer3 Module routine is called.
 *              This routine should only be called once during system initialization.
 * @Preconditions None
 * @param timer3_obj : Pointer to the Timer3 Module configurations.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType Timer3_Initialize(const Timer3_t *timer3_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == timer3_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        
        /*Disable the Timer3 Module*/
        TIMER3_MODULE_DISABLE();
        /*Configure the Prescaler */
        TIMER3_PRESCALER_SELECT((timer3_obj->Timer3_Prescaler_Value));
        /*Configure The Timer3 Module in Timer Mode or Counter Mode*/
        Timer3_Mode_Select(timer3_obj);
        /*Configure WR register size as a 8-bit or as a 16-bit */
        Timer3_WR_Register_Size_config(timer3_obj);
        /*Write the Preload_value in Timer3 Module*/
        ret |= Timer3_Write_value(timer3_obj, timer3_obj->Timer3_Preload_Value);
        /* Copy to use in Overflow*/
        Timer3_preload = (timer3_obj->Timer3_Preload_Value);
        /* Configure the interrupt */
        #if TIMER3_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER3_InterruptEnable();
        TIMER3_InterruptFlagClear();
        /* Interrupt Priority Configurations */
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY==timer3_obj->priority){
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER3_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY==timer3_obj->priority){
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER3_LowPrioritySet();
        }
        else{/* Nothing */}
        #else
        /* This macro will enable global interrupts. */
        INTERRUPT_GlobalInterruptEnable();
        /* This macro will enable peripheral interrupts. */
        INTERRUPT_PeripheralInterruptEnable();
        #endif /*End of INTERRUPT_PRIORITY_LEVELS_ENABLE*/
        /* Initialize The Callback */
        Timer3_InterruptHandler = timer3_obj->Timer3_InterruptHandler;
        #endif /* End of TIMER1_INTERRUPT_FEATURE_ENABLE*/
        
        /*Enable the Timer3 Module*/
        TIMER3_MODULE_ENABLE();
    }
    return ret;
}
/**
 * @Summary Deinitialize the Timer3 Module.
 * @Description This routine is used to deinitialize the Timer3 module by disabling the module itself 
                and optionally disabling its interrupt feature.
 * @Preconditions Timer3_Initialize() function should have been called before calling this function.
 * @param timer3_obj : Pointer to the Timer3 Module configurations.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType Timer3_Deinitialize(const Timer3_t *timer3_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == timer3_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        /*Disable the Timer3 Module*/
        TIMER3_MODULE_DISABLE();
        /* Disable the interrupt */
        #if TIMER3_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER3_InterruptDisable();
        #endif
    }
    return ret;
}
/**
 * @Summary Write a value in the Timer3 Module.
 * @Description This routine is used to write a value to the Timer3 registers (TMR3H and TMR3L) 
                in order to set the timer value.
 * @Preconditions Timer1_Initialize() function should have been called before calling this function.
 * @param timer3_obj : Pointer to the Timer3 Module configurations.
 * @param value : The value to be written to the Timer3 Module.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType Timer3_Write_value(const Timer3_t *timer3_obj, uint16_t value){
    Std_ReturnType ret = E_OK;
    if(NULL == timer3_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        TMR3H = (value) >> 8;
        TMR3L = ((value) & 0x00ff);
    }
    return ret;
}
/**
 * @Summary Read a value from the Timer3 Module.
 * @Description This routine is used to read the value from Timer3 registers (TMR3H and TMR3L) 
                and store it in the provided memory location.
 * @Preconditions Timer3_Initialize() function should have been called before calling this function.
 * @param timer3_obj : Pointer to the Timer3 Module configurations.
 * @param value : Pointer to a variable where the read value will be stored.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType Timer3_Read_value(const Timer3_t *timer3_obj, uint16_t *value){
    Std_ReturnType ret = E_OK;
    uint8_t Timer3_L = ZERO_INIT;
    uint8_t Timer3_H = ZERO_INIT;
    if((NULL == timer3_obj)||(NULL == value))
    {
        ret = E_NOT_OK;
    }else
    {
        Timer3_L = TMR3L;
        Timer3_H = TMR3H;
        *value = (uint16_t)((Timer3_H << 8) + Timer3_L);
    }
    return ret;
}
/**
 * @brief  Configure Timer3 Module to work in Timer mode or Counter mode.
 * @param timer3_obj : Pointer to the Timer3 Module configurations.
 */
static inline void Timer3_Mode_Select(const Timer3_t *timer3_obj){
    /*Configuration of Timer3 Module if works as a timer or Counter*/
    if(TIMER3_TIMER_MODE == timer3_obj->Timer3_Mode){
        TIMER3_TIMER_MODE_ENABLE();
    }
    else if(TIMER3_COUNTER_MODE == timer3_obj->Timer3_Mode){
        TIMER3_COUNTER_MODE_ENABLE();
        /*Configure if Synchronous or ASynchronous Counter*/
        if(TIMER3_ASYNC_COUNTER_ENABLE_CFG == timer3_obj->Timer3_Counter_Mode){
            TIMER3_ASYNC_COUNTER_ENABLE();
        }
        else if(TIMER3_SYNC_COUNTER_ENABLE_CFG == timer3_obj->Timer3_Counter_Mode){
            TIMER3_SYNC_COUNTER_ENABLE();
        }
        else{/* Nothing */}
    }
    else{/* Nothing */}
}
static inline void Timer3_WR_Register_Size_config(const Timer3_t *timer3_obj){
    if(TIMER3_RW_REG_16BIT_MODE==timer3_obj->Timer3_WR_Reg_Mode){
        TIMER3_RW_REG_16BIT_MODE_ENABLE(); 
    }
    else if(TIMER3_RW_REG_8BIT_MODE==timer3_obj->Timer3_WR_Reg_Mode){
        TIMER3_RW_REG_8BIT_MODE_ENABLE();
    }
    else{/* Nothing */}
}
/**
 * @brief TIMER3_ISR - Timer3 Interrupt Service Routine (ISR)
 */
#if TIMER3_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
void TIMER3_ISR(void){
    /* The Timer3 internal interrupt occurred (must be cleared in software) */
    TIMER3_InterruptFlagClear();
    /*in Overflow we set Timer1 Module  */
    TMR3H = (Timer3_preload) >> 8;
    TMR3L = (uint8_t)(Timer3_preload);
    /* Code : To be executed from MCAL Interrupt Context */
    /* Application Callback function gets called every time this ISR executes */
    if(Timer3_InterruptHandler){
        Timer3_InterruptHandler();
    }
    else{/* Nothing */}
}
#endif