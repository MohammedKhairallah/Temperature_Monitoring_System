/* 
 * File:   mcal_external_interrupt.c
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on October 12, 2023, 8:10 AM
 */
#include"mcal_external_interrupt.h"

/* Pointer to function to hold the call backs for INTx */
static InterruptHandler INT0_InterruptHandler = NULL; /*This line equal to"static void (*INT0_InterruptHandler)(void) = NULL;"*/
static InterruptHandler INT1_InterruptHandler = NULL;
static InterruptHandler INT2_InterruptHandler = NULL;

static InterruptHandler RB4_InterruptHandler_High = NULL;
static InterruptHandler RB4_InterruptHandler_Low = NULL;
static InterruptHandler RB5_InterruptHandler_High = NULL;
static InterruptHandler RB5_InterruptHandler_Low = NULL;
static InterruptHandler RB6_InterruptHandler_High = NULL;
static InterruptHandler RB6_InterruptHandler_Low = NULL;
static InterruptHandler RB7_InterruptHandler_High = NULL;
static InterruptHandler RB7_InterruptHandler_Low = NULL;

static Std_ReturnType Interrupt_INTx_Enable(const Interrupt_INTx_t *INTx_obj);
static Std_ReturnType Interrupt_INTx_Disable(const Interrupt_INTx_t *INTx_obj);
static Std_ReturnType Interrupt_INTx_Priority_Initialize(const Interrupt_INTx_t *INTx_obj);
static Std_ReturnType Interrupt_INTx_Edge_Initialize(const Interrupt_INTx_t *INTx_obj);
static Std_ReturnType Interrupt_INTx_Pin_Initialize(const Interrupt_INTx_t *INTx_obj);
static Std_ReturnType Interrupt_INTx_Clear_Flag(const Interrupt_INTx_t *INTx_obj);

static Std_ReturnType INT0_SetInterruptHandler(void (*InterruptHandler)(void));
static Std_ReturnType INT1_SetInterruptHandler(void (*InterruptHandler)(void));
static Std_ReturnType INT2_SetInterruptHandler(void (*InterruptHandler)(void));
static Std_ReturnType Interrupt_INTx_SetInterruptHandler(const Interrupt_INTx_t *INTx_obj);

static Std_ReturnType Interrupt_RBx_Enable(const Interrupt_RBx_t *RBx_obj);
static Std_ReturnType Interrupt_RBx_Disable(const Interrupt_RBx_t *RBx_obj);
static Std_ReturnType Interrupt_RBx_Priority_Initialize(const Interrupt_RBx_t *RBx_obj);
static Std_ReturnType Interrupt_RBx_Pin_Initialize(const Interrupt_RBx_t *RBx_obj);

/**
 * @brief Initialize the external interrupt INT0, INT1 and INT2
 * @param INTx_obj : Pointer to the Interrupt configuration object  @ref : Interrupt_INTx_t
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType Interrupt_INTx_initialize(const Interrupt_INTx_t *INTx_obj){
    Std_ReturnType ret =E_OK;
    if(NULL == INTx_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        /* Disable the External interrupt */
        ret = Interrupt_INTx_Disable(INTx_obj);
        /* Clear Interrupt Flag : External interrupt did not occur */
        ret |= Interrupt_INTx_Clear_Flag(INTx_obj);
        /* Configure External interrupt edge */
        ret |= Interrupt_INTx_Edge_Initialize(INTx_obj);
        /* Configure External interrupt Priority */
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        ret |= Interrupt_INTx_Priority_Initialize(INTx_obj);
        #endif
        /* Configure External interrupt I/O pin */
        ret |= Interrupt_INTx_Pin_Initialize(INTx_obj);
        /* Configure Default Interrupt CallBack */
        ret |= Interrupt_INTx_SetInterruptHandler(INTx_obj);
        /* Enable the External interrupt */
        ret |= Interrupt_INTx_Enable(INTx_obj);
    }
    return ret;
}
/**
 * @brief External Interrupt 0 MCAL Helper function 
 */
#if EXTERNAL_INTERRUPT_INTx_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
void INT0_ISR(void){
    /* The INT0 external interrupt occurred (must be cleared in software) */
    EXT_INT0_InterruptFlagClear();
    /* Code : To be executed from MCAL Interrupt Context */
    /* Application Callback function gets called every time this ISR executes */
    if(INT0_InterruptHandler){
        INT0_InterruptHandler();
    }
    else{/* Nothing */}
}
#endif
/**
 * @brief External Interrupt 1 MCAL Helper function 
 */
#if EXTERNAL_INTERRUPT_INTx_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
void INT1_ISR(void){
    /* The INT1 external interrupt occurred (must be cleared in software) */
    EXT_INT1_InterruptFlagClear();
    /* Code : To be executed from MCAL Interrupt Context */
    /* Application Callback function gets called every time this ISR executes */
    if(INT1_InterruptHandler){
        INT1_InterruptHandler();
    }
    else{/* Nothing */}
}
#endif
/**
 * @brief External Interrupt 2 MCAL Helper function 
 */
#if EXTERNAL_INTERRUPT_INTx_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
void INT2_ISR(void){
    /* The INT2 external interrupt occurred (must be cleared in software) */
    EXT_INT2_InterruptFlagClear();
    /* Code : To be executed from MCAL Interrupt Context */
    /* Application Callback function gets called every time this ISR executes */
    if(INT2_InterruptHandler){
        INT2_InterruptHandler();
    }
    else{/* Nothing */}
}
#endif
/**
 * @brief External Interrupt RB4 MCAL Helper function 
 * @param RB4_Source
 */
#if EXTERNAL_INTERRUPT_Onchange_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
void RB4_ISR(uint8_t RB4_Source){
    /* The RB4 external OnChange interrupt occurred (must be cleared in software) */
    EXT_RBx_InterruptFlagClear();
    /* Code : To be executed from MCAL Interrupt Context */
    /* Application Callback function gets called every time this ISR executes */
    if(0==RB4_Source)
    {
        if(RB4_InterruptHandler_High)
        {
            RB4_InterruptHandler_High();
        }
        else{/* Nothing */}
    }
    else if(1==RB4_Source)
    {
        if(RB4_InterruptHandler_Low)
        {
            RB4_InterruptHandler_Low();
        }
        else{/* Nothing */}
    }
    else{/* Nothing */}
}
#endif
/**
 * @brief External Interrupt RB5 MCAL Helper function
 * @param RB5_Source
 */
#if EXTERNAL_INTERRUPT_Onchange_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
void RB5_ISR(uint8_t RB5_Source){
    /* The RB5 external OnChange interrupt occurred (must be cleared in software) */
    EXT_RBx_InterruptFlagClear();
    /* Code : To be executed from MCAL Interrupt Context */
    /* Application Callback function gets called every time this ISR executes */
    if(0==RB5_Source)
    {
        if(RB5_InterruptHandler_High)
        {
            RB5_InterruptHandler_High();
        }
        else{/* Nothing */}
    }
    else if(1==RB5_Source)
    {
        if(RB5_InterruptHandler_Low)
        {
            RB5_InterruptHandler_Low();
        }
        else{/* Nothing */}
    }
    else{/* Nothing */}
}
#endif
/**
 * @brief External Interrupt RB6 MCAL Helper function
 * @param RB6_Source
 */
#if EXTERNAL_INTERRUPT_Onchange_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
void RB6_ISR(uint8_t RB6_Source){
    /* The RB6 external OnChange interrupt occurred (must be cleared in software) */
    EXT_RBx_InterruptFlagClear();
    /* Code : To be executed from MCAL Interrupt Context */
    /* Application Callback function gets called every time this ISR executes */
    if(0==RB6_Source)
    {
        if(RB6_InterruptHandler_High)
        {
            RB6_InterruptHandler_High();
        }
        else{/* Nothing */}
    }else if(1==RB6_Source)
    {
        if(RB6_InterruptHandler_Low)
        {
            RB6_InterruptHandler_Low();
        }
        else{/* Nothing */}
    }
    else{/* Nothing */}
}
#endif
/**
 * @brief External Interrupt RB7 MCAL Helper function
 * @param RB7_Source
 */
#if EXTERNAL_INTERRUPT_Onchange_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
void RB7_ISR(uint8_t RB7_Source){
    /* The RB7 external OnChange interrupt occurred (must be cleared in software) */
    EXT_RBx_InterruptFlagClear();
    /* Code : To be executed from MCAL Interrupt Context */
    /* Application Callback function gets called every time this ISR executes */
    if(0==RB7_Source)
    {
        if(RB7_InterruptHandler_High)
        {
            RB7_InterruptHandler_High();
        }
        else{/* Nothing */}
    }
    else if(1==RB7_Source)
    {
        if(RB7_InterruptHandler_Low)
        {
            RB7_InterruptHandler_Low();
        }
        else{/* Nothing */}
    }
    else{/* Nothing */}
}
#endif
/**
 * @brief DeInitialize the interrupt module
 * @param INTx_obj : Pointer to the Interrupt configuration object  @ref : Interrupt_INTx_t
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType Interrupt_INTx_deinitialize(const Interrupt_INTx_t *INTx_obj){
    Std_ReturnType ret =E_OK;
    if(NULL == INTx_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        ret = Interrupt_INTx_Disable(INTx_obj);
    }
    return ret;
}  
/**
 * @brief Initialize the external interrupt RBx 
 * @param RBx_obj : Pointer to the Interrupt configuration object  @ref : Interrupt_RBx_t
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType Interrupt_RBx_initialize(const Interrupt_RBx_t *RBx_obj){
    Std_ReturnType ret =E_OK;
    if(NULL == RBx_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        /* This routine clears the interrupt disable for the external interrupt, RBx */
        EXT_RBx_InterruptDisable();
        /* This routine clears the interrupt flag for the external interrupt, RBx */
        EXT_RBx_InterruptFlagClear();
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_LOW_PRIORITY==RBx_obj->Priority){
            /* This macro will enable low priority global interrupts. */
            INTERRUPT_GlobalInterruptLowEnable();
            /* This routine set the RBx External Interrupt Priority to be Low priority */
            EXT_RBx_Priority_Low();
        }else if(INTERRUPT_HIGH_PRIORITY==RBx_obj->Priority){
            /* This macro will enable High priority global interrupts. */
            INTERRUPT_GlobalInterruptHighEnable();
            /* This routine set the RBx External Interrupt Priority to be High priority */
            EXT_RBx_Priority_High();
        }else{/* Nothing */}
        #else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        #endif
        /* Initialize the RBx pin to be input */
        ret = gpio_pin_direction_intialize(&(RBx_obj->mcu_pin));
        /* Initialize the call back functions */
        switch(RBx_obj->mcu_pin.pin){
            case GPIO_PIN4 : 
                RB4_InterruptHandler_High = RBx_obj->EXT_InterruptHandler_HIGH;
                RB4_InterruptHandler_Low = RBx_obj->EXT_InterruptHandler_LOW;
                break;
            case GPIO_PIN5 :
                RB5_InterruptHandler_High = RBx_obj->EXT_InterruptHandler_HIGH;
                RB5_InterruptHandler_Low = RBx_obj->EXT_InterruptHandler_LOW;
                break;
            case GPIO_PIN6 :
                RB6_InterruptHandler_High = RBx_obj->EXT_InterruptHandler_HIGH;
                RB6_InterruptHandler_Low = RBx_obj->EXT_InterruptHandler_LOW;
                break;
            case GPIO_PIN7 :
                RB7_InterruptHandler_High = RBx_obj->EXT_InterruptHandler_HIGH;
                RB7_InterruptHandler_Low = RBx_obj->EXT_InterruptHandler_LOW;
                break;
            default: ret = E_NOT_OK;
                break;
        }
        /* This routine sets the interrupt enable for the external interrupt, RBx */
        EXT_RBx_InterruptEnable();
    }
    return ret;
}
/**
 * @brief DeInitialize the interrupt module
 * @param RBx_obj : Pointer to the Interrupt configuration object  @ref : Interrupt_RBx_t
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType Interrupt_RBx_deinitialize(const Interrupt_RBx_t *RBx_obj){
    Std_ReturnType ret =E_OK;
    if(NULL == RBx_obj)
    {
        ret = E_NOT_OK;
    }else
    {
    }
    return ret;
}
/**
 * @brief Enable the INTx interrupt source
 * @param INTx_obj : Pointer to the Interrupt configuration object  @ref : Interrupt_INTx_t
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
static Std_ReturnType Interrupt_INTx_Enable(const Interrupt_INTx_t *INTx_obj){
    Std_ReturnType ret =E_OK;
    if(NULL == INTx_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        switch(INTx_obj->source){
            case INTERRUPT_EXTERNAL_INT0 :
                #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
                INTERRUPT_GlobalInterruptHighEnable();
                #else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
                #endif
                EXT_INT0_InterruptEnable();
                break;
            case INTERRUPT_EXTERNAL_INT1 :
                #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
                INTERRUPT_PriorityLevelsEnable();
                if(INTERRUPT_LOW_PRIORITY==INTx_obj->Priority){
                    INTERRUPT_GlobalInterruptLowEnable();
                }else if(INTERRUPT_HIGH_PRIORITY==INTx_obj->Priority){
                    INTERRUPT_GlobalInterruptHighEnable();
                }else{/* Nothing */}
                #else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
                #endif
                EXT_INT1_InterruptEnable(); 
                break;
            case INTERRUPT_EXTERNAL_INT2 : 
                #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
                INTERRUPT_PriorityLevelsEnable();
                if(INTERRUPT_LOW_PRIORITY==INTx_obj->Priority){
                    INTERRUPT_GlobalInterruptLowEnable();
                }else if(INTERRUPT_HIGH_PRIORITY==INTx_obj->Priority){
                    INTERRUPT_GlobalInterruptHighEnable();
                }else{/* Nothing */}
                #else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
                #endif
                EXT_INT2_InterruptEnable(); 
                break;
            default : 
                ret = E_NOT_OK; 
                break;
        }
    }
    return ret;
}
/**
 * @brief Disable the INTx interrupt source
 * @param INTx_obj : Pointer to the Interrupt configuration object  @ref : Interrupt_INTx_t
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
static Std_ReturnType Interrupt_INTx_Disable(const Interrupt_INTx_t *INTx_obj){
    Std_ReturnType ret =E_OK;
    if(NULL == INTx_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        switch(INTx_obj->source){
            case INTERRUPT_EXTERNAL_INT0 : 
                EXT_INT0_InterruptDisable(); 
                break;
            case INTERRUPT_EXTERNAL_INT1 : 
                EXT_INT1_InterruptDisable(); 
                break;
            case INTERRUPT_EXTERNAL_INT2 : 
                EXT_INT2_InterruptDisable(); 
                break;
            default : 
                ret = E_NOT_OK; 
                break;
        }
    }
    return ret;
}
/**
 * @brief Set the INT1 and INT2 External Interrupt Priority to be High or Low priority
 * @param INTx_obj : Pointer to the Interrupt configuration object  @ref : Interrupt_INTx_t
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
static Std_ReturnType Interrupt_INTx_Priority_Initialize(const Interrupt_INTx_t *INTx_obj){
    Std_ReturnType ret =E_OK;
    if(NULL == INTx_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        switch(INTx_obj->source){ /* INT0 always High Priority */
            case INTERRUPT_EXTERNAL_INT1 : 
                if(INTERRUPT_LOW_PRIORITY==INTx_obj->Priority){
                    EXT_INT1_LowPrioritySet();
                }else if(INTERRUPT_HIGH_PRIORITY==INTx_obj->Priority){
                    EXT_INT1_HighPrioritySet();
                }else{/* Nothing */}
                break;
            case INTERRUPT_EXTERNAL_INT2 : 
                if(INTERRUPT_LOW_PRIORITY==INTx_obj->Priority){
                    EXT_INT2_LowPrioritySet();
                }else if(INTERRUPT_HIGH_PRIORITY==INTx_obj->Priority){
                    EXT_INT2_HighPrioritySet();
                }else{/* Nothing */}
                break;
            default : 
                ret = E_NOT_OK; 
                break;
        }
    }
    return ret;
}
#endif
/**
 * @brief Set the edge detect of the extern interrupt to negative or positive edge
 * @param INTx_obj : Pointer to the Interrupt configuration object  @ref : Interrupt_INTx_t
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
static Std_ReturnType Interrupt_INTx_Edge_Initialize(const Interrupt_INTx_t *INTx_obj){
    Std_ReturnType ret =E_OK;
    if(NULL == INTx_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        switch(INTx_obj->source){
            case INTERRUPT_EXTERNAL_INT0 : 
                if(INTERRUPT_FALLING_EDGE==INTx_obj->edge){
                    EXT_INT0_FallingEdgeSet();
                }else if(INTERRUPT_RISING_EDGE==INTx_obj->edge){
                    EXT_INT0_RisingEdgeSet();
                }else{/* Nothing */}
                break;
            case INTERRUPT_EXTERNAL_INT1 : 
                if(INTERRUPT_FALLING_EDGE==INTx_obj->edge){
                    EXT_INT1_FallingEdgeSet();
                }else if(INTERRUPT_RISING_EDGE==INTx_obj->edge){
                    EXT_INT1_RisingEdgeSet();
                }else{/* Nothing */}
                break;
            case INTERRUPT_EXTERNAL_INT2 : 
                if(INTERRUPT_FALLING_EDGE==INTx_obj->edge){
                    EXT_INT2_FallingEdgeSet();
                }else if(INTERRUPT_RISING_EDGE==INTx_obj->edge){
                    EXT_INT2_RisingEdgeSet();
                }else{/* Nothing */}
                break;
            default : 
                ret = E_NOT_OK; 
                break;
        }
    }
    return ret;
}
/**
 * @brief Initialize the Pin for INTx with the needed configurations
 * @param INTx_obj : Pointer to the Interrupt configuration object  @ref : Interrupt_INTx_t
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
static Std_ReturnType Interrupt_INTx_Pin_Initialize(const Interrupt_INTx_t *INTx_obj){
    Std_ReturnType ret =E_OK;
    if(NULL == INTx_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        /* Initialize the MCU Pin for INTx with the needed configurations */
        ret = gpio_pin_direction_intialize(&(INTx_obj->mcu_pin));
    }
    return ret;
}
/**
 * @brief Clear flag of the external interrupt INT0, INT1 and INT2 
 * @param INTx_obj : Pointer to the Interrupt configuration object  @ref : Interrupt_INTx_t
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
static Std_ReturnType Interrupt_INTx_Clear_Flag(const Interrupt_INTx_t *INTx_obj){
    Std_ReturnType ret =E_OK;
    if(NULL == INTx_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        switch(INTx_obj->source){
            case INTERRUPT_EXTERNAL_INT0 : 
                EXT_INT0_InterruptFlagClear(); 
                break;
            case INTERRUPT_EXTERNAL_INT1 : 
                EXT_INT1_InterruptFlagClear(); 
                break;
            case INTERRUPT_EXTERNAL_INT2 : 
                EXT_INT2_InterruptFlagClear(); 
                break;
            default : 
                ret = E_NOT_OK; 
                break;
        }
    }
    return ret;
}

static Std_ReturnType INT0_SetInterruptHandler(void (*InterruptHandler)(void)){
    Std_ReturnType ret =E_OK;
    if(NULL == InterruptHandler)
    {
        ret = E_NOT_OK;
    }else
    {
        INT0_InterruptHandler = InterruptHandler;
    }
    return ret;
}
static Std_ReturnType INT1_SetInterruptHandler(void (*InterruptHandler)(void)){
    Std_ReturnType ret =E_OK;
    if(NULL == InterruptHandler)
    {
        ret = E_NOT_OK;
    }else
    {
        INT1_InterruptHandler = InterruptHandler;
    }
    return ret;
}
static Std_ReturnType INT2_SetInterruptHandler(void (*InterruptHandler)(void)){
    Std_ReturnType ret =E_OK;
    if(NULL == InterruptHandler)
    {
        ret = E_NOT_OK;
    }else
    {
        INT2_InterruptHandler = InterruptHandler;
    }
    return ret;
}
static Std_ReturnType Interrupt_INTx_SetInterruptHandler(const Interrupt_INTx_t *INTx_obj){
    Std_ReturnType ret =E_OK;
    if(NULL == INTx_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        switch(INTx_obj->source){
            case INTERRUPT_EXTERNAL_INT0 : 
                ret = INT0_SetInterruptHandler(INTx_obj->EXT_InterruptHandler); 
                break;
            case INTERRUPT_EXTERNAL_INT1 : 
                ret = INT1_SetInterruptHandler(INTx_obj->EXT_InterruptHandler);
                break;
            case INTERRUPT_EXTERNAL_INT2 : 
                ret = INT2_SetInterruptHandler(INTx_obj->EXT_InterruptHandler);
                break;
            default : 
                ret = E_NOT_OK; 
                break;
        }
    }
    return ret;
}