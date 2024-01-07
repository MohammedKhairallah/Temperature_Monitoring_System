/* 
 * File:   mcal_layer_init.h
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on October 21, 2023, 9:26 AM
 */

#ifndef MCAL_LAYER_INIT_H
#define	MCAL_LAYER_INIT_H

/* Section : Includes */
#include"Interrupt/mcal_external_interrupt.h"
#include"EEPROM/hal_eeprom.h"
#include"ADC/hal_adc.h"
#include"Timer0/hal_timer0.h"
#include"Timer1/hal_timer1.h"
#include"Timer2/hal_timer2.h"
#include"Timer3/hal_timer3.h"
#include"CCP/hal_ccp.h"
#include"USART/hal_usart.h"
#include"SPI/hal_spi.h"
#include"I2C/hal_i2c.h"

/* Section : Macro Declarations */


/* Section : Macro Functions Declarations */


/* Section : Data type Declarations */

extern adc_conf_t ADC1;

/* Section : Functions Declaration */
void mcal_layer_intialize(void);

//void EUSART_Rx_DefaultInterruptHandler(void);
void MSSP_I2C_DefaultInterruptHandler(void);
#endif	/* MCAL_LAYER_INIT_H */

