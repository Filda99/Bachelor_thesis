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
#include "control_unit.h"
#include "MKL25Z4.h"
#include "fsl_tpm.h"
#include "fsl_gpio.h"

/************************************
 * EXTERN VARIABLES
 ************************************/
extern unsigned char LineDetected;
extern unsigned LeftSensorValue;

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
#include "fsl_debug_console.h"
volatile bool tpmIsrFlag = false;





void checkLeftSen()
{
	//PRINTF("Left sensor value = %x\r\n", LeftSensorValue);

	/*time_t timeRise;
	time_t timeFall;


	while(!GPIO_ReadPinInput(PORTD, 0));
	time(&timeRise);
	time()
	while(GPIO_ReadPinInput(PORTD, 0));
	time(&timeFall);

	uint32_t timeSum = timeFall - timeRise;
	PRINTF("\r\nCapture value C(n)V=%x\r\n", timeSum);
*/
}

static void checkLine()
{
	checkLeftSen();


	/*while (tpmIsrFlag != true)
	{
	}

	uint32_t statusLeft = TPM0->CONTROLS[kTPM_Chnl_0].CnV;
	PRINTF("\r\nCapture value C(n)V=%x\r\n", statusLeft);
	while (1)
	{
	}*/
	/*
	static uint8_t prevLine = LineNone;
	//uint32_t statusLeft = TPM0->CONTROLS[kTPM_Chnl_0].CnV;
	uint16_t statusLeft = TPM_CnV_VAL(0);
	uint16_t statusRight = TPM_STATUS_CH0F(1);
	uint16_t statusCenter = TPM_STATUS_CH0F(2);

	int temp = 0;
    PRINTF("Left: %i\r\n", statusLeft);
	if (statusLeft > temp)
	{
		LineDetected = LineLeft;
		prevLine = LineLeft;
	}
	if (statusRight > temp)
	{
		LineDetected = LineRight;
		prevLine = LineRight;
	}
	if (statusCenter > temp)
	{
		if (prevLine == LineLeft)
		{
			LineDetected = LineCenter_Left;
		}
		else
		{
			LineDetected = LineCenter_Right;
		}
	}*/
}

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void routine(void)
{
	checkLine();
	//controlUnit();
}

