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

//* In the middle is the pulse width for STOP.
//* Otherwise we add/sub 0.127 for higher/slower speed.
static const float SpeedMap[11] = {
		6.73,
		6.857,
		6.984,
		7.111,	// ...
		7.238,	// Backwards
		7.365, 	// STOP motors
		7.492,	// Forwards
		7.619,	// ...
		7.746,
		7.873,
		8.0
};

//* In the middle is the pulse width for GO DIRECT.
//* Otherwise we add/sub 10 pct. for turning LEFT/RIGHT.
static const float SteerMap[7] = {
		20.0,
		30.0,
		40.0,
		50.0,
		60.0,
		70.0,
		80.0
};

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void setMotorSpeed(int speed);
void setMotorSteer(int steer);

void addSpeed();
void addSpeedCustom(int speed);
void slackUpSpeed();
void slackUpSpeedCustom(int speed);
void stopCar();

void turnRight();
void turnRightCustom(int steer);
void turnLeft();
void turnLeftCustom(int steer);
void goDirect();





#ifdef __cplusplus
}
#endif

#endif /* ENGINES_H_ */
