/**
 ********************************************************************************
 * @file    interrupts.c
 * @author  xjahnf00
 * @date    Oct 19, 2022
 * @brief   
 ********************************************************************************
 */

//**************************************************************************************************
//* INCLUDES
//**************************************************************************************************
#include <motors/engines.h>
#include "fsl_port.h"
#include "global_macros.h"
#include "fsl_gpio.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

//**************************************************************************************************
//* EXTERN VARIABLES
//**************************************************************************************************
extern unsigned int HalfWheelRotations;
extern unsigned int LeftSensorValue;
extern unsigned int RightSensorValue;
extern unsigned int CenterSensorValue;
extern bool InitStop;

//**************************************************************************************************
//* PRIVATE MACROS AND DEFINES
//**************************************************************************************************
#define GPIO_HALL_IRQHandler		PORTD_IRQHandler
#define GPIO_COLOR_MINOR_IRQHandler	PORTA_IRQHandler
#define GPIO_COLOR_MAIN_IRQHandler	TPM0_IRQHandler
#define FIELD_SIZE 5

#define LEFT_TPM_IC					kTPM_Chnl_0
#define RIGHT_TPM_IC				kTPM_Chnl_4
#define CENTER_TPM_IC				kTPM_Chnl_3

#define MAX_UINT16	65535

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
	HalfWheelRotations++;
	PORT_ClearPinsInterruptFlags(GPIO_HALL, HALL_IRQ_MASK);

	// TBD
	/*if (InitStop)
	{
		stopCar();
		HalfWheelRotations = 0;
		InitStop = false;
	}*/
}


// TODO: Pokud se vyvola preruseni, zjisti od koho a zjisti CnV a uloz do glob. prom.
void GPIO_COLOR_MAIN_IRQHandler()
{
	//uint32_t pinInterrupt = GPIO_GetPinsInterruptFlags(PTD);
	uint32_t pinInterrupt = TPM_GetStatusFlags(MAIN_SEN_TPM_BASE);

	// Left sensor
	if (pinInterrupt & kTPM_Chnl0Flag)
	{
		static uint32_t firstCaptureLeft = 0;
		static uint32_t secondCaptureLeft = 0;
		static bool risingLeft = true;

		if (risingLeft)
		{
			firstCaptureLeft = MAIN_SEN_TPM_BASE->CONTROLS[LEFT_TPM_IC].CnV;
		}
		else
		{
			secondCaptureLeft = MAIN_SEN_TPM_BASE->CONTROLS[LEFT_TPM_IC].CnV;
			LeftSensorValue = secondCaptureLeft - firstCaptureLeft;
			if (LeftSensorValue > 1000)
			{
				uint32_t firstPartLeft = 30000 - firstCaptureLeft;
				LeftSensorValue = secondCaptureLeft + firstPartLeft;
			}
		}
		risingLeft = !risingLeft;
		MAIN_SEN_TPM_BASE->CONTROLS[LEFT_TPM_IC].CnSC |= TPM_CnSC_CHF_MASK;
		PORT_ClearPinsInterruptFlags(GPIO_COLOR_MAIN_SEN, kTPM_Chnl0Flag);
	}
	// Right sensor
	else if (pinInterrupt & kTPM_Chnl4Flag)
	{
		static uint32_t firstCaptureRight = 0;
		static uint32_t secondCaptureRight = 0;
		static bool risingRight = true;

		if (risingRight)
		{
			firstCaptureRight = MAIN_SEN_TPM_BASE->CONTROLS[RIGHT_TPM_IC].CnV;
		}
		else
		{
			secondCaptureRight = MAIN_SEN_TPM_BASE->CONTROLS[RIGHT_TPM_IC].CnV;
			RightSensorValue = secondCaptureRight - firstCaptureRight;
			if (RightSensorValue > 1000)
			{
				uint32_t firstPartRight = 30000 - firstCaptureRight;
				RightSensorValue = secondCaptureRight + firstPartRight;
			}
		}
		risingRight = !risingRight;
		MAIN_SEN_TPM_BASE->CONTROLS[RIGHT_TPM_IC].CnSC |= TPM_CnSC_CHF_MASK;
		PORT_ClearPinsInterruptFlags(GPIO_COLOR_MAIN_SEN, kTPM_Chnl4Flag);
	}
	// Center sensor
	else if (pinInterrupt & kTPM_Chnl3Flag)
	{
		static uint32_t firstCaptureCenter = 0;
		static uint32_t secondCaptureCenter = 0;
		static bool risingCenter = true;

		if (risingCenter)
		{
			firstCaptureCenter = MAIN_SEN_TPM_BASE->CONTROLS[CENTER_TPM_IC].CnV;
		}
		else
		{
			secondCaptureCenter = MAIN_SEN_TPM_BASE->CONTROLS[CENTER_TPM_IC].CnV;
			CenterSensorValue = secondCaptureCenter - firstCaptureCenter;
			if (CenterSensorValue > 1000)
			{
				uint32_t firstPartCenter = 30000 - firstCaptureCenter;
				CenterSensorValue = secondCaptureCenter + firstPartCenter;
			}
		}
		risingCenter = !risingCenter;
		MAIN_SEN_TPM_BASE->CONTROLS[CENTER_TPM_IC].CnSC |= TPM_CnSC_CHF_MASK;
		PORT_ClearPinsInterruptFlags(GPIO_COLOR_MAIN_SEN, kTPM_Chnl3Flag);
	}
}
