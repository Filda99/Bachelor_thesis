/**
 ***************************************************************************************************
 * @file    control_unit.c
 * @author  user
 * @date    Dec 1, 2022
 * @brief
 ***************************************************************************************************
 */

//**************************************************************************************************
//* INCLUDES
//**************************************************************************************************
#include "global_macros.h"
#include "motors/engines.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

//**************************************************************************************************
//* EXTERN VARIABLES
//**************************************************************************************************
extern unsigned char LineDetected;

//**************************************************************************************************
//* PRIVATE MACROS AND DEFINES
//**************************************************************************************************

//**************************************************************************************************
//* PRIVATE TYPEDEFS
//**************************************************************************************************

//**************************************************************************************************
//* STATIC VARIABLES
//**************************************************************************************************
static line_location_t prevTurning = LineNone;
static uint8_t lineCnt = 0;

//**************************************************************************************************
//* GLOBAL VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTION PROTOTYPES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTIONS
//**************************************************************************************************

//**************************************************************************************************
//* GLOBAL FUNCTIONS
//**************************************************************************************************

void setWheelToInitPosition()
{
	//setInitSpeed();
	addSpeed();
}

//!*************************************************************************************************
//! void controlUnit(void)
//!
//! @description
//! TBD
//!
//! @param	None
//!
//! @return	None
//!*************************************************************************************************
void controlUnit()
{
	PRINTF("Control unit: \r\n");
	static uint32_t distanceWithoutInterrupt = 0;

	if (LineDetected & LineNone)
	{
		PRINTF("- Line detected.: None r\n");
		//addSpeed();
		goDirect();
		PRINTF("-> Go direct. \r\n");
		distanceWithoutInterrupt++;

		// If car goes straight for too long
		/*if (distanceWithoutInterrupt > MAX_DISTANCE_WITHOUT_IRQ_LINE)
		{
			stopCar();
		}*/
		LineDetected &= ~LineNone;
		prevTurning = LineNone;
	}

	if (LineDetected & LineLeft)
	{
		PRINTF("- Line detected.: Left \r\n");
		if (prevTurning == LineLeft)
		{
			lineCnt++;
		}
		else
		{
			lineCnt = 0;
		}

		if (lineCnt > MAX_CNT_ON_LINE)
		{
			PRINTF("-> Turning left. \r\n");
			turnRight();
			//slackUpSpeed();
		}

		prevTurning = LineLeft;
		LineDetected &= ~LineLeft;
	}

	if (LineDetected & LineRight)
	{
		if (prevTurning == LineRight)
		{
			lineCnt++;
		}
		else
		{
			lineCnt = 0;
		}

		if (lineCnt > MAX_CNT_ON_LINE)
		{
			turnLeft();
			slackUpSpeed();
		}

		prevTurning = LineRight;
		LineDetected &= ~LineRight;
	}

	if (LineDetected & LineCenter_Left)
	{
		turnRightCustom(MAX_STEER_RIGHT);
		slackUpSpeedCustom(2);
		LineDetected &= ~LineCenter_Left;
	}

	if (LineDetected & LineCenter_Right)
	{
		turnLeftCustom(MAX_STEER_LEFT);
		slackUpSpeedCustom(2);
		LineDetected &= ~LineCenter_Right;
	}
}
