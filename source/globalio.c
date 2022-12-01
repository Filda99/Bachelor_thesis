/**
 ********************************************************************************
 * @file    globalio.h
 * @author  user
 * @date    Jul 18, 2022
 * @brief
 ********************************************************************************
 */


/************************************
 * GLOBAL VARIABLES
 ************************************/
//! Current speed of right motor. Left motor is calculated when writing
//! new speed.
float RightMotorSpeed = 7.365;

//! Current steer of car.
float CurrentSteer = 7.365;

//! Number of wheel rotations
//! This is incremented every time hall sensor is in logic 0
unsigned int noOfRotations = 0;

uint8_t lineDetected = 0;

/************************************
 * MACROS AND DEFINES
 ************************************/
