/**
 ********************************************************************************
 * @file    isl29125.c
 * @author  user
 * @date    Sep 26, 2022
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "isl29125.h"
#include "fsl_i2c.h"
#include "MKL25Z4.h"

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

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
bool config(uint8_t config1, uint8_t config2, uint8_t config3);

void write(uint8_t reg, uint8_t* data, uint8_t len);
void read(uint8_t reg, uint8_t* data, uint8_t len);

/************************************
 * STATIC FUNCTIONS
 ************************************/
// Setup Configuration registers (three registers) - returns true if successful
// Use CONFIG1 variables from SFE_ISL29125.h for first parameter config1, CONFIG2 for config2, 3 for 3
// Use CFG_DEFAULT for default configuration for that register
bool config(uint8_t config1, uint8_t config2, uint8_t config3)
{
  bool ret = true;
  uint8_t data = 0x00;

  // Set 1st configuration register
  write(ISL_CONFIG_1, &config1, 1);
  // Set 2nd configuration register
  write(ISL_CONFIG_2, &config2, 1);
  // Set 3rd configuration register
  write(ISL_CONFIG_3, &config3, 1);

  // Check if configurations were set correctly
  read(ISL_CONFIG_1, &data, 1);
  if (data != config1)
  {
    ret &= false;
  }
  read(ISL_CONFIG_2, &data, 1);
  if (data != config2)
  {
    ret &= false;
  }
  read(ISL_CONFIG_3, &data, 1);
  if (data != config3)
  {
    ret &= false;
  }
  return ret;
}

// Generic I2C write data to register (single byte)
void write(uint8_t reg, uint8_t* data, uint8_t len)
{
    i2c_master_transfer_t masterXfer;

    memset(&masterXfer, 0, sizeof(masterXfer));

    masterXfer.slaveAddress = ISL_I2C_ADDR;
    masterXfer.direction = kI2C_Write;
    masterXfer.subaddress = reg;
    masterXfer.subaddressSize = 1U;
    masterXfer.data = data;
    masterXfer.dataSize = len;
    masterXfer.flags = kI2C_TransferDefaultFlag;
    I2C_MasterTransferBlocking(I2C0, &masterXfer);
}


// Generic I2C read register (single byte)
void read(uint8_t reg, uint8_t* data, uint8_t len)
{
    i2c_master_transfer_t masterXfer;

    memset(&masterXfer, 0, sizeof(masterXfer));

    masterXfer.slaveAddress = ISL_I2C_ADDR;
    masterXfer.direction = kI2C_Read;
    masterXfer.subaddress = reg;
    masterXfer.subaddressSize = 1U;
    masterXfer.data = data;
    masterXfer.dataSize = len;
    masterXfer.flags = kI2C_TransferDefaultFlag;
    I2C_MasterTransferBlocking(I2C0, &masterXfer);
}


/************************************
 * GLOBAL FUNCTIONS
 ************************************/
// Initialize - returns true if successful
// Starts Wire/I2C Communication
// Verifies sensor is there by checking its device ID
// Resets all registers/configurations to factory default
// Sets configuration registers for the common use case
bool ISL_ColorSensorInit()
{
  bool ret = true;
  uint8_t data = 0x00;

  // Check device ID
  read(ISL_DEVICE_ID, &data, 1);
  if (data != 0x7D)
  {
    ret &= false;
  }

  // Reset registers
  ret &= ISL_reset();

  // Set to RGB mode, 10k lux, and high IR compensation
  ret &= config(CFG1_MODE_RGB | CFG1_10KLUX, CFG2_IR_ADJUST_HIGH, CFG_DEFAULT);

  return ret;
}

// Reset all registers - returns true if successful
bool ISL_reset()
{
  uint8_t data = 0x00;
  uint8_t addr = 0x46;
  // Reset registers
  write(ISL_DEVICE_ID, &addr, 1);
  // Check reset
  /*data = read(ISL_CONFIG_1, &data, 1);
  data |= read(ISL_CONFIG_2, &data, 1);
  data |= read(ISL_CONFIG_3, &data, 1);
  data |= read(ISL_STATUS, &data, 1);*/
  if (data != 0x00)
  {
    return false;
  }
  return true;
}

// Read the latest Sensor ADC reading for the color Red
void ISL_readRed(uint8_t* red)
{

  return read(ISL_RED_L, red, 2);
}

// Read the latest Sensor ADC reading for the color Green
void ISL_readGreen(uint8_t* green)
{
  return read(ISL_GREEN_L, green, 2);
}

// Read the latest Sensor ADC reading for the color Blue
void ISL_readBlue(uint8_t* blue)
{
  return read(ISL_BLUE_L, blue, 2);
}
