/* 
 * File:   hal_timer2.c
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on October 26, 2023, 8:23 AM
 */
#include"hal_timer2.h"

/* Pointer to function to hold the call backs for Timer2 Module */
#if TIMER2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static InterruptHandler Timer2_InterruptHandler = NULL;
#endif

static uint8_t Timer2_preload = ZERO_INIT; 
/**
 * @Summary Initializes the Timer2 Module.
 * @Description This routine initializes the Timer2 Module.
 *              This routine must be called before any other Timer2 Module routine is called.
 *              This routine should only be called once during system initialization.
 * @Preconditions None
 * @param timer2_obj : Pointer to the Timer2 Module configurations.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType Timer2_Initialize(const Timer2_t *timer2_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == timer2_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        /*Disable the Timer0 Module*/
        TIMER2_MODULE_DISABLE();
        /*Configure the Prescaler */
        TIMER2_PRESCALER_SELECT(timer2_obj->Timer2_Prescaler_Value);
        /*Configure the Postscaler */
        TIMER2_POSTSCALER_SELECT(timer2_obj->Timer2_Postscaler_Value);
        /*Write the Preload_value in Timer2 Module*/
        TMR2 = timer2_obj->Timer2_Preload_Value;
        /* Copy to use in Overflow*/
        Timer2_preload = timer2_obj->Timer2_Preload_Value;
        /* Configure the interrupt */
        #if TIMER2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER2_InterruptEnable();
        TIMER2_InterruptFlagClear();
        /* Interrupt Priority Configurations */
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY==timer2_obj->priority){
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER2_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY==timer2_obj->priority){
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER2_LowPrioritySet();
        }
        else{/* Nothing */}
        #else
        /* This macro will enable global interrupts. */
        INTERRUPT_GlobalInterruptEnable();
        /* This macro will enable peripheral interrupts. */
        INTERRUPT_PeripheralInterruptEnable();
        #endif /* End of INTERRUPT_PRIORITY_LEVELS_ENABLE*/
        /* Initialize The Callback */
        Timer2_InterruptHandler = timer2_obj->Timer2_InterruptHandler;
        #endif /* End of TIMER2_INTERRUPT_FEATURE_ENABLE*/
        
        /*Enable the Timer1 Module*/
        TIMER2_MODULE_ENABLE();
    }
    return ret;
}
/**
 * @Summary Deinitialize the Timer2 Module.
 * @Description This routine is used to deinitialize the Timer2 module by disabling the module itself 
                and optionally disabling its interrupt feature.
 * @Preconditions Timer2_Initialize() function should have been called before calling this function.
 * @param timer2_obj : Pointer to the Timer2 Module configurations.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType Timer2_Deinitialize(const Timer2_t *timer2_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == timer2_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        /*Disable the Timer2 Module*/
        TIMER2_MODULE_DISABLE();
        /* Disable the interrupt */
        #if TIMER2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER2_InterruptDisable();
        #endif
    }
    return ret;
}
/**
 * @Summary Write a value in the Timer2 Module.
 * @Description This routine is used to write a value to the Timer2 registers (TMR2) 
                in order to set the timer value.
 * @Preconditions Timer2_Initialize() function should have been called before calling this function.
 * @param timer2_obj : Pointer to the Timer2 Module configurations.
 * @param value : The value to be written to the Timer2 Module.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType Timer2_Write_value(const Timer2_t *timer2_obj, uint8_t value){
    Std_ReturnType ret = E_OK;
    if(NULL == timer2_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        TMR2 = value;
    }
    return ret;
}
/**
 * @Summary Read a value from the Timer2 Module.
 * @Description This routine is used to read the value from Timer2 registers (TMR2) 
                and store it in the provided memory location.
 * @Preconditions Timer2_Initialize() function should have been called before calling this function.
 * @param timer2_obj : Pointer to the Timer2 Module configurations.
 * @param value : Pointer to a variable where the read value will be stored.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType Timer2_Read_value(const Timer2_t *timer2_obj, uint8_t *value){
    Std_ReturnType ret = E_OK;
    if((NULL == timer2_obj)||(NULL == value))
    {
        ret = E_NOT_OK;
    }else
    {
        *value = TMR2;
    }
    return ret;
}
/**
 * @brief TIMER2_ISR - Timer2 Interrupt Service Routine (ISR)
 */
#if TIMER2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
void TIMER2_ISR(void){
    /* The Timer2 internal interrupt occurred (must be cleared in software) */
    TIMER2_InterruptFlagClear();
    /*in Overflow we set Timer1 Module  */
    TMR2 = Timer2_preload;
    /* Code : To be executed from MCAL Interrupt Context */
    /* Application Callback function gets called every time this ISR executes */
    if(Timer2_InterruptHandler){
        Timer2_InterruptHandler();
    }
    else{/* Nothing */}
}
#endif