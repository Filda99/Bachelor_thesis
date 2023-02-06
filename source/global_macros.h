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
// Map size
/*************************************/

//* User defined, how many rows should one map block have
#define MAP_ROWS		3
//* User defined, how many columns should one map block have
#define MAP_COLUMNS	3

//* First useable row
#define MAP_BLOCK_MIN_ROW	0
//* First useable column
#define MAP_BLOCK_MIN_COL	0

//* Last useable row in map block
#define MAP_BLOCK_MAX_ROW  (MAP_ROWS - 1)
//* Last useable column in map block
#define MAP_BLOCK_MAX_COL  (MAP_COLUMNS - 1)

//* Size of one field in map block in centimetres
#define MAP_BLOCK_SIZE	10	// One decimetre

/*************************************/
// Hall sensor
/*************************************/
#define GPIO_HALL 		PORTD
#define HALL_PIN		2

// Clear hall sensor
#define HALL_IRQ_MASK	0x04

/*************************************/
// Color sensors
/*************************************/
// Main color sensors GPIO
#define GPIO_COLOR_MAIN_SEN		PORTD
#define LEFT_MAIN_SEN_PIN		0
#define RIGHT_MAIN_SEN_PIN		4
#define CENTER_MAIN_SEN_PIN		3

// Main color sensor TPM
#define MAIN_SEN_TPM_BASE		TPM0
#define MAIN_SEN_TPM_IRQ		TPM0_IRQn

// Minor color sensors GPIO
#define GPIO_COLOR_MINOR_SEN	PORTA
#define LEFT_MINOR_SEN_PIN		0
#define RIGHT_MINOR_SEN_PIN		5

// Helping pins for setting up main color sensors
#define GPIO_SET_UP_COLOR_SEN	PORTC
#define S0S2_COLOR_SEN			4
#define S1S3_COLOR_SEN			5

/*************************************/
// Distance 
/*************************************/
// Counter how long are we going straight without any interrupt
#define MAX_DISTANCE_WITHOUT_IRQ_LINE	25000

/*************************************/
// Line detected counter
/*************************************/
// Counter which says how long are we over the line
#define MAX_CNT_ON_LINE	20

// If there is white color under sensor, no more than 2500 will occure
#define COLOR_TRESHOLD	40

/*************************************/
// Engine
/*************************************/
#define MAX_SPEED_FORWARDS	11
#define MAX_SPEED_BACKWARDS	0

#define MAX_STEER_RIGHT		7
#define MAX_STEER_LEFT		0

#define STOP_SPEED			((MAX_SPEED_FORWARDS - 1) / 2)
#define GO_DIRECT			((MAX_STEER_RIGHT - 1) / 2)

/************************************
 * TYPEDEFS
 ************************************/
typedef enum _line_location {
	LineLeft  = 1U,		// Line on the left side detected
	LineRight = 2U,				// Line on the right side detected
	LineCenter_Left = 4U,	// Line in the middle detected, on the left before
	LineCenter_Right = 8U,	// Line in the middle detected, on the right before
	LineNone = 16U					// No line detected
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
