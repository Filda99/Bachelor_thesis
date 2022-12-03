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
//! Current speed of right motor. Left motor is calculated when writing
//! new speed.
//float _RightMotorSpeed = 7.365;

//! Current steer of car.
//float CurrentSteer = 7.365;

//! Number of wheel rotations
//! This is incremented every time hall sensor is in logic 0
float distance = 0.0;

line_location_t lineDetected = LineNone;

bool isCmdToStopCar = false;

/************************************
 * MACROS AND DEFINES
 ************************************/
