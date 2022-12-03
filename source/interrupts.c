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
	// distance = 2*pi*r
	// One rotation is ~9.5cm
	// Interrupt every half spin (two magnets)
	distance += 4.75;

	PORT_ClearPinsInterruptFlags(GPIO_HALL, HALL_IRQ_MASK);
}

// TODO: Pokud se vyvola preruseni zde, uz musime zacit zatacet na danou stranu!
void GPIO_COLOR_MAIN_IRQHandler()
{
	



	/*static uint32_t riseCnt = 0;
	static uint32_t fallCnt = 0;
	static bool wasRising = false;

	uint32_t pinInterrupt = GPIO_GetPinsInterruptFlags(PORTD);


	if(GPIO_ReadPinInput(PORTD, 0))
	{
		riseCnt = TPM_GetCurrentTimerCount(DEMO_TPM_BASEADDR);
		wasRising = true;
	}
	else
	{
		if(wasRising)
		{
			fallCnt = TPM_GetCurrentTimerCount(DEMO_TPM_BASEADDR);
			tpmIsrFlag = true;
		}
		wasRising = false;
	}

	if (tpmIsrFlag)
	{
		currTime = fallCnt - riseCnt;
	}
	tpmIsrFlag = false;
	tpmIsrFlag = true;*/

}

