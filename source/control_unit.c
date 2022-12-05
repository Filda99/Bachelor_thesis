/**
 ********************************************************************************
 * @file    control_unit.c
 * @author  user
 * @date    Dec 1, 2022
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "global_macros.h"
#include "motors/engines.h"
#include "MKL25Z4.h"

/************************************
 * EXTERN VARIABLES
 ************************************/
extern unsigned char LineDetected;

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/
static line_location_t prevTurning = LineNone;
static uint8_t lineCnt = 0;

/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/************************************
 * STATIC FUNCTIONS
 ************************************/

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void controlUnit()
{
  static uint32_t distanceWithoutInterrupt = 0;

  switch (LineDetected)
  {
  case LineNone:
  {
    addSpeed();
    goDirect();
    distanceWithoutInterrupt++;

    if(distanceWithoutInterrupt > MAX_DISTANCE_WITHOUT_IRQ_LINE)
    {
      stopCar();
    }
    prevTurning = LineNone;
    break;
  }
    
  case LineLeft:
  {
    if (prevTurning == LineLeft)
    {
      lineCnt++;
    }
    else
    {
      lineCnt = 0;
    }

    if (lineCnt > MAX_CNT_ON_LINE)
    {
      turnRight();
      slackUpSpeed();
    }
    
    prevTurning = LineLeft;
    break;
  }
   
  case LineRight:
  {
    if (prevTurning == LineRight)
    {
      lineCnt++;
    }
    else
    {
      lineCnt = 0;
    }

    if (lineCnt > MAX_CNT_ON_LINE)
    {
      turnLeft();
      slackUpSpeed();
    }
    
    prevTurning = LineRight;
    break;    
  }

  case LineCenter_Left:
    turnRightCustom(MAX_STEER_RIGHT);
    slackUpSpeedCustom(2);
    break;

  case LineCenter_Right:
    turnLeftCustom(MAX_STEER_LEFT);
    slackUpSpeedCustom(2);
    break;
  
  default:
    break;
  }
}


