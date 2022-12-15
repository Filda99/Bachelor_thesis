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


void checkLeftSen()
{
	static int enableIrq = 0;
	static uint32_t prevVal = 0;

	if (prevVal != LeftSensorValue)
	{
		LineDetected = LineLeft;
	}
	prevVal = LeftSensorValue;

	if(enableIrq >= 100)
	{
		EnableIRQ(MAIN_SEN_TPM_IRQ);
		enableIrq = 0;
	}
	enableIrq++;
}

static void checkLine()
{
	checkLeftSen();
	//checkRightSen();
	//checkCenterSen();
}

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void routine(void)
{
	checkLine();
	//controlUnit();
}

