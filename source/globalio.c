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

// Stop car, because some error has occured
// When IsCmdToStopCar is true, no more action will be taken
// Last thing is to save map
bool IsCmdToStopCar = false;

// In initial initialization there is need to rotate wheel to starting position and hold this
// position until car starts. With this we can be sure, that distance measurement is correct.
bool InitStop = true;

//! Current position in speed array.
//! 0 = max backwards
//! 5 = stop engines
//! 11 = max forwards
uint8_t currentSpeed = ENGINE_STOP;

//! Current position in steer array.
//! 0 = max left
//! 3 = direct
//! 6 = max right
uint8_t currentSteer = GO_DIRECT;
