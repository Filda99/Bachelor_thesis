/**
 ********************************************************************************
 * @file    interrupts.c
 * @author  user
 * @date    Oct 19, 2022
 * @brief   
 ********************************************************************************
 */

//**************************************************************************************************
//* INCLUDES
//**************************************************************************************************
#include "fsl_port.h"
#include "global_macros.h"
#include "fsl_gpio.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

//**************************************************************************************************
//* EXTERN VARIABLES
//**************************************************************************************************
extern unsigned int WheelRotations;
extern unsigned int LeftSensorValue;

//**************************************************************************************************
//* PRIVATE MACROS AND DEFINES
//**************************************************************************************************
#define GPIO_HALL_IRQHandler		PORTD_IRQHandler
#define GPIO_COLOR_MINOR_IRQHandler	PORTA_IRQHandler
#define GPIO_COLOR_MAIN_IRQHandler	TPM0_IRQHandler
#define FIELD_SIZE 20

#define LEFT_TPM_IC					kTPM_Chnl_0
#define RIGHT_TPM_IC				kTPM_Chnl_4
#define CENTER_TPM_IC				kTPM_Chnl_3

#define MAX_UINT16	65536
//**************************************************************************************************
//* PRIVATE TYPEDEFS
//**************************************************************************************************

//**************************************************************************************************
//* STATIC VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* GLOBAL VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTION PROTOTYPES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTIONS
//**************************************************************************************************

//**************************************************************************************************
//* GLOBAL FUNCTIONS
//**************************************************************************************************
void GPIO_HALL_IRQHandler()
{
	// Interrupt every half spin (two magnets)
	WheelRotations++;
	PORT_ClearPinsInterruptFlags(GPIO_HALL, HALL_IRQ_MASK);
}


// TODO: Pokud se vyvola preruseni, zjisti od koho a zjisti CnV a uloz do glob. prom.
void GPIO_COLOR_MAIN_IRQHandler()
{
	//uint32_t pinInterrupt = GPIO_GetPinsInterruptFlags(PTD);
	uint32_t pinInterrupt = TPM_GetStatusFlags(MAIN_SEN_TPM_BASE);


	if(pinInterrupt & kTPM_Chnl0Flag)
	{
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
			uint32_t pulseWidth = secondCapture - firstCapture;
			if (pulseWidth > 10000)
			{
				uint32_t firstPart = MAX_UINT16 - firstCapture;
				pulseWidth = secondCapture + firstPart;
			}


			if(head != FIELD_SIZE)
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
		MAIN_SEN_TPM_BASE->CONTROLS[LEFT_TPM_IC].CnSC |= TPM_CnSC_CHF_MASK;
		PORT_ClearPinsInterruptFlags(GPIO_COLOR_MAIN_SEN, kTPM_Chnl0Flag);
	}


}

// TODO: Pokud se vyvola preruseni zde, uz musime zacit zatacet na danou stranu!
void GPIO_COLOR_MINOR_IRQHandler()
{

}

