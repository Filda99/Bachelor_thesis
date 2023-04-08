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
#include <motors/engines.h>
#include "global_macros.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

//**************************************************************************************************
//* EXTERN VARIABLES
//**************************************************************************************************
extern unsigned char LineDetected;
extern unsigned int HalfWheelRotations;
extern uint8_t currentSteer;

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
	//PRINTF("Control unit: \r\n");
	static uint16_t stopCarCnt = 0;

	stopCarCnt++;
	// If car goes straight for too long
	/*if (stopCarCnt > MAX_DISTANCE_WITHOUT_LINE)
	{
		PRINTF("\t-> Distance without interrupt -> STOP. \r\n");
		stopCar();
	}*/

	switch(LineDetected)
	{
		case LineNone:
		{
			// If there is no line for some time, add speed
			if (HalfWheelRotations > CNT_OUT_OF_LANE)
			{
				PRINTF("\t-> Add speed. \r\n");
				addSpeed();
				goDirect();
			}

			prevTurning = LineNone;
			break;
		}

		case LineLeft:
		{
			//PRINTF("\t- Line detected: Left \r\n");
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
				if (currentSteer >= GO_DIRECT)
				{
					goDirect();
				}

				PRINTF("\t-> Turning right! \r\n");
				if (currentSteer == 2)
				{
					turnRight();
					slackUpSpeed();
				}
				else if (currentSteer == 1)
				{
					turnRightCustom(2);
					slackUpSpeed();
					slackUpSpeedOnWheel(1);
				}
				else if (currentSteer == 0)
				{
					turnRightCustom(MAX_STEER_LEFT);
					slackUpSpeedOnWheel(2);
				}

				lineCnt = 0;
			}
			stopCarCnt = 0;
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
				if (currentSteer <= GO_DIRECT)
				{
					goDirect();
				}

				PRINTF("\t-> Turning left! \r\n");
				if (currentSteer == 4)
				{
					turnLeft();
					slackUpSpeed();
				}
				else if (currentSteer == 5)
				{
					turnLeftCustom(2);
					slackUpSpeed();
					slackUpSpeedOnWheel(1);
				}
				else if (currentSteer == 6)
				{
					turnLeftCustom(MAX_STEER_LEFT);
					slackUpSpeedOnWheel(2);
				}
				lineCnt = 0;
			}

			stopCarCnt = 0;
			prevTurning = LineRight;
			break;
		}

		case LineCenter_Left:
		{
			turnRightCustom(MAX_STEER_RIGHT);
			slackUpSpeedCustom(2);
			PRINTF("\t-> Turning HARD right! \r\n");
			stopCarCnt = 0;
			break;
		}

		case LineCenter_Right:
		{
			turnLeftCustom(MAX_STEER_LEFT);
			slackUpSpeedCustom(2);
			PRINTF("\t-> Turning HARD left! \r\n");
			stopCarCnt = 0;
			break;
		}

		case LineCenter_None:
		{
			// todo: go backwards until some sensor detects line
			goDirect();
			goBackwards();
			stopCarCnt = 0;
			break;
		}
	}

}
