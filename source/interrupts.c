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
extern float distance;

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define GPIO_HALL_IRQHandler		PORTD_IRQHandler
#define GPIO_COLOR_MINOR_IRQHandler	PORTA_IRQHandler
#define GPIO_COLOR_MAIN_IRQHandler	TPM0_IRQHandler

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
	// distance = 2*pi*r
	// One rotation is ~9.5cm
	// Interrupt every half spin (two magnets)
	distance += 4.75;

	PORT_ClearPinsInterruptFlags(GPIO_HALL, HALL_IRQ_MASK);
}

// TODO: Pokud se vyvola preruseni, zjisti od koho a zjisti CnV a uloz do glob. prom.
void GPIO_COLOR_MAIN_IRQHandler()
{

	uint32_t pinInterrupt = GPIO_GetPinsInterruptFlags(PORTD);

}

// TODO: Pokud se vyvola preruseni zde, uz musime zacit zatacet na danou stranu!
void GPIO_COLOR_MINOR_IRQHandler()
{

}

