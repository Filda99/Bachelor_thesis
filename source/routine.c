/**
 ********************************************************************************
 * @file    routine.c
 * @author  xjahnf00
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
#include "fsl_dmamux.h"

#include "control_unit.h"
#include "map/mapping.h"
#include "map/map_operations.h"

#include "fsl_adc16.h"
#include "math.h"

#include "common/delay.h"

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
extern uint8_t sensorsDataFromArduino[I2C_DATA_LENGTH];

//**************************************************************************************************
//* PRIVATE MACROS AND DEFINES
//**************************************************************************************************
#define EXAMPLE_I2C_DMAMUX_BASEADDR DMAMUX0
#define EXAMPLE_I2C_DMA_BASEADDR DMA0
#define I2C_DMA_CHANNEL 0U

#define DEMO_ADC16_USER_CHANNEL 0U /*PTE20, ADC0_SE0 */

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
uint8_t leftLaserValue = 0;
uint8_t rightLaserValue = 0;


//**************************************************************************************************
//* STATIC FUNCTION PROTOTYPES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTIONS
//**************************************************************************************************

//!*************************************************************************************************
//! void printBlock(void)
//!
//! @description
//! Debug purpose only.
//! This function will print current block.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
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
		if (CenterSensorValue > COLOR_TRESHOLD && CenterSensorValue < 130)
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


//!*************************************************************************************************
//! void processSensorData(void)
//!
//! @description
//! Save laser sensor data from dma buffer into the proper variables.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
static void processSensorData()
{
	DMAMUX_DisableChannel(EXAMPLE_I2C_DMAMUX_BASEADDR, I2C_DMA_CHANNEL);
	for (int i = 0; i < I2C_DATA_LENGTH; i++)
	{
		// Left sensor ID
		if (sensorsDataFromArduino[i] == 0x1 && i != (I2C_DATA_LENGTH - 1))
		{
			leftLaserValue = sensorsDataFromArduino[i + 1];
			i++;
		}
		// Right sensor ID
		else if (sensorsDataFromArduino[i] == 0x2 && i != (I2C_DATA_LENGTH - 1))
		{
			rightLaserValue = sensorsDataFromArduino[i + 1];
			i++;
		}
		else if (i == 0)
		{
			// Left sensor ID
			if (sensorsDataFromArduino[I2C_DATA_LENGTH-1] == 0x1)
			{
				leftLaserValue = sensorsDataFromArduino[i];
			}
			// Right sensor ID
			else if (sensorsDataFromArduino[I2C_DATA_LENGTH-1] == 0x2)
			{
				rightLaserValue = sensorsDataFromArduino[i];
			}
		}
		else
		{
			rightLaserValue = 0;
			leftLaserValue = 0;
		}
	}

	for (int i = 0; i < I2C_DATA_LENGTH; i++)
	{
		sensorsDataFromArduino[i] = 0;
	}
	DMAMUX_EnableChannel(EXAMPLE_I2C_DMAMUX_BASEADDR, I2C_DMA_CHANNEL);
}


//!*************************************************************************************************
//! void checkStop(void)
//!
//! @description
//! Check if IR sensor on the front of a car detects some barrier.
//! If the distance is smaller than 3, stop the car.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
static void checkStop()
{
	static uint8_t count = 0;
	adc16_channel_config_t adc16ChannelConfigStruct;\

	adc16ChannelConfigStruct.channelNumber = DEMO_ADC16_USER_CHANNEL;
	adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
    adc16ChannelConfigStruct.enableDifferentialConversion = false;

	ADC16_SetChannelConfig(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP, &adc16ChannelConfigStruct);
	while (0U == (kADC16_ChannelConversionDoneFlag &
				  ADC16_GetChannelStatusFlags(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP)))
	{
	}
	float distance = ADC16_GetChannelConversionValue(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP);
	distance = distance * 5 / 1023.0;
	distance = 60.374 * pow(distance , -1.16);

	if (distance <= 3)
	{
		count++;
	}

	// If there is barrier, stop the car and wait, if it disappears
	while (count > 20 || distance <= 2.5)
	{
		stopCar();
		distance = ADC16_GetChannelConversionValue(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP);
		distance = distance * 5 / 1023.0;
		distance = 60.374 * pow(distance , -1.16);
		count = 0;
	}
	addSpeed();
}

//**************************************************************************************************
//* GLOBAL FUNCTIONS
//**************************************************************************************************

//!*************************************************************************************************
//! void routine(void)
//!
//! @description
//! Main routine function.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
void routine(void)
{
	checkLines();

	mapping();
	controlUnit();

	processSensorData();
	checkStop();
	saveSensorData();
}


//!*************************************************************************************************
//! void i2c_slave_callback(I2C_Type *base, i2c_slave_transfer_t *xfer, void *userData)
//!
//! @description
//! Callback function for I2C slave DMA.
//! Function saves incoming data to the buffer to the position, where the
//! oldest data are. Buffer seems to be like circular buffer.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
void i2c_slave_callback(I2C_Type *base, i2c_slave_transfer_t *xfer, void *userData)
{
	static uint8_t counter = 0;
	if (counter >= I2C_DATA_LENGTH)
		counter = 0;
    switch (xfer->event)
    {
        /*  Transmit request */
        case kI2C_SlaveTransmitEvent:
            xfer->data = &g_slave_buff;
            xfer->dataSize = 1;
            break;

		/*  Receive request */
        case kI2C_SlaveReceiveEvent:
            xfer->data = &g_slave_buff;
            xfer->dataSize = 1;
            sensorsDataFromArduino[counter++] = g_slave_buff;
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
