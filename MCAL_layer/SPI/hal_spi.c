/* 
 * File:   hal_spi.c
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on November 11, 2023, 09:26 AM
 */
#include"hal_spi.h"

/* Pointer to function to hold the call backs for SPI Module */
#if MSSP_SPI_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    static InterruptHandler SPI_InterruptHandler = NULL;
#endif

static void MSSP_SPI_Interrupt_Init(const SPI_t *_spi);
static void MSSP_SPI_Master_Mode_GPIO_PIN_Configurations(const SPI_t *_spi);
static void MSSP_SPI_Slave_Mode_GPIO_PIN_Configurations(const SPI_t *_spi);

/**
 * @Summary Initializes the SPI Module.
 * @Description This routine initializes the SPI Module.
 *              This routine must be called before any other SPI Module routine is called.
 *              This routine should only be called once during system initialization.
 * @Preconditions None
 * @param _spi : Pointer to the SPI Module configurations.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType SPI_Initialize(const SPI_t *_spi){
    Std_ReturnType ret = E_OK;
    if(NULL == _spi)
    {
        ret = E_NOT_OK;
    }else
    {
        /*Disable the SPI Module*/
        SPI_MODULE_DISABLE();
        /*Select the SPI Mode*/
        SSPCON1bits.SSPM = _spi->spi_mode;
        /* Configure Pins (MOSI, MISO, SCK, SS/CS)
         * And Select Master or Slave Mode
         **/
        if((_spi->spi_mode == SPI_MASTER_FOSC_DIV_4) || (_spi->spi_mode == SPI_MASTER_FOSC_DIV_16) ||
            (_spi->spi_mode == SPI_MASTER_FOSC_DIV_64) || (_spi->spi_mode == SPI_MASTER_FOSC_TMR2)){
            MSSP_SPI_Master_Mode_GPIO_PIN_Configurations(_spi);
        }
        else if((_spi->spi_mode == SPI_SLAVE_SS_ENABLE) || (_spi->spi_mode == SPI_SLAVE_SS_DISABLE)){
            MSSP_SPI_Slave_Mode_GPIO_PIN_Configurations(_spi);
        }
        else{/* Nothing */}
        /*Set Clock Frequency*/
        SSPSTATbits.CKE = _spi->spi_config.ClockSelect;
        /*Set Clock Polarity */
        SSPCON1bits.CKP = _spi->spi_config.ClockPolarity;
        /*Select SPI Sample */
        SSPSTATbits.SMP = _spi->spi_config.SampleSelect;
        /* MSSP SPI Interrupt Configurations*/
        #if MSSP_SPI_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        MSSP_SPI_Interrupt_Init(_spi);
        #endif
        /*Enable the SPI Module*/
        SPI_MODULE_ENABLE();
    }
    return ret;
}
/**
 * @Summary Deinitialize the SPI Module.
 * @Description This routine is used to deinitialize the SPI module by disabling the module itself 
                and optionally disabling its interrupt feature.
 * @Preconditions SPI_Initialize() function should have been called before calling this function.
 * @param _eusart : Pointer to the SPI Module configurations.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType SPI_Deinitialize(const SPI_t *_spi){
    Std_ReturnType ret = E_OK;
    if(NULL == _spi)
    {
        ret = E_NOT_OK;
    }else
    {
        /*Disable the SPI Module*/
        SPI_MODULE_DISABLE();
        /* Interrupt Configurations */ 
        #if MSSP_SPI_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        MSSP_SPI_InterruptDisable();
        #endif
    }
    return ret;
}

Std_ReturnType SPI_Send_Byte_Blocking(const SPI_t *_spi, const uint8_t _data){
    Std_ReturnType ret = E_OK;
    if(NULL == _spi)
    {
        ret = E_NOT_OK;
    }else
    {
        SSPBUF = _data;
        while(!(PIR1bits.SSPIF)); /* Waiting to transmit */
        PIR1bits.SSPIF = 0;
    }
    return ret;
}

Std_ReturnType SPI_Read_Byte_Blocking(const SPI_t *_spi, uint8_t *_data){
    Std_ReturnType ret = E_OK;
    if((NULL == _spi)||(NULL == _data))
    {
        ret = E_NOT_OK;
    }else
    {
        while(SSPSTATbits.BF == 0); /* Receive not complete, SSPBUF is empty */
        *_data = SSPBUF;
    }
    return ret;
}

Std_ReturnType SPI_Send_Byte_NonBlocking(const SPI_t *_spi, const uint8_t _data){
    Std_ReturnType ret = E_OK;
    if(NULL == _spi)
    {
        ret = E_NOT_OK;
    }else
    {
        if(1 == SSPSTATbits.BF){
        SSPBUF = _data;
        }else{
        ret = E_NOT_OK;
        }
    }
    return ret;
}

Std_ReturnType SPI_Read_Byte_NonBlocking(const SPI_t *_spi, uint8_t *_data){
    Std_ReturnType ret = E_OK;
    if((NULL == _spi)||(NULL == _data))
    {
        ret = E_NOT_OK;
    }else
    {
        if(1==SSPSTATbits.BF)
        {
            *_data = SSPBUF;
        }else{
        ret = E_NOT_OK;
        }
    }
    return ret;
}

static void MSSP_SPI_Master_Mode_GPIO_PIN_Configurations(const SPI_t *_spi){
    Std_ReturnType ret = E_OK;
    pin_config_t SPI_SDO = {.port = PORTC_INDEX, .pin = GPIO_PIN5, .direction = GPIO_direction_OUTPUT};
    pin_config_t SPI_SDI = {.port = PORTC_INDEX, .pin = GPIO_PIN4, .direction = GPIO_direction_INPUT};
    pin_config_t SPI_CLK = {.port = PORTC_INDEX, .pin = GPIO_PIN3, .direction = GPIO_direction_OUTPUT};
    
    ret = gpio_pin_direction_intialize(&SPI_SDO); 
    ret = gpio_pin_direction_intialize(&SPI_SDI);
    ret = gpio_pin_direction_intialize(&SPI_CLK);
}

static void MSSP_SPI_Slave_Mode_GPIO_PIN_Configurations(const SPI_t *_spi){
    Std_ReturnType ret = E_OK;
    pin_config_t SPI_SDO = {.port = PORTC_INDEX, .pin = GPIO_PIN5, .direction = GPIO_direction_OUTPUT};
    pin_config_t SPI_SDI = {.port = PORTC_INDEX, .pin = GPIO_PIN4, .direction = GPIO_direction_INPUT};
    pin_config_t SPI_CLK = {.port = PORTC_INDEX, .pin = GPIO_PIN3, .direction = GPIO_direction_INPUT};
    pin_config_t SPI_SS = {.port = PORTA_INDEX, .pin = GPIO_PIN5, .direction = GPIO_direction_INPUT};
    
    ret = gpio_pin_direction_intialize(&SPI_SDO); 
    ret = gpio_pin_direction_intialize(&SPI_SDI);
    ret = gpio_pin_direction_intialize(&SPI_CLK); 
    
    if(SPI_SLAVE_SS_ENABLE == _spi->spi_mode){
        ret = gpio_pin_direction_intialize(&SPI_SS);
    }
    else{/* Nothing */}
}
#if MSSP_SPI_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static void MSSP_SPI_Interrupt_Init(const SPI_t *_spi){
    /* Interrupt Configurations */
    MSSP_SPI_InterruptEnable();
    MSSP_SPI_InterruptFlagClear();
    /* Initialize The Callback */
    SPI_InterruptHandler = _spi->MSSP_SPI_InterruptHandler;
    /* Interrupt Priority Configurations */
    #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
    INTERRUPT_PriorityLevelsEnable();
    if(INTERRUPT_HIGH_PRIORITY == _spi->priority){
        /* Enables all high-priority interrupts */
        INTERRUPT_GlobalInterruptHighEnable();
        MSSP_SPI_HighPrioritySet();
    }
    else if(INTERRUPT_LOW_PRIORITY == _spi->priority){
        /* Enables all unmasked peripheral interrupts */
        INTERRUPT_GlobalInterruptLowEnable();
        MSSP_SPI_LowPrioritySet();
    }
    else{ /* Nothing */ }
   #else
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
   #endif      
   
}
#endif 
/**
 * @brief MSSP_SPI_ISR - SPI Interrupt Service Routine (ISR)
 */
#if MSSP_SPI_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
void MSSP_SPI_ISR(void){
    /* The SPI internal interrupt occurred (must be cleared in software) */
    MSSP_SPI_InterruptFlagClear();
    /* Code : To be executed from MCAL Interrupt Context */
    /* Application Callback function gets called every time this ISR executes */
    if(SPI_InterruptHandler){
        SPI_InterruptHandler();
    }
    else{ /* Nothing */ }
}
#endif