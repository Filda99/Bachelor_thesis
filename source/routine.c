/**
 ********************************************************************************
 * @file    routine.c
 * @author  user
 * @date    Jul 18, 2022
 * @brief
 ********************************************************************************
 */

//**************************************************************************************************
//* INCLUDES
//**************************************************************************************************
#include <motors/engines.h>
#include "routine.h"
#include "MKL25Z4.h"
#include "global_macros.h"

#include "fsl_tpm.h"
#include "fsl_gpio.h"
#include "fsl_debug_console.h"

#include "control_unit.h"
#include "map/mapping.h"
#include "map/map_operations.h"

//**************************************************************************************************
//* EXTERN VARIABLES
//**************************************************************************************************
extern line_location_t LineDetected;
extern unsigned int LeftSensorValue;
extern unsigned int CenterSensorValue;
extern unsigned int RightSensorValue;

// todo: remove. This is only for program without car.
extern unsigned int HalfWheelRotations;
extern map_block *currentBlockInMap;

extern uint8_t g_slave_buff;
extern uint8_t glob_data[I2C_DATA_LENGTH];

//**************************************************************************************************
//* PRIVATE MACROS AND DEFINES
//**************************************************************************************************

//**************************************************************************************************
//* PRIVATE TYPEDEFS
//**************************************************************************************************

//**************************************************************************************************
//* STATIC VARIABLES
//**************************************************************************************************
dma_handle_t dmaHandle;
volatile bool completionFlag = false;

//**************************************************************************************************
//* GLOBAL VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTION PROTOTYPES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTIONS
//**************************************************************************************************

static void printBlock()
{
	PRINTF("Current block looks like this:\r\n");
	for (int i = 0; i < MAP_ROWS; i++)
	{
		for (int j = 0; j < MAP_COLUMNS; j++)
		{
			PRINTF(" %d |", currentBlockInMap->block[i][j]);
		}
		PRINTF("\r\n");
	}
}

//!*************************************************************************************************
//! void checkLine(void)
//!
//! @description
//! Check left, right and center sensors value.
//! If the value is greater than COLOR_TRESHOLD, LineDetected is set to that side.
//! That means that we detected line under the sensor.
//!
//! Function after 100 cycles enables interrupts for main sensors.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
static void checkLines()
{
	bool isLineDetected = false;
	LineDetected = 0;

	if (LeftSensorValue > COLOR_TRESHOLD && LeftSensorValue < 400)
	{
		LineDetected = LineLeft;
		if (CenterSensorValue > COLOR_TRESHOLD && CenterSensorValue < 400)
		{
			LineDetected = LineCenter_Left;
		}
		isLineDetected = true;
	}
	if (RightSensorValue > COLOR_TRESHOLD && RightSensorValue < 400)
	{
		LineDetected |= LineRight;
		if (CenterSensorValue > COLOR_TRESHOLD && CenterSensorValue < 400)
		{
			LineDetected = LineCenter_Right;
		}
		isLineDetected = true;
	}


	if (!isLineDetected)
	{
		LineDetected = LineNone;
	}
}

//**************************************************************************************************
//* GLOBAL FUNCTIONS
//**************************************************************************************************

//!*************************************************************************************************
//! void function(void)
//!
//! @description
//! Function
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
void routine(void)
{
	checkLines();
	controlUnit();

	//static int i = 0;
	//PRINTF("Cycle: %i\r\n", i++);
 	//printBlock();
	mapping();
	//PRINTF("-------------------------------------\r\n");
}

void i2c_slave_callback(I2C_Type *base, i2c_slave_transfer_t *xfer, void *userData)
{
	static uint8_t counter = 0;
	if (counter >= I2C_DATA_LENGTH)
		counter = 0;
    switch (xfer->event)
    {
        /*  Transmit request */
        case kI2C_SlaveTransmitEvent:
            /*  Update information for transmit process */
            xfer->data = &g_slave_buff;
            xfer->dataSize = 1;
            break;

        /*  Receive request */
        case kI2C_SlaveReceiveEvent:
            /*  Update information for received process */
            xfer->data = &g_slave_buff;
            xfer->dataSize = 1;
            glob_data[counter++] = g_slave_buff;
            break;

        /*  Transfer done */
        case kI2C_SlaveCompletionEvent:
            completionFlag = true;
            break;

        default:
            completionFlag = false;
            break;
    }
}
