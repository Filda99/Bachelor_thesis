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
#include "motors/engines.h"
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

#define LASER_ADDR 0x52

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
int main2(void)
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


#include "fsl_i2c.h"
#include "fsl_port.h"

#define I2C_MASTER_BASEADDR I2C1

int main()
{
	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();
	PRINTF("Starting board...\r\n");

	// Init
	i2c_master_config_t masterConfig;
	I2C_MasterGetDefaultConfig(&masterConfig);
	I2C_MasterInit(I2C_MASTER_BASEADDR, &masterConfig, 400000);

	uint8_t deviceAddress = 0x52;
	uint8_t registerAddress = 0x00;
	uint8_t data[2] = {0x01};
	uint8_t dataSize = sizeof(data)/sizeof(data[0]);
	I2C_MasterStart(I2C_MASTER_BASEADDR, deviceAddress, kI2C_Write);
	I2C_MasterWriteBlocking(I2C_MASTER_BASEADDR, data, dataSize, kI2C_TransferDefaultFlag);
	I2C_MasterStop(I2C_MASTER_BASEADDR);

	while (1)
	{

		I2C_MasterStart(I2C_MASTER_BASEADDR, deviceAddress, kI2C_Write);
		I2C_MasterWriteBlocking(I2C_MASTER_BASEADDR, &registerAddress, 1, kI2C_TransferDefaultFlag);
		I2C_MasterStop(I2C_MASTER_BASEADDR);
		I2C_MasterStart(I2C_MASTER_BASEADDR, deviceAddress, kI2C_Read);
		I2C_MasterReadBlocking(I2C_MASTER_BASEADDR, data, dataSize, kI2C_TransferDefaultFlag);
		I2C_MasterStop(I2C_MASTER_BASEADDR);


		uint16_t distance = ((uint16_t)data[0] << 8) | data[1];

		// Print the distance measurement
		PRINTF("Distance: %d mm\r\n", distance);

		delay_ms(100);
	}

}

int main5()
{
    uint8_t buf[2];
    uint16_t distance;

    // Initialize I2C module
    i2c_init_my();

    // Configure the laser sensor
    buf[0] = 0x00; // Register for configuration data
    buf[1] = 0x01; // Configuring the sensor for short range mode
    i2c_write_reg(LASER_ADDR, *buf, 2);

    // Turn on the sensor
    buf[0] = 0x00; // Register for sensor enable
    buf[1] = 0x01; // Enable the sensor
    i2c_write_reg(LASER_ADDR, *buf, 2);

    // Wait for sensor to initialize
    delay_ms(10);

    while (1)
    {
    	// Read distance measurement from the sensor
		buf[0] = 0x14; // Register for distance measurement
		i2c_write_reg(LASER_ADDR, *buf, 1);
		i2c_read_reg(LASER_ADDR, *buf, buf, 2);
		distance = ((uint16_t)buf[0] << 8) | buf[1];

		// Print the distance measurement
		PRINTF("Distance: %d mm\r\n", distance);

		delay_ms(1000);
    }


    return 0;
}
