/* 
 * File:   hal_gpio.c
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on August 20, 2023, 1:46 PM
 */

#include "hal_gpio.h"

/* Reference to the Data Direction Control Register */
volatile uint8_t *tris_registers[] = {&TRISA,&TRISB,&TRISC,&TRISD,&TRISE};
/* Reference to the Data Late Register (Read and Write to Data Late)*/
volatile uint8_t *lat_registers[] = {&LATA,&LATB,&LATC,&LATD,&LATE};
/* Reference to the Port Status Register */
volatile uint8_t *port_registers[] = {&PORTA,&PORTB,&PORTC,&PORTD,&PORTE};

/**
 * @brief  initialize the direction of specific pin  @ref : direction_t
 * @param _pin_config  : pointer to the configurations   @ref : pin_config_t
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
#if GPIO_PORT_PIN_CONFIGURATIONS == CONFIG_ENABLE
Std_ReturnType gpio_pin_direction_intialize(const pin_config_t *_pin_config){
    Std_ReturnType ret =E_OK;
    if((NULL == _pin_config)||(_pin_config->pin > PORT_PIN_MAX_NUMBER-1))
    {
        ret = E_NOT_OK;
    }else
    {
        switch(_pin_config->direction){
            case(GPIO_direction_OUTPUT):
                CLEAR_BIT(*tris_registers[_pin_config->port], _pin_config->pin);
                break;
            case(GPIO_direction_INPUT):
                SET_BIT(*tris_registers[_pin_config->port], _pin_config->pin);
                break;   
            default : ret = E_NOT_OK;
        }
    }
    return ret;
}
#endif
/**
 * 
 * @param _pin_config  : pointer to the configurations   @ref : port_index_t
 * @param dic_status : pointer points to the direction state  @ref : direction_t
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
#if GPIO_PORT_PIN_CONFIGURATIONS == CONFIG_ENABLE
Std_ReturnType gpio_pin_get_direction_status(const pin_config_t *_pin_config, direction_t *dic_status){
    Std_ReturnType ret =E_OK;
    if((NULL==_pin_config)||(NULL==dic_status)||(_pin_config->pin > PORT_PIN_MAX_NUMBER-1))
    {
        ret = E_NOT_OK;
    }else
    {
        *dic_status = READ_BIT(*tris_registers[_pin_config->port], _pin_config->pin);            
    }
    return ret;
}
#endif
/**
 * 
 * @param _pin_config  : pointer to the configurations   @ref : port_index_t
 * @param logic : pointer points to the logic state  @ref : logic_t
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
#if GPIO_PORT_PIN_CONFIGURATIONS == CONFIG_ENABLE
Std_ReturnType gpio_pin_write_logic(const pin_config_t *_pin_config, logic_t logic){
    Std_ReturnType ret =E_OK;
    if((NULL == _pin_config)||(_pin_config->pin > PORT_PIN_MAX_NUMBER-1))
    {
        ret = E_NOT_OK;
    }else
    {
        switch(logic){
            case(GPIO_LOW):
                CLEAR_BIT(*lat_registers[_pin_config->port], _pin_config->pin);
                break;
            case(GPIO_HIGH):
                SET_BIT(*lat_registers[_pin_config->port], _pin_config->pin);
                break;   
            default : ret = E_NOT_OK;
        }
    }
    return ret;
}
#endif
/**
 * 
 * @param _pin_config  : pointer to the configurations   @ref : port_index_t
 * @param logic : pointer points to the logic state  @ref : logic_t
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
#if GPIO_PORT_PIN_CONFIGURATIONS == CONFIG_ENABLE
Std_ReturnType gpio_pin_read_logic(const pin_config_t *_pin_config, logic_t *logic){
    Std_ReturnType ret =E_OK;
    if((NULL==_pin_config)||(NULL==logic)||(_pin_config->pin > PORT_PIN_MAX_NUMBER-1))
    {
        ret = E_NOT_OK;
    }else
    {
        *logic = READ_BIT(*port_registers[_pin_config->port], _pin_config->pin);
    }
    return ret;
}
#endif
/**
 * 
 * @param _pin_config  : pointer to the configurations   @ref : port_index_t
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
#if GPIO_PORT_PIN_CONFIGURATIONS == CONFIG_ENABLE
Std_ReturnType gpio_pin_toggle_logic(const pin_config_t *_pin_config){
    Std_ReturnType ret =E_OK;
    if((NULL == _pin_config)||(_pin_config->pin > PORT_PIN_MAX_NUMBER-1))
    {
        ret = E_NOT_OK;
    }else
    {
        ret = TOGGLE_BIT(*lat_registers[_pin_config->port], _pin_config->pin);
    }
    return ret;
}
#endif
/**
 * 
 * @param _pin_config  : pointer to the configurations   @ref : port_index_t
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
#if GPIO_PORT_PIN_CONFIGURATIONS == CONFIG_ENABLE
Std_ReturnType gpio_pin_intialize(const pin_config_t *_pin_config){
    Std_ReturnType ret =E_OK;
    if((NULL == _pin_config)||(_pin_config->pin > PORT_PIN_MAX_NUMBER-1))
    {
        ret = E_NOT_OK;
    }else
    {
        ret = gpio_pin_direction_intialize(_pin_config);
        ret = gpio_pin_write_logic(_pin_config, _pin_config->logic);
    }
    return ret;
}
#endif
/**
 * 
 * @param port
 * @param direction : direction state 
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
#if GPIO_PORT_CONFIGURATIONS == CONFIG_ENABLE
Std_ReturnType gpio_port_direction_intialize(port_index_t port, uint8_t direction){
    Std_ReturnType ret =E_OK;
    if(port>PORT_MAX_NUMBER-1)
    {
        ret = E_NOT_OK;
    }else
    {
        *tris_registers[port] = direction;
    }        
    return ret;
}
#endif
/**
 * 
 * @param port
 * @param dic_status : pointer points to the direction state 
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
#if GPIO_PORT_CONFIGURATIONS == CONFIG_ENABLE
Std_ReturnType gpio_port_get_direction_intialize(port_index_t port, uint8_t *dic_status){
    Std_ReturnType ret =E_OK;
    if((NULL==dic_status)||(port>PORT_MAX_NUMBER-1))
    {
        ret = E_NOT_OK;
    }else
    {
        *dic_status = *tris_registers[port];
    }
    return ret;
}
#endif
/**
 * 
 * @param port
 * @param logic : pointer points to the logic state 
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
#if GPIO_PORT_CONFIGURATIONS == CONFIG_ENABLE
Std_ReturnType gpio_port_write_logic(port_index_t port, uint8_t logic){
    Std_ReturnType ret =E_OK;
    if(port>PORT_MAX_NUMBER-1)
    {
        ret = E_NOT_OK;
    }else
    {
        *lat_registers[port] = logic;
    }
    return ret;
}
#endif
/**
 * 
 * @param port
 * @param logic : pointer points to the logic state
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
#if GPIO_PORT_CONFIGURATIONS == CONFIG_ENABLE
Std_ReturnType gpio_port_read_logic(port_index_t port, uint8_t *logic){
    Std_ReturnType ret =E_OK;
    if((NULL==logic)||(port>PORT_MAX_NUMBER-1))
    {
        ret = E_NOT_OK;
    }else
    {
        *logic = *port_registers[port];
    }
    return ret;
}
#endif
/**
 * 
 * @param port
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
#if GPIO_PORT_CONFIGURATIONS == CONFIG_ENABLE
Std_ReturnType gpio_port_toggle_logic(port_index_t port){
    Std_ReturnType ret =E_OK;
    if(port>PORT_MAX_NUMBER-1)
    {
        ret = E_NOT_OK;
    }else
    {
        *lat_registers[port] = *lat_registers[port] ^ PORT_MASK; 
    }
    return ret;
}
#endif