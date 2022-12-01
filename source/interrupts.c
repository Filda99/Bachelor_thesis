/**
 ********************************************************************************
 * @file    interrupts.c
 * @author  user
 * @date    Oct 19, 2022
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "fsl_port.h"
#include "global_macros.h"
#include "fsl_gpio.h"
#include "MKL25Z4.h"

/************************************
 * EXTERN VARIABLES
 ************************************/
extern uint32_t noOfRotations;

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define GPIO_HALL_IRQHandler	PORTA_IRQHandler
#define GPIO_COLOR_MAIN_IRQHandler	PORTD_IRQHandler

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/
static uint32_t timerLeftSensor = 0;

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
void GPIO_HALL_IRQHandler()
{
	noOfRotations++;
	PORT_ClearPinsInterruptFlags(GPIO_HALL, HALL_IRQ_MASK);
}

// TODO: Pokud se vyvola preruseni zde, uz musime zacit zatacet na danou stranu!
void GPIO_COLOR_MAIN_IRQHandler()
{

}

