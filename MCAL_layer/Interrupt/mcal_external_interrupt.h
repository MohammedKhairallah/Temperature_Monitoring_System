/* 
 * File:   mcal_external_interrupt.h
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on October 12, 2023, 8:10 AM
 */

#ifndef MCAL_EXTERNAL_INTERRUPT_H
#define	MCAL_EXTERNAL_INTERRUPT_H

/* Section : Includes */
#include"mcal_interrupt_cfg.h"

/* Section : Macro Declarations */


/* Section : Macro Functions Declarations */

#if EXTERNAL_INTERRUPT_INTx_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
/* This routine clears the interrupt enable for the external interrupt, INT0 */
#define EXT_INT0_InterruptDisable()        (INTCONbits.INT0IE = 0)
/* This routine sets the interrupt enable for the external interrupt, INT0 */
#define EXT_INT0_InterruptEnable()         (INTCONbits.INT0IE = 1)
/* This routine clears the interrupt flag for the external interrupt, INT0 */
#define EXT_INT0_InterruptFlagClear()      (INTCONbits.INT0IF = 0)
/* This routine set the edge detect of the extern interrupt to negative edge */
#define EXT_INT0_RisingEdgeSet()           (INTCON2bits.INTEDG0 = 1)
/* This routine set the edge detect of the extern interrupt to positive edge */
#define EXT_INT0_FallingEdgeSet()          (INTCON2bits.INTEDG0 = 0)

/* This routine clears the interrupt enable for the external interrupt, INT1 */
#define EXT_INT1_InterruptDisable()        (INTCON3bits.INT1IE = 0)
/* This routine sets the interrupt enable for the external interrupt, INT1 */
#define EXT_INT1_InterruptEnable()         (INTCON3bits.INT1IE = 1)
/* This routine clears the interrupt flag for the external interrupt, INT1 */
#define EXT_INT1_InterruptFlagClear()      (INTCON3bits.INT1IF = 0)
/* This routine set the edge detect of the extern interrupt to negative edge */
#define EXT_INT1_RisingEdgeSet()           (INTCON2bits.INTEDG1 = 1)
/* This routine set the edge detect of the extern interrupt to positive edge */
#define EXT_INT1_FallingEdgeSet()          (INTCON2bits.INTEDG1 = 0)

/* This routine clears the interrupt enable for the external interrupt, INT2 */
#define EXT_INT2_InterruptDisable()        (INTCON3bits.INT2IE = 0)
/* This routine sets the interrupt enable for the external interrupt, INT2 */
#define EXT_INT2_InterruptEnable()         (INTCON3bits.INT2IE = 1)
/* This routine clears the interrupt flag for the external interrupt, INT2 */
#define EXT_INT2_InterruptFlagClear()      (INTCON3bits.INT2IF = 0)
/* This routine set the edge detect of the extern interrupt to negative edge */
#define EXT_INT2_RisingEdgeSet()           (INTCON2bits.INTEDG2 = 1)
/* This routine set the edge detect of the extern interrupt to positive edge */
#define EXT_INT2_FallingEdgeSet()          (INTCON2bits.INTEDG2 = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
/* INT0 always High Priority */
/* This routine set the INT1 External Interrupt Priority to be High priority */
#define EXT_INT1_HighPrioritySet()         (INTCON3bits.INT1IP = 1)
/* This routine set the INT1 External Interrupt Priority to be Low priority */
#define EXT_INT1_LowPrioritySet()          (INTCON3bits.INT1IP = 0)
/* This routine set the INT2 External Interrupt Priority to be High priority */
#define EXT_INT2_HighPrioritySet()         (INTCON3bits.INT2IP = 1)
/* This routine set the INT2 External Interrupt Priority to be Low priority */
#define EXT_INT2_LowPrioritySet()          (INTCON3bits.INT2IP = 0)
#endif

#endif

#if EXTERNAL_INTERRUPT_Onchange_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
/* This routine clears the interrupt disable for the external interrupt, RBx */
#define EXT_RBx_InterruptDisable()          (INTCONbits.RBIE = 0)
/* This routine sets the interrupt enable for the external interrupt, RBx */
#define EXT_RBx_InterruptEnable()           (INTCONbits.RBIE = 1)
/* This routine clears the interrupt flag for the external interrupt, RBx */
#define EXT_RBx_InterruptFlagClear()        (INTCONbits.RBIF = 0) 

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
/* This routine set the RBx External Interrupt Priority to be High priority */
#define EXT_RBx_Priority_High()             (INTCON2bits.RBIP = 1)
/* This routine set the RBx External Interrupt Priority to be Low priority */
#define EXT_RBx_Priority_Low()              (INTCON2bits.RBIP = 0)
#endif

#endif

/* Section : Data type Declarations */

typedef enum{
    INTERRUPT_FALLING_EDGE = 0,
    INTERRUPT_RISING_EDGE 
}Interrupt_INTx_edge;

typedef enum{
    INTERRUPT_EXTERNAL_INT0 = 0,
    INTERRUPT_EXTERNAL_INT1 ,
    INTERRUPT_EXTERNAL_INT2  
}Interrupt_INTx_src;

typedef struct{
    void (* EXT_InterruptHandler)(void);
    pin_config_t mcu_pin;
    Interrupt_INTx_edge edge;
    Interrupt_INTx_src source;
    Interrupt_Priority_cfg Priority;
}Interrupt_INTx_t;

typedef struct{
    void (* EXT_InterruptHandler_HIGH)(void);
    void (* EXT_InterruptHandler_LOW)(void);
    pin_config_t mcu_pin;
    Interrupt_Priority_cfg Priority;
}Interrupt_RBx_t;
/* Section : Functions Declaration */
Std_ReturnType Interrupt_INTx_initialize(const Interrupt_INTx_t *INTx_obj);
Std_ReturnType Interrupt_INTx_deinitialize(const Interrupt_INTx_t *INTx_obj);  
Std_ReturnType Interrupt_RBx_initialize(const Interrupt_RBx_t *RBx_obj);
Std_ReturnType Interrupt_RBx_deinitialize(const Interrupt_RBx_t *RBx_obj);       
#endif	/* MCAL_EXTERNAL_INTERRUPT_H */

