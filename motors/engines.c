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
#include "../drivers/fsl_tpm.h"
#include "../source/global_macros.h"
#include "../source/common.h"

#include "utilities/fsl_debug_console.h"

/************************************
 * EXTERN VARIABLES
 ************************************/
extern bool IsCmdToStopCar;
extern bool InitStop;
extern uint8_t currentSpeed;
extern uint8_t currentSteer;

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define STOP			7.365

#define TURNING_LEFT	0
#define TURNING_RIGHT	180

//! Denominator of a division
#define SLACK_DIV_AMOUNT	5

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/
float leftMotorSpeed;
float rightMotorSpeed;

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
	rightMotorSpeed = setSpeed;
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


//!*************************************************************************************************
//! void setMotorSpeed(int speed)
//!
//! @description
//! Update PWM duty cycle based on variables rightMotorSpeed and leftMotorSpeed.
//!
//! @param	speed	Number which points to position in SpeedMap array.
//!
//! @return	None
//!*************************************************************************************************
static void setMotorSpeed(int speed, uint8_t wheelSide,uint8_t slackWheel)
{
	float setSpeed = SpeedMap[speed];
	getDutyCycle(setSpeed);

	float slackValue = slackWheel / SLACK_DIV_AMOUNT;

	if (wheelSide == TURNING_LEFT)
	{
		// Set left motor duty cycle.
		TPM_UpdatePwmDutycycle(TPM1, kTPM_Chnl_0,
				kTPM_EdgeAlignedPwm, leftMotorSpeed - slackValue);

		// Set right motor duty cycle.
		TPM_UpdatePwmDutycycle(TPM1, kTPM_Chnl_1,
				kTPM_EdgeAlignedPwm, rightMotorSpeed);
	}
	else if (wheelSide == TURNING_RIGHT)
	{
		// Set left motor duty cycle.
		TPM_UpdatePwmDutycycle(TPM1, kTPM_Chnl_0,
				kTPM_EdgeAlignedPwm, leftMotorSpeed);

		// Set right motor duty cycle.
		TPM_UpdatePwmDutycycle(TPM1, kTPM_Chnl_1,
				kTPM_EdgeAlignedPwm, rightMotorSpeed - slackValue);
	}
	else
	{
		// Set left motor duty cycle.
		TPM_UpdatePwmDutycycle(TPM1, kTPM_Chnl_0,
				kTPM_EdgeAlignedPwm, leftMotorSpeed);

		// Set right motor duty cycle.
		TPM_UpdatePwmDutycycle(TPM1, kTPM_Chnl_1,
				kTPM_EdgeAlignedPwm, rightMotorSpeed);
	}

	delay_ms(50);
}

static void setMotorSteer(int steer)
{
	float setSteer = SteerMap[steer];
	TPM_UpdatePwmDutycycle(TPM0, kTPM_Chnl_5,
				kTPM_EdgeAlignedPwm, setSteer);
}


//**************************************************************************************************
//* GLOBAL FUNCTIONS
//**************************************************************************************************


//-------------------
//------ SPEED ------
//-------------------
void addSpeed()
{
	if (currentSpeed == MAX_FORWARD_ENGINE_LIMIT) return;

	currentSpeed++;
	setMotorSpeed(currentSpeed, 0, 0);
}

void addSpeedCustom(int speed)
{
	if ((currentSpeed + speed) >= MAX_FORWARD_ENGINE_LIMIT)
	{
		currentSpeed = MAX_FORWARD_ENGINE_LIMIT;
	}
	else
	{
		currentSpeed += speed;
	}

	setMotorSpeed(currentSpeed, 0, 0);
}

void slackUpSpeed()
{
	if (currentSpeed == MAX_REVERSE_ENGINE_LIMIT) return;

	currentSpeed--;
	if(currentSpeed < ENGINE_STOP)
	{
		currentSpeed = ENGINE_STOP + 1;
	}
	setMotorSpeed(currentSpeed, 0, 0);
}

void slackUpSpeedCustom(int speed)
{
	if ((currentSpeed - speed) < ENGINE_STOP)
	{
		currentSpeed = ENGINE_STOP + 1;
	}
	else
	{
		currentSpeed -= speed;
	}

	setMotorSpeed(currentSpeed, 0, 0);
}

void slackUpSpeedOnWheel(uint8_t slackAmount)
{
	uint8_t turningSide = (currentSteer > GO_DIRECT) ? TURNING_RIGHT : TURNING_LEFT;
	setMotorSpeed(currentSpeed, turningSide, slackAmount);
}

void goBackwards()
{
	currentSpeed = MAX_REVERSE_ENGINE_LIMIT;
	setMotorSpeed(currentSpeed, 0, 0);
}

void stopCar()
{
	currentSpeed = ENGINE_STOP;
	setMotorSpeed(currentSpeed, 0, 0);
}

void hardStop()
{
	IsCmdToStopCar = true;

	currentSpeed = ENGINE_STOP;
	setMotorSpeed(currentSpeed, 0, 0);
}


//-------------------
//------ STEER ------
//-------------------
void turnRight()
{
	if(currentSteer == MAX_STEER_RIGHT) return;

	PRINTF("Turning Right\r\n");
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

	PRINTF("Turning Right\r\n");
	setMotorSteer(currentSteer);
}

void turnLeft()
{
	if(currentSteer == MAX_STEER_LEFT) return;

	PRINTF("Turning Left\r\n");
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

	PRINTF("Turning Left\r\n");
	setMotorSteer(currentSteer);
}

void goDirect()
{
	PRINTF("Going direct\r\n");
	currentSteer = GO_DIRECT;
	setMotorSteer(currentSteer);
}


















