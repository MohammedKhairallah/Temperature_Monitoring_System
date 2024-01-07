/* 
 * File:   hal_ccp_cfg.h
 * Author: Mohammed Khairallah
 * linkedin : linkedin.com/in/mohammed-khairallah
 * Created on October 29, 2023, 9:41 AM
 */

#ifndef HAL_CCP_CFG_H
#define	HAL_CCP_CFG_H

/* Section : Includes */


/* Section : Macro Declarations */

#define CCP_CFG_CAPTURE_MODE_SELECTED     0x00
#define CCP_CFG_COMPARE_MODE_SELECTED     0x01
#define CCP_CFG_PWM_MODE_SELECTED         0x02

#define CCP1_CFG_SELECTED_MODE     (CCP_CFG_PWM_MODE_SELECTED)
#define CCP2_CFG_SELECTED_MODE     (CCP_CFG_COMPARE_MODE_SELECTED)

/* Section : Macro Functions Declarations */


/* Section : Data type Declarations */


/* Section : Functions Declaration */

#endif	/* HAL_CCP_CFG_H */

