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
#include "routine.h"

//**************************************************************************************************
//* EXTERN VARIABLES
//**************************************************************************************************
extern unsigned int HalfWheelRotations;
extern vector masterVector;
extern vector vLeft, vRight;

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


//!*************************************************************************************************
//! static void turningByAngle(float angle)
//!
//! @description
//! Function will calculate if we should start turning by the given angle.
//! If the angle is greater than we just can go direct, start turning to that side.
//!
//! @param	float angle		Angle of the curve we are heading
//!
//! @return	None
//!*************************************************************************************************
static void turningByAngle(float angle)
{
	static uint16_t counter = 0;
	static float prevAngle = 90.0;	// TODO

	if (angle >= 85 && angle <= 95)
	{
		goDirect();
	}
	// Turning left
	else if (angle >= 75 && angle <= 85)
	{
		turnLeft();
	}
	// Turning left
	else if (angle >= 60 && angle <= 75)
	{
		turnLeftCustom(2);
	}
	// Turning left
	else if (angle <= 60)
	{
		turnLeftCustom(MAX_STEER_LEFT);
	}

	// Turning right
	else if (angle >= 95 && angle <= 105)
	{
		turnRight();
	}
	// Turning right
	else if (angle >= 105 && angle <= 120)
	{
		turnRightCustom(2);
	}
	// Turning right
	else if (angle >= 120)
	{
		turnRightCustom(MAX_STEER_RIGHT);
	}

	if (counter++ > 50)
	{
		addSpeed();
	}

	prevAngle = angle;
}

//!*************************************************************************************************
//! static void turningByPosition()
//!
//! @description
//! Function will start turning, if we are way out from the center.
//!
//! @param	None
//!
//! @return	None
//!*************************************************************************************************
static void turningByPosition()
{
	if (vLeft.initial.x >= MAX_LEFT_X)
	{
		turnRight();
	}
	else if (vRight.initial.x <= MAX_RIGHT_X)
	{
		turnLeft();
	}
}

//**************************************************************************************************
//* GLOBAL FUNCTIONS
//**************************************************************************************************

// TODO
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
void controlUnit(float angle)
{
	turningByAngle(angle);
	turningByPosition();
}
