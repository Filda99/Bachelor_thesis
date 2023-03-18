/**
 ********************************************************************************
 * @file    global_macros.h
 * @author  user
 * @date    Sep 16, 2022
 * @brief   
 ********************************************************************************
 */

#ifndef GLOBAL_MACROS_H_
#define GLOBAL_MACROS_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include "../drivers/fsl_tpm.h"

/************************************
 * MACROS AND DEFINES
 ************************************/

/*************************************/
// Hall sensor
/*************************************/
#define GPIO_HALL 		PORTD
#define HALL_PIN		2

// Clear hall sensor
#define HALL_IRQ_MASK	0x04

/*************************************/
// Camera
/*************************************/


/*************************************/
// Distance 
/*************************************/
// Counter how long are we going straight without any interrupt
#define MAX_DISTANCE_WITHOUT_LINE	20000

/*************************************/
// Line detected counter
/*************************************/
// Maximum number which says how long can we be on the line
#define MAX_CNT_ON_LINE	40
// The number that tells us after how long we should increase the speed
// if there is no line
#define CNT_OUT_OF_LANE 80

// If there is white color under sensor, no more than 2500 will occure
#define COLOR_TRESHOLD	40

// TODO: rozmery kamery
#define MAX_LEFT_X	250
#define MAX_RIGHT_X	450

/*************************************/
// Engine
/*************************************/
#define MAX_FORWARD_ENGINE_LIMIT	6
#define MAX_REVERSE_ENGINE_LIMIT	0

#define MAX_STEER_RIGHT		6
#define MAX_STEER_LEFT		0

#define ENGINE_STOP			1	// Pulse width for STOP is the second value in the speed field
#define GO_DIRECT			(MAX_STEER_RIGHT / 2)

/*************************************/
// Math
/*************************************/
#define M_PI 3.1415926536

/*************************************/
// I2C
/*************************************/
#define CAM_I2C	I2C0

/************************************
 * TYPEDEFS
 ************************************/
typedef enum _line_location {
	LineLeft  = 1U,		// Line on the left side detected
	LineRight,			// Line on the right side detected
	LineCenter_Left,	// Line in the middle detected, on the left before
	LineCenter_Right,	// Line in the middle detected, on the right before
	LineCenter_None,	// Line in the middle detected, be we didn't detected any line before
	LineNone			// No line detected
}line_location_t;

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/

#ifdef __cplusplus
}
#endif

#endif 
