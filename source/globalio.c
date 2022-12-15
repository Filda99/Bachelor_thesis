/**
 ********************************************************************************
 * @file    globalio.h
 * @author  user
 * @date    Jul 18, 2022
 * @brief
 ********************************************************************************
 */
#include "global_macros.h"
#include "stdbool.h"

/************************************
 * GLOBAL VARIABLES
 ************************************/
//* Distance measured by wheel rotations
unsigned int Distance = 0.0;

//* Which line is detected at the moment
line_location_t LineDetected = LineNone;

// Stop car, because some error has occured
// When IsCmdToStopCar is true, no more action will be taken
// Last thing is to save map
bool IsCmdToStopCar = false;

// Left sensor color value
unsigned int LeftSensorValue = 0;
unsigned int CenterSensorValue = 0;
unsigned int RightSensorValue = 0;

unsigned char posX = BLK_ROWS / 2;
unsigned char posY = BLK_COLS / 2;

