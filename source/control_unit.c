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
	static uint32_t distanceWithoutInterrupt = 0;

	if (LineDetected & LineNone)
	{
		addSpeed();
		goDirect();
		distanceWithoutInterrupt++;

		// If car goes straight for too long
		if (distanceWithoutInterrupt > MAX_DISTANCE_WITHOUT_IRQ_LINE)
		{
			stopCar();
		}
		prevTurning = LineNone;
	}

	if (LineDetected & LineLeft)
	{
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
			turnRight();
			slackUpSpeed();
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
