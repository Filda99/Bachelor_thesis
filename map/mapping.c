/**
 ***************************************************************************************************
 * @file    mapping.c
 * @author  user
 * @date    Dec 15, 2022
 * @brief   
 ***************************************************************************************************
 */

//**************************************************************************************************
//* INCLUDES
//**************************************************************************************************
#include "global_macros.h"
#include "stdint.h"
#include "map_operations.h"
#include <math.h>
#include "fsl_debug_console.h"
#include <stdio.h>

//**************************************************************************************************
//* EXTERN VARIABLES
//**************************************************************************************************
extern unsigned int HalfWheelRotations;
extern uint8_t currentSpeed;
extern uint8_t currentSteer;

extern uint8_t currentPosRows;
extern uint8_t currentPosCols;

//**************************************************************************************************
//* PRIVATE MACROS AND DEFINES
//**************************************************************************************************
#define X 0
#define Y 1

#define TURNING_LEFT	0
#define TURNING_RIGHT	180

# define M_PI           3.14159265358979323846

#define STARTING_COORDINATE_X	15
#define STARTING_COORDINATE_Y	10

//**************************************************************************************************
//* PRIVATE TYPEDEFS
//**************************************************************************************************

//**************************************************************************************************
//* STATIC VARIABLES
//**************************************************************************************************

//! The angle at which the robot is heading.
//! We start in upward direction, so it is 90 deg.
static float angleHeading = 90.0;
static float centerOfCircle[2] = {0.0};

//**************************************************************************************************
//* GLOBAL VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTION PROTOTYPES
//**************************************************************************************************

//**************************************************************************************************
 //* STATIC FUNCTIONS
//**************************************************************************************************
static void calc_new_position(float *current_x, float *current_y, uint32_t distance)
{
	static uint8_t previousSteer = GO_DIRECT;

	// TODO: get r from the knowledge of steering angle (table)
	int r = 5;
	float dir = 2 * M_PI * r / 4;
	// init angle is heading angle
	int init_angle = 0;
	int wheel_angle = (currentSteer > GO_DIRECT) ? TURNING_RIGHT : TURNING_LEFT;

	// If we have changed the steering and we are steering to some side, we need to calculate
	// the center of a circle.
	if (currentSteer != previousSteer && currentSteer != GO_DIRECT)
	{
		int angle = wheel_angle - init_angle;
		centerOfCircle[X] = *current_x - r * cos(angle*M_PI/180);
		centerOfCircle[Y] = *current_y - r * sin(angle*M_PI/180);
	    PRINTF("Center of circle coordinates: %d, %d\r\n", (int)centerOfCircle[X], (int)centerOfCircle[Y]);
	}

	// We are steering to some side
	if (currentSteer != GO_DIRECT)
	{
		// Calculate the angle between the initial point and the center of the circle
		float theta = atan2(*current_y - centerOfCircle[Y], *current_x - centerOfCircle[X]);

		// Calculate the angular displacement
		float delta_theta = dir / r;

		// Calculate the final angle
		float final_theta;
		if (wheel_angle == TURNING_LEFT)
		{
			final_theta = theta + delta_theta;
		}
		else
		{
			final_theta = theta - delta_theta;
		}

		// Calculate the x and y coordinates of the endpoint
		// We don't need to add result to current position, bcs
		// we are counting with the center, which tells us
		// the coordinates
		*current_x = centerOfCircle[X] + r * cos(final_theta);
		*current_y = centerOfCircle[Y] + r * sin(final_theta);
	}
	// We are going direct
	else
	{
		*current_x += distance * cos(angleHeading*M_PI/180);
		*current_y += distance * sin(angleHeading*M_PI/180);
	}
	previousSteer = currentSteer;
}

//**************************************************************************************************
//* GLOBAL FUNCTIONS
//**************************************************************************************************

//!*************************************************************************************************
//! void mapping(void)
//!
//! @description
//! Brief description
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
void mapping()
{
	static float current_x = STARTING_COORDINATE_X;
	static float current_y = STARTING_COORDINATE_Y;

	// distance = 2*pi*r
	// r = ~1.7
	// One rotation is ~10.5cm, so one HalfWheelRotation is 5cm
	uint32_t newRotations = 0;
	static uint32_t newDistance = 0; 	// Distance traveled from previous mapping
	static uint32_t prevNoWheelRot = 0; // Number of half-wheel rotations from previous mapping

	// Get distance from previous position
	newRotations = HalfWheelRotations - prevNoWheelRot;
	newDistance += newRotations * 5;

	// If the wheel moves a full turn, add one cm to distance due PI is float
	if ((HalfWheelRotations % 2) == 0)
	{
		newDistance += newRotations;
	}
	calc_new_position(&current_x, &current_y, newDistance);

	PRINTF("Endpoint coordinates: %d, %d\r\n", (int)current_x, (int)current_y);

	uint8_t moveToNewBlock_x = (int)ceil(current_x) / MAP_BLOCK_SIZE;
	uint8_t moveToNewBlock_y = (int)ceil(current_y) / MAP_BLOCK_SIZE;

	// Move in map when traveled to new block
	while (newDistance > MAP_BLOCK_SIZE)
	{
		// Check movement in the corner to next field
		if (moveToNewBlock_x != currentPosCols && moveToNewBlock_y != currentPosRows)
		{
			// Up-left
			if (moveToNewBlock_y > currentPosRows && moveToNewBlock_x > currentPosCols)
			{
				moveInMap(move_upLeft);
				newDistance -= MAP_BLOCK_SIZE;
			}

			// Up-right
			else if (moveToNewBlock_y > currentPosRows && moveToNewBlock_x > currentPosCols)
			{
				moveInMap(move_upRight);
				newDistance -= MAP_BLOCK_SIZE;
			}

			// Down-left
			else if (moveToNewBlock_y < currentPosRows && moveToNewBlock_x < currentPosCols)
			{
				moveInMap(move_downLeft);
				newDistance -= MAP_BLOCK_SIZE;
			}
			// Down-right
			else if (moveToNewBlock_y < currentPosRows && moveToNewBlock_x < currentPosCols)
			{
				moveInMap(move_downRight);
				newDistance -= MAP_BLOCK_SIZE;
			}
		}
		else if (moveToNewBlock_x != currentPosRows || moveToNewBlock_y != currentPosCols)
		{
			// Up
			/* ! 2D fields in C are computed from top-left as [0, 0].
			 * But if we move upwards, we will have higher number.
			 * -------------------------------------------------------
			 * e.g. we will be facing up, we will travel 5cm (one field is 4cm..)
			 * So we will have coordinates [0, 5]. But we need to travel up, so
			 * we have higher number in the coordinate system, but in the 2D field
			 * we will move up one square (from [1,1] in the beginning to [1, 0].
			 */

			if (moveToNewBlock_y > currentPosRows)
			{
				moveInMap(move_up);
				newDistance -= MAP_BLOCK_SIZE;
			}
			// Down
			else if (moveToNewBlock_y < currentPosRows)
			{
				moveInMap(move_down);
				newDistance -= MAP_BLOCK_SIZE;
			}
			// Left
			else if (moveToNewBlock_x < currentPosCols)
			{
				moveInMap(move_left);
				newDistance -= MAP_BLOCK_SIZE;
			}
			// Right
			else if (moveToNewBlock_x > currentPosCols)
			{
				moveInMap(move_right);
				newDistance -= MAP_BLOCK_SIZE;
			}
		}
		else
		{
			PRINTF("ERROR! I should have moved but still on the same position!\r\n");
		}
	}

	prevNoWheelRot = HalfWheelRotations;
}


