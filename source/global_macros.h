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
#include "fsl_tpm.h"

/************************************
 * MACROS AND DEFINES
 ************************************/

/*************************************/
// Map size
/*************************************/
#define BLK_ROWS  3
#define BLK_COLS  3

/*************************************/
// PWM for STOP the car
/*************************************/
#define STOP			7.365

/*************************************/
// Hall sensor
/*************************************/
#define GPIO_HALL 		PORTD
#define HALL_PIN			2

// Clear hall sensor
#define HALL_IRQ_MASK	0x04

/*************************************/
// Color sensors
/*************************************/
#define GPIO_COLOR_MAIN_SEN		PORTD
#define LEFT_MAIN_SEN_PIN		0
#define RIGHT_MAIN_SEN_PIN		4
#define CENTER_MAIN_SEN_PIN		3

#define MAIN_SEN_TPM_BASE		TPM0
#define MAIN_SEN_TPM_IRQ		TPM0_IRQn


#define GPIO_COLOR_MINOR_SEN	PORTA
#define LEFT_MINOR_SEN_PIN		0
#define RIGHT_MINOR_SEN_PIN		5

// Helping pins for setting color sensors
#define GPIO_SET_UP_COLOR_SEN	PORTC
#define S0S2_COLOR_SEN			4
#define S1S3_COLOR_SEN			5

/*************************************/
// Distance 
/*************************************/
#define MAX_DISTANCE_WITHOUT_IRQ_LINE	25000

/*************************************/
// Line detected counter
/*************************************/
// Counter which says how long are we over the line
#define MAX_CNT_ON_LINE	20

// If there is white color under sensor, no more than 2500 will occure
#define COLOR_TRESHOLD	2500

/*************************************/
// Engine
/*************************************/
#define MAX_SPEED_FORWARDS	11
#define MAX_SPEED_BACKWARDS	0
#define MAX_STEER_LEFT		0
#define MAX_STEER_RIGHT		7

#define STOP_SPEED			5
#define GO_DIRECT			3

/************************************
 * TYPEDEFS
 ************************************/
typedef enum _line_location {
	LineLeft = 0U,		// Line on the left side detected
	LineRight,				// Line on the right side detected
	LineCenter_Left,	// Line in the middle detected, on the left before
	LineCenter_Right,	// Line in the middle detected, on the right before
	LineNone					// No line detected
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
