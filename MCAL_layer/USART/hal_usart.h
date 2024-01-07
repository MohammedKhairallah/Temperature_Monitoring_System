/* 
 * File:   hal_usart.h
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on November 2, 2023, 4:23 PM
 */

#ifndef HAL_USART_H
#define	HAL_USART_H

/* Section : Includes */
#include"../proc/pic18f4620.h"
#include"../mcal_std_types.h"
#include"../GPIO/hal_gpio.h"
#include"../Interrupt/mcal_internal_interrupt.h"
#include"hal_usart_cfg.h"

/* Section : Macro Declarations */

/* EUSART Transmit Enable */
#define EUSART_ASYNCHRONOUS_TX_ENABLE    1
#define EUSART_ASYNCHRONOUS_TX_DISABLE   0
/* EUSART Transmit Interrupt Enable */
#define EUSART_ASYNCHRONOUS_INTERRUPT_TX_ENABLE    1
#define EUSART_ASYNCHRONOUS_INTERRUPT_TX_DISABLE   0
/* EUSART 9-Bit Transmit Enable */
#define EUSART_ASYNCHRONOUS_9Bit_TX_ENABLE    1
#define EUSART_ASYNCHRONOUS_9Bit_TX_DISABLE   0
/* EUSART Receiver Enable */
#define EUSART_ASYNCHRONOUS_RX_ENABLE    1
#define EUSART_ASYNCHRONOUS_RX_DISABLE   0
/* EUSART Receiver Interrupt Enable */
#define EUSART_ASYNCHRONOUS_INTERRUPT_RX_ENABLE    1
#define EUSART_ASYNCHRONOUS_INTERRUPT_RX_DISABLE   0
/* EUSART 9-Bit Receiver Enable */
#define EUSART_ASYNCHRONOUS_9Bit_RX_ENABLE    1
#define EUSART_ASYNCHRONOUS_9Bit_RX_DISABLE   0
/* EUSART Framing Error */
#define EUSART_FRAMING_ERROR_DETECTED   1
#define EUSART_FRAMING_ERROR_CLEARED    0
/* EUSART Overrun Error */
#define EUSART_OVERRUN_ERROR_DETECTED   1
#define EUSART_OVERRUN_ERROR_CLEARED    0

/* Section : Macro Functions Declarations */

/**
 * @brief  Serial Port Enable bit.
 * @note   EUSART_MODULE_ENABLE()  : Enable the EUSART module.
 *         EUSART_MODULE_DISABLE() : Disable the EUSART module.
 */
#define EUSART_MODULE_ENABLE()     (RCSTAbits.SPEN = 1)
#define EUSART_MODULE_DISABLE()    (RCSTAbits.SPEN = 0)

/**
 * @brief  Selecting EUSART Working Mode.
 * @note   EUSART_SYNCHRONOUS_MODE_ENABLE()  : Enable EUSART synchronous mode.
 *         EUSART_ASYNCHRONOUS_MODE_ENABLE() : Enable EUSART asynchronous mode.
 */
#define EUSART_SYNCHRONOUS_MODE_ENABLE()     (TXSTAbits.SYNC = 1)
#define EUSART_ASYNCHRONOUS_MODE_ENABLE()    (TXSTAbits.SYNC = 0)
/**
 * @brief  Baud Rate Generator Register Size.
 * @note   EUSART_16BIT_BAUDRATE_GEN_ENABLE()  : Enable 16-bit baud rate generator(SPBRGH and SPBRG).
 *         EUSART_8BIT_BAUDRATE_GEN_ENABLE()   :  Enable 8-bit baud rate generator(SPBRG only (Compatible mode), SPBRGH value ignored).
 */
#define EUSART_16BIT_BAUDRATE_GEN_ENABLE()    (BAUDCONbits.BRG16 = 1) 
#define EUSART_8BIT_BAUDRATE_GEN_ENABLE()     (BAUDCONbits.BRG16 = 0)
/**
 * @brief  Baud Rate Generator Asynchronous Speed Mode.
 * @note   EUSART_ASYNCHRONOUS_HIGH_SPEED_ENABLE()  : Enable high-speed mode for asynchronous communication.
 *         EUSART_ASYNCHRONOUS_LOW_SPEED_ENABLE()   : Enable low-speed mode for asynchronous communication.
 */
#define EUSART_ASYNCHRONOUS_HIGH_SPEED_ENABLE()  (TXSTAbits.BRGH = 1)
#define EUSART_ASYNCHRONOUS_LOW_SPEED_ENABLE()   (TXSTAbits.BRGH = 0)

/* Section : Data type Declarations */

typedef enum{
    BAUDRATE_ASYN_8BIT_lOW_SPEED,
    BAUDRATE_ASYN_8BIT_HIGH_SPEED,
    BAUDRATE_ASYN_16BIT_lOW_SPEED,   
    BAUDRATE_ASYN_16BIT_HIGH_SPEED,
    BAUDRATE_SYN_8BIT,
    BAUDRATE_SYN_16BIT
}baudrate_gen_t;

typedef struct{
    Interrupt_Priority_cfg usart_tx_int_priority;
	uint8_t usart_tx_enable           : 1;
	uint8_t usart_tx_interrupt_enable : 1;
	uint8_t usart_tx_9bit_enable      : 1;
    uint8_t usart_tx_reserved         : 5;
}usart_tx_cfg_t;

typedef struct{
    Interrupt_Priority_cfg usart_rx_int_priority;
	uint8_t usart_rx_enable           : 1;
	uint8_t usart_rx_interrupt_enable : 1;
	uint8_t usart_rx_9bit_enable      : 1;
    uint8_t usart_rx_reserved         : 5;
}usart_rx_cfg_t;

typedef union{
	struct{
		uint8_t usart_tx_reserved : 6;
		uint8_t usart_framing_err : 1;  /* Framing Error */
		uint8_t usart_overrun_err : 1;  /* Overrun Error  */
	};
	uint8_t status;
}usart_error_status_t;

typedef struct{
    uint32_t baudrate;   /* Desired Baud Rate */
    baudrate_gen_t baudrate_gen_gonfig;
    usart_tx_cfg_t usart_tx_cfg;
	usart_rx_cfg_t usart_rx_cfg;
	usart_error_status_t error_status;
	void (*EUSART_TxDefaultInterruptHandler)(void);
    void (*EUSART_RxDefaultInterruptHandler)(void);
    void (*EUSART_FramingErrorHandler)(void);
    void (*EUSART_OverrunErrorHandler)(void);
}usart_t;

/* Section : Functions Declaration */
Std_ReturnType EUSART_ASYNC_Initialize(const usart_t *_eusart);
Std_ReturnType EUSART_ASYNC_Deinitialize(const usart_t *_eusart);

Std_ReturnType EUSART_ASYNC_ReadByteBlocking(uint8_t *_data);
Std_ReturnType EUSART_ASYNC_ReadByteNonBlocking(uint8_t *_data);
Std_ReturnType EUSART_ASYNC_RX_Restart(void);

Std_ReturnType EUSART_ASYNC_WriteByteBlocking(uint8_t _data);
Std_ReturnType EUSART_ASYNC_WriteStringBlocking(uint8_t *_data, uint16_t str_len);
Std_ReturnType EUSART_ASYNC_WriteByteNonBlocking(uint8_t _data);
Std_ReturnType EUSART_ASYNC_WriteStringNonBlocking(uint8_t *_data, uint16_t str_len);

#endif	/* HAL_USART_H */

