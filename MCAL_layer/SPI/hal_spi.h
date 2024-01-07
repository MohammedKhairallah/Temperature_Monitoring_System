/* 
 * File:   hal_spi.h
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on November 11, 2023, 09:26 AM
 */

#ifndef HAL_SPI_H
#define	HAL_SPI_H

/* Section : Includes */
#include"../proc/pic18f4620.h"
#include"../mcal_std_types.h"
#include"../GPIO/hal_gpio.h"
#include"../Interrupt/mcal_internal_interrupt.h"

/* Section : Macro Declarations */

/*
 * @brief  Master Synchronous Serial Port Mode Select.
 * @note   This macros define the value of "spi_mode" in "SPI_t" Structure.
 */
#define SPI_MASTER_FOSC_DIV_4    0   /* SPI Master mode, clock = FOSC/4 */
#define SPI_MASTER_FOSC_DIV_16   1   /* SPI Master mode, clock = FOSC/16 */
#define SPI_MASTER_FOSC_DIV_64   2   /* SPI Master mode, clock = FOSC/64 */
#define SPI_MASTER_FOSC_TMR2     3   /* SPI Master mode, clock = TMR2 output/2 */
#define SPI_SLAVE_SS_ENABLE      4   /* SPI Slave mode, clock = SCK pin, SS pin control enabled */
#define SPI_SLAVE_SS_DISABLE     5   /* SPI Slave mode, clock = SCK pin, SS pin control disabled, SS can be used as I/O pin */
/*
 * @brief  Clock Polarity Select.
 * @note   This Two macros define the value of "ClockPolarity" in "SPI_Control_Config" Structure.
 */
#define SPI_IDLE_STATE_HIGH_LEVEL   1  /* Idle state for clock is a high level */
#define SPI_IDLE_STATE_LOW_LEVEL    0  /* Idle state for clock is a low level */
/*
 * @brief  SPI Sample Select.
 * @note   This Two macros define the value of "SampleSelect" in "SPI_Control_Config" Structure.
 */
#define SPI_DATA_SAMPLE_MIDDLE      0  /* Input data sampled at middle of data output time */
#define SPI_DATA_SAMPLE_END         1  /* Input data sampled at end of data output time */
/*
 * @brief  SPI Clock Select.
 * @note   This Two macros define the value of "ClockSelect" in "SPI_Control_Config" Structure.
 */
#define SPI_TRANSMIT_ACTIVE_TO_IDLE   1  /* Transmit occurs on transition from active to Idle clock state */
#define SPI_TRANSMIT_IDLE_TO_ACTIVE   0  /* Transmit occurs on transition from Idle to active clock state */

/* Receive Overflow Indicator */
#define SPI_SLAVE_RECEIVE_OVERFLOW_DETECTED      1
#define SPI_SLAVE_RECEIVE_OVERFLOW_NOT_DETECTED  0

/* Write Collision Detect */
#define SPI_WRITE_COLLISION_DETECTED             1
#define SPI_WRITE_COLLISION_NOT_DETECTED         0

/* Buffer Full Status bit (Receive mode only) */ 
#define SPI_BUFFER_FULL_RECEIVE_COMPLETE         1
#define SPI_BUFFER_FULL_RECEIVE_NOT_COMPLETE     0

/* Section : Macro Functions Declarations */

/**
 * @brief  Master Synchronous Serial Port Enable bit.
 * @note   SPI_MODULE_ENABLE()  : Enable the SPI Module
 *         SPI_MODULE_DISABLE() : Disable the SPI Module
 */
#define SPI_MODULE_ENABLE()     (SSPCON1bits.SSPEN = 1) 
#define SPI_MODULE_DISABLE()    (SSPCON1bits.SSPEN = 0) 




/* Section : Data type Declarations */

typedef struct{
    uint8_t ClockPolarity : 2;    /* Clock Polarity Select */
    uint8_t SampleSelect  : 2;    /* SPI Sample Select */
    uint8_t ClockSelect   : 2;    /* SPI Clock Select */
    uint8_t Reserved      : 2;
}SPI_Control_Config;

typedef struct{
#if MSSP_SPI_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void (* MSSP_SPI_InterruptHandler)(void);
    Interrupt_Priority_cfg priority;
#endif
    uint8_t spi_mode;
    SPI_Control_Config spi_config;
}SPI_t;


/* Section : Functions Declaration */
Std_ReturnType SPI_Initialize(const SPI_t *_spi);
Std_ReturnType SPI_Deinitialize(const SPI_t *_spi);

Std_ReturnType SPI_Send_Byte_Blocking(const SPI_t *_spi, const uint8_t _data);
Std_ReturnType SPI_Read_Byte_Blocking(const SPI_t *_spi, uint8_t *_data);
Std_ReturnType SPI_Send_Byte_NonBlocking(const SPI_t *_spi, const uint8_t _data);
Std_ReturnType SPI_Read_Byte_NonBlocking(const SPI_t *_spi, uint8_t *_data);

#endif	/* HAL_SPI_H */

