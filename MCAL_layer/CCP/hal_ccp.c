/* 
 * File:   hal_ccp.h
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on October 29, 2023, 9:41 AM
 */
#include"hal_ccp.h"

/* Pointer to function to hold the call backs for CCP1 Module */
#if CCP1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    static InterruptHandler CCP1_InterruptHandler = NULL;
#endif
/* Pointer to function to hold the call backs for CCP2 Module */
#if CCP2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    static InterruptHandler CCP2_InterruptHandler = NULL;
#endif

static void CCP_Interrupt_Config(const ccp_t *_ccp_obj);
static void CCP_PWM_Mode_Config(const ccp_t *_ccp_obj);
static void CCP_Mode_Timer_Select(const ccp_t *_ccp_obj);
static Std_ReturnType CCP_Capture_Mode_Config(const ccp_t *_ccp_obj);
static Std_ReturnType CCP_Compare_Mode_Config(const ccp_t *_ccp_obj);


/**
 * @brief Initializes the CCP module.
 * @Description This function initializes the CCP module based on the provided parameters.
 * @param _ccp_obj : pointer to the CCP Module configurations.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType CCP_Initialize(const ccp_t *_ccp_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == _ccp_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        /* CCP Module Disable */
        if(CCP1_MODULE == _ccp_obj->ccp_module){
            CCP1_SET_MODE(CCP_MODULE_DISABLE);
        }
        else if(CCP2_MODULE == _ccp_obj->ccp_module){
            CCP2_SET_MODE(CCP_MODULE_DISABLE);
        }
        else{/* Nothing */}
        /* CCP Module Capture Mode Initialization */
        if(CCP_CAPTURE_MODE_SELECTED == _ccp_obj->ccp_mode){
            ret = CCP_Capture_Mode_Config(_ccp_obj);
        }
        /* CCP Module Compare Mode Initialization */
        else if(CCP_COMPARE_MODE_SELECTED == _ccp_obj->ccp_mode){
            ret = CCP_Compare_Mode_Config(_ccp_obj);
        }
        #if (CCP1_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED)
        /* CCP Module PWM Mode Initialization */
        else if(CCP_PWM_MODE_SELECTED == _ccp_obj->ccp_mode){
            CCP_PWM_Mode_Config(_ccp_obj);
        }
        #endif
        else{/* Nothing */}
        /* PIN Configurations */
        ret = gpio_pin_intialize(&(_ccp_obj->ccp_pin));
        /* Interrupt Configurations for CCP1 and CCP2 Modules */
        CCP_Interrupt_Config(_ccp_obj);
        
    }
    return ret;
}
/**
 *  @brief Deinitialize the CCP module.
 * This function deinitializes the specified CCP (Capture/Compare/PWM) module.
 * It disables the module, performs interrupt configurations, and returns a status code.
 * @param _ccp_obj : pointer to the CCP Module configurations.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType CCP_DeInitialize(const ccp_t *_ccp_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == _ccp_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        if(CCP1_MODULE == _ccp_obj->ccp_module){
            CCP1_SET_MODE(CCP_MODULE_DISABLE);   /* CCP1 Module Disable */
            /* Interrupt Configurations */ 
            #if CCP1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
            CCP1_InterruptDisable();
            #endif
        }
        else if(CCP2_MODULE == _ccp_obj->ccp_module){
            CCP2_SET_MODE(CCP_MODULE_DISABLE);   /* CCP2 Module Disable */
            /* Interrupt Configurations */ 
            #if CCP2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
            CCP2_InterruptDisable();
            #endif
        }
        else{}
    }
    return ret;
}

#if (CCP1_CFG_SELECTED_MODE==CCP_CFG_CAPTURE_MODE_SELECTED)
/**
 * @brief Check if captured data is ready and clear interrupt flag if set.
 * This function checks if captured data is ready in CCP1 module and
 * clears the corresponding interrupt flag if set. It returns the status
 * and whether data is ready or not.
 * @param _ccp_obj : pointer to the CCP Module configurations.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType CCP1_IsCapturedDataReady(uint8_t *_capture_status){
    Std_ReturnType ret = E_OK;
    if(NULL == _capture_status)
    {
        ret = E_NOT_OK;
    }else
    {
        if(CCP1_CAPTURE_READY  == PIR1bits.CCP1IF){
            *_capture_status = CCP1_CAPTURE_READY;
            CCP1_InterruptFlagClear();
        }
        else{
            *_capture_status = CCP1_CAPTURE_NOT_READY;
        }
    }
    return ret;
}
/**
 * @brief Read the captured value in 16-bit mode.
 * This function reads the captured value in 16-bit mode from CCP1 module
 * and stores it in the provided variable.
 * @param _ccp_obj : pointer to the CCP Module configurations.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType CCP1_Capture_Mode_Read_Value(uint16_t *capture_value){
    Std_ReturnType ret = E_OK;
    CCP_REG_T capture_temp_value = {.ccpr_low = 0, .ccpr_high = 0};
    if(NULL == capture_value)
    {
        ret = E_NOT_OK;
    }else
    {
        /* Copy captured value */
        capture_temp_value.ccpr_low = CCPR1L;
        capture_temp_value.ccpr_high = CCPR1H;
        /* Read the 16-bit capture value */
        *capture_value = capture_temp_value.ccpr_16Bit;
    }
    return ret;
}
#endif
/**
 * 
 * @param _ccp_obj
 * @param _duty
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
#if (CCP1_CFG_SELECTED_MODE==CCP_CFG_COMPARE_MODE_SELECTED)
Std_ReturnType CCP_IsCompareComplete(uint8_t *_compare_status){
    Std_ReturnType ret = E_OK;
    if(NULL == _compare_status)
    {
        ret = E_NOT_OK;
    }else
    {
        if(CCP1_COMPARE_READY  == PIR1bits.CCP1IF){
            *_compare_status = CCP1_COMPARE_READY;
            CCP1_InterruptFlagClear();
        }
        else{
            *_compare_status = CCP1_CAPTURE_NOT_READY;
        }
    }
    return ret;
};
Std_ReturnType CCP_Compare_Mode_Set_Value(const ccp_t *_ccp_obj, uint16_t compare_value){
    Std_ReturnType ret = E_OK;
    CCP_REG_T capture_temp_value = {.ccpr_low = 0, .ccpr_high = 0};
    if(NULL == _ccp_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        /* Copy compared value */
        capture_temp_value.ccpr_16Bit = compare_value;
        
        if(CCP1_MODULE == _ccp_obj->ccp_module){
            CCPR1L = capture_temp_value.ccpr_low;
            CCPR1H = capture_temp_value.ccpr_high;
        }
        else if(CCP2_MODULE == _ccp_obj->ccp_module){
            CCPR2L = capture_temp_value.ccpr_low;
            CCPR2H = capture_temp_value.ccpr_high;
        }
        else{ /* Nothing */ }
    }
    return ret;
}
#endif
/**
 * 
 * @param _ccp_obj : pointer to the CCP Module configurations.
 * @param _duty
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
#if (CCP1_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED)
Std_ReturnType CCP_PWM_Set_Duty(const ccp_t *_ccp_obj, const uint8_t _duty){
    Std_ReturnType ret = E_OK;
    uint16_t l_duty_temp = ZERO_INIT;
    if(NULL == _ccp_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        l_duty_temp = (uint16_t)((float)4 * ((float)PR2 + 1.0) * ((float)_duty / 100.0));
        if(CCP1_MODULE == _ccp_obj->ccp_module){
            CCP1CONbits.DC1B = (uint8_t)(l_duty_temp & 0x0003);
            CCPR1L = (uint8_t)(l_duty_temp >> 2);
        }
        else if(CCP2_MODULE == _ccp_obj->ccp_module){
            CCP2CONbits.DC2B = (uint8_t)(l_duty_temp & 0x0003);
            CCPR2L = (uint8_t)(l_duty_temp >> 2);
        }
    }
    return ret;
}
/**
 * 
 * @param _ccp_obj : pointer to the CCP Module configurations.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType CCP_PWM_Start(const ccp_t *_ccp_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == _ccp_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        if(CCP1_MODULE == _ccp_obj->ccp_module){
            CCP1CONbits.CCP1M = CCP_PWM_MODE;
        }
        else if(CCP2_MODULE == _ccp_obj->ccp_module){
            CCP2CONbits.CCP2M = CCP_PWM_MODE;
        }
        else{ /* Nothing */ }
    }
    return ret;
}
/**
 * 
 * @param _ccp_obj : pointer to the CCP Module configurations.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType CCP_PWM_Stop(const ccp_t *_ccp_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == _ccp_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        if(CCP1_MODULE == _ccp_obj->ccp_module){
            CCP1CONbits.CCP1M = CCP_MODULE_DISABLE;
        }
        else if(CCP2_MODULE == _ccp_obj->ccp_module){
            CCP2CONbits.CCP2M = CCP_MODULE_DISABLE;
        }
        else{ /* Nothing */ }
    }
    return ret;
}
#endif

static void CCP_Interrupt_Config(const ccp_t *_ccp_obj){
    /* CCP1 Interrupt Configurations */ 
    #if CCP1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        CCP1_InterruptEnable();
        CCP1_InterruptFlagClear();
        /* Interrupt Priority Configurations */
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY==_ccp_obj->CCP1_priority){
            /* Enables all high-priority interrupts */
            INTERRUPT_GlobalInterruptHighEnable();
            CCP1_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY==_ccp_obj->CCP1_priority){
            /* Enables all low-priority interrupts */
            INTERRUPT_GlobalInterruptLowEnable();
            CCP1_LowPrioritySet();
        }
        else{/* Nothing */}
        #else
        /* This macro will enable global interrupts. */
        INTERRUPT_GlobalInterruptEnable();
        /* This macro will enable peripheral interrupts. */
        INTERRUPT_PeripheralInterruptEnable();
        #endif /* End of INTERRUPT_PRIORITY_LEVELS_ENABLE*/
        /* Initialize The Callback */
        CCP1_InterruptHandler = _ccp_obj->CCP1_InterruptHandler;
    #endif /* End of TIMER2_INTERRUPT_FEATURE_ENABLE*/
    
    /* CCP2 Interrupt Configurations */ 
    #if CCP2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        CCP2_InterruptEnable();
        CCP2_InterruptFlagClear();
        /* Interrupt Priority Configurations */
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY==_ccp_obj->CCP2_priority){
            /* Enables all high-priority interrupts */
            INTERRUPT_GlobalInterruptHighEnable();
            CCP2_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY==_ccp_obj->CCP2_priority){
            /* Enables all low-priority interrupts */
            INTERRUPT_GlobalInterruptLowEnable();
            CCP2_LowPrioritySet();
        }
        else{/* Nothing */}
        #else
        /* This macro will enable global interrupts. */
        INTERRUPT_GlobalInterruptEnable();
        /* This macro will enable peripheral interrupts. */
        INTERRUPT_PeripheralInterruptEnable();
        #endif /* End of INTERRUPT_PRIORITY_LEVELS_ENABLE*/
        /* Initialize The Callback */
        CCP2_InterruptHandler = _ccp_obj->CCP2_InterruptHandler;
    #endif
}

#if (CCP1_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED)
static void CCP_PWM_Mode_Config(const ccp_t *_ccp_obj){
    /* PWM Frequency Initialization */
    PR2 = (uint8_t)(((float)_XTAL_FREQ / ((float)_ccp_obj->PWM_Frequency * 4.0 * (float)_ccp_obj->timer2_prescaler_value * 
                    (float)_ccp_obj->timer2_postscaler_value)) - 1);
    if(CCP1_MODULE == _ccp_obj->ccp_module){
        if(CCP_PWM_MODE == _ccp_obj->ccp_mode_variant){
            CCP1_SET_MODE(CCP_PWM_MODE);
        }
        else {/* Nothing */}
    }
    else if(CCP2_MODULE == _ccp_obj->ccp_module){
        if(CCP_PWM_MODE == _ccp_obj->ccp_mode_variant){
            CCP2_SET_MODE(CCP_PWM_MODE);
        }
        else {/* Nothing */}
    }
    else{/* Nothing */}
}
#endif
/**
 * @brief Selects the capture/compare timer source for the CCP modules based on the provided CCP configuration.
 * @note Ensure that Timer3 and Timer1 are properly configured before calling this function.
 * @param _ccp_obj : Pointer to the CCP configuration object.
 */
static void CCP_Mode_Timer_Select(const ccp_t *_ccp_obj){
    if(CCP1_CCP2_TIMER3 == _ccp_obj->ccp_capture_timer){
        /* Timer3 is the capture/compare clock source for the CCP modules */
        T3CONbits.T3CCP1 = 0;
        T3CONbits.T3CCP2 = 1;
    }
    else if(CCP1_TIMER1_CCP2_TIMER3 == _ccp_obj->ccp_capture_timer){
        /* Timer3 is the capture/compare clock source for CCP2 */
        /* Timer1 is the capture/compare clock source for CCP1 */
        T3CONbits.T3CCP1 = 1;
        T3CONbits.T3CCP2 = 0;
    }
    else if(CCP1_CCP2_TIMER1 == _ccp_obj->ccp_capture_timer){
        /* Timer1 is the capture/compare clock source for the CCP modules */
        T3CONbits.T3CCP1 = 0;
        T3CONbits.T3CCP2 = 0;
    }
    else{ /* Nothing */ }
}
/**
 * @brief Configures the Capture/Compare/PWM (CCP) module in capture mode based on the provided CCP configuration.
 * This function configures the CCP module in capture mode for the specified module (CCP1 or CCP2) and the
 * selected capture mode variant (CCP_CAPTURE_MODE_1_FALLING_EDGE, CCP_CAPTURE_MODE_1_RISING_EDGE,
 * CCP_CAPTURE_MODE_4_RISING_EDGE, or CCP_CAPTURE_MODE_16_RISING_EDGE).
 * @note Ensure that the CCP module is properly initialized before calling this function.
 * @param _ccp_obj : Pointer to the CCP configuration object.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
static Std_ReturnType CCP_Capture_Mode_Config(const ccp_t *_ccp_obj){
    Std_ReturnType ret = E_OK;
    if(CCP1_MODULE == _ccp_obj->ccp_module){
        /* CCP1 Module Capture variants Mode Initialization */
        switch(_ccp_obj->ccp_mode_variant){
            case CCP_CAPTURE_MODE_1_FALLING_EDGE:
                CCP1_SET_MODE(CCP_CAPTURE_MODE_1_FALLING_EDGE);
                break;
            case CCP_CAPTURE_MODE_1_RISING_EDGE:
                CCP1_SET_MODE(CCP_CAPTURE_MODE_1_RISING_EDGE);
                break;
            case CCP_CAPTURE_MODE_4_RISING_EDGE:
                CCP1_SET_MODE(CCP_CAPTURE_MODE_4_RISING_EDGE);
                break;
            case CCP_CAPTURE_MODE_16_RISING_EDGE:
                CCP1_SET_MODE(CCP_CAPTURE_MODE_16_RISING_EDGE);
                break;
            default :  ret = E_NOT_OK; /* Not supported variant */
        }
    }
    else if(CCP2_MODULE == _ccp_obj->ccp_module){
        /* CCP2 Module Capture variants Mode Initialization */
        switch(_ccp_obj->ccp_mode_variant){
            case CCP_CAPTURE_MODE_1_FALLING_EDGE:
                CCP2_SET_MODE(CCP_CAPTURE_MODE_1_FALLING_EDGE);
                break;
            case CCP_CAPTURE_MODE_1_RISING_EDGE:
                CCP2_SET_MODE(CCP_CAPTURE_MODE_1_RISING_EDGE);
                break;
            case CCP_CAPTURE_MODE_4_RISING_EDGE:
                CCP2_SET_MODE(CCP_CAPTURE_MODE_4_RISING_EDGE);
                break;
            case CCP_CAPTURE_MODE_16_RISING_EDGE:
                CCP2_SET_MODE(CCP_CAPTURE_MODE_16_RISING_EDGE);
                break;
            default :  ret = E_NOT_OK; /* Not supported variant */
        }
    }
    else{/* Nothing */}
    
    CCP_Mode_Timer_Select(_ccp_obj);
    
    return ret;
}
/**
 * 
 * @param _ccp_obj : Pointer to the CCP configuration object.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
static Std_ReturnType CCP_Compare_Mode_Config(const ccp_t *_ccp_obj){
    Std_ReturnType ret = E_OK;
    if(CCP1_MODULE == _ccp_obj->ccp_module){
        /* CCP1 Module Compare variants Mode Initialization */
        switch(_ccp_obj->ccp_mode_variant){
            case CCP_COMPARE_MODE_SET_PIN_LOW:
                CCP1_SET_MODE(CCP_COMPARE_MODE_SET_PIN_LOW);
                break;
            case CCP_COMPARE_MODE_SET_PIN_HIGH:
                CCP1_SET_MODE(CCP_COMPARE_MODE_SET_PIN_HIGH);
                break;
            case CCP_COMPARE_MODE_TOGGLE_ON_MATCH:
                CCP1_SET_MODE(CCP_COMPARE_MODE_TOGGLE_ON_MATCH);
                break;
            case CCP_COMPARE_MODE_GEN_SW_INTERRUPT:
                CCP1_SET_MODE(CCP_COMPARE_MODE_GEN_SW_INTERRUPT);
                break;
            case CCP_COMPARE_MODE_GEN_EVENT:
                CCP1_SET_MODE(CCP_COMPARE_MODE_GEN_EVENT);
                break;
            default :  ret = E_NOT_OK; /* Not supported variant */
        }
    }
    else if(CCP2_MODULE == _ccp_obj->ccp_module){
        switch(_ccp_obj->ccp_mode_variant){
            case CCP_COMPARE_MODE_SET_PIN_LOW:
                CCP2_SET_MODE(CCP_COMPARE_MODE_SET_PIN_LOW);
                break;
            case CCP_COMPARE_MODE_SET_PIN_HIGH:
                CCP2_SET_MODE(CCP_COMPARE_MODE_SET_PIN_HIGH);
                break;
            case CCP_COMPARE_MODE_TOGGLE_ON_MATCH:
                CCP2_SET_MODE(CCP_COMPARE_MODE_TOGGLE_ON_MATCH);
                break;
            case CCP_COMPARE_MODE_GEN_SW_INTERRUPT:
                CCP2_SET_MODE(CCP_COMPARE_MODE_GEN_SW_INTERRUPT);
                break;
            case CCP_COMPARE_MODE_GEN_EVENT:
                CCP2_SET_MODE(CCP_COMPARE_MODE_GEN_EVENT);
                break;
            default :  ret = E_NOT_OK; /* Not supported variant */
        }
    }
    else{/* Nothing */}
    
    CCP_Mode_Timer_Select(_ccp_obj);
    
    return ret;
}
/**
 * @brief CCP1_ISR - CCP1 Interrupt Service Routine (ISR)
 */
#if CCP1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
void CCP1_ISR(void){
    /* The CCP1 internal interrupt occurred (must be cleared in software) */
    CCP1_InterruptFlagClear();
    /* Code : To be executed from MCAL Interrupt Context */
    /* Application Callback function gets called every time this ISR executes */
    if(CCP1_InterruptHandler){
        CCP1_InterruptHandler();
    }
    else{ /* Nothing */ }
}
#endif
/**
 * @brief CCP2_ISR - CCP2 Interrupt Service Routine (ISR)
 */
#if CCP2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
void CCP2_ISR(void){
    /* The CCP2 internal interrupt occurred (must be cleared in software) */
    CCP2_InterruptFlagClear();
    /* Code : To be executed from MCAL Interrupt Context */
    /* Application Callback function gets called every time this ISR executes */
    if(CCP2_InterruptHandler){
        CCP2_InterruptHandler();
    }
    else{ /* Nothing */ }
}
#endif