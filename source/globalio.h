/**
 ********************************************************************************
 * @file    globalio.h
 * @author  user
 * @date    Jul 18, 2022
 * @brief
 ********************************************************************************
 */

#ifndef GLOBALIO_H_
#define GLOBALIO_H_


#ifdef __cplusplus
extern "C" {
#endif


/************************************
 * GLOBAL VARIABLES
 ************************************/
//! Current speed of right motor. Left motor is calculated when writing
//! new speed.
float RightMotorSpeed = 7.365;

//! Current steer of car.
float CurrentSteer = 7.365;

/************************************
 * MACROS AND DEFINES
 ************************************/
#define STOP			7.365


#ifdef __cplusplus
}
#endif

#endif /* GLOBALIO_H_ */
