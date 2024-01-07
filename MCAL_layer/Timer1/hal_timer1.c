/* 
 * File:   hal_timer1.c
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on October 24, 2023, 5:44 PM
 */
#include"hal_timer1.h" 

/* Pointer to function to hold the call backs for Timer1 Module */
#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static InterruptHandler Timer1_InterruptHandler = NULL;
#endif

static inline void Timer1_Mode_Select(const Timer1_t *timer1_obj);
static inline void Timer1_WR_Register_Size_config(const Timer1_t *timer1_obj);


static uint16_t Timer1_preload = ZERO_INIT;
/**
 * @Summary Initializes the Timer1 Module.
 * @Description This routine initializes the Timer1 Module.
 *              This routine must be called before any other Timer1 Module routine is called.
 *              This routine should only be called once during system initialization.
 * @Preconditions None
 * @param timer1_obj : Pointer to the Timer1 Module configurations.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType Timer1_Initialize(const Timer1_t *timer1_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == timer1_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        /*Disable the Timer1 Module*/
        TIMER1_MODULE_DISABLE();
        /*Configure the Prescaler */
        TIMER1_PRESCALER_SELECT((timer1_obj->Timer1_Prescaler_Value));
        /*Configure The Timer1 Module in Timer Mode or Counter Mode*/
        Timer1_Mode_Select(timer1_obj);
        /*Configure WR register size as a 8-bit or as a 16-bit */
        Timer1_WR_Register_Size_config(timer1_obj);
        /*Write the Preload_value in Timer1 Module*/
        ret |= Timer1_Write_value(timer1_obj, timer1_obj->Timer1_Preload_Value);
        /* Copy to use in Overflow*/
        Timer1_preload = (timer1_obj->Timer1_Preload_Value);
        /* Configure the interrupt */
        #if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER1_InterruptEnable();
        TIMER1_InterruptFlagClear();
        /* Interrupt Priority Configurations */
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY==timer1_obj->priority){
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER1_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY==timer1_obj->priority){
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER1_LowPrioritySet();
        }
        else{/* Nothing */}
        #else
        /* This macro will enable global interrupts. */
        INTERRUPT_GlobalInterruptEnable();
        /* This macro will enable peripheral interrupts. */
        INTERRUPT_PeripheralInterruptEnable();
        #endif /*End of INTERRUPT_PRIORITY_LEVELS_ENABLE*/
        /* Initialize The Callback */
        Timer1_InterruptHandler = timer1_obj->Timer1_InterruptHandler;
        #endif /* End of TIMER1_INTERRUPT_FEATURE_ENABLE*/
        
        /*Enable the Timer1 Module*/
        TIMER1_MODULE_ENABLE();
    }
    return ret;
}
/**
 * @Summary Deinitialize the Timer1 Module.
 * @Description This routine is used to deinitialize the Timer1 module by disabling the module itself 
                and optionally disabling its interrupt feature.
 * @Preconditions Timer1_Initialize() function should have been called before calling this function.
 * @param timer1_obj : Pointer to the Timer1 Module configurations.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType Timer1_Deinitialize(const Timer1_t *timer1_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == timer1_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        /*Disable the Timer1 Module*/
        TIMER1_MODULE_DISABLE();
        /* Disable the interrupt */
        #if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER1_InterruptDisable();
        #endif
    }
    return ret;
}
/**
 * @Summary Write a value in the Timer1 Module.
 * @Description This routine is used to write a value to the Timer1 registers (TMR1H and TMR1L) 
                in order to set the timer value.
 * @Preconditions Timer1_Initialize() function should have been called before calling this function.
 * @param timer1_obj : Pointer to the Timer1 Module configurations.
 * @param value : The value to be written to the Timer1 Module.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType Timer1_Write_value(const Timer1_t *timer1_obj, uint16_t value){
    Std_ReturnType ret = E_OK;
    if(NULL == timer1_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        TMR1H = (value) >> 8;
        TMR1L = ((value) & 0x00ff);
    }
    return ret;
}
/**
 * @Summary Read a value from the Timer1 Module.
 * @Description This routine is used to read the value from Timer1 registers (TMR1H and TMR1L) 
                and store it in the provided memory location.
 * @Preconditions Timer1_Initialize() function should have been called before calling this function.
 * @param timer1_obj : Pointer to the Timer1 Module configurations.
 * @param value : Pointer to a variable where the read value will be stored.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType Timer1_Read_value(const Timer1_t *timer1_obj, uint16_t *value){
    Std_ReturnType ret = E_OK;
    uint8_t Timer1_L = ZERO_INIT;
    uint8_t Timer1_H = ZERO_INIT;
    if((NULL == timer1_obj)||(NULL == value))
    {
        ret = E_NOT_OK;
    }else
    {
        Timer1_L = TMR1L;
        Timer1_H = TMR1H;
        *value = (uint16_t)((Timer1_H << 8) + Timer1_L);
    }
    return ret;
}
/**
 * @brief  Configure Timer1 Module to work in Timer mode or Counter mode.
 * @param timer1_obj : Pointer to the Timer1 Module configurations.
 */
static inline void Timer1_Mode_Select(const Timer1_t *timer1_obj){
    /*Configuration of Timer1 Module if works as a timer or Counter*/
    if(TIMER1_TIMER_MODE == timer1_obj->Timer1_Mode){
        TIMER1_TIMER_MODE_ENABLE();
    }
    else if(TIMER1_COUNTER_MODE == timer1_obj->Timer1_Mode){
        TIMER1_COUNTER_MODE_ENABLE();
        /*Configure if Synchronous or ASynchronous Counter*/
        if(TIMER1_ASYNC_COUNTER_ENABLE_CFG == timer1_obj->Timer1_Counter_Mode){
            TIMER1_ASYNC_COUNTER_ENABLE();
        }
        else if(TIMER1_SYNC_COUNTER_ENABLE_CFG == timer1_obj->Timer1_Counter_Mode){
            TIMER1_SYNC_COUNTER_ENABLE();
        }
        else{/* Nothing */}
    }
    else{/* Nothing */}
}

static inline void Timer1_WR_Register_Size_config(const Timer1_t *timer1_obj){
    if(TIMER1_RW_REG_16BIT_MODE==timer1_obj->Timer1_WR_Reg_Mode){
        TIMER1_RW_REG_16BIT_MODE_ENABLE(); 
    }
    else if(TIMER1_RW_REG_8BIT_MODE==timer1_obj->Timer1_WR_Reg_Mode){
        TIMER1_RW_REG_8BIT_MODE_ENABLE();
    }
    else{/* Nothing */}
}
/**
 * @brief TIMER1_ISR - Timer1 Interrupt Service Routine (ISR)
 */
#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
void TIMER1_ISR(void){
    /* The Timer1 internal interrupt occurred (must be cleared in software) */
    TIMER1_InterruptFlagClear();
    /*in Overflow we set Timer1 Module  */
    TMR1H = (Timer1_preload) >> 8;
    TMR1L = (uint8_t)(Timer1_preload);
    /* Code : To be executed from MCAL Interrupt Context */
    /* Application Callback function gets called every time this ISR executes */
    if(Timer1_InterruptHandler){
        Timer1_InterruptHandler();
    }
    else{/* Nothing */}
}
#endif