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

/************************************
 * EXTERN VARIABLES
 ************************************/
extern float RightMotorSpeed;

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define MAX_FORWARD		8
#define MAX_BACKWARD	5.25
#define SLOW_FORWARD	7.565

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

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void routine(void)
{
	while(1)
		{
			//vTaskDelay(2000);
			RightMotorSpeed = STOP;
			setMotorSpeed(RightMotorSpeed);
			//vTaskDelay(2000);

			//RightMotorSpeed = SLOW_FORWARD;
			//setMotorSpeed(RightMotorSpeed);
			//vTaskDelay(2000);

			//RightMotorSpeed = MAX_FORWARD;
			//setMotorSpeed(RightMotorSpeed);
			//vTaskDelay(2000);
		}


}

