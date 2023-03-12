/**
 ***************************************************************************************************
 * @file    main.c
 * @author  user
 * @date    Jul 18, 2022
 * @brief
 ***************************************************************************************************
 */

//**************************************************************************************************
//* INCLUDES
//**************************************************************************************************
#include <motors/engines.h>
#include "map/map_operations.h"
#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include "board.h"
#include "startup_board.h"
#include "pin_mux.h"
#include "routine.h"
#include "fsl_port.h"
#include "MKL25Z4.h"
#include "control_unit.h"
#include "common.h"

#include "peripherals/i2c.h"

//**************************************************************************************************
//* EXTERN VARIABLES
//**************************************************************************************************
extern unsigned LeftSensorValue;
extern unsigned char LineDetected;
extern bool IsCmdToStopCar;

//**************************************************************************************************
//* PRIVATE MACROS AND DEFINES
//**************************************************************************************************
#define LED_BLUE_TURN_ON() GPIOD->PCOR |= (1<<1)
#define LED_BLUE_TURN_OFF() GPIOD->PSOR |= (1<<1)

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

//!*************************************************************************************************
//! int main(void)
//!
//! @description
//! Main function.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
int main(void)
{
	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();
	PRINTF("Starting board...\r\n");

	startupInit();
	uint16_t touch_value;
	uint8_t nextAction = 0;

	PRINTF("Waiting for initialization -> Press capacitive sensor.\r\n");

	while (1)
	{
		// Get data from touch sensor
		TSI0->DATA |= TSI_DATA_SWTS_MASK;
		while (!(TSI0->GENCS & TSI_GENCS_EOSF_MASK))
		{
		}
		touch_value = TSI0->DATA & TSI_DATA_TSICNT_MASK;
		TSI0->GENCS |= TSI_GENCS_EOSF_MASK;

		// Wait for initialization
		if (touch_value > 2 && touch_value < 10 && (nextAction == 0))
		{
			LED_RED_ON();
			LED_GREEN_OFF();
			LED_BLUE_TURN_OFF();

			nextAction++;
		}
		// Initial initialization
		else if (touch_value > 2 && touch_value < 10 && (nextAction == 1))
		{
			PRINTF("-----------------\r\n");
			LED_RED_ON();
			LED_GREEN_ON();

			startupBoard();
			// todo: Reach the starting line
			//setWheelToInitPosition();
			createMap();

			nextAction++;
			PRINTF("Waiting for routine -> Press capacitive sensor.\r\n");
		}
		// Start routine
		else if (touch_value > 2 && touch_value < 10 && (nextAction == 2))
		{
			PRINTF("-----------------\r\n");
			PRINTF("Starting routine.\r\n");
			LED_RED_OFF();
			addSpeed();

			while (!IsCmdToStopCar)
			{
				routine();
			}

			// If car stopped, wait for command to continue.
			nextAction = 2;
		}
	}
}
