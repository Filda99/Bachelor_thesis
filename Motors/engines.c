/**
 ********************************************************************************
 * @file    engines.c
 * @author  user
 * @date    Jul 18, 2022
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "engines.h"
#include "fsl_tpm.h"
#include "MKL25Z4.h"

/************************************
 * EXTERN VARIABLES
 ************************************/
extern int8_t CurrentSpeed;

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define BOARD_TPM_BASEADDR TPM1

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/

/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/************************************
 * STATIC FUNCTIONS
 ************************************/

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void setMotorSpeed(void)
{
	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, kTPM_Chnl_0,
			kTPM_EdgeAlignedPwm, CurrentSpeed);
	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, kTPM_Chnl_1,
			kTPM_EdgeAlignedPwm, CurrentSpeed);

}
