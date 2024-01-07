/* 
 * File:   hal_eeprom.c
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on October 17, 2023, 5:06 PM
 */
#include"hal_eeprom.h"

/**
 * @brief This routine writes a data byte to given Data EEPROM location
 * @param Address_Size : Data EEPROM location to which data to be written
 * @param Data : Data to be written to Data EEPROM location
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType Data_EEPROM_WriteByte(uint16_t Address_Size, uint8_t Data){
    Std_ReturnType ret =E_OK;
    /* Read the Interrupt Status "Enabled or Disabled" */
    uint8_t Global_Interrupt_Status = INTCONbits.GIE;
    /* Update the Address Registers */
    EEADRH = (uint8_t)((Address_Size>>8) & (0x03)); /* MSbits */
    EEADR = (uint8_t)(Address_Size & 0xFF); /* LSbits */
    /* Update the Data Register */
    EEDATA = Data;
    /* Select Access data EEPROM memory */
    EECON1bits.EEPGD = ACCESS_DATA_EEPROM_MEMORY;
    EECON1bits.CFGS = ACCESS_FLASH_OR_DATA_EEPROM;
    /* Allows write cycles to Flash program/data EEPROM */
    EECON1bits.WREN = ALLOWS_WRITE_CYCLES_TO_FLASH_OR_EEPROM;
    /* Disable all interrupts "General Interrupt" */
    #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
    #else
    INTERRUPT_GlobalInterruptDisable();
    #endif
    /* Write the required sequence : 0x55 -> 0xAA */
    EECON2 = 0x55;
    EECON2 = 0xAA; 
    /* Initiates a data EEPROM erase/write cycle */
    EECON1bits.WR = INITIATE_DATA_EEPROM_WRITE_ERASE;
    /* Wait for write to complete */
    while(EECON1bits.WR);
    /* Inhibits write cycles to Flash program/data EEPROM */
    EECON1bits.WREN = INHIBITS_WRITE_CYCLES_TO_FLASH_OR_EEPROM;
    /* Restore the Interrupt Status "Enabled or Disabled" */
    INTCONbits.GIE = Global_Interrupt_Status;
    return ret;
}
/**
 * @brief This routine reads a data byte from given Data EEPROM location
 * @param Address_Size : Data EEPROM location from which data has to be read 
 * @param Data : pointer pointe to Data byte read from given Data EEPROM location
 * @return status of the function
 *         (E_OK)      : The function done successfully
 *         (E_NOT_OK)  : The function has issue to perform this action
 */
Std_ReturnType Data_EEPROM_ReadByte(uint16_t Address_Size, uint8_t *Data){
    Std_ReturnType ret =E_OK;
    if(NULL == Data)
    {
        ret = E_NOT_OK;
    }else
    {
        /* Update the Address Registers */
        EEADRH = (uint8_t)((Address_Size>>8) & (0x03)); /* MSbits */
        EEADR = (uint8_t)(Address_Size & 0xFF); /* LSbits */
        /* Select Access data EEPROM memory */
        EECON1bits.EEPGD = ACCESS_DATA_EEPROM_MEMORY;
        EECON1bits.CFGS = ACCESS_FLASH_OR_DATA_EEPROM;
        /* Initiates a data EEPROM read cycle */
        EECON1bits.RD = INITIATE_DATA_EEPROM_READ;
        NOP(); /* NOPs may be required for latency at high frequencies */
        NOP(); /* NOPs may be required for latency at high frequencies */
        /* Return data */
        *Data = EEDATA;
    }
    return ret;
}
