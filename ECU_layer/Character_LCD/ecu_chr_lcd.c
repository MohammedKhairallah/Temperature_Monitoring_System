/* 
 * File:   ecu_chr_lcd.c
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on September 27, 2023, 12:08 PM
 */
#include"ecu_chr_lcd.h"

static Std_ReturnType lcd_send_4bit(const chr_lcd_4bit_t *lcd, uint8_t command);

static Std_ReturnType lcd_4bit_send_enable_signal(const chr_lcd_4bit_t *lcd);
static Std_ReturnType lcd_8bit_send_enable_signal(const chr_lcd_8bit_t *lcd);

static Std_ReturnType lcd_4bit_set_cursor(const chr_lcd_4bit_t *lcd, uint8_t row, uint8_t column);
static Std_ReturnType lcd_8bit_set_cursor(const chr_lcd_8bit_t *lcd, uint8_t row, uint8_t column);
/**
 * @brief Initialize the LCD assigned pins 
 * @param lcd : pointer to the LCD configurations
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType lcd_4bit_initialize(const chr_lcd_4bit_t * lcd){
    Std_ReturnType ret = E_OK;
    uint8_t count = ZERO_INIT; 
    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }else
    {
        ret = gpio_pin_intialize(&(lcd->lcd_rs_pin));
        ret = gpio_pin_intialize(&(lcd->lcd_enable_pin));
        for(count=ZERO_INIT;count<4;count++)
        {
            ret = gpio_pin_intialize(&(lcd->lcd_data_pins[count]));
        }
        __delay_ms(20);
        ret = lcd_4bit_send_command(lcd, LCD_8BIT_MODE_2LINE);
        __delay_ms(5);
        ret = lcd_4bit_send_command(lcd, LCD_8BIT_MODE_2LINE);
        __delay_ms(150);
        ret = lcd_4bit_send_command(lcd, LCD_8BIT_MODE_2LINE);
        
        ret = lcd_4bit_send_command(lcd, LCD_CLEAR);
        ret = lcd_4bit_send_command(lcd, LCD_RETURN_HOME);
        ret = lcd_4bit_send_command(lcd, LCD_ENTER_MODE_INC_SHIFT_OFF);
        ret = lcd_4bit_send_command(lcd, LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_OFF);
        ret = lcd_4bit_send_command(lcd, LCD_4BIT_MODE_2LINE);
        ret = lcd_4bit_send_command(lcd, LCD_DDRAM_START);
    }
    return ret;
}
/**
 * @brief Sends a command to the 4-bit LCD display.
 * @param lcd : pointer to the LCD configurations
 * @param command : The command that we need to the LCD
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType lcd_4bit_send_command(const chr_lcd_4bit_t *lcd, uint8_t command){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }else
    {
        /* R/W pin connected to GND -> logic(0) Hard Wired */
        /* Write logic (0)  to the "Register Select" to select the "Instruction Register" */
        ret = gpio_pin_write_logic(&(lcd->lcd_rs_pin), GPIO_LOW);
        /* Send commend through the (4-pins) "Data Lines" */
        /**First Send High bit (commend >> 4)*/
        ret = lcd_send_4bit(lcd, command >> 4);
        /**Send the Enable signal on the "E" pin */
        ret = lcd_4bit_send_enable_signal(lcd);
        /* Send commend through the (4-pins) "Data Lines" */
        /**Second Send low bit (commend)*/
        ret = lcd_send_4bit(lcd, command);
        /**Send the Enable signal on the "E" pin */
        ret = lcd_4bit_send_enable_signal(lcd);
    }
    return ret;
}
/**
 * @brief Sends a character data to the 4-bit LCD display.  
 * @param lcd : pointer to the LCD configurations
 * @param data : data which user want to show
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType lcd_4bit_send_char_data(const chr_lcd_4bit_t * lcd, uint8_t data){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }else
    {
        /* R/W pin connected to GND -> logic(0) Hard Wired */
        /* Write logic (1)  to the "Register Select" to select the "Data Register" */
        ret = gpio_pin_write_logic(&(lcd->lcd_rs_pin), GPIO_HIGH);
        /* Send data through the (4-pins) "Data Lines" */
        /**First Send High bit (data >> 4)*/
        ret = lcd_send_4bit(lcd, data >> 4);
        /**Send the Enable signal on the "E" pin */
        ret = lcd_4bit_send_enable_signal(lcd);
        /* Send data through the (4-pins) "Data Lines" */
        /**Second Send low bit (data)*/
        ret = lcd_send_4bit(lcd, data);
        /**Send the Enable signal on the "E" pin */
        ret = lcd_4bit_send_enable_signal(lcd);
    }
    return ret;
}
/**
 * @brief Sends a character data to the specified position on a 4-bit LCD display.
 * @param lcd : pointer to the LCD configurations
 * @param row : The row number(starting from 1) on the LCD display where the character data will be displayed.
 * @param column : The column number(starting from 1) on the LCD display where the character data will be displayed
 * @param data : The character data to be displayed on the LCD display.
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType lcd_4bit_send_char_data_pos(const chr_lcd_4bit_t *lcd, uint8_t row, uint8_t column, uint8_t data){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }else
    {
        ret = lcd_4bit_set_cursor(lcd, row, column);
        ret = lcd_4bit_send_char_data(lcd, data);
    }
    return ret;
}
/**
 * @brief Sends a string to the 4-bit LCD display.
 * @param lcd : pointer to the LCD configurations
 * @param str
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType lcd_4bit_send_string(const chr_lcd_4bit_t * lcd, uint8_t *str){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }else
    {
        while(*str)
        {
            ret = lcd_4bit_send_char_data(lcd, *str++);
        }
    }
    return ret;
}
/**
 * @brief Sends a string to the specified position on a 4-bit LCD display.
 * @param lcd : pointer to the LCD configurations
 * @param row : The row number(starting from 1) on the LCD display where the string will be displayed.
 * @param column : : The column number(starting from 1) on the LCD display where the string will be displayed
 * @param str
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType lcd_4bit_send_string_pos(const chr_lcd_4bit_t * lcd, uint8_t row, uint8_t column, uint8_t *str){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }else
    {
        ret = lcd_4bit_set_cursor(lcd, row, column);
        ret = lcd_4bit_send_string(lcd, str);
    }
    return ret;
}
/**
 * @brief sends a custom character to the specified position on the LCD module.
 * @param lcd : pointer to the LCD configurations
 * @param row : The row number(starting from 1) on the LCD display where the custom character will be displayed.
 * @param column : The column number(starting from 1) on the LCD display where the custom character will be positioned. 
 * @param _chr : Array containing the custom character data (8 bytes).
 * @param mem_pos : Memory position of the custom character (0-7 for 8 custom characters).
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType lcd_4bit_send_custom_char(const chr_lcd_4bit_t * lcd, uint8_t row, uint8_t column, const uint8_t _chr[], uint8_t mem_pos){
    Std_ReturnType ret = E_OK;
    uint8_t count = ZERO_INIT;
    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }else
    {
        ret = lcd_4bit_send_command(lcd, (LCD_CGRAM_START + (mem_pos*8)));
        for(count=ZERO_INIT;count<=7;count++)
        {
            ret = lcd_4bit_send_char_data(lcd, _chr[count]);
        }
        ret = lcd_4bit_send_char_data_pos(lcd, row, column, mem_pos);
    }
    return ret;
}
/**
 * @brief Initialize the LCD assigned pins 
 * @param lcd : pointer to the LCD configurations
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType lcd_8bit_initialize(const chr_lcd_8bit_t * lcd){
    Std_ReturnType ret = E_OK;
    uint8_t count = ZERO_INIT; 
    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }else
    {
        ret = gpio_pin_intialize(&(lcd->lcd_rs_pin));
        ret = gpio_pin_intialize(&(lcd->lcd_enable_pin));
        for(count=ZERO_INIT;count<8;count++)
        {
            ret = gpio_pin_intialize(&(lcd->lcd_data_pins[count]));
        } 
        __delay_ms(20);
        ret = lcd_8bit_send_command(lcd, LCD_8BIT_MODE_2LINE);
        __delay_ms(5);
        ret = lcd_8bit_send_command(lcd, LCD_8BIT_MODE_2LINE);
        __delay_ms(150);
        ret = lcd_8bit_send_command(lcd, LCD_8BIT_MODE_2LINE);
        
        ret = lcd_8bit_send_command(lcd, LCD_CLEAR);
        ret = lcd_8bit_send_command(lcd, LCD_RETURN_HOME);
        ret = lcd_8bit_send_command(lcd, LCD_ENTER_MODE_INC_SHIFT_OFF);
        ret = lcd_8bit_send_command(lcd, LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_OFF);
        ret = lcd_8bit_send_command(lcd, LCD_8BIT_MODE_2LINE);
        ret = lcd_8bit_send_command(lcd, LCD_DDRAM_START);
    }
    return ret;
}
/**
 * @brief Sends a command to the specified LCD module using the 8-bit data mode.
 * @param lcd : pointer to the LCD configurations
 * @param command : The command that we need to the LCD
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType lcd_8bit_send_command(const chr_lcd_8bit_t * lcd, uint8_t command){
    Std_ReturnType ret = E_OK;
    uint8_t count = ZERO_INIT;
    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }else
    {
        /* R/W pin connected to GND -> logic(0) Hard Wired */
        /* Write logic (0)  to the "Register Select" to select the "Instruction Register" */
        ret = gpio_pin_write_logic(&(lcd->lcd_rs_pin), GPIO_LOW);
        /* Send commend through the (8-pins) "Data Lines" */
        for(count=ZERO_INIT;count<8;count++)
        {
            ret = gpio_pin_write_logic(&(lcd->lcd_data_pins[count]), (command >> count) & (uint8_t)0x01);
        }
        /**Send the Enable signal on the "E" pin */
        ret = lcd_8bit_send_enable_signal(lcd);
    }
    return ret;
}
/**
 * @brief Sends a character data to the specified LCD module using the 8-bit data mode.
 * @param lcd : pointer to the LCD configurations
 * @param data : The character data to be displayed on the LCD display.
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType lcd_8bit_send_char_data(const chr_lcd_8bit_t * lcd, uint8_t data){
    Std_ReturnType ret = E_OK;
    uint8_t count = ZERO_INIT;
    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }else
    {
        /* R/W pin connected to GND -> logic(0) Hard Wired */
        /* Write logic (1)  to the "Register Select" to select the "Data Register" */
        ret = gpio_pin_write_logic(&(lcd->lcd_rs_pin), GPIO_HIGH);
        /* Send data through the (8-pins) "Data Lines" */
        for(count=ZERO_INIT;count<8;count++)
        {
            ret = gpio_pin_write_logic(&(lcd->lcd_data_pins[count]), (data >> count) & (uint8_t)0x01);
        }
        /**Send the Enable signal on the "E" pin */
        ret = lcd_8bit_send_enable_signal(lcd);
    }
    return ret;
}
/**
 * @brief Sends a character data to the specified position on a 8-bit LCD display.
 * @param lcd : pointer to the LCD configurations
 * @param row : Row position on the LCD display
 * @param column : Column position on the LCD display
 * @param data : The character data to be displayed on the LCD display.
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType lcd_8bit_send_char_data_pos(const chr_lcd_8bit_t * lcd, uint8_t row, uint8_t column, uint8_t data){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }else
    {
        ret = lcd_8bit_set_cursor(lcd, row, column);
        ret = lcd_8bit_send_char_data(lcd, data);
    }
    return ret;
}
/**
 * @brief Sends a string to the specified LCD module using the 8-bit data mode.
 * @param lcd : pointer to the LCD configurations
 * @param str : Pointer to the string to be sent.
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType lcd_8bit_send_string(const chr_lcd_8bit_t * lcd, uint8_t *str){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }else
    {
        while(*str)
        {
            ret = lcd_8bit_send_char_data(lcd, *str++);
        }
    }
    return ret;
}
/**
 * @brief Sends a string to an LCD module at the specified position using 8-bit mode.
 * @param lcd : pointer to the LCD configurations
 * @param row : Row position on the LCD display
 * @param column : Column position on the LCD display
 * @param str : Pointer to the string to be sent.
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType lcd_8bit_send_string_pos(const chr_lcd_8bit_t * lcd, uint8_t row, uint8_t column, uint8_t *str){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }else
    {
        ret = lcd_8bit_set_cursor(lcd, row, column);
        ret = lcd_8bit_send_string(lcd, str);
    }
    return ret;
}
/**
 * @brief sends a custom character to the specified position on the LCD module.
 * @param lcd : pointer to the LCD configurations
 * @param row : The row number(starting from 1) on the LCD display where the custom character will be displayed.
 * @param column : The column number(starting from 1) on the LCD display where the custom character will be positioned. 
 * @param _chr : Array containing the custom character data (8 bytes).
 * @param mem_pos : Memory position of the custom character (0-7 for 8 custom characters).
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType lcd_8bit_send_custom_char(const chr_lcd_8bit_t * lcd, uint8_t row, uint8_t column, const uint8_t _chr[], uint8_t mem_pos){
    Std_ReturnType ret = E_OK;
    uint8_t count = ZERO_INIT;
    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }else
    {
        ret = lcd_8bit_send_command(lcd, (LCD_CGRAM_START + (mem_pos*8)));
        for(count=ZERO_INIT;count<=7;count++)
        {
            ret = lcd_8bit_send_char_data(lcd, _chr[count]);
        }
        ret = lcd_8bit_send_char_data_pos(lcd, row, column, mem_pos);
    }
    return ret;
}
/**
 * @brief Converts a 8-bit unsigned integer value to a null-terminated string representation.
 * @param value : 8-bit unsigned integer value to be converted.
 * @param str : 8-bit unsigned integer value to be converted.
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType convert_uint8_to_string(uint8_t value, uint8_t *str){
    Std_ReturnType ret = E_OK;
    uint8_t Tempstr[4] = {0};
    uint8_t DataCounter = 0;
    if(NULL == str)
    {
        ret = E_NOT_OK;
    }else
    {
        memset(str, ' ', 3); /* uint8_t -> 255 so -> str = {'2','5','5','\0'} -> 4 byte*/
        str[3] = '\0';
        sprintf(Tempstr, "%i", value);
        while(Tempstr[DataCounter] != '\0'){
            str[DataCounter] = Tempstr[DataCounter];
            DataCounter++;
        }
    }
    return ret;
}
/**
 * @brief Converts a 16-bit unsigned integer value to a null-terminated string representation.
 * @param value : 16-bit unsigned integer value to be converted.
 * @param str : Pointer to the buffer where the resulting string will be stored.
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType convert_uint16_to_string(uint16_t value, uint8_t *str){
    Std_ReturnType ret = E_OK;
    uint8_t Tempstr[6] = {0};
    uint8_t DataCounter = 0;
    if(NULL == str)
    {
        ret = E_NOT_OK;
    }else
    {
        memset(str, ' ', 5);  /* uint8_t -> 65,535*/
        str[5] = '\0';
        sprintf(Tempstr, "%i", value);
        while(Tempstr[DataCounter] != '\0'){
            str[DataCounter] = Tempstr[DataCounter];
            DataCounter++;
        }
    }
    return ret;
}
/**
 * @brief Converts a 32-bit unsigned integer value to a null-terminated string representation.
 * @param value : 32-bit unsigned integer value to be converted.
 * @param str : Pointer to the buffer where the resulting string will be stored.
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType convert_uint32_to_string(uint32_t value, uint8_t *str){
    Std_ReturnType ret = E_OK;
    uint8_t Tempstr[11] = {0};
    uint8_t DataCounter = 0;
    if(NULL == str)
    {
        ret = E_NOT_OK;
    }else
    {
        memset(str, ' ', 10);
        str[10] = '\0';
        sprintf(Tempstr, "%i", value);
        while(Tempstr[DataCounter] != '\0'){
            str[DataCounter] = Tempstr[DataCounter];
            DataCounter++;
        }
    }
    return ret;
}
/**
 * @brief sends a 4-bit command to the specified LCD module using the 4-bit data bus.
 * @param lcd : pointer to the LCD configurations
 * @param command : 4-bit command to be sent.
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
static Std_ReturnType lcd_send_4bit(const chr_lcd_4bit_t *lcd, uint8_t command){
    Std_ReturnType ret = E_OK;
    ret = gpio_pin_write_logic(&(lcd->lcd_data_pins[0]), (command >> 0) & (uint8_t)0x01);
    ret = gpio_pin_write_logic(&(lcd->lcd_data_pins[1]), (command >> 1) & (uint8_t)0x01);
    ret = gpio_pin_write_logic(&(lcd->lcd_data_pins[2]), (command >> 2) & (uint8_t)0x01);
    ret = gpio_pin_write_logic(&(lcd->lcd_data_pins[3]), (command >> 3) & (uint8_t)0x01);
    return ret;
}
/**
 * @brief Sends an enable signal to the 4-bit LCD display.
 * @param lcd : pointer to the LCD configurations
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
static Std_ReturnType lcd_4bit_send_enable_signal(const chr_lcd_4bit_t *lcd){
    Std_ReturnType ret = E_OK;
    ret = gpio_pin_write_logic(&(lcd->lcd_enable_pin), GPIO_HIGH);
    __delay_us(5);
    ret = gpio_pin_write_logic(&(lcd->lcd_enable_pin), GPIO_LOW);
    return ret;
}
/**
 * @brief Sends an enable signal to the 8-bit LCD display.
 * @param lcd : : pointer to the LCD configurations
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
static Std_ReturnType lcd_8bit_send_enable_signal(const chr_lcd_8bit_t *lcd){
    Std_ReturnType ret = E_OK;
    ret = gpio_pin_write_logic(&(lcd->lcd_enable_pin), GPIO_HIGH);
    __delay_us(5);
    ret = gpio_pin_write_logic(&(lcd->lcd_enable_pin), GPIO_LOW);
    return ret;
}
/**
 * @brief Sets the cursor position on an 4-bit LCD display.
 * @param lcd : pointer to the LCD configurations
 * @param row : The row number (starting from 1) on the LCD display where the cursor will be positioned.
 * @param column : The column number(starting from 1) on the LCD display where the cursor will be positioned.
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
static Std_ReturnType lcd_4bit_set_cursor(const chr_lcd_4bit_t *lcd, uint8_t row, uint8_t column){
    Std_ReturnType ret = E_OK;
    column--;
    switch(row)
    {
        case (ROW1) : ret = lcd_4bit_send_command(lcd, (0x80 +column)); break; /* 0x80 -> 0x00 + 0x80 */
        case (ROW2) : ret = lcd_4bit_send_command(lcd, (0xC0 +column)); break; /* 0xC0 -> 0x40 + 0x80 */
        case (ROW3) : ret = lcd_4bit_send_command(lcd, (0x94 +column)); break; /* 0x94 -> 0x14 + 0x80 */
        case (ROW4) : ret = lcd_4bit_send_command(lcd, (0xD4 +column)); break; /* 0xD4 -> 0x54 + 0x80 */
        default : ;
    }
    return ret;
}
/**
 * @brief Sets the cursor position on an 8-bit LCD display.
 * @param lcd : pointer to the LCD configurations
 * @param row : The row number (starting from 1) on the LCD display where the cursor will be positioned.
 * @param column : The column number(starting from 1) on the LCD display where the cursor will be positioned.
 * @return status of function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
static Std_ReturnType lcd_8bit_set_cursor(const chr_lcd_8bit_t *lcd, uint8_t row, uint8_t column){
    Std_ReturnType ret = E_OK;
    column--;
    switch(row)
    {
        case (ROW1) : ret = lcd_8bit_send_command(lcd, (0x80 +column)); break; /* 0x80 -> 0x00 + 0x80 */
        case (ROW2) : ret = lcd_8bit_send_command(lcd, (0xC0 +column)); break; /* 0xC0 -> 0x40 + 0x80 */
        case (ROW3) : ret = lcd_8bit_send_command(lcd, (0x94 +column)); break; /* 0x94 -> 0x14 + 0x80 */
        case (ROW4) : ret = lcd_8bit_send_command(lcd, (0xD4 +column)); break; /* 0xD4 -> 0x54 + 0x80 */
        default : ;
    }
    return ret;
}