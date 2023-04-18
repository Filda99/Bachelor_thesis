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
//! Keep it odd!
#define MAP_ROWS		2
//* User defined, how many columns should one map block have
	//! Keep it odd!
#define MAP_COLUMNS		2

//* First useable row
#define MAP_BLOCK_MIN_ROW	0
//* First useable column
#define MAP_BLOCK_MIN_COL	0

//* Last useable row in map block
#define MAP_BLOCK_MAX_ROW  (MAP_ROWS - 1)
//* Last useable column in map block
#define MAP_BLOCK_MAX_COL  (MAP_COLUMNS - 1)

//* Size of one field in map block in centimeters
#define MAP_BLOCK_SIZE	10

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


/*************************************/
// Distance 
/*************************************/
// Counter how long are we going straight without any interrupt
#define MAX_DISTANCE_WITHOUT_LINE	20000

/*************************************/
// Line detected counter
/*************************************/
// Maximum number which says how long can we be on the line
#define MAX_CNT_ON_LINE	3
// The number that tells us after how long we should increase the speed
// if there is no line
#define CNT_OUT_OF_LANE 20

// If there is white color under sensor, no more than 2500 will occure
#define COLOR_TRESHOLD	100

/*************************************/
// Engine
/*************************************/
#define MAX_FORWARD_ENGINE_LIMIT	2//6
#define MAX_REVERSE_ENGINE_LIMIT	0

#define MAX_STEER_RIGHT		0
#define MAX_STEER_LEFT		6

#define ENGINE_STOP			1	// Pulse width for STOP is the second value in the speed field
#define GO_DIRECT			(MAX_STEER_LEFT / 2)

/*************************************/
// I2C
/*************************************/
#define SAVING_I2C	I2C0
#define DATA_I2C	I2C1

#define I2C_DATA_LENGTH	6

/************************************
 * TYPEDEFS
 ************************************/
typedef enum _line_location {
	LineLeft  = 0b1,			// Line on the left side detected
	LineRight = 0b10,			// Line on the right side detected
	LineCenter_Left = 0b100,	// Line in the middle detected, on the left before
	LineCenter_Right = 0b1000,	// Line in the middle detected, on the right before
	LineCenter_None,			// Line in the middle detected, be we didn't detected any line before
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
