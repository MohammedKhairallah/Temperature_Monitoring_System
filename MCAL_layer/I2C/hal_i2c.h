/* 
 * File:   hal_i2c.h
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on November 14, 2023, 1:03 PM
 */

#ifndef HAL_I2C_H
#define	HAL_I2C_H

/* Section : Includes */
#include"../proc/pic18f4620.h"
#include"../mcal_std_types.h"
#include"../GPIO/hal_gpio.h"
#include"../Interrupt/mcal_internal_interrupt.h"

/* Section : Macro Declarations */ 
/*
 * @brief  Master Synchronous Serial Port Mode Select bits.
 * @note   This macros define the value of "i2c_mode_cfg" in "I2C_t" Structure.
 */
#define I2C_SLAVE_MODE_7_BIT_ADDRESS     0X06U
#define I2C_SLAVE_MODE_10_BIT_ADDRESS    0X07U

#define I2C_SLAVE_MODE_7_BIT_ADDRESS_SS_INT_ENABLE     0X0EU
#define I2C_SLAVE_MODE_10_BIT_ADDRESS_SS_INT_ENABLE    0X0FU

#define I2C_MASTER_MODE_DEFINED_CLOCK           0X08U
#define I2C_MASTER_MODE_FIRMWARE_CONTROLLED     0X0BU
/*
 * @brief  Slew Rate Control bit.
 * @note   This macros define the value of "i2c_slew_rate" in "I2C_t" Structure.
 */
#define I2C_SLEW_RATE_DISABLE   1
#define I2C_SLEW_RATE_ENABLE    0
/*
 * @brief  General Call Enable bit (Slave mode only).
 * @note   This macros define the value of "i2c_general_call" in "I2C_t" Structure.
 */
#define I2C_GENERAL_CALL_ENABLE    1
#define I2C_GENERAL_CALL_DISABLE   0
/*
 * @brief  Select Master or Slave Mode.
 * @note   This two macros define the value of "i2c_mode" in "I2C_t" Structure.
 */
#define I2C_MASTER_MODE_ENABLE     1
#define I2C_SLAVE_MODE_ENABLE      0
/*
 * @brief  SMBus Select bit.
 * @note   This two macros define the value of "i2c_SMBus_control" in "I2C_t" Structure.
 */
#define I2C_SMBus_ENABLE      1
#define I2C_SMBus_DISABLE     0
/* Start Condition Indication */
#define I2C_START_BIT_DETECTED       1
#define I2C_START_BIT_NOT_DETECTED   0
/* Stop Condition Indication */
#define I2C_STOP_BIT_DETECTED        1
#define I2C_STOP_BIT_NOT_DETECTED    0
/* Acknowledge Status bit (Master Transmit mode only) */
#define I2C_ACK_RECEIVED_FROM_SLAVE       0
#define I2C_ACK_NOT_RECEIVED_FROM_SLAVE   1
/* Acknowledge Data bit (Master Receive mode only) */
#define I2C_MASTER_SEND_ACK     0
#define I2C_MASTER_SEND_NACK    1

/* Section : Macro Functions Declarations */

/**
 * @brief  Master Synchronous Serial Port Enable bit.
 * @note   I2C_MODULE_ENABLE()  : Enable the I2C Module
 *         I2C_MODULE_DISABLE() : Disable the I2C Module
 */
#define I2C_MODULE_ENABLE()     (SSPCON1bits.SSPEN = 1) 
#define I2C_MODULE_DISABLE()    (SSPCON1bits.SSPEN = 0) 


/* Section : Data type Declarations */

typedef struct{
    #if MSSP_I2C_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void (*I2C_Report_Write_Collision)(void);	/* Write Collision Indicator */
    void (*I2C_DefaultInterruptHandler)(void); 	/* Default Interrupt Handler */
    void (*I2C_Report_Receive_Overflow)(void); 	/* Receive Overflow Indicator */
    Interrupt_Priority_cfg i2c_priority;
    Interrupt_Priority_cfg i2c_bc_priority;
    #endif 
    uint32_t i2c_clock;                     /* Master Clock Frequency */
    uint8_t i2c_mode_cfg;                   /* Master Synchronous Serial Port Mode Select */
    uint8_t i2c_slave_address;              /* I2C Slave Address */
    uint8_t i2c_mode                : 1;    /* I2C : Master or Slave Mode */
    uint8_t i2c_slew_rate           : 1; 	/* Slew Rate Enable/Disable */
	uint8_t i2c_SMBus_control       : 1; 	/* SMBus Enable/Disable */
	uint8_t i2c_general_call        : 1;    /* General Call Enable/Disable */
	uint8_t i2c_master_receive_mode : 1; 	/* Master Receive Mode Enable/Disable */ 
    uint8_t i2c_reserved            : 3;
}I2C_t;

/* Section : Functions Declaration */
Std_ReturnType I2C_Initialize(const I2C_t *_i2c);
Std_ReturnType I2C_Deinitialize(const I2C_t *_i2c);

Std_ReturnType I2C_Master_Send_Start(const I2C_t *_i2c);
Std_ReturnType I2C_Master_Send_Repeated_Start(const I2C_t *_i2c);
Std_ReturnType I2C_Master_Send_Stop(const I2C_t *_i2c);

Std_ReturnType I2C_Master_Write_Blocking(const I2C_t *_i2c, uint8_t i2c_data, uint8_t *_ack);
Std_ReturnType I2C_Master_Read_Blocking(const I2C_t *_i2c, uint8_t ack, uint8_t *i2c_data);

Std_ReturnType I2C_Master_Write_NOnBlocking(const I2C_t *_i2c, uint8_t i2c_data, uint8_t *_ack);
Std_ReturnType I2C_Master_Read_NonBlocking(const I2C_t *_i2c, uint8_t ack, uint8_t *i2c_data);

#endif	/* HAL_I2C_H */
