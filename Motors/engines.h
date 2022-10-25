/**
 ********************************************************************************
 * @file    engines.h
 * @author  user
 * @date    Jul 18, 2022
 * @brief   
 ********************************************************************************
 */

#ifndef ENGINES_H_
#define ENGINES_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/

/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/

//! In middle position there is state for STOP.
//! Otherwise we add/sub 0.127 for higher/slower speed.
static float speed[11] = {
		6.73,
		6.857,
		6.984,
		7.111,
		7.238,
		7.365,
		7.492,
		7.619,
		7.746,
		7.873,
		8.0
};

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void setMotorSpeed(float RightMotorSpeed);

#ifdef __cplusplus
}
#endif

#endif /* ENGINES_H_ */
