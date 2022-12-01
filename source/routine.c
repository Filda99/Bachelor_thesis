/**
 ********************************************************************************
 * @file    routine.c
 * @author  user
 * @date    Jul 18, 2022
 * @brief
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "routine.h"
#include "motors/engines.h"
#include "global_macros.h"
#include "globalio.c"

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
static void checkLine()
{
	static prevLine = LineNone;
	uint16_t statusLeft = TPM0_C0V;
	uint16_t statusRight = TPM0_C1V;
	uint16_t statusCenter = TPM0_C2V;

	int temp = 0;
	if (statusLeft > temp)
	{
		lineDetected = LineLeft;
		prevLine = LineLeft;
	}
	if (statusRight > temp)
	{
		lineDetected = LineRight;
		prevLine = LineRight;
	}
	if (statusCenter > temp)
	{
		if (prevLine == LineLeft)
		{
			lineDetected = LineCenter_Left;
		}
		else
		{
			lineDetected = LineCenter_Right;
		}
	}
}

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void routine(void)
{
	while(1)
	{
		checkLine();
		//vTaskDelay(2000);
		//RightMotorSpeed = STOP;
		//setMotorSpeed(RightMotorSpeed);
		//vTaskDelay(2000);

		//RightMotorSpeed = SLOW_FORWARD;
		//setMotorSpeed(RightMotorSpeed);
		//vTaskDelay(2000);

		//RightMotorSpeed = MAX_FORWARD;
		//setMotorSpeed(RightMotorSpeed);
		//vTaskDelay(2000);
	}


}

