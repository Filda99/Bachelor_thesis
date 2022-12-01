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

// Hall sensors *********************/
#define GPIO_HALL 		PORTD
#define HALL_LEFT		2
#define HALL_RIGHT		3

// Clear both hall sensors
#define HALL_IRQ_MASK	0x06

// Color sensors *********************/
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

/************************************
 * TYPEDEFS
 ************************************/
typedef enum _line_location {
	LineLeft = 0U,
	LineRight,
	LineCenter_Left,
	LineCenter_Right,
	LineNone
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
