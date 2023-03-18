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
#include "fsl_tpm.h"
#include "fsl_debug_console.h"

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

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

//!*************************************************************************************************
//! static void initMotors()
//!
//! @description
//! Function initialize motors.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
static void initMotors()
{
	PRINTF("\t\t- Motors initialization started.");
	static float initDutyCycleStep = 0.06;

	float initDutyCycle = 2.88;

	delay_ms(400);

	for (int i = 0; i < 100; i++)
	{
		TPM_UpdatePwmDutycycle(TPM1, kTPM_Chnl_0, kTPM_CenterAlignedPwm, initDutyCycle);
		TPM_UpdatePwmDutycycle(TPM1, kTPM_Chnl_1, kTPM_CenterAlignedPwm, initDutyCycle);
		delay_ms(2);

		initDutyCycle += initDutyCycleStep;
		if( (i % 10) == 0) PRINTF(".");
	}

	// After inicialization, stop motors
	TPM_UpdatePwmDutycycle(TPM1, kTPM_Chnl_0, kTPM_CenterAlignedPwm, 7.365000);
	TPM_UpdatePwmDutycycle(TPM1, kTPM_Chnl_1, kTPM_CenterAlignedPwm, 7.365000);
	delay_ms(50);
	PRINTF("\r\n\t\t- Motors initialization complete.\r\n");
}

//!*************************************************************************************************
//! static void initServo()
//!
//! @description
//! Function initialize servomotor.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
static void initServo()
{
	PRINTF("\t\t- Servo initialization started.\r\n");
	TPM_UpdatePwmDutycycle(TPM0, kTPM_Chnl_5, kTPM_CenterAlignedPwm, 7.37);
	delay_ms(100);
	PRINTF("\t\t- Servo initialization complete.\r\n");
}

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void startupPeripherals()
{
	initMotors();
	initServo();
}
