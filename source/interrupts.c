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

/************************************
 * EXTERN VARIABLES
 ************************************/
extern uint32_t noOfRotations;

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

#define GPIO_HALL_IRQHandler	PORTA_IRQHandler
#define GPIO_CS_IRQHandler		PORTC_IRQHandler


/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/

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

void GPIO_CS_IRQHandler()
{

}
