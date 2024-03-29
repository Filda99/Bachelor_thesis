/**
 ********************************************************************************
 * @file    engines.h
 * @author  xjahnf00
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
/*static const float SpeedMap[6] = {
		6.7,		// Backwards
		7.365, 		// STOP motors
		7.72375,	// Forwards
		8.0825,		// ...
		8.44125,
		8.8
};*/

static const float SpeedMap[6] = {
		6.7,		// Backwards
		7.36500000, // STOP motors
		7.5,		// Forwards
		7.55,		// ...
		7.6,
		8.2
};


//* In the middle is the pulse width for GO DIRECT.1
//* Otherwise we add/sub 10 pct. for turning LEFT/RIGHT.
static const float SteerMap[7] = {
		3.75,	// 45
		5,		// 60
		6.25,	// To the left, 75
		7.5,	// Center, Go direct, 90
		8.75,	// To the right, 105
		10,		// 120
		11.25	// 135
};

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void addSpeed();
void addSpeedCustom(int speed);
void slackUpSpeed();
void slackUpSpeedCustom(int speed);
void slackUpSpeedOnWheel(unsigned char slackAmount);
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
