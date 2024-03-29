/**
 ***************************************************************************************************
 * @file    mapping.c
 * @author  xjahnf00
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
#include "mapping.h"
#include <stdio.h>
#include "../motors/engines.h"

//**************************************************************************************************
//* EXTERN VARIABLES
//**************************************************************************************************
extern unsigned int HalfWheelRotations;
extern uint8_t currentSpeed;
extern uint8_t currentSteer;

extern curr_pos_map currPosInBlk;

extern uint8_t leftLaserValue;
extern uint8_t rightLaserValue;

extern uint32_t totalDistanceTraveled;

//**************************************************************************************************
//* PRIVATE MACROS AND DEFINES
//**************************************************************************************************
#define X 0
#define Y 1

#define TURNING_LEFT	180
#define TURNING_RIGHT	0

//**************************************************************************************************
//* PRIVATE TYPEDEFS
//**************************************************************************************************

//**************************************************************************************************
//* STATIC VARIABLES
//**************************************************************************************************

//! The angle at which the robot is heading in degrees.
static float angleHeading = STARTING_HEADING_ANGLE;
static float centerOfCircle[2] = {0.0};

//! Radius field of the circles, which are dependent on the rotation of the servo
static float radius[7] = {
		17.5, 30.3, 65.3, 0, 65.3, 30.3, 17.5
};


//! Calculated new position in a map.
coordinates newPos  = {
		.x = MAP_BLOCK_SIZE * MAP_COLUMNS / 2,
		.y = MAP_BLOCK_SIZE * MAP_ROWS / 2
};

//! Saved position in a map from previous calculation.
coordinates prevPos = {
		.x = MAP_BLOCK_SIZE * MAP_COLUMNS / 2,
		.y = MAP_BLOCK_SIZE * MAP_ROWS / 2
};


//**************************************************************************************************
//* GLOBAL VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTION PROTOTYPES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTIONS
//**************************************************************************************************

//!*************************************************************************************************
//! static float calcNewDistance(uint8_t turningSide)
//!
//! @description
//! Function calculates traveled distance from previous step based on knowledge of the wheel.
//! Radius is 1.7cm, so one whole rotation is approximately 10.5cm. We have two magnets on the wheel
//! so one HalfWheelRotation is 5cm. We add 1cm after the complete turn of the wheel so we can
//! calculate with whole numbers.
//!
//! Then based on type of curve(if there is - meaning we can go direct) we calculate the distance.
//!
//! @param    uint8_t turningSide Turning direction
//!
//! @return   Distance traveled from previous step
//!*************************************************************************************************
static float calcNewDistance(uint8_t turningSide)
{
	static uint32_t prevNoWheelRot = 0; 	// Number of half-wheel rotations from previous mapping
	float currentRadius = radius[currentSteer];

	// Get distance from previous position
	uint32_t newRotations = HalfWheelRotations - prevNoWheelRot;
	if (newRotations == 0)  return 0.0;

	// Distance of LEFT WHEEL traveled from previous position
	uint32_t newDistanceLW = newRotations * 10;

	// If the wheel moves a full turn, add one cm to distance due PI is float
	if ((HalfWheelRotations % 2) == 0)
	{
		newDistanceLW += newRotations;
	}

	float newDistance = 0;
	if (turningSide != GO_DIRECT)
	{
		// Calculate phi angle, which is in radians heading angle
		float phi = newDistanceLW / currentRadius * 180 / M_PI;

		float distanceLeftCenterWheel = 4.5; // 45mm
		if (turningSide == TURNING_LEFT)
		{
			currentRadius += distanceLeftCenterWheel;
			angleHeading += phi;
		}
		else if (turningSide == TURNING_RIGHT)
		{
			currentRadius -= distanceLeftCenterWheel;
			angleHeading -= phi;
		}

		newDistance = phi * currentRadius * M_PI / 180;
	}
	else
	{
		newDistance = newDistanceLW;
	}

	prevNoWheelRot = HalfWheelRotations;


	angleHeading = fmodf(angleHeading, 360.0);

	return newDistance;
}


//!*************************************************************************************************
//! static void calcNewPosition(float *current_x, float *current_y, float distance, uint8_t turningSide)
//!
//! @description
//! Function calculates new coordinates [x, y] based on odometry.
//! We know, how far did we go and turning angle, so we can calculate new position.
//!
//! @param    float *current_x	x coordinate
//! @param    float *current_y	y coordinate
//! @param    float *distance	Distance traveled from previous step
//! @param    uint8_t turningSide Turning direction
//!
//! @return   None
//!*************************************************************************************************
static void calcNewPosition(float distance, uint8_t turningSide)
{
	static uint8_t previousSteer = GO_DIRECT;
	static float prevHeadingAngle = STARTING_HEADING_ANGLE;

	// If we have changed the steering and we are steering to some side, we need to calculate
	// the center of a circle.
	if (currentSteer != previousSteer && currentSteer != GO_DIRECT)
	{
		int angle = turningSide - prevHeadingAngle;
		centerOfCircle[X] = newPos.x - radius[currentSteer] * sin(angle*M_PI/180);
		centerOfCircle[Y] = newPos.y - radius[currentSteer] * cos(angle*M_PI/180);
	}

	// We are steering to some side
	if (currentSteer != GO_DIRECT)
	{
		// Calculate the angle between the initial point and the center of the circle
		float theta = atan2(prevPos.y - centerOfCircle[Y], prevPos.x - centerOfCircle[X]);

		// Calculate the angular displacement
		float deltaTheta = distance / radius[currentSteer];

		// Calculate the final angle
		float finalTheta = theta + deltaTheta;

		// Calculate the x and y coordinates of the end point.
		// We don't need to add result to current position, because
		// we are counting with the center, which tells us the coordinates
		newPos.x = centerOfCircle[X] + radius[currentSteer] * cos(finalTheta);
		newPos.y = centerOfCircle[Y] + radius[currentSteer] * sin(finalTheta);
	}
	// We are going direct
	else
	{
		newPos.x += distance * cos(angleHeading*M_PI/180);
		newPos.y += distance * sin(angleHeading*M_PI/180);
	}

	previousSteer = currentSteer;
	prevHeadingAngle = angleHeading;
}


//!*************************************************************************************************
//! static map_move_direction getDirection(uint8_t moveToNewBlock_x, uint8_t moveToNewBlock_y, float current_x,
//! 										float current_y, float prev_x, float prev_y)
//!
//! @description
//! Function calculates in which direction should we move.
//!
//! @param    uint8_t moveToNewBlock_x	Number of block x (where we should be, in which column)
//! @param    uint8_t moveToNewBlock_y	Number of block y (where we should be, in which row)
//! @param    float current_x			Current x coordinate
//! @param    float current_y			Current x coordinate
//! @param    float prev_x				x coordinate from previous step
//! @param    float prev_y				y coordinate from previous step
//!
//! @return   Direction in which we should move.
//!*************************************************************************************************
static map_move_direction getDirection(uint8_t moveToNewBlock_x, uint8_t moveToNewBlock_y)
{
	map_move_direction direction = move_unknown;



	int blockY = currPosInBlk.Row;
	// Move in map when traveled to new block
	if (blockY < (MAP_ROWS / 2))
	{
		blockY = MAP_ROWS / 2 - (blockY - MAP_ROWS / 2);
	}
	else if (blockY > (MAP_ROWS / 2))
	{
		blockY = MAP_ROWS / 2 + (blockY - MAP_ROWS / 2);
	}

	/* ! 2D fields in C are computed from top-left as [0, 0].
	 * But if we move upwards, we will have higher number.
	 * -------------------------------------------------------
	 * e.g. we will be facing up, we will travel 5cm (one field is 4cm..)
	 * So we will have coordinates [0, 5]. But we need to travel up, so
	 * we have higher number in the coordinate system, but in the 2D field
	 * we will move up one square (from [1,1] in the beginning to [1, 0].
	 */
	// Up
	if (newPos.y > prevPos.y && moveToNewBlock_y != blockY)
	{
		direction |= move_up;
	}
	// Down
	if (newPos.y < prevPos.y && moveToNewBlock_y != blockY)
	{
		direction |= move_down;
	}
	// Left
	if (newPos.x < prevPos.x && moveToNewBlock_x != currPosInBlk.Col)
	{
		direction |= move_left;
	}
	// Right
	if (newPos.x > prevPos.x && moveToNewBlock_x != currPosInBlk.Col)
	{
		direction |= move_right;
	}

	// Up-left
	if (direction & move_up && direction & move_left)
	{
		direction = move_upLeft;
	}
	// Up-right
	else if (direction & move_up && direction & move_right)
	{
		direction = move_upRight;
	}
	// Down-left
	else if (direction & move_down && direction & move_left)
	{
		direction = move_downLeft;
	}
	// Down-right
	else if (direction & move_down && direction & move_right)
	{
		direction = move_downRight;
	}

	return direction;
}


//**************************************************************************************************
//* GLOBAL FUNCTIONS
//**************************************************************************************************

//!*************************************************************************************************
//! void mapping(void)
//!
//! @description
//! Main mapping function.
//! Calculation based on odometry, if we should move to next field in our map or not.
//!
//! Step 1) Get info about circle (if we are steering)
//! Step 2) Calculate new position
//! Step 3) Based on new position, calculate if we moved to another field
//! Step 4) Save info's for next calculations
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
void mapping()
{
	// Get info about circle
	uint8_t turningSide;
	if (currentSteer == GO_DIRECT)
	{
		turningSide = GO_DIRECT;
	}
	else
	{
		turningSide = (currentSteer < GO_DIRECT) ? TURNING_RIGHT : TURNING_LEFT;
	}

	// Calculate new traveled distance
	float newDistance = calcNewDistance(turningSide);
	if (newDistance == 0) return;

	totalDistanceTraveled += (int)newDistance;

	// Calculate our new position
	calcNewPosition(newDistance, turningSide);

	// Calculate if we should move to another field in the map
	uint8_t moveToField_x = ((int)fabs(floor(newPos.x) / MAP_BLOCK_SIZE)) % MAP_COLUMNS;
	uint8_t moveToField_y = ((int)fabs(floor(newPos.y) / MAP_BLOCK_SIZE)) % MAP_ROWS;
	// Rotate the y component through the center row
	// due difference between 2D field and coordinate system
	moveToField_y = MAP_ROWS / 2 - (moveToField_y - MAP_ROWS / 2);

	// Move to another field
	if (moveToField_x != currPosInBlk.Col || moveToField_y != currPosInBlk.Row)
	{
		map_move_direction direction = getDirection(moveToField_x, moveToField_y);
		if (direction != move_unknown)
		{
			moveInMap(direction);
		}
	}

	prevPos.x = newPos.x;
	prevPos.y = newPos.y;

	PRINTF("Heading angle: %i\r\n", (int)(angleHeading));
	PRINTF("Distance traveled: %d\r\n", totalDistanceTraveled);
	PRINTF("Coordinates: %d, %d\r\n", (int)newPos.x, (int)newPos.y);
    PRINTF("Center of circle coordinates: %d, %d\r\n", (int)centerOfCircle[X], (int)centerOfCircle[Y]);
}


//!*************************************************************************************************
//! void saveSensorData(void)
//!
//! @description
//! Laser sensor data mapping.
//! If left or right sensor detects some barrier in distance, we need to save this data
//! into the map.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
void saveSensorData(void)
{
	if (leftLaserValue < 250 && leftLaserValue > 0)
	{
		saveBarrierToMap(sensor_Left, leftLaserValue, angleHeading);
	}
	if (rightLaserValue < 250 && rightLaserValue > 0)
	{
		saveBarrierToMap(sensor_Right, rightLaserValue, angleHeading);
	}
}


