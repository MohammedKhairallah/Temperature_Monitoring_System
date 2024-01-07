/* 
 * File:   hal_usart.c
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on November 2, 2023, 4:23 PM
 */
#include"hal_usart.h"

/* Pointer to function to hold the call backs for EUSART Module */
#if EUSART_TX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    static InterruptHandler EUSART_TxInterruptHandler = NULL;
#endif

#if EUSART_RX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    static InterruptHandler EUSART_RxInterruptHandler = NULL;
    static InterruptHandler EUSART_FramingErrorHandler = NULL;
    static InterruptHandler EUSART_OverrunErrorHandler = NULL;
#endif  

static void EUSART_Baud_Rate_Calculation(const usart_t *_eusart);
static void EUSART_ASYNC_TX_Initialize(const usart_t *_eusart);
static void EUSART_ASYNC_RX_Initialize(const usart_t *_eusart);
/**
 * @Summary Initializes the EUSART Module.
 * @Description This routine initializes the EUSART Module.
 *              This routine must be called before any other EUSART Module routine is called.
 *              This routine should only be called once during system initialization.
 * @Preconditions None
 * @param _eusart : Pointer to the EUSART Module configurations.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType EUSART_ASYNC_Initialize(const usart_t *_eusart){
    Std_ReturnType ret = E_OK;
    if(NULL == _eusart)
    {
        ret = E_NOT_OK;
    }else
    {
        /* Disable EUSART Module */
        EUSART_MODULE_DISABLE();
        /* RX : Input Configuration as needed */
        TRISCbits.RC7 = 1;
        /* TX : Input Configuration as needed */
        TRISCbits.RC6 = 1;
        /*Set the baud rate.*/
        EUSART_Baud_Rate_Calculation(_eusart);
        
        EUSART_ASYNC_TX_Initialize(_eusart);
        
        EUSART_ASYNC_RX_Initialize(_eusart);
        
        /* Enable EUSART Module */
        EUSART_MODULE_ENABLE();
    }
    return ret;
}
/**
 * @Summary Deinitialize the EUSART Module.
 * @Description This routine is used to deinitialize the EUSART module by disabling the module itself 
                and optionally disabling its interrupt feature.
 * @Preconditions EUSART_ASYNC_Initialize() function should have been called before calling this function.
 * @param _eusart : Pointer to the EUSART Module configurations.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType EUSART_ASYNC_Deinitialize(const usart_t *_eusart){
    Std_ReturnType ret = E_OK;
    if(NULL == _eusart)
    {
        ret = E_NOT_OK;
    }else
    {
        /* Disable EUSART Module */
        EUSART_MODULE_DISABLE();
    }
    return ret;
}
/**
 * 
 * @param _data : Pointer to a variable where the read value will be stored.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType EUSART_ASYNC_ReadByteBlocking(uint8_t *_data){
    Std_ReturnType ret = E_OK;
    if(NULL == _data)
    {
        ret = E_NOT_OK;
    }else
    {
        while(!(PIR1bits.RCIF));
        *_data = RCREG;
    }
    return ret;
}
/**
 * @brief Reads a byte from the EUSART in a non-blocking manner.
 * This function reads a byte from the EUSART module if data is available and stores it in the provided '_data' pointer.
 * It checks for data availability by examining the 'RCIF' bit (receive interrupt flag). If data is available, it is read
 * from the 'RCREG' register.
 * @param _data : Pointer to a variable where the received byte will be stored.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType EUSART_ASYNC_ReadByteNonBlocking(uint8_t *_data){
    Std_ReturnType ret = E_OK;
    if(NULL == _data)
    {
        ret = E_NOT_OK;
    }else
    {
        if(1==(PIR1bits.RCIF)){
            *_data = RCREG;
        }
        else{
            ret = E_NOT_OK;
        }
    }
    return ret;
}
/**
 * @brief Restarts asynchronous reception of data on the EUSART.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType EUSART_ASYNC_RX_Restart(void){
    Std_ReturnType ret = E_OK;
    RCSTAbits.CREN = EUSART_ASYNCHRONOUS_RX_DISABLE;    /* Disables receiver */
    RCSTAbits.CREN = EUSART_ASYNCHRONOUS_RX_ENABLE;     /* Enables receiver */
    return ret;
}
/**
 * 
 * @param _data
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType EUSART_ASYNC_WriteByteBlocking(uint8_t _data){
    Std_ReturnType ret = E_OK;
    while(!(TXSTAbits.TRMT));
    #if EUSART_TX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    EUSART_TX_InterruptEnable();
    #endif
    TXREG = _data;
    return ret;
}
/**
 * 
 * @param _data
 * @param str_len
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType EUSART_ASYNC_WriteStringBlocking(uint8_t *_data, uint16_t str_len){
    Std_ReturnType ret = E_OK;
    uint16_t char_counter = ZERO_INIT;
    if(NULL == _data)
    {
        ret = E_NOT_OK;
    }else
    {  
        for(char_counter=ZERO_INIT; char_counter < str_len; char_counter++){
            ret = EUSART_ASYNC_WriteByteBlocking(_data[char_counter]);
        }   
    }
    return ret;
}

Std_ReturnType EUSART_ASYNC_WriteByteNonBlocking(uint8_t _data){
    Std_ReturnType ret = E_OK;
    
    return ret;
}
Std_ReturnType EUSART_ASYNC_WriteStringNonBlocking(uint8_t *_data, uint16_t str_len){
    Std_ReturnType ret = E_OK;
    
    return ret;
}
/**
 * 
 * @param _eusart : Pointer to the EUSART Module configurations.
 */
static void EUSART_Baud_Rate_Calculation(const usart_t *_eusart){
    float Baud_Rate_Temp = ZERO_INIT;
    switch(_eusart->baudrate_gen_gonfig){
        case(BAUDRATE_ASYN_8BIT_lOW_SPEED):
            EUSART_ASYNCHRONOUS_MODE_ENABLE();
            EUSART_8BIT_BAUDRATE_GEN_ENABLE();
            EUSART_ASYNCHRONOUS_LOW_SPEED_ENABLE();
            Baud_Rate_Temp = ((_XTAL_FREQ / (float)(_eusart->baudrate)) / 64) - 1;  /* X = ((FOSC/Desired Baud Rate)/64) - 1 */
            break;
        case(BAUDRATE_ASYN_8BIT_HIGH_SPEED):
            EUSART_ASYNCHRONOUS_MODE_ENABLE();
            EUSART_8BIT_BAUDRATE_GEN_ENABLE();
            EUSART_ASYNCHRONOUS_HIGH_SPEED_ENABLE();
            Baud_Rate_Temp = ((_XTAL_FREQ / (float)(_eusart->baudrate)) / 16) - 1;
            break;
        case(BAUDRATE_ASYN_16BIT_lOW_SPEED):
            EUSART_ASYNCHRONOUS_MODE_ENABLE();
            EUSART_16BIT_BAUDRATE_GEN_ENABLE();
            EUSART_ASYNCHRONOUS_LOW_SPEED_ENABLE();
            Baud_Rate_Temp = ((_XTAL_FREQ / (float)(_eusart->baudrate)) / 16) - 1;
            break;
        case(BAUDRATE_ASYN_16BIT_HIGH_SPEED):
            EUSART_ASYNCHRONOUS_MODE_ENABLE();
            EUSART_16BIT_BAUDRATE_GEN_ENABLE();
            EUSART_ASYNCHRONOUS_HIGH_SPEED_ENABLE();
            Baud_Rate_Temp = ((_XTAL_FREQ / (float)(_eusart->baudrate)) / 4) - 1;
            break;
        case(BAUDRATE_SYN_8BIT):
            EUSART_SYNCHRONOUS_MODE_ENABLE();
            EUSART_8BIT_BAUDRATE_GEN_ENABLE();
            Baud_Rate_Temp = ((_XTAL_FREQ / (float)(_eusart->baudrate)) / 4) - 1;
            break;
        case(BAUDRATE_SYN_16BIT):
            EUSART_SYNCHRONOUS_MODE_ENABLE();
            EUSART_16BIT_BAUDRATE_GEN_ENABLE();
            Baud_Rate_Temp = ((_XTAL_FREQ / (float)(_eusart->baudrate)) / 4) - 1;
            break;
        default:  ;
    }
    SPBRG = (uint8_t)((uint32_t)Baud_Rate_Temp);
    SPBRGH = (uint8_t)(((uint32_t)Baud_Rate_Temp)>>8);
}
/**
 * 
 * @param _eusart : Pointer to the EUSART Module configurations.
 */
static void EUSART_ASYNC_TX_Initialize(const usart_t *_eusart){
    if(EUSART_ASYNCHRONOUS_TX_ENABLE==_eusart->usart_tx_cfg.usart_tx_enable){
        TXSTAbits.TXEN = EUSART_ASYNCHRONOUS_TX_ENABLE;
        /* EUSART Transmit Interrupt Configuration */
        if(EUSART_ASYNCHRONOUS_INTERRUPT_TX_ENABLE==_eusart->usart_tx_cfg.usart_tx_interrupt_enable){
            /* Interrupt Configurations */
            #if EUSART_TX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
                /*Enables the EUSART transmit interrupt*/
                EUSART_TX_InterruptEnable(); 
                /* Interrupt Priority Configurations */
                #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
                    INTERRUPT_PriorityLevelsEnable();
                    if(INTERRUPT_HIGH_PRIORITY==_eusart->usart_rx_cfg.usart_rx_int_priority){
                        /* Enables all high-priority interrupts */
                        INTERRUPT_GlobalInterruptHighEnable();
                        EUSART_TX_HighPrioritySet();
                    }
                    else if(INTERRUPT_LOW_PRIORITY==_eusart->usart_rx_cfg.usart_rx_int_priority){
                        /* Enables all low-priority interrupts */
                        INTERRUPT_GlobalInterruptLowEnable();
                        EUSART_TX_LowPrioritySet();
                    }
                    else{ /* Nothing */ }
                #else
                    INTERRUPT_GlobalInterruptEnable();
                    INTERRUPT_PeripheralInterruptEnable();
                #endif
                /* Initialize The Callback */
                EUSART_TxInterruptHandler = _eusart->EUSART_TxDefaultInterruptHandler;
            #endif
        }
        else if(EUSART_ASYNCHRONOUS_INTERRUPT_TX_DISABLE==_eusart->usart_tx_cfg.usart_tx_interrupt_enable){
            /*Disables the EUSART transmit interrupt*/
            EUSART_TX_InterruptDisable(); 
        }
        else{ /* Nothing */ }
        /* EUSART  9-Bit Transmit Configuration */
        if(EUSART_ASYNCHRONOUS_9Bit_TX_ENABLE==_eusart->usart_tx_cfg.usart_tx_9bit_enable){
            TXSTAbits.TX9 = EUSART_ASYNCHRONOUS_9Bit_TX_ENABLE;
        }
        else if(EUSART_ASYNCHRONOUS_9Bit_TX_DISABLE==_eusart->usart_tx_cfg.usart_tx_9bit_enable){
            TXSTAbits.TX9 = EUSART_ASYNCHRONOUS_9Bit_TX_DISABLE;
        }
        else{ /* Nothing */ }
    }
    else{ /* Nothing */ }
}
/**
 * 
 * @param _eusart : Pointer to the EUSART Module configurations.
 */
static void EUSART_ASYNC_RX_Initialize(const usart_t *_eusart){
    if(EUSART_ASYNCHRONOUS_RX_ENABLE==_eusart->usart_rx_cfg.usart_rx_enable){
        RCSTAbits.CREN = EUSART_ASYNCHRONOUS_RX_ENABLE;
        /* EUSART Receiver Interrupt Configuration */
        if(EUSART_ASYNCHRONOUS_INTERRUPT_RX_ENABLE==_eusart->usart_rx_cfg.usart_rx_interrupt_enable){
            /* Interrupt Configurations */
            #if EUSART_RX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
                /*Enables the EUSART Receiver interrupt*/
                EUSART_RX_InterruptEnable(); 
                /* Interrupt Priority Configurations */
                #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
                    INTERRUPT_PriorityLevelsEnable();
                    if(INTERRUPT_HIGH_PRIORITY==_eusart->usart_rx_cfg.usart_rx_int_priority){
                        /* Enables all high-priority interrupts */
                        INTERRUPT_GlobalInterruptHighEnable();
                        EUSART_RX_HighPrioritySet();
                    }
                    else if(INTERRUPT_LOW_PRIORITY==_eusart->usart_rx_cfg.usart_rx_int_priority){
                        /* Enables all low-priority interrupts */
                        INTERRUPT_GlobalInterruptLowEnable();
                        EUSART_RX_LowPrioritySet();
                    }
                    else{ /* Nothing */ }
                #else
                    INTERRUPT_GlobalInterruptEnable();
                    INTERRUPT_PeripheralInterruptEnable();
                #endif
                /* Initialize The Callback */
                EUSART_RxInterruptHandler = _eusart->EUSART_RxDefaultInterruptHandler;
                EUSART_FramingErrorHandler = _eusart->EUSART_FramingErrorHandler;
                EUSART_OverrunErrorHandler = _eusart->EUSART_OverrunErrorHandler;
            #endif
        }
        else if(EUSART_ASYNCHRONOUS_INTERRUPT_RX_DISABLE==_eusart->usart_rx_cfg.usart_rx_interrupt_enable){
            /*Disables the EUSART Receiver interrupt*/
            EUSART_RX_InterruptDisable(); 
        }
        else{ /* Nothing */ }
        /* EUSART  9-Bit Receiver Configuration */
        if(EUSART_ASYNCHRONOUS_9Bit_RX_ENABLE==_eusart->usart_rx_cfg.usart_rx_9bit_enable){
            RCSTAbits.RX9 = EUSART_ASYNCHRONOUS_9Bit_RX_ENABLE;
        }
        else if(EUSART_ASYNCHRONOUS_9Bit_RX_DISABLE==_eusart->usart_rx_cfg.usart_rx_9bit_enable){
            RCSTAbits.RX9 = EUSART_ASYNCHRONOUS_9Bit_RX_DISABLE;
        }
        else{ /* Nothing */ }
    }
    else{ /* Nothing */ }
}
/**
 * @brief EUSART_TX_ISR - EUSART_TX Interrupt Service Routine (ISR)
 */
#if EUSART_TX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
void EUSART_TX_ISR(void){
    EUSART_TX_InterruptDisable();
    /* Code : To be executed from MCAL Interrupt Context */
    /* Application Callback function gets called every time this ISR executes */
    if(EUSART_TxInterruptHandler){
        EUSART_TxInterruptHandler();
    }else { /* Nothing */}
}
#endif
/**
 * @brief EUSART_RX_ISR - EUSART_RX Interrupt Service Routine (ISR)
 */
#if EUSART_RX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
void EUSART_RX_ISR(void){
    /* Code : To be executed from MCAL Interrupt Context */
    /* Application Callback function gets called every time this ISR executes */
    if(EUSART_RxInterruptHandler){
        EUSART_RxInterruptHandler();
    }else { /* Nothing */}
    if(EUSART_FramingErrorHandler){
        EUSART_FramingErrorHandler();
    }else { /* Nothing */}
    if(EUSART_OverrunErrorHandler){
        EUSART_OverrunErrorHandler();
    }else { /* Nothing */}
}
#endif
