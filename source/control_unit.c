/**
 ***************************************************************************************************
 * @file    control_unit.c
 * @author  xjahnf00
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
extern uint8_t currentSpeed;

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
			goDirect();
			// If there is no line for some time, add speed
			if (HalfWheelRotations > CNT_OUT_OF_LANE)
			{
				PRINTF("\t-> Add speed. \r\n");
				//addSpeed();
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

				turnRightCustom(MAX_STEER_RIGHT);

				//PRINTF("\t-> Turning right! \r\n");
				/*if (currentSteer >= 2)
				{
					turnRight();
				}
				else if (currentSteer == 1)
				{
					turnRightCustom(2);
					slackUpSpeedOnWheel(1);
				}
				else if (currentSteer == 0)
				{
					turnRightCustom(MAX_STEER_LEFT);
					slackUpSpeedOnWheel(2);
				}

				if (currentSpeed > 2)
				{
					slackUpSpeed();
				}*/


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
				turnLeftCustom(MAX_STEER_LEFT);

				//PRINTF("\t-> Turning left! \r\n");
				/*if (currentSteer <= 4)
				{
					turnLeft();
				}
				else if (currentSteer == 5)
				{
					turnLeftCustom(2);
					slackUpSpeedOnWheel(1);
				}
				else if (currentSteer == 6)
				{
					turnLeftCustom(MAX_STEER_LEFT);
					slackUpSpeedOnWheel(2);
				}

				if (currentSpeed > 2)
				{
					slackUpSpeed();
				}*/


				lineCnt = 0;
			}

			stopCarCnt = 0;
			prevTurning = LineRight;
			break;
		}

		case LineCenter_Left:
		{
			//slackUpSpeed();
			turnRightCustom(MAX_STEER_LEFT);
			//slackUpSpeedOnWheel(2);
			//PRINTF("\t-> Turning HARD right! \r\n");
			stopCarCnt = 0;
			break;
		}

		case LineCenter_Right:
		{
			//slackUpSpeed();
			turnLeftCustom(MAX_STEER_LEFT);
			//slackUpSpeedOnWheel(2);
			//PRINTF("\t-> Turning HARD left! \r\n");
			stopCarCnt = 0;
			break;
		}

		/*case LineCenter_None:
		{
			// todo: go backwards until some sensor detects line
			goDirect();
			goBackwards();
			stopCarCnt = 0;
			break;
		}*/
	}

}
