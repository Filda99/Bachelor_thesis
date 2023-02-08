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

//* First one in the field is pulse width for reverse.
//* Then to stop. And the following for higher speed forwards.
//* Otherwise we add/sub 0.35875 for higher/slower speed.
static const float SpeedMap[6] = {
		6.7,		// Backwards
		7.365, 		// STOP motors
		7.72375,	// Forwards
		8.0825,		// ...
		8.44125,
		8.8
};

//* In the middle is the pulse width for GO DIRECT.
//* Otherwise we add/sub 10 pct. for turning LEFT/RIGHT.
static const float SteerMap[7] = {
		2.741,
		4.284,
		5.827,		// To the left
		7.37,		// Center, Go direct
		8.913,		// To the right
		10.456,
		11.999
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
void goBackwards();
void stopCar();
void hardStop();

void turnRight();
void turnRightCustom(int steer);
void turnLeft();
void turnLeftCustom(int steer);
void goDirect();





#ifdef __cplusplus
}
#endif

#endif /* ENGINES_H_ */
