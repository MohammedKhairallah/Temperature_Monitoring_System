/* 
 * File:   hal_timer0.c
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on October 22, 2023, 5:35 PM
 */
#include"hal_timer0.h"

/* Pointer to function to hold the call backs for Timer0 Module */
#if TIMER0_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static InterruptHandler Timer0_InterruptHandler = NULL;
#endif

static inline void Timer0_Prescaler_config(const Timer0_t *timer0_obj);
static inline void Timer0_Mode_Select(const Timer0_t *timer0_obj);
static inline void Timer0_Register_Size_config(const Timer0_t *timer0_obj);

static uint16_t Timer0_preload = ZERO_INIT;

/**
 * @Summary Initializes the Timer0 Module.
 * @Description This routine initializes the Timer0 Module.
 *              This routine must be called before any other Timer0 Module routine is called.
 *              This routine should only be called once during system initialization.
 * @Preconditions None
 * @param timer0_obj : Pointer to the Timer0 Module configurations.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType Timer0_Initialize(const Timer0_t *timer0_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == timer0_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        /*Disable the Timer0 Module*/
        TIMER0_MODULE_DISABLE();
        /*Configure the Prescaler */
        Timer0_Prescaler_config(timer0_obj);
        /*Configure The Timer0 Module in Timer Mode or Counter Mode*/
        Timer0_Mode_Select(timer0_obj);
        /*Configure Timer0 Module as an 8-bit timer/counter or as a 16-bit timer/counter*/
        Timer0_Register_Size_config(timer0_obj);
        /*Write the Preload_value in Timer0 Module*/
        TMR0H = (timer0_obj->Timer0_Preload_value) >> 8;
        TMR0L = (uint8_t)(timer0_obj->Timer0_Preload_value);
        /* Copy to use in Overflow*/
        Timer0_preload = (timer0_obj->Timer0_Preload_value);
        /* Configure the interrupt */
        #if TIMER0_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER0_InterruptEnable();
        TIMER0_InterruptFlagClear();
        /* Interrupt Priority Configurations */
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY==timer0_obj->priority){
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER0_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY==timer0_obj->priority){
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER0_LowPrioritySet();
        }
        else{/* Nothing */}
        #else
        /* This macro will enable global interrupts. */
        INTERRUPT_GlobalInterruptEnable();
        /* This macro will enable peripheral interrupts. */
        INTERRUPT_PeripheralInterruptEnable();
        #endif /*End of INTERRUPT_PRIORITY_LEVELS_ENABLE*/
        /* Initialize The Callback */
        Timer0_InterruptHandler = timer0_obj->Timer0_InterruptHandler;
        #endif /*End of TIMER0_INTERRUPT_FEATURE_ENABLE*/
        
        /*Enable the Timer0 Module*/
        TIMER0_MODULE_ENABLE();
    }
    return ret;
}
/**
 * @Summary Deinitialize the Timer0 Module.
 * @Description This routine is used to deinitialize the Timer0 module by disabling the module itself 
                and optionally disabling its interrupt feature.
 * @Preconditions Timer0_Initialize() function should have been called before calling this function.
 * @param timer0_obj : Pointer to the Timer0 Module configurations.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType Timer0_Deinitialize(const Timer0_t *timer0_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == timer0_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        /*Disable the Timer0 Module*/
        TIMER0_MODULE_DISABLE();
        /* Disable the interrupt */
        #if TIMER0_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER0_InterruptDisable(); 
        #endif
    }
    return ret;
}
/**
 * @Summary Write a value in the Timer0 Module.
 * @Description This routine is used to write a value to the Timer0 registers (TMR0H and TMR0L) 
                in order to set the timer value.
 * @Preconditions Timer0_Initialize() function should have been called before calling this function.
 * @param timer0_obj : Pointer to the Timer0 Module configurations.
 * @param value : The value to be written to the Timer0 Module.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType Timer0_Write_value(const Timer0_t *timer0_obj, uint16_t value){
    Std_ReturnType ret = E_OK;
    if(NULL == timer0_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        TMR0H = (value) >> 8;
        TMR0L = (uint8_t)(value);
        
    }
    return ret;
}
/**
 * @Summary Read a value from the Timer0 Module.
 * @Description This routine is used to read the value from Timer0 registers (TMR0H and TMR0L) 
                and store it in the provided memory location.
 * @Preconditions Timer0_Initialize() function should have been called before calling this function.
 * @param timer0_obj : Pointer to the Timer0 Module configurations.
 * @param value : Pointer to a variable where the read value will be stored.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType Timer0_Read_value(const Timer0_t *timer0_obj, uint16_t *value){
    Std_ReturnType ret = E_OK;
    uint8_t Timer0_L = ZERO_INIT;
    uint8_t Timer0_H = ZERO_INIT;
    if((NULL == timer0_obj)||(NULL == value))
    {
        ret = E_NOT_OK;
    }else
    {
        Timer0_L = TMR0L;
        Timer0_H = TMR0H;
        *value = (uint16_t)((Timer0_H << 8) + Timer0_L);
    }
    return ret;
}
/**
 *  @brief  Configure Prescaler mode in Timer0 Module.
 * @param timer0_obj : Pointer to the Timer0 Module configurations
 */
static inline void Timer0_Prescaler_config(const Timer0_t *timer0_obj){
    if(TIMER0_PRESCALER_ENABLE_CFG==timer0_obj->Prescaler_Enable){
        TIMER0_PRESCALER_ENABLE();
        T0CONbits.T0PS = (timer0_obj->Prescaler_Value);
    }
    else if(TIMER0_PRESCALER_DISABLE_CFG==timer0_obj->Prescaler_Enable){
        TIMER0_PRESCALER_DISABLE();
    }
    else{/* Nothing */}
}
/**
 *  @brief  Configure Timer0 Module to work in timer mode or Counter mode.
 * @param timer0_obj : Pointer to the Timer0 Module configurations.
 */
static inline void Timer0_Mode_Select(const Timer0_t *timer0_obj){
    /*Configuration of Timer0 Module if works as a timer or Counter*/
    if(TIMER0_TIMER_MODE==timer0_obj->Timer0_Mode){
        TIMER0_TIMER_MODE_ENABLE();
    }
    else if(TIMER0_COUNTER_MODE==timer0_obj->Timer0_Mode){
        TIMER0_COUNTER_MODE_ENABLE();
        /*Configuration of Counter Mode if Counts in falling or rising edge*/
        if(TIMER0_RISING_EDGE_CFG==timer0_obj->Timer0_Counter_Edge){
            TIMER0_RISING_EDGE_ENABLE();
        }
        else if(TIMER0_FALLING_EDGE_CFG==timer0_obj->Timer0_Counter_Edge){
            TIMER0_FALLING_EDGE_ENABLE();
        }
         else{/* Nothing */}
    }
    else{/* Nothing */}
}
/**
 *  @brief  Configure Timer0 Module as an 8-bit timer/counter or as a 16-bit timer/counter
 * @param timer0_obj : Pointer to the Timer0 Module configurations
 */
static inline void Timer0_Register_Size_config(const Timer0_t *timer0_obj){
    if(TIMER0_8BIT_REGISTER_MODE==timer0_obj->Timer0_Register_Size){
        TIMER0_8BIT_MODE_ENABLE(); 
    }
    else if(TIMER0_16BIT_REGISTER_MODE==timer0_obj->Timer0_Register_Size){
        TIMER0_16BIT_MODE_ENABLE();
    }
    else{/* Nothing */}
}
/**
 * @brief TIMER0_ISR - Timer0 Interrupt Service Routine (ISR)
 */
#if TIMER0_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
void TIMER0_ISR(void){
    /* The Timer0 internal interrupt occurred (must be cleared in software) */
    TIMER0_InterruptFlagClear();
    /*in Overflow we set Timer0 Module  */
    TMR0H = (Timer0_preload) >> 8;
    TMR0L = (uint8_t)(Timer0_preload);
    /* Code : To be executed from MCAL Interrupt Context */
    /* Application Callback function gets called every time this ISR executes */
    if(Timer0_InterruptHandler){
        Timer0_InterruptHandler();
    }
    else{/* Nothing */}
}
#endif