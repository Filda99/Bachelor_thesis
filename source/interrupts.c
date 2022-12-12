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
	//Distance += 4.75;
	Distance++;
	PORT_ClearPinsInterruptFlags(GPIO_HALL, HALL_IRQ_MASK);
}

#define FIELD_SIZE 200

// TODO: Pokud se vyvola preruseni, zjisti od koho a zjisti CnV a uloz do glob. prom.
void GPIO_COLOR_MAIN_IRQHandler()
{
	//LeftSensorValue++;
	//PRINTF("Time: %i\r\n", MAIN_SEN_TPM_BASE->CONTROLS[LEFT_TPM_IC].CnV);
	uint32_t pinInterrupt = GPIO_GetPinsInterruptFlags(PTD);
	static uint32_t firstCapture = 0;
	static uint32_t secondCapture = 0;

	static uint32_t colorDetection[FIELD_SIZE] = {0, };
	static uint8_t head = 0;

	static bool rising = true;

	if (rising)
	{
		firstCapture = MAIN_SEN_TPM_BASE->CONTROLS[LEFT_TPM_IC].CnV;
	}
	else
	{
		secondCapture = MAIN_SEN_TPM_BASE->CONTROLS[LEFT_TPM_IC].CnV;
		int pulseWidth = secondCapture - firstCapture;
		//PRINTF("PulseWidth: %i\r\n", pulseWidth);
		if(head != 9)
		{
			colorDetection[head] = pulseWidth;
			head++;
		}
		else
		{
			colorDetection[head] = pulseWidth;

			uint32_t color = 0;
			for (int i = 0; i < FIELD_SIZE; i++)
			{
				color += colorDetection[i];
			}
			LeftSensorValue = color/FIELD_SIZE;
			head = 0;
			DisableIRQ(MAIN_SEN_TPM_IRQ);
		}
	}
	rising = !rising;
	// Left sensor
	//if(pinInterrupt & LEFT_MAIN_SEN_PIN)
	//{
		//LeftSensorValue = MAIN_SEN_TPM_BASE->CONTROLS[LEFT_TPM_IC].CnV;


	//}
	MAIN_SEN_TPM_BASE->CONTROLS[LEFT_TPM_IC].CnSC |= TPM_CnSC_CHF_MASK;
	PORT_ClearPinsInterruptFlags(GPIO_COLOR_MAIN_SEN, pinInterrupt);
}

// TODO: Pokud se vyvola preruseni zde, uz musime zacit zatacet na danou stranu!
void GPIO_COLOR_MINOR_IRQHandler()
{

}

