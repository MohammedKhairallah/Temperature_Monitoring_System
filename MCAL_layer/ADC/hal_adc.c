/* 
 * File:   hal_adc.c
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on October 19, 2023, 10:31 AM
 */

#include"hal_adc.h"

/* Pointer to function to hold the call back for ADC */
#if ADC_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static InterruptHandler ADC_InterruptHandler = NULL;
#endif


static inline void adc_input_channel_port_configure(const adc_Channel_Select_t channel);
static inline void select_result_format(const adc_conf_t *ADC_obj);
static inline void configure_voltage_reference(const adc_conf_t *ADC_obj);

/**
 * @Summary Initializes the ADC
 * @Description This routine initializes the ADC.
 *              This routine must be called before any other ADC routine is called.
 *              This routine should only be called once during system initialization.
 * @Preconditions None
 * @param ADC_obj : Pointer to the ADC configurations
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType ADC_initialize(const adc_conf_t *ADC_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == ADC_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        /* Disable the ADC */
        ADC_CONVERTER_DISABLE();
        /* Configure the acquisition time */
        ADCON2bits.ACQT = ADC_obj->Acquisition_Time;
        /* Configure the conversion clock */
        ADCON2bits.ADCS = ADC_obj->Conversion_Clock;
        /* Configure the default channel */
        ADCON0bits.CHS = ADC_obj->ADC_Channel;
        adc_input_channel_port_configure(ADC_obj->ADC_Channel);
        /* Configure the interrupt */
        #if ADC_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        ADC_InterruptEnable();
        ADC_InterruptFlagClear();
        /* Interrupt Priority Configurations */
       #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();  /* This macro will enable priority levels on interrupts. */
        if(INTERRUPT_HIGH_PRIORITY==ADC_obj->priority){
            INTERRUPT_GlobalInterruptHighEnable(); /* This macro will enable high priority global interrupts. */
            ADC_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY==ADC_obj->priority){
            INTERRUPT_GlobalInterruptLowEnable();  /* This macro will enable low priority global interrupts. */
            ADC_LowPrioritySet();
        }
        else{/* Nothing */}
        #else
        /* This macro will enable global interrupts. */
        INTERRUPT_GlobalInterruptEnable();
        /* This macro will enable peripheral interrupts. */
        INTERRUPT_PeripheralInterruptEnable();
        #endif /* end of INTERRUPT_PRIORITY_LEVELS_ENABLE*/
        /* Initialize The Call back */
        ADC_InterruptHandler = ADC_obj->ADC_InterruptHandler;
        #endif /* end of ADC_INTERRUPT_FEATURE_ENABLE*/
        /* Configure the result format */
        select_result_format(ADC_obj);
        /* Configure the voltage reference */
        configure_voltage_reference(ADC_obj);
        /* Enable the ADC */
        ADC_CONVERTER_ENABLE();
    }
    return ret;
}
/**
 * @Summary De-Initializes the ADC
 * @param ADC_obj : Pointer to the ADC configurations
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType ADC_deinitialize(const adc_conf_t *ADC_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == ADC_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        /* Disable the ADC */
        ADC_CONVERTER_DISABLE();
        /* Disable the interrupt */
        #if ADC_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        ADC_InterruptDisable();
        #endif
    }
    return ret;
}
/**
 * @Summary Allows selection of a channel for conversion
 * @Description This routine is used to select desired channel for conversion.
 * @Preconditions ADC_initialize() function should have been called before calling this function.
 * @param ADC_obj : Pointer to the ADC configurations.
 * @param channel : Defines the channel available for conversion.
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType ADC_Selsect_Channel(const adc_conf_t *ADC_obj, adc_Channel_Select_t channel){
    Std_ReturnType ret = E_OK;
    if(NULL == ADC_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        /* Configure the selected channel */
        ADCON0bits.CHS = channel;
        adc_input_channel_port_configure(channel);
    }
    return ret;
}
/**
 * @Summary Starts conversion
 * @Description This routine is used to start conversion of desired channel.
 * @Preconditions ADC_initialize() function should have been called before calling this function.
 * @param ADC_obj : Pointer to the ADC configurations.
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType ADC_Start_Conversion(const adc_conf_t *ADC_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == ADC_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        ADC_START_CONVERSION();
    }
    return ret;
}
/**
 * @Summary Returns true when the conversion is completed otherwise false.
 * @Description This routine is used to determine if conversion is completed.
 *              When conversion is complete routine returns true. It returns false otherwise.
 * @Preconditions ADC_initialize() function should have been called before calling this function.
 *                ADC_Start_Conversion() should have been called before calling this function.
 * @param ADC_obj : Pointer to the ADC configurations.
 * @param Conversion_Status : The conversion status
 *          true  - If conversion is complete
 *          false - If conversion is not completed 
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType ADC_IS_Conversion_Done(const adc_conf_t *ADC_obj, uint8_t *Conversion_Status){
    Std_ReturnType ret = E_OK;
    if((NULL == ADC_obj)||(NULL == Conversion_Status))
    {
        ret = E_NOT_OK;
    }else
    {
        /*  *Conversion_Status -> True  - If conversion is complete
                               -> False - If conversion is not completed */
        *Conversion_Status = (uint8_t)(!(ADC_CONVERSION_STATUS()));
    }
    return ret;
}
/**
 * @Summary Returns the ADC conversion value.
 * @Description This routine is used to get the analog to digital converted value.
 *              This routine gets converted values from the channel specified.
 * @Preconditions   This routine returns the conversion value only after the conversion is complete.
 *                  Completion status can be checked using ADC_IS_Conversion_Done() routine.
 * @param ADC_obj : Pointer to the ADC configurations
 * @param Conversion_Result : The converted value.
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType ADC_Get_Conversion_Result(const adc_conf_t *ADC_obj, uint16_t *Conversion_Result){
    Std_ReturnType ret = E_OK;
    if((NULL == ADC_obj)||(NULL == Conversion_Result))
    {
        ret = E_NOT_OK;
    }else
    {
        if(ADC_RESULT_RIGHT==ADC_obj->Result_Format){
            *Conversion_Result = (uint16_t)((ADRESH << 8) + ADRESL);
        }
        else if(ADC_RESULT_LEFT==ADC_obj->Result_Format){
            *Conversion_Result = (uint16_t)((ADRESH << 8) + (ADRESL >> 6));
        }
        else{
            *Conversion_Result = (uint16_t)((ADRESH << 8) + ADRESL);
        }
    }
    return ret;
}
/**
 * @Summary Returns the ADC conversion value, also allows selection of a channel for conversion.
 * @Description This routine is used to select desired channel for conversion.
 *              This routine is get the analog to digital converted value.
 * @Preconditions ADC_initialize() function should have been called before calling this function.
 * @param ADC_obj : Pointer to the ADC configurations
 * @param channel : Defines the channel available for conversion.
 * @param Conversion_Result : The converted value.
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType ADC_Get_Conversion_Blocking(const adc_conf_t *ADC_obj, adc_Channel_Select_t channel, uint16_t *Conversion_Result){
    Std_ReturnType ret = E_OK;
    if((NULL == ADC_obj)||(NULL == Conversion_Result))
    {
        ret = E_NOT_OK;
    }else
    {
        /* select the A/D channel */
        ret = ADC_Selsect_Channel(ADC_obj, channel);
        /* Start the conversion */
        ret = ADC_Start_Conversion(ADC_obj);
        /* Check if conversion is completed */
        while(ADCON0bits.GO_nDONE);
        ret = ADC_Get_Conversion_Result(ADC_obj, Conversion_Result);
    }
    return ret;
}
/**
 * 
 * @param ADC_obj : Pointer to the ADC configurations
 * @param channel : Defines the channel available for conversion.
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
#if ADC_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
Std_ReturnType ADC_Start_Conversion_Interrupt(const adc_conf_t *ADC_obj, adc_Channel_Select_t channel){
    Std_ReturnType ret = E_OK;
    if(NULL == ADC_obj)
    {
        ret = E_NOT_OK;
    }else
    {
        ADC_InterruptFlagClear();
        /* select the A/D channel */
        ret = ADC_Selsect_Channel(ADC_obj, channel);
        /* Start the conversion */
        ret |= ADC_Start_Conversion(ADC_obj);
    }
    return ret;
}
#endif
/**
 * @brief Configures the input port for the specified ADC channel.
 *
 * @param channel The ADC channel to configure.
 */
static inline void adc_input_channel_port_configure(const adc_Channel_Select_t channel){
    switch(channel){
        case ADC_CHANNEL_AN0 :
            SET_BIT(TRISA,_TRISA_RA0_POSN);  // Configure RA0 as input
            break;
        case ADC_CHANNEL_AN1 :
            SET_BIT(TRISA,_TRISA_RA1_POSN);  // Configure RA1 as input
            break;
        case ADC_CHANNEL_AN2 :
            SET_BIT(TRISA,_TRISA_RA2_POSN);  // Configure RA2 as input
            break;
        case ADC_CHANNEL_AN3 :
            SET_BIT(TRISA,_TRISA_RA3_POSN);  // Configure RA3 as input
            break;
        case ADC_CHANNEL_AN4 :
            SET_BIT(TRISA,_TRISA_RA5_POSN);  // Configure RA5 as input
            break;
        case ADC_CHANNEL_AN5 :
            SET_BIT(TRISE,_TRISE_RE0_POSN);  // Configure RE0 as input
            break;
        case ADC_CHANNEL_AN6 :
            SET_BIT(TRISE,_TRISE_RE1_POSN);  // Configure RE1 as input
            break;
        case ADC_CHANNEL_AN7 :
            SET_BIT(TRISE,_TRISE_RE2_POSN);  // Configure RE2 as input
            break;
        case ADC_CHANNEL_AN8 :
            SET_BIT(TRISB,_TRISB_RB2_POSN);  // Configure RB2 as input
            break;
        case ADC_CHANNEL_AN9 :
            SET_BIT(TRISB,_TRISB_RB3_POSN);  // Configure RB3 as input
            break;
        case ADC_CHANNEL_AN10 :
            SET_BIT(TRISB,_TRISB_RB1_POSN);  // Configure RB1 as input
            break;
        case ADC_CHANNEL_AN11 :
            SET_BIT(TRISB,_TRISB_RB4_POSN);  // Configure RB4 as input
            break;
        case ADC_CHANNEL_AN12 :
            SET_BIT(TRISB,_TRISB_RB0_POSN);  // Configure RB0 as input
            break;
        default :
            break;
    }
}
/**
 * @brief Selects the result format for an ADC object.
 *
 * This function selects the result format for an ADC object based on the value of `Result_Format`
 * field in the `ADC_obj` structure. It can either be right-justified or left-justified.
 *
 * @param ADC_obj Pointer to the ADC configuration object.
 */
static inline void select_result_format(const adc_conf_t *ADC_obj){
    // Check the result format field
    if(ADC_RESULT_RIGHT==ADC_obj->Result_Format){
        // Right-justified format selected
        ADC_RIGHT_JUSTIFIED();
    }
    else if(ADC_RESULT_LEFT==ADC_obj->Result_Format){
        // Left-justified format selected
        ADC_LEFT_JUSTIFIED();
    }
    else{
        // Default to right-justified format
        ADC_RIGHT_JUSTIFIED();
    }
}
/**
 * @brief Configures the voltage reference for an ADC object.
 *
 * This function configures the voltage reference for an ADC object based on the value of
 * `Voltage_Reference` field in the `ADC_obj` structure. It can enable or disable the voltage reference.
 *
 * @param ADC_obj Pointer to the ADC configuration object.
 */
static inline void configure_voltage_reference(const adc_conf_t *ADC_obj){
    // Check the voltage reference field
    if(ADC_VOLTAGE_REFERENCE_ENABLED==ADC_obj->Voltage_Reference){
        // Enable voltage reference
        ADC_ENABLE_VOLTAGE_REFERENCE();
    }
    else if(ADC_VOLTAGE_REFERENCE_DISABLED==ADC_obj->Voltage_Reference){
        // Disable voltage reference
        ADC_DISABLE_VOLTAGE_REFERENCE();
    }
    else{
        // Default to disabling voltage reference
        ADC_DISABLE_VOLTAGE_REFERENCE();
    }
}
/**
 * @brief ADC_ISR - ADC Interrupt Service Routine (ISR)
 */
#if ADC_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
void ADC_ISR(void){
    /* The ADC internal interrupt occurred (must be cleared in software) */
    ADC_InterruptFlagClear();
    /* Code : To be executed from MCAL Interrupt Context */
    /* Application Callback function gets called every time this ISR executes */
    if(ADC_InterruptHandler){
        ADC_InterruptHandler();
    }
    else{/* Nothing */}
}
#endif