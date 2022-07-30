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
//! Current speed of car in pct - pwm duty cycle
//	50% = stop
//	 0% = max backwards
// 100% = max forward
int8_t CurrentSpeed = 50;

//! Current steer of car in pct - pwm duty cycle
//	50% = straight
//	0%  = max right
// 100% = max left
int8_t CurrentSteer = 50;

/************************************
 * MACROS AND DEFINES
 ************************************/





#ifdef __cplusplus
}
#endif

#endif /* GLOBALIO_H_ */
