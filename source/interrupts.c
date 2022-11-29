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
	/*if(GPIO_ReadPinInput(GPIO_COLOR_MAIN_SEN, 0))
	{
		uint8_t res = GPIO_PDIR_PDI(0);
		res++;
	}*/
	PORT_ClearPinsInterruptFlags(GPIO_HALL, HALL_IRQ_MASK);
}

// TODO:
// Vzdycky spustit danej pin jen kdyz potrebuji, pak to vyvola interrupty,
// ktery si odchytnu a spocitam cas, nasledne vypnout danej pin...
void GPIO_COLOR_MAIN_IRQHandler()
{
	// Check logic level of GPIO pins

	// Check left sensor
	if(GPIO_ReadPinInput(GPIO_COLOR_MAIN_SEN, 0))
	{
		uint8_t res = GPIO_PDIR_PDI(0);
		res++;
	}

}

