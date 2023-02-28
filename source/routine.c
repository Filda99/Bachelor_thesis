/**
 ********************************************************************************
 * @file    routine.c
 * @author  user
 * @date    Jul 18, 2022
 * @brief
 ********************************************************************************
 */

//**************************************************************************************************
//* INCLUDES
//**************************************************************************************************
#include "routine.h"
#include "MKL25Z4.h"
#include "global_macros.h"

#include "fsl_tpm.h"
#include "fsl_gpio.h"
#include "fsl_debug_console.h"

#include "motors/engines.h"
#include "control_unit.h"
#include "map/mapping.h"
#include "map/map_operations.h"

//**************************************************************************************************
//* EXTERN VARIABLES
//**************************************************************************************************
extern line_location_t LineDetected;
extern unsigned int LeftSensorValue;
extern unsigned int CenterSensorValue;
extern unsigned int RightSensorValue;

// todo: remove. This is only for program without car.
extern unsigned int HalfWheelRotations;
extern map_block *currentBlockInMap;
//**************************************************************************************************
//* PRIVATE MACROS AND DEFINES
//**************************************************************************************************

//**************************************************************************************************
//* PRIVATE TYPEDEFS
//**************************************************************************************************

//**************************************************************************************************
//* STATIC VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* GLOBAL VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTION PROTOTYPES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTIONS
//**************************************************************************************************

static void printBlock()
{
	PRINTF("Current block looks like this:\r\n");
	for (int i = 0; i < MAP_ROWS; i++)
	{
		for (int j = 0; j < MAP_COLUMNS; j++)
		{
			PRINTF(" %d |", currentBlockInMap->currentBlock[i][j]);
		}
		PRINTF("\r\n");
	}
}

//!*************************************************************************************************
//! void checkLine(void)
//!
//! @description
//! Check left, right and center sensors value.
//! If the value is greater than COLOR_TRESHOLD, LineDetected is set to that side.
//! That means that we detected line under the sensor.
//!
//! Function after 100 cycles enables interrupts for main sensors.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
static void checkLines()
{
	static int enableIrq = 0;
	bool lineDetected = false;

	if (LeftSensorValue < COLOR_TRESHOLD)
	{
		LineDetected = LineLeft;
		lineDetected = true;
	}
	/*if (RightSensorValue < COLOR_TRESHOLD)
	{
		LineDetected |= LineRight;
		lineDetected = true;
	}
	if (CenterSensorValue < COLOR_TRESHOLD)
	{
		// TODO: LineDetected |= LineCenter;
		lineDetected = true;
	}*/


	if (!lineDetected)
	{
		LineDetected = LineNone;
	}

	if(enableIrq >= 10)
	{
		EnableIRQ(MAIN_SEN_TPM_IRQ);
		enableIrq = 0;
	}
	enableIrq++;
	PRINTF("Left sensor value: %i\r\n", LeftSensorValue);
}

//**************************************************************************************************
//* GLOBAL FUNCTIONS
//**************************************************************************************************

//!*************************************************************************************************
//! void function(void)
//!
//! @description
//! Function
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
void routine(void)
{
	HalfWheelRotations++;
	//checkLines();
	//controlUnit();

	if (HalfWheelRotations == 1)
	{
		turnRightCustom(3);
	}
	if (HalfWheelRotations == 26)
	{
		turnLeftCustom(6);
	}

	static int i = 0;
	PRINTF("Cycle: %i\r\n", i++);
 	mapping();
 	printBlock();
	PRINTF("-------------------------------------\r\n");
}

