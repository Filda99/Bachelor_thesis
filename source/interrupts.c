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
#include "fsl_debug_console.h"

/************************************
 * EXTERN VARIABLES
 ************************************/
extern float Distance;
extern uint32_t LeftSensorValue;

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
	Distance += 4.75;

	PORT_ClearPinsInterruptFlags(GPIO_HALL, HALL_IRQ_MASK);
}

// TODO: Pokud se vyvola preruseni, zjisti od koho a zjisti CnV a uloz do glob. prom.
void GPIO_COLOR_MAIN_IRQHandler()
{
	uint32_t pinInterrupt;// = GPIO_GetPinsInterruptFlags(PTD);
	uint32_t timeNow = TPM0->CNT;

	static uint32_t colorDetection[10] = {0, };
	static uint8_t head = 0;

	static bool rising = true;

	if (rising)
	{
		//LeftSensorValue = MAIN_SEN_TPM_BASE->CONTROLS[LEFT_TPM_IC].CnV;
		LeftSensorValue = timeNow;
	}
	else
	{
		//pinInterrupt = MAIN_SEN_TPM_BASE->CONTROLS[LEFT_TPM_IC].CnV;
		pinInterrupt = timeNow;
		int highTime = pinInterrupt - LeftSensorValue;
		if(head != 10)
		{
			colorDetection[head] = highTime;
			head++;
		}
		else
		{
			uint32_t color = 0;
			for (int i = 0; i < 10; i++)
			{
				color += colorDetection[i];
			}
			color /= 10;
			head = 0;
			PRINTF("Left sensor value = %i\r\n", color);
		}
	}
	rising = !rising;
	// Left sensor
	//if(pinInterrupt & LEFT_MAIN_SEN_PIN)
	//{
		//LeftSensorValue = MAIN_SEN_TPM_BASE->CONTROLS[LEFT_TPM_IC].CnV;


	//}
	for(int i = 0; i < 50000; i++);
	TPM0->STATUS |= 0x01;
	PORT_ClearPinsInterruptFlags(GPIO_COLOR_MAIN_SEN, 1);
}

// TODO: Pokud se vyvola preruseni zde, uz musime zacit zatacet na danou stranu!
void GPIO_COLOR_MINOR_IRQHandler()
{

}

