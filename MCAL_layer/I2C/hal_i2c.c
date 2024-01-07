/* 
 * File:   hal_i2c.c
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on November 14, 2023, 1:03 PM
 */
#include <pic18f4620.h>

#include"hal_i2c.h"

/* Pointer to function to hold the call backs for I2C Module */
#if MSSP_I2C_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static InterruptHandler I2C_Report_Write_Collision_InterruptHandler = NULL;
static InterruptHandler I2C_DefaultInterruptHandle = NULL;
static InterruptHandler I2C_Report_Receive_Overflow_InterruptHandle = NULL;
#endif

static inline void I2C_Master_Mode_Clock_Configurations(const I2C_t *_i2c);
static inline void I2C_Mode_GPIO_Configurations(void);
static inline void I2C_Interrupt_Configurations(const I2C_t *_i2c);
static inline void I2C_Slave_Mode_Configurations(const I2C_t *_i2c);

/**
 * @Summary Initializes the I2C Module.
 * @Description This routine initializes the I2C Module.
 *              This routine must be called before any other I2C Module routine is called.
 *              This routine should only be called once during system initialization.
 * @Preconditions None
 * @param _i2c : Pointer to the I2C Module configurations.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType I2C_Initialize(const I2C_t *_i2c){
    Std_ReturnType ret = E_OK;
    if(NULL == _i2c)
    {
        ret = E_NOT_OK;
    }else
    {
        /*Disable the I2C Module*/
        I2C_MODULE_DISABLE();
        /* Configure Pins (SDA/SCL)
         * And Select Master or Slave Mode
        **/
        if(_i2c->i2c_mode==I2C_MASTER_MODE_ENABLE){
            I2C_Master_Mode_Clock_Configurations(_i2c);
        }
        else if(_i2c->i2c_mode==I2C_SLAVE_MODE_ENABLE){
            /* I2C Slave Mode General Call Configurations */
            if(I2C_GENERAL_CALL_ENABLE == _i2c->i2c_general_call){
                SSPCON2bits.GCEN = I2C_GENERAL_CALL_ENABLE;
            }
            else if(I2C_GENERAL_CALL_DISABLE == _i2c->i2c_general_call){
                SSPCON2bits.GCEN = I2C_GENERAL_CALL_DISABLE;
            }
            else {/* Nothing */}
            /* Clear the Write Collision Detect */
            SSPCON1bits.WCOL = 0; /* No Collision */
            /* Clear the Receive Overflow Indicator */
            SSPCON1bits.SSPOV = 0; /* No Overflow */
            /* Release the clock */
            SSPCON1bits.CKP = 1;
            /* Assign the I2C Slave Address */
            SSPADD = _i2c->i2c_slave_address;
            /* I2C Slave Mode Configurations */
            I2C_Slave_Mode_Configurations(_i2c);
        }
        else{/* Nothing */}
        /* I2C Mode GPIO Configurations */
        I2C_Mode_GPIO_Configurations();
        /* MSSP I2C Slew Rate Control */
        if(I2C_SLEW_RATE_ENABLE==_i2c->i2c_slew_rate){
            SSPSTATbits.SMP = I2C_SLEW_RATE_ENABLE;
        }
        else if(I2C_SLEW_RATE_DISABLE==_i2c->i2c_slew_rate){
            SSPSTATbits.SMP = I2C_SLEW_RATE_DISABLE;
        }
        else{/* Nothing */}
        /* MSSP I2C SMBus Control */
        if(I2C_SMBus_ENABLE==_i2c->i2c_SMBus_control){
            SSPSTATbits.CKE = I2C_SMBus_ENABLE;
        }
        else if(I2C_SMBus_DISABLE==_i2c->i2c_SMBus_control){
            SSPSTATbits.CKE = I2C_SMBus_DISABLE;
        }
        else{/* Nothing */}
        /* Interrupt Configurations */
        #if MSSP_I2C_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        I2C_Interrupt_Configurations(_i2c);
        #endif
        /*Enable the I2C Module*/
        I2C_MODULE_ENABLE();
    }
    return ret;
}
/**
 * @Summary Deinitialize the I2C Module.
 * @Description This routine is used to deinitialize the I2C module by disabling the module itself 
                and optionally disabling its interrupt feature.
 * @Preconditions I2C_Initialize() function should have been called before calling this function.
 * @param _i2c : Pointer to the I2C Module configurations.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType I2C_Deinitialize(const I2C_t *_i2c){
    Std_ReturnType ret = E_OK;
    if(NULL == _i2c)
    {
        ret = E_NOT_OK;
    }else
    {
        /*Disable the I2C Module*/
        I2C_MODULE_DISABLE();
        /* Interrupt Configurations */
        #if MSSP_I2C_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        MSSP_I2C_InterruptDisable();
        MSSP_I2C_BUS_COL_InterruptDisable();
        #endif
    }
    return ret;
}
/**
 * @Summary 
 * @param _i2c : Pointer to the I2C Module configurations.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType I2C_Master_Send_Start(const I2C_t *_i2c){
    Std_ReturnType ret = E_OK;
    if(NULL == _i2c)
    {
        ret = E_NOT_OK;
    }else
    {
        /* Initiates Start condition on SDA and SCL pins */
        SSPCON2bits.SEN = 1; /* Automatically cleared by hardware. */
        /* Wait for the completion of the Start condition */
        while(SSPCON2bits.SEN);
        /* Clear The MSSP Interrupt Flag bit -> SSPIF */
        PIR1bits.SSPIF = 0;
        /* Report The Start Condition Detection */
        if(I2C_START_BIT_DETECTED==SSPSTATbits.S){
            ret = E_OK; /* Indicates that a Start bit has been detected last */
        }
        else if(I2C_START_BIT_NOT_DETECTED==SSPSTATbits.S){
            ret = E_NOT_OK; /* Start bit was not detected last */
        }
        else{/* Nothing */} 
    }
    return ret;
}
/**
 * @Summary 
 * @param _i2c : Pointer to the I2C Module configurations.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType I2C_Master_Send_Repeated_Start(const I2C_t *_i2c){
    Std_ReturnType ret = E_OK;
    if(NULL == _i2c)
    {
        ret = E_NOT_OK;
    }else
    {
        /* Initiates Repeated Start condition on SDA and SCL pins */
        SSPCON2bits.RSEN = 1;   /* Automatically cleared by hardware. */
        /* Wait for the completion of the Repeated Start condition */
        while(SSPCON2bits.RSEN);
        /* Clear The MSSP Interrupt Flag bit -> SSPIF */
        PIR1bits.SSPIF = 0;
    }
    return ret;
}
/**
 * @Summary 
 * @param _i2c : Pointer to the I2C Module configurations.
 * @return status of function
 *         (E_OK)      : The function done successfully.
 *         (E_NOT_OK)  : The function has issue to perform this action.
 */
Std_ReturnType I2C_Master_Send_Stop(const I2C_t *_i2c){
    Std_ReturnType ret = E_OK;
    if(NULL == _i2c)
    {
        ret = E_NOT_OK;
    }else
    {
        /* Initiates Stop condition on SDA and SCL pins */
        SSPCON2bits.PEN = 1;   /* Automatically cleared by hardware. */
        /* Wait for the completion of the Stop condition */
        while(SSPCON2bits.PEN);
        /* Clear The MSSP Interrupt Flag bit -> SSPIF */
        PIR1bits.SSPIF = 0;
        /* Report The Stop Condition Detection */
        if(I2C_STOP_BIT_DETECTED==SSPSTATbits.P){
            ret = E_OK; /* Indicates that a Stop bit has been detected last */
        }
        else if(I2C_STOP_BIT_NOT_DETECTED==SSPSTATbits.P){
            ret = E_NOT_OK; /* Stop bit was not detected last */
        }
        else{/* Nothing */} 
    }
    return ret;
}

Std_ReturnType I2C_Master_Write_Blocking(const I2C_t *_i2c, uint8_t i2c_data, uint8_t *_ack){
    Std_ReturnType ret = E_OK;
    if((NULL == _i2c)||(NULL == _ack))
    {
        ret = E_NOT_OK;
    }else
    {
        /* Write Data to the Data register */
        SSPBUF = i2c_data;
        /* Wait The transmission to be completed */
        while(SSPSTATbits.BF);
        /* Clear The MSSP Interrupt Flag bit -> SSPIF */
        PIR1bits.SSPIF = 0;
        /* Report the acknowledge received from the slave */
        if(I2C_ACK_RECEIVED_FROM_SLAVE == SSPCON2bits.ACKSTAT){
            *_ack = I2C_ACK_RECEIVED_FROM_SLAVE; /* Acknowledge was received from slave */
        }
        else if(I2C_ACK_NOT_RECEIVED_FROM_SLAVE == SSPCON2bits.ACKSTAT){
            *_ack = I2C_ACK_NOT_RECEIVED_FROM_SLAVE;  /* Acknowledge was not received from slave */
        }
        else{/* Nothing */}
    }
    return ret;
}

Std_ReturnType I2C_Master_Read_Blocking(const I2C_t *_i2c, uint8_t ack, uint8_t *i2c_data){
    Std_ReturnType ret = E_OK;
    if((NULL == _i2c)||(NULL == i2c_data))
    {
        ret = E_NOT_OK;
    }else
    {
        /* Master Mode Receive Enable */
        SSPCON2bits.RCEN = 1;
        /* Wait for buffer full flag : A complete byte received */
        while(!(SSPSTATbits.BF));
        /* Copy The data registers to buffer variable */
        *i2c_data = SSPBUF;
        /* Send ACK or NACK after read */
        if(I2C_MASTER_SEND_ACK == ack){
            SSPCON2bits.ACKDT = I2C_MASTER_SEND_ACK;
            /* Initiates Acknowledge sequence on SDA and SCL pins and transmit ACKDT data bit. */
            SSPCON2bits.ACKEN = 1;  /* Automatically cleared by hardware. */
            while(SSPCON2bits.ACKEN);
        }
        else if(I2C_MASTER_SEND_NACK == ack){
            SSPCON2bits.ACKDT = I2C_MASTER_SEND_NACK;
            /* Initiates Acknowledge sequence on SDA and SCL pins and transmit ACKDT data bit. */
            SSPCON2bits.ACKEN = 1;  /* Automatically cleared by hardware. */
            while(SSPCON2bits.ACKEN);
        }
        else{/* Nothing */}
    }
    return ret;
}

Std_ReturnType I2C_Master_Write_NOnBlocking(const I2C_t *_i2c, uint8_t i2c_data, uint8_t *_ack){
    Std_ReturnType ret = E_OK;
    if((NULL == _i2c)||(NULL == _ack))
    {
        ret = E_NOT_OK;
    }else
    {
    }
    return ret;
}
Std_ReturnType I2C_Master_Read_NonBlocking(const I2C_t *_i2c, uint8_t ack, uint8_t *i2c_data){
    Std_ReturnType ret = E_OK;
    if((NULL == _i2c)||(NULL == i2c_data))
    {
        ret = E_NOT_OK;
    }else
    {
    }
    return ret;
}


static inline void I2C_Master_Mode_Clock_Configurations(const I2C_t *_i2c){
    /*Select the I2C Mode*/
    SSPCON1bits.SSPM = _i2c->i2c_mode_cfg;
    /*  I2C Master mode, clock = FOSC/(4 * (SSPADD + 1)) */
    SSPADD = (uint8_t)(((_XTAL_FREQ / 4) / _i2c->i2c_clock) - 1);
}


static inline void I2C_Mode_GPIO_Configurations(void){
    TRISCbits.TRISC3 = 1;   /* Serial clock (SCL) is Input */
    TRISCbits.TRISC4 = 1;   /* Serial data (SDA) is Input */
}

#if MSSP_I2C_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static inline void I2C_Interrupt_Configurations(const I2C_t *_i2c){
    /* Interrupt Configurations */
    MSSP_I2C_InterruptEnable();
    MSSP_I2C_BUS_COL_InterruptEnable();
    MSSP_I2C_InterruptFlagClear();
    MSSP_I2C_BUS_COL_InterruptFlagClear();
    /* Initialize The Callback */
    I2C_Report_Write_Collision_InterruptHandler = _i2c->I2C_Report_Write_Collision;
    I2C_DefaultInterruptHandle = _i2c->I2C_DefaultInterruptHandler;
    I2C_Report_Receive_Overflow_InterruptHandle = _i2c->I2C_Report_Receive_Overflow;
    /* Interrupt Priority Configurations */
    #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
    INTERRUPT_PriorityLevelsEnable();
    if(INTERRUPT_HIGH_PRIORITY == _i2c->i2c_priority){
        /* Enables all high-priority interrupts */
        INTERRUPT_GlobalInterruptHighEnable();
        MSSP_I2C_HighPrioritySet();
    }
    else if(INTERRUPT_LOW_PRIORITY == _i2c->i2c_priority){
        /* Enables all unmasked peripheral interrupts */
        INTERRUPT_GlobalInterruptLowEnable();
        MSSP_I2C_LowPrioritySet();
    }
    else{ /* Nothing */ }
    
    if(INTERRUPT_HIGH_PRIORITY == _i2c->i2c_bc_priority){
        /* Enables all high-priority interrupts */
        INTERRUPT_GlobalInterruptHighEnable();
        MSSP_I2C_BUS_COL_HighPrioritySet();
    }
    else if(INTERRUPT_LOW_PRIORITY == _i2c->i2c_bc_priority){
        /* Enables all unmasked peripheral interrupts */
        INTERRUPT_GlobalInterruptLowEnable();
        MSSP_I2C_BUS_COL_LowPrioritySet();
    }
    else{ /* Nothing */ }
    #else
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    #endif  
}
#endif

static inline void I2C_Slave_Mode_Configurations(const I2C_t *_i2c){
    SSPCON1bits.SSPM = _i2c->i2c_mode_cfg;
}

/**
 * @brief MSSP_I2C_ISR - I2C Interrupt Service Routine (ISR)
 */
#if MSSP_I2C_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
void MSSP_I2C_ISR(void){
    /* The I2C internal interrupt occurred (must be cleared in software) */
    MSSP_I2C_InterruptFlagClear();
    /* Code : To be executed from MCAL Interrupt Context */
    /* Application Callback function gets called every time this ISR executes */
    if(I2C_DefaultInterruptHandle){
        I2C_DefaultInterruptHandle();
    }
    else{ /* Nothing */ }
}
#endif

/**
 * @brief MSSP_I2C_BC_ISR - I2C Bus Collision Interrupt Service Routine (ISR)
 */
#if MSSP_I2C_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
void MSSP_I2C_BC_ISR(void){
    /* The I2C internal interrupt occurred (must be cleared in software) */
    MSSP_I2C_BUS_COL_InterruptFlagClear();
    /* Code : To be executed from MCAL Interrupt Context */
    /* Application Callback function gets called every time this ISR executes */
    if(I2C_Report_Write_Collision_InterruptHandler){
        I2C_Report_Write_Collision_InterruptHandler();
    }
    else{ /* Nothing */ }
}
#endif