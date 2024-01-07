/* 
 * File:   hal_eeprom.h
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on October 17, 2023, 5:06 PM
 */

#ifndef HAL_EEPROM_H
#define	HAL_EEPROM_H

/* Section : Includes */
#include"../proc/pic18f4620.h"
#include"../mcal_std_types.h"
#include"../Interrupt/mcal_internal_interrupt.h"

/* Section : Macro Declarations */
#define ACCESS_FLASH_PROGRAM_MEMORY    1
#define ACCESS_DATA_EEPROM_MEMORY      0

#define ACCESS_CONFIG_REGISTERS        1
#define ACCESS_FLASH_OR_DATA_EEPROM    0

#define ALLOWS_WRITE_CYCLES_TO_FLASH_OR_EEPROM      1
#define INHIBITS_WRITE_CYCLES_TO_FLASH_OR_EEPROM    0

#define INITIATE_DATA_EEPROM_WRITE_ERASE    1   
#define DATA_EEPROM_WRITE_ERASE_COMPLETED   0

#define INITIATE_DATA_EEPROM_READ    1
/* Section : Macro Functions Declarations */


/* Section : Data type Declarations */


/* Section : Functions Declaration */
Std_ReturnType Data_EEPROM_WriteByte(uint16_t Address_Size, uint8_t Data);
Std_ReturnType Data_EEPROM_ReadByte(uint16_t Address_Size, uint8_t *Data);

#endif	/* HAL_EEPROM_H */

