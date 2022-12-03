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

/************************************
 * MACROS AND DEFINES
 ************************************/
#define STOP			7.365

/*************************************/
// Hall sensors 
/*************************************/
#define GPIO_HALL 		PORTD
#define HALL_LEFT		2
#define HALL_RIGHT		3

// Clear both hall sensors
#define HALL_IRQ_MASK	0x06

/*************************************/
// Color sensors
/*************************************/
#define GPIO_COLOR_MAIN_SEN		PORTD
#define LEFT_MAIN_SEN		0
#define RIGHT_MAIN_SEN		1
#define CENTER_MAIN_SEN		2

#define GPIO_COLOR_MINOR_SEN	PORTA
#define LEFT_MINOR_SEN		4
#define RIGHT_MINOR_SEN		5

// Clear sensors
#define COLOR_MAIN_IRQ_MASK		0x45
#define COLOR_MINOR_IRQ_MASK	0x18

// Helping pins
#define S0S2_COLOR_SEN		4
#define S1S3_COLOR_SEN		5

/*************************************/
// Distance 
/*************************************/
#define MAX_DISTANCE_WITHOUT_IRQ_LINE	25000

/*************************************/
// Line detected counter
/*************************************/
#define MAX_CNT_ON_LINE	20

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
