/* 
 * File:   mcal_interrupt_manager.h
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on October 12, 2023, 8:11 AM
 */

#ifndef MCAL_INTERRUPT_MANAGER_H
#define	MCAL_INTERRUPT_MANAGER_H

/* Section : Includes */
#include"mcal_interrupt_cfg.h"

/* Section : Macro Declarations */


/* Section : Macro Functions Declarations */


/* Section : Data type Declarations */


/* Section : Functions Declaration */
void INT0_ISR(void);
void INT1_ISR(void);
void INT2_ISR(void);

void RB4_ISR(uint8_t RB4_Source);
void RB5_ISR(uint8_t RB5_Source);
void RB6_ISR(uint8_t RB6_Source);
void RB7_ISR(uint8_t RB7_Source);
/**
 * @brief ADC_ISR - ADC Interrupt Service Routine (ISR)
 * @Description This function is the interrupt handler for the ADC interrupt. It performs the following actions:
 *              - Clears the ADC interrupt flag.
 *              - Checks if a callback function (ADC_InterruptHandler) is registered and calls it.
 */
void ADC_ISR(void);
/**
 * @brief TIMER0_ISR - Timer0 Interrupt Service Routine (ISR)
 * @Description This function is the interrupt handler for Timer0 interrupts. 
 *              It is called when a Timer0 interrupt occurs.It performs the following actions:
 *              - Clearing the interrupt flag.
 *              - updating Timer0 module values.
 *              - Checks if a callback function (Timer0_InterruptHandler) is registered and calls it.
 */
void TIMER0_ISR(void);
/**
 * @brief TIMER1_ISR - Timer1 Interrupt Service Routine (ISR)
 * @Description This function is the interrupt handler for Timer1 interrupts. 
 *              It is called when a Timer1 interrupt occurs.It performs the following actions:
 *              - Clearing the interrupt flag.
 *              - updating Timer1 module values.
 *              - Checks if a callback function (Timer1_InterruptHandler) is registered and calls it.
 */
void TIMER1_ISR(void);
/**
 * @brief TIMER2_ISR - Timer2 Interrupt Service Routine (ISR)
 * @Description This function is the interrupt handler for Timer2 interrupts. 
 *              It is called when a Timer2 interrupt occurs.It performs the following actions:
 *              - Clearing the interrupt flag.
 *              - updating Timer2 module values.
 *              - Checks if a callback function (Timer2_InterruptHandler) is registered and calls it.
 */
void TIMER2_ISR(void);
/**
 * @brief TIMER3_ISR - Timer3 Interrupt Service Routine (ISR)
 * @Description This function is the interrupt handler for Timer3 interrupts. 
 *              It is called when a Timer3 interrupt occurs.It performs the following actions:
 *              - Clearing the interrupt flag.
 *              - updating Timer3 module values.
 *              - Checks if a callback function (Timer2_InterruptHandler) is registered and calls it.
 */
void TIMER3_ISR(void);
/**
 * @brief CCP1_ISR - CCP1 Interrupt Service Routine (ISR)
 * @Description This function is the interrupt handler for CCP1 interrupts. 
 *              It is called when a CCP1 interrupt occurs.It performs the following actions:
 *              - Clearing the interrupt flag.
 *              - Checks if a callback function (CCP1_InterruptHandler) is registered and calls it.
 */
void CCP1_ISR(void);
/**
 * @brief CCP2_ISR - CCP2 Interrupt Service Routine (ISR)
 * @Description This function is the interrupt handler for CCP1 interrupts. 
 *              It is called when a CCP2 interrupt occurs.It performs the following actions:
 *              - Clearing the interrupt flag.
 *              - Checks if a callback function (CCP2_InterruptHandler) is registered and calls it.
 */
void CCP2_ISR(void);

void EUSART_TX_ISR(void);
void EUSART_RX_ISR(void);

void MSSP_SPI_ISR(void);
void MSSP_I2C_ISR(void);
void MSSP_I2C_BC_ISR(void);


#endif	/* MCAL_INTERRUPT_MANAGER_H */

