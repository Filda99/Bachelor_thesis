/**
 ***************************************************************************************************
 * @file    vector.c
 * @author  user
 * @date    Mar 15, 2023
 * @brief   
 ***************************************************************************************************
 */

//**************************************************************************************************
//* INCLUDES
//**************************************************************************************************
#include "vector.h"
#include "math.h"
#include "stdint.h"
#include "global_macros.h"

//**************************************************************************************************
//* EXTERN VARIABLES
//**************************************************************************************************

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

//! Main vector, which is calculated in vectorCalculation() func.
struct vector masterVector;

//! Two vectors, which represents traffic lanes
struct vector vLeft, vRight = {
		.initial = {0, }, .final = {0, }
};


//**************************************************************************************************
//* STATIC FUNCTION PROTOTYPES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTIONS
//**************************************************************************************************

//**************************************************************************************************
//* GLOBAL FUNCTIONS
//**************************************************************************************************


//!*************************************************************************************************
//! void checkLine(void)
//!
//! @description
//! Get the data from the camera and based on that decide how much and where to turn.
//!
//!
//! @param    None
//!
//! @return   Returns angle of how much should we will face.
//!*************************************************************************************************
float vectorCalculation()
{
	// Get delta y, which one is longer will be main vector
	uint32_t deltaYLeft = vLeft.final.y - vLeft.initial.y;
	uint32_t deltaYRight = vRight.final.y - vRight.initial.y;

	masterVector = (deltaYLeft > deltaYRight) ? vLeft : vRight;

	// Get angle of a turn
	uint32_t deltaX = masterVector.final.x - masterVector.initial.x;
	uint32_t deltaY = masterVector.final.y - masterVector.initial.y;
	return (atan(deltaY / deltaX) * 180 / M_PI);
}
