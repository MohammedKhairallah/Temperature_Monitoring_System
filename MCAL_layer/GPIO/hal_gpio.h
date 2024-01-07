/* 
 * File:   hal_gpio.h
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on August 20, 2023, 1:46 PM
 */

#ifndef HAL_GPIO_H
#define	HAL_GPIO_H

/* Section : Includes */
#include"../device_config.h"
#include"../mcal_std_types.h"
#include"../proc/pic18f4620.h"
#include"hal_gpio_cfg.h"
/* Section : Macro Declarations */
#define MASK_BIT    ((uint8_t)1)

#define PORT_PIN_MAX_NUMBER  8
#define PORT_MAX_NUMBER      5

#define PORT_MASK          0xFF

/* Section : Macro Functions Declarations */
#define HWREG8(_X)        (*((volatile uint8_t *)(_X)))

#define SET_BIT(REG,BIT_POSN)        (REG |= (MASK_BIT<<BIT_POSN))
#define CLEAR_BIT(REG,BIT_POSN)      (REG &= ~(MASK_BIT<<BIT_POSN))
#define TOGGLE_BIT(REG,BIT_POSN)     (REG ^= (MASK_BIT<<BIT_POSN))
#define READ_BIT(REG,BIT_POSN)       ((REG >>BIT_POSN) & MASK_BIT)

/* Section : Data type Declarations */
typedef enum{
    GPIO_LOW = 0,
    GPIO_HIGH
}logic_t;

typedef enum{
    GPIO_direction_OUTPUT = 0,
    GPIO_direction_INPUT
}direction_t;

typedef enum{
    GPIO_PIN0 = 0,
    GPIO_PIN1,
    GPIO_PIN2,
    GPIO_PIN3,
    GPIO_PIN4,
    GPIO_PIN5,
    GPIO_PIN6,
    GPIO_PIN7
}pin_index_t;

typedef enum{
    PORTA_INDEX = 0,
    PORTB_INDEX,
    PORTC_INDEX,
    PORTD_INDEX,
    PORTE_INDEX       
}port_index_t;

typedef struct{
    uint8_t port         :3;    /* @ref : port_index_t */
    uint8_t pin          :3;    /* @ref : pin_index_t */
    uint8_t direction    :1;    /* @ref : direction_t */
    uint8_t logic        :1;    /* @ref : logic_t */
}pin_config_t;

/* Section : Functions Declaration */
Std_ReturnType gpio_pin_direction_intialize(const pin_config_t *_pin_config);
Std_ReturnType gpio_pin_get_direction_status(const pin_config_t *_pin_config, direction_t *dic_status);
Std_ReturnType gpio_pin_write_logic(const pin_config_t *_pin_config, logic_t logic);
Std_ReturnType gpio_pin_read_logic(const pin_config_t *_pin_config, logic_t *logic);
Std_ReturnType gpio_pin_toggle_logic(const pin_config_t *_pin_config);
Std_ReturnType gpio_pin_intialize(const pin_config_t *_pin_config);

Std_ReturnType gpio_port_direction_intialize(port_index_t port, uint8_t direction);
Std_ReturnType gpio_port_get_direction_intialize(port_index_t port, uint8_t *dic_status);
Std_ReturnType gpio_port_write_logic(port_index_t port, uint8_t logic);
Std_ReturnType gpio_port_read_logic(port_index_t port, uint8_t *logic);
Std_ReturnType gpio_port_toggle_logic(port_index_t port);

#endif	/* HAL_GPIO_H */

