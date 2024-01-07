/* 
 * File:   hal_ccp.h
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on October 29, 2023, 9:41 AM
 */

#ifndef HAL_CCP_H
#define	HAL_CCP_H

/* Section : Includes */
#include"../proc/pic18f4620.h"
#include"../mcal_std_types.h"
#include"../GPIO/hal_gpio.h"
#include"../Interrupt/mcal_internal_interrupt.h"
#include"hal_ccp_cfg.h"

/* Section : Macro Declarations */
/**
 * @brief CCP1 Module Mode Select
 * This macros define the value of "ccp_mode_variant" in "ccp_t" Structure.
 */
#define CCP_MODULE_DISABLE                 ((uint8_t)0x00)    /* Disables the CCP module */

#define CCP_CAPTURE_MODE_1_FALLING_EDGE    ((uint8_t)0x04)    /* Capture mode, every falling edge (Mode 1) */
#define CCP_CAPTURE_MODE_1_RISING_EDGE     ((uint8_t)0x05)     /* Capture mode, every rising edge (Mode 2) */
#define CCP_CAPTURE_MODE_4_RISING_EDGE     ((uint8_t)0x06)    /* Capture mode, every 4th rising edge (Mode 3) */
#define CCP_CAPTURE_MODE_16_RISING_EDGE    ((uint8_t)0x07)    /* Capture mode, every 16th rising edge (Mode 4) */

#define CCP_COMPARE_MODE_SET_PIN_LOW       ((uint8_t)0x08)     /* Sets pin low on compare match */
#define CCP_COMPARE_MODE_SET_PIN_HIGH      ((uint8_t)0x09)    /* Sets pin high on compare match */
#define CCP_COMPARE_MODE_TOGGLE_ON_MATCH   ((uint8_t)0x02)    /* Toggles pin on compare match */
#define CCP_COMPARE_MODE_GEN_SW_INTERRUPT  ((uint8_t)0x0A)    /* Generates software interrupt on compare match */
#define CCP_COMPARE_MODE_GEN_EVENT         ((uint8_t)0x0B)    /* Generates event on compare match */

#define CCP_PWM_MODE                       ((uint8_t)0x0C)    /* Uses CCP for Pulse Width Modulation (PWM) */

/**
 * @brief CCP1 Capture Mode State
 */
#define CCP1_CAPTURE_NOT_READY              0X00    /* Capture mode not ready. */
#define CCP1_CAPTURE_READY                  0X01    /* Capture mode ready. */

/**
 * @brief CCP1 Compare Mode State 
 */
#define CCP1_COMPARE_NOT_READY              0X00    /* Compare mode not ready. */
#define CCP1_COMPARE_READY                  0X01    /* Compare mode not ready. */

/**
 * @brief Timer2 Input Clock Post-scaler 
 * This macros define the value of "timer2_postscaler_value" in "ccp_t" Structure.
 */
#define CCP_TIMER2_POSTSCALER_DIV_BY_1       1
#define CCP_TIMER2_POSTSCALER_DIV_BY_2       2
#define CCP_TIMER2_POSTSCALER_DIV_BY_3       3
#define CCP_TIMER2_POSTSCALER_DIV_BY_4       4
#define CCP_TIMER2_POSTSCALER_DIV_BY_5       5
#define CCP_TIMER2_POSTSCALER_DIV_BY_6       6
#define CCP_TIMER2_POSTSCALER_DIV_BY_7       7
#define CCP_TIMER2_POSTSCALER_DIV_BY_8       8
#define CCP_TIMER2_POSTSCALER_DIV_BY_9       9
#define CCP_TIMER2_POSTSCALER_DIV_BY_10      10
#define CCP_TIMER2_POSTSCALER_DIV_BY_11      11
#define CCP_TIMER2_POSTSCALER_DIV_BY_12      12
#define CCP_TIMER2_POSTSCALER_DIV_BY_13      13
#define CCP_TIMER2_POSTSCALER_DIV_BY_14      14
#define CCP_TIMER2_POSTSCALER_DIV_BY_15      15
#define CCP_TIMER2_POSTSCALER_DIV_BY_16      16

/**
 * @brief Timer2 Input Clock Pre-scaler
 * This macros define the value of "timer2_prescaler_value" in "ccp_t" Structure.
 */
#define CCP_TIMER2_PRESCALER_DIV_BY_1        1
#define CCP_TIMER2_PRESCALER_DIV_BY_4        4
#define CCP_TIMER2_PRESCALER_DIV_BY_16       16


/* Section : Macro Functions Declarations */

/* Set the CCP1 Mode Variant */
#define CCP1_SET_MODE(_CONFIG)  (CCP1CONbits.CCP1M = _CONFIG)
#define CCP2_SET_MODE(_CONFIG)  (CCP2CONbits.CCP2M = _CONFIG)

/* Section : Data type Declarations */

/**
 * @brief Define the values to select the CCP1 Main mode
 * This Enumeration define the value of "ccp_mode" in "ccp_t" Structure.
 */
typedef enum{
    CCP_CAPTURE_MODE_SELECTED = 0,   /* Select CCP Capture Mode */
    CCP_COMPARE_MODE_SELECTED,       /* Select CCP Compare Mode */
    CCP_PWM_MODE_SELECTED            /* Select CCP PWM Mode */
}ccp_mode_t;

/*
  @Summary      Defines the values to convert from 16bit to two 8 bit and vice versa
  @Description  Used to get two 8 bit values from 16bit also two 8 bit value are combine to get 16bit.
*/
typedef union{
    struct{
        uint8_t ccpr_low;  /* CCPR1 low register */
        uint8_t ccpr_high;   /* CCPR1 high register */
    };
    struct{
        uint16_t ccpr_16Bit;  /* Read CCPR1 as a 16 Bit value */
    };
}CCP_REG_T;
/**
 * @brief Enumeration for CCP (Capture/Compare/PWM) module selection.
 * This enumeration defines constants to represent different CCP modules
 * for configuration and selection purposes. 
 */
typedef enum{
    CCP1_MODULE = 0,    /* CCP Module 1 selection. */
    CCP2_MODULE         /* CCP Module 2 selection. */
}ccp_module_t;
/**
 * @brief Enumeration for CCP (Capture/Compare/PWM) Timer selection.
 */
typedef enum{
    CCP1_CCP2_TIMER3 = 0,        /* CCP1 and CCP2 with Timer 3 selection. */
    CCP1_TIMER1_CCP2_TIMER3,     /* CCP1 with Timer 1 and CCP2 with Timer 3 selection. */
    CCP1_CCP2_TIMER1             /* CCP1 and CCP2 with Timer 1 selection. */
}ccp_capture_timer_t;

/**
 * @brief CCP Module configurations
 * @Description  This data type used to describe the module initialization configuration
 */
typedef struct{
    ccp_module_t ccp_module;       /* Select CCP1 Module or CCp2 Module */
    ccp_mode_t ccp_mode;           /* CCP main mode(Capture-Compare-PWM) */
    uint8_t ccp_mode_variant;      /* CCP selected mode variant */
    pin_config_t ccp_pin;          /* CCP Pin I/O configurations */
    ccp_capture_timer_t ccp_capture_timer;
#if (CCP1_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED)
    uint32_t PWM_Frequency;      /* CCP PWM mode frequency */
    uint8_t timer2_postscaler_value;
    uint8_t timer2_prescaler_value;
#endif
#if CCP1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void (* CCP1_InterruptHandler)(void);   /* Call back used for all CCP1 Modes */
    Interrupt_Priority_cfg CCP1_priority;   /* Configure the CCP1 mode interrupt */
#endif
#if CCP2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void (* CCP2_InterruptHandler)(void);   /* Call back used for all CCP2 Modes */
    Interrupt_Priority_cfg CCP2_priority;   /* Configure the CCP1 mode interrupt */
#endif
}ccp_t;

/* Section : Functions Declaration */

Std_ReturnType CCP_Initialize(const ccp_t *_ccp_obj);
Std_ReturnType CCP_DeInitialize(const ccp_t *_ccp_obj);

#if (CCP1_CFG_SELECTED_MODE==CCP_CFG_CAPTURE_MODE_SELECTED)
Std_ReturnType CCP1_IsCapturedDataReady(uint8_t *_capture_status);
Std_ReturnType CCP1_Capture_Mode_Read_Value(uint16_t *capture_value);
#endif

#if (CCP1_CFG_SELECTED_MODE==CCP_CFG_COMPARE_MODE_SELECTED)
Std_ReturnType CCP_IsCompareComplete(uint8_t *_compare_status);
Std_ReturnType CCP_Compare_Mode_Set_Value(const ccp_t *_ccp_obj, uint16_t compare_value);
#endif

#if (CCP1_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED)
Std_ReturnType CCP_PWM_Set_Duty(const ccp_t *_ccp_obj, const uint8_t _duty);
Std_ReturnType CCP_PWM_Start(const ccp_t *_ccp_obj);
Std_ReturnType CCP_PWM_Stop(const ccp_t *_ccp_obj);
#endif

#endif	/* HAL_CCP_H */

