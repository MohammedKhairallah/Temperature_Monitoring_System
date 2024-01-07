/* 
 * File:   mcal_std_types.h
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on August 20, 2023, 2:07 PM
 */

#ifndef MCAL_STD_TYPES_H
#define	MCAL_STD_TYPES_H

/* Section : Includes */
#include"std_libraries.h"
#include"compiler.h"

/* Section : Macro Declarations */
#define STD_HIGH        0x01
#define STD_LOW         0x00

#define STD_ON          0x01
#define STD_OFF         0x00

#define STD_ACTIVE      0x01
#define STD_IDLE        0x00

#define CONFIG_ENABLE   0x01
#define CONFIG_DISABLE  0x00

#define E_OK        (Std_ReturnType)0x01
#define E_NOT_OK    (Std_ReturnType)0x00

#define ZERO_INIT       0x00

/* Section : Macro Functions Declarations */


/* Section : Data type Declarations */
typedef uint8_t    Std_ReturnType; 

/* Section : Functions Declaration */


#endif	/* MCAL_STD_TYPES_H */

