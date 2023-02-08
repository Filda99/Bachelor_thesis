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
	static uint32_t cntLineNone = 0;

	switch(LineDetected)
	{
		case LineNone:
		{
			PRINTF("\t- Line detected.: None r\n");
			PRINTF("\t-> Go direct. \r\n");
			cntLineNone++;
			distanceWithoutInterrupt++;

			// If car goes straight for too long
			if (distanceWithoutInterrupt > MAX_DISTANCE_WITHOUT_IRQ_LINE)
			{
				stopCar();
			}
			// If there is no line for some time, add speed
			else if (cntLineNone > CNT_OUT_OF_LANE)
			{
				addSpeed();
				goDirect();
			}

			prevTurning = LineNone;
		}

		case LineLeft:
		{
			PRINTF("\t- Line detected.: Left \r\n");
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
				PRINTF("\t-> Turning left. \r\n");
				turnRight();
				slackUpSpeedCustom(2);
			}

			prevTurning = LineLeft;
		}

		case LineRight:
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
		}

		case LineCenter_Left:
		{
			turnRightCustom(MAX_STEER_RIGHT);
			slackUpSpeedCustom(2);
			LineDetected &= ~LineCenter_Left;
		}

		case LineCenter_Right:
		{
			turnLeftCustom(MAX_STEER_LEFT);
			slackUpSpeedCustom(2);
			LineDetected &= ~LineCenter_Right;
		}

		case LineCenter_None:
		{
			// todo: go backwards until some sensor detects line
			goDirect();
			goBackwards();
		}
	}

}
