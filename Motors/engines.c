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
extern bool IsCmdToStopCar;

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define STOP			7.365

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/
float leftMotorSpeed;
float rightMotorSpeed;

//! Current position in speed array.
//! 0 = max backwards
//! 5 = stop engines
//! 11 = max forwards
static int currentSpeed = STOP_SPEED;

//! Current position in steer array.
//! 0 = max left
//! 3 = direct
//! 7 = max right
static int currentSteer = GO_DIRECT;

/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/************************************
 * STATIC FUNCTIONS
 ************************************/
static void getDutyCycle(float setSpeed)
{
	float rightMotorSpeed = setSpeed;
	// Car wants to move forwards
	if (rightMotorSpeed > STOP)
	{
		leftMotorSpeed = STOP - (rightMotorSpeed - STOP);
	}
	// Backwards
	else if (rightMotorSpeed < STOP)
	{
		leftMotorSpeed = STOP + (rightMotorSpeed - STOP);
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
void setMotorSpeed(int speed)
{
	float setSpeed = SpeedMap[speed];
	getDutyCycle(setSpeed);

	// Set left motor duty cycle.
	TPM_UpdatePwmDutycycle(TPM1, kTPM_Chnl_0,
			kTPM_EdgeAlignedPwm, rightMotorSpeed);

	// Set right motor duty cycle.
	TPM_UpdatePwmDutycycle(TPM1, kTPM_Chnl_1,
			kTPM_EdgeAlignedPwm, leftMotorSpeed);
}

void setMotorSteer(int steer)
{
	float setSteer = SteerMap[steer];
	TPM_UpdatePwmDutycycle(TPM0, kTPM_Chnl_1,
				kTPM_EdgeAlignedPwm, setSteer);
}

//-------------------
//------ SPEED ------
//-------------------
void addSpeed()
{
	if (currentSpeed == MAX_SPEED_FORWARDS) return;

	currentSpeed++;
	setMotorSpeed(currentSpeed);
}

void addSpeedCustom(int speed)
{
	if ((currentSpeed + speed) >= MAX_SPEED_FORWARDS)
	{
		currentSpeed = MAX_SPEED_FORWARDS;
	}
	else
	{
		currentSpeed += speed;
	}

	setMotorSpeed(currentSpeed);
}

void slackUpSpeed()
{
	if (currentSpeed == MAX_SPEED_BACKWARDS) return;

	currentSpeed--;
	setMotorSpeed(currentSpeed);
}

void slackUpSpeedCustom(int speed)
{
	if ((currentSpeed - speed) <= MAX_SPEED_BACKWARDS)
	{
		currentSpeed = MAX_SPEED_BACKWARDS;
	}
	else
	{
		currentSpeed -= speed;
	}

	setMotorSpeed(currentSpeed);
}

void stopCar()
{
	currentSpeed = STOP_SPEED;
	IsCmdToStopCar = true;
	setMotorSpeed(currentSpeed);
}


//-------------------
//------ STEER ------
//-------------------
void turnRight()
{
	if(currentSteer == MAX_STEER_RIGHT) return;

	currentSteer++;
	setMotorSteer(currentSteer);
}

void turnRightCustom(int steer)
{
	if ((currentSteer + steer) >= MAX_STEER_RIGHT)
	{
		currentSteer = MAX_STEER_RIGHT;
	}
	else
	{
		currentSteer += steer;
	}

	setMotorSteer(currentSteer);
}

void turnLeft()
{
	if(currentSteer == MAX_STEER_LEFT) return;

	currentSteer--;
	setMotorSteer(currentSteer);
}

void turnLeftCustom(int steer)
{
	if ((currentSteer - steer) <= MAX_STEER_LEFT)
	{
		currentSteer = MAX_STEER_LEFT;
	}
	else
	{
		currentSteer += steer;
	}

	setMotorSteer(currentSteer);
}

void goDirect()
{
	currentSteer = GO_DIRECT;
	setMotorSteer(currentSteer);
}


















