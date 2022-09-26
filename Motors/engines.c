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
#include "global_macros.h"

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
float leftMotorSpeed = 0.0;

/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/************************************
 * STATIC FUNCTIONS
 ************************************/
static void getDutyCycle(float RightMotorSpeed)
{
	// Car wants to move forwards
	if (RightMotorSpeed > STOP)
	{
		leftMotorSpeed = STOP - (RightMotorSpeed - STOP);
	}
	// Backwards
	else if (RightMotorSpeed < STOP)
	{
		leftMotorSpeed = STOP + (RightMotorSpeed - STOP);
	}
	// Stop state
	else
	{
		leftMotorSpeed = STOP;
	}
}


/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void setMotorSpeed(float RightMotorSpeed)
{
	getDutyCycle(RightMotorSpeed);

	// Set left motor duty cycle.
	// chnl_0 == PTA13
	TPM_UpdatePwmDutycycle(TPM1, kTPM_Chnl_0,
			kTPM_EdgeAlignedPwm, RightMotorSpeed);

	// Set right motor duty cycle.
	// chnl_1 == PTA12
	TPM_UpdatePwmDutycycle(TPM1, kTPM_Chnl_1,
			kTPM_EdgeAlignedPwm, leftMotorSpeed);
}
