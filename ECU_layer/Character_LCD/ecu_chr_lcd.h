/* 
 * File:   ecu_chr_lcd.h
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on September 27, 2023, 12:08 PM
 */

#ifndef ECU_CHR_LCD_H
#define	ECU_CHR_LCD_H

/* Section : Includes */
#include"../../MCAL_layer/GPIO/hal_gpio.h"
#include"ecu_chr_lcd_cfg.h"

/* Section : Macro Declarations */
#define LCD_CLEAR                       0x01

#define LCD_RETURN_HOME                 0x02

#define LCD_ENTER_MODE_DEC_SHIFT_OFF    0x04
#define LCD_ENTER_MODE_DEC_SHIFT_ON     0x05
#define LCD_ENTER_MODE_INC_SHIFT_OFF    0x06
#define LCD_ENTER_MODE_INC_SHIFT_ON     0x07

#define LCD_CURSOR_MOVE_SHIFT_LEFT      0x10
#define LCD_CURSOR_MOVE_SHIFT_RIGHT     0x14
#define LCD_DISPLAY_SHIFT_LEFT          0x18
#define LCD_DISPLAY_SHIFT_RIGHT         0x1C

#define LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_OFF      0x0C
#define LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_ON       0x0D
#define LCD_DISPLAY_ON_UNDERLINE_ON_CURSOR_OFF       0x0E
#define LCD_DISPLAY_ON_UNDERLINE_ON_CURSOR_ON        0x0F
#define LCD_DISPLAY_OFF_CURSOR_OFF                   0x08

#define LCD_8BIT_MODE_2LINE     0x38
#define LCD_4BIT_MODE_2LINE     0x28

#define LCD_CGRAM_START         0x40
#define LCD_DDRAM_START         0x80

#define ROW1    1
#define ROW2    2
#define ROW3    3
#define ROW4    4

/* Section : Macro Functions Declarations */


/* Section : Data type Declarations */
typedef struct{
    pin_config_t lcd_rs_pin;
    pin_config_t lcd_enable_pin;
    pin_config_t lcd_data_pins[4];
}chr_lcd_4bit_t;

typedef struct{
    pin_config_t lcd_rs_pin;
    pin_config_t lcd_enable_pin;
    pin_config_t lcd_data_pins[8];
}chr_lcd_8bit_t;

/* Section : Functions Declaration */
Std_ReturnType lcd_4bit_initialize(const chr_lcd_4bit_t * lcd);
Std_ReturnType lcd_4bit_send_command(const chr_lcd_4bit_t * lcd, uint8_t command);
Std_ReturnType lcd_4bit_send_char_data(const chr_lcd_4bit_t * lcd, uint8_t data);
Std_ReturnType lcd_4bit_send_char_data_pos(const chr_lcd_4bit_t * lcd, uint8_t row, uint8_t column, uint8_t data);
Std_ReturnType lcd_4bit_send_string(const chr_lcd_4bit_t * lcd, uint8_t *str);
Std_ReturnType lcd_4bit_send_string_pos(const chr_lcd_4bit_t * lcd, uint8_t row, uint8_t column, uint8_t *str);
Std_ReturnType lcd_4bit_send_custom_char(const chr_lcd_4bit_t * lcd, uint8_t row, uint8_t column, const uint8_t _chr[], uint8_t mem_pos);

Std_ReturnType lcd_8bit_initialize(const chr_lcd_8bit_t * lcd);
Std_ReturnType lcd_8bit_send_command(const chr_lcd_8bit_t * lcd, uint8_t command);
Std_ReturnType lcd_8bit_send_char_data(const chr_lcd_8bit_t * lcd, uint8_t data);
Std_ReturnType lcd_8bit_send_char_data_pos(const chr_lcd_8bit_t * lcd, uint8_t row, uint8_t column, uint8_t data);
Std_ReturnType lcd_8bit_send_string(const chr_lcd_8bit_t * lcd, uint8_t *str);
Std_ReturnType lcd_8bit_send_string_pos(const chr_lcd_8bit_t * lcd, uint8_t row, uint8_t column, uint8_t *str);
Std_ReturnType lcd_8bit_send_custom_char(const chr_lcd_8bit_t * lcd, uint8_t row, uint8_t column, const uint8_t _chr[], uint8_t mem_pos);

Std_ReturnType convert_uint8_to_string(uint8_t value, uint8_t *str);
Std_ReturnType convert_uint16_to_string(uint16_t value, uint8_t *str);
Std_ReturnType convert_uint32_to_string(uint32_t value, uint8_t *str);

#endif	/* ECU_CHR_LCD_H */

