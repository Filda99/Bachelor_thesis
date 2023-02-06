/**
 ***************************************************************************************************
 * @file    globalio.h
 * @author  user
 * @date    Jul 18, 2022
 * @brief
 ***************************************************************************************************
 */

//**************************************************************************************************
//* INCLUDES
//**************************************************************************************************
#include "global_macros.h"
#include "stdbool.h"

//**************************************************************************************************
//* GLOBAL VARIABLES
//**************************************************************************************************
//* Number of wheel rotations
unsigned int HalfWheelRotations = 0;

//* Which lines are detected at the moment
uint8_t LineDetected = LineNone;

// Stop car, because some error has occured
// When IsCmdToStopCar is true, no more action will be taken
// Last thing is to save map
bool IsCmdToStopCar = false;

// In initial initialization there is need to rotate wheel to starting position and hold this
// position until car starts. With this we can be sure, that distance measurement is correct.
bool InitStop = true;

// Left sensor color value
unsigned int LeftSensorValue = 0;
unsigned int CenterSensorValue = 0;
unsigned int RightSensorValue = 0;
