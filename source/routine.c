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

//**************************************************************************************************
//* EXTERN VARIABLES
//**************************************************************************************************
extern line_location_t LineDetected;
extern unsigned int LeftSensorValue;
extern unsigned int CenterSensorValue;
extern unsigned int RightSensorValue;

// todo: remove. This is only for program without car.
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

//**************************************************************************************************
//* GLOBAL VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTION PROTOTYPES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTIONS
//**************************************************************************************************

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
		LineDetected |= LineLeft;
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
		LineDetected |= LineNone;
	}

	if(enableIrq >= 100)
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
void routine(void)
{
	PRINTF("----------------\r\n");
	//checkLines();
	//controlUnit();
	HalfWheelRotations++;
	mapping();
}

