/**
 ********************************************************************************
 * @file    startup_peripherals.c
 * @author  user
 * @date    Jul 30, 2022
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "startup_peripherals.h"
#include "MKL25Z4.h"
#include "fsl_tpm.h"
#include "board.h"

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
/* Get source clock for TPM driver */
#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_PllFllSelClk)

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
static void startupPWM(void)
{
	tpm_config_t tpmInfo;
	tpm_chnl_pwm_signal_param_t tpmParam[2];

	// Configure tpm params with frequency 24kHZ
	tpmParam[0].chnlNumber = kTPM_Chnl_0;
	tpmParam[0].level = kTPM_HighTrue;

	tpmParam[1].chnlNumber = kTPM_Chnl_1;
	tpmParam[1].level = kTPM_HighTrue;

	// Select the clock source for the TPM counter as kCLOCK_PllFllSelClk
	CLOCK_SetTpmClock(1U);

	TPM_GetDefaultConfig(&tpmInfo);
	tpmInfo.prescale = kTPM_Prescale_Divide_32;
	// Initialize TPM module
	TPM_Init(TPM1, &tpmInfo);

	TPM_SetupPwm(TPM1, tpmParam, 2U, kTPM_EdgeAlignedPwm, 50U,
			TPM_SOURCE_CLOCK);
	TPM_StartTimer(TPM1, kTPM_SystemClock);
}


/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void startupPeripherals(void)
{
	startupPWM();
}
