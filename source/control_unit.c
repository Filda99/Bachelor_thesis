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
extern unsigned int HalfWheelRotations;

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
	switch(LineDetected)
	{
		case LineNone:
		{
			// If car goes straight for too long
			if (HalfWheelRotations > MAX_DISTANCE_WITHOUT_LINE)
			{
				PRINTF("\t-> Distance without interrupt -> STOP. \r\n");
				stopCar();
			}
			// If there is no line for some time, add speed
			else if (HalfWheelRotations > CNT_OUT_OF_LANE)
			{
				PRINTF("\t-> Add speed. \r\n");
				//addSpeed();
				goDirect();
			}

			prevTurning = LineNone;
			break;
		}

		case LineLeft:
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
				PRINTF("\t-> Turning right! \r\n");
				turnRight();
				slackUpSpeedCustom(2);
			}

			prevTurning = LineLeft;
			break;
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
			break;
		}

		case LineCenter_Left:
		{
			turnRightCustom(MAX_STEER_RIGHT);
			slackUpSpeedCustom(2);
			LineDetected &= ~LineCenter_Left;
			break;
		}

		case LineCenter_Right:
		{
			turnLeftCustom(MAX_STEER_LEFT);
			slackUpSpeedCustom(2);
			LineDetected &= ~LineCenter_Right;
			break;
		}

		case LineCenter_None:
		{
			// todo: go backwards until some sensor detects line
			goDirect();
			goBackwards();
			break;
		}
	}

}
