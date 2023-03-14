/**
 ***************************************************************************************************
 * @file    i2c.c
 * @author  user
 * @date    Mar 4, 2023
 * @brief   
 ***************************************************************************************************
 */

//**************************************************************************************************
//* INCLUDES
//**************************************************************************************************
#include "fsl_i2c.h"

//**************************************************************************************************
//* EXTERN VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* PRIVATE MACROS AND DEFINES
//**************************************************************************************************
#define I2C_MASTER_CLOCK_FREQUENCY CLOCK_GetFreq(I2C1_CLK_SRC)

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
//! void i2c_init(uint8_t slave_addr, uint8_t *data, size_t dataSize)
//!
//! @description
//! Function initialize i2c slave device.
//!
//! @note	Function will wait until slave device corresponds.
//!
//! @param    uint8_t deviceAddr	Address of device we want to communicate.
//! @param    uint8_t *data			Initialization data, which should be written.
//! @param    size_t dataSize		Size of sending data.
//!
//! @return   None
//!*************************************************************************************************
void i2cInit(uint8_t deviceAddr, uint32_t baudRate, uint8_t *data, size_t dataSize)
{
    i2c_master_config_t masterConfig;
    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = baudRate;
    I2C_MasterInit(I2C1, &masterConfig, I2C_MASTER_CLOCK_FREQUENCY);

    i2c_master_transfer_t masterXfer;
    masterXfer.flags = kI2C_TransferDefaultFlag;
    masterXfer.slaveAddress = deviceAddr;
    masterXfer.direction = kI2C_Write;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = data;
    masterXfer.dataSize = dataSize;

    I2C_MasterTransferBlocking(I2C1, &masterXfer);
}

//!*************************************************************************************************
//! void i2cWrite(uint8_t deviceAddr, uint8_t reg_addr, uint8_t regAddr, uint8_t value)
//!
//! @description
//! Function calls api function for sending data (one byte) to the slave device.
//!
//! @param    uint8_t deviceAddr	Address of device we want to communicate.
//! @param    uint8_t regAddr		Slave register address, if needed to write to specific reg.
//! @param    uint8_t data			One byte of data to be send.
//!
//! @code
//! uint8_t regAddr = 0x52;
//! uint8_t regAddr = 0;	// If we don't want to read data from specific register
//! uint8_t data = 1;		// One byte of data to be send
//! i2cWrite(regAddr, regAddr, data);
//!
//! @return   kStatus_Fail if data was send unsuccessfully, kStatus_Success otherwise.
//!*************************************************************************************************
status_t i2cWrite(uint8_t deviceAddr, uint8_t regAddr, uint8_t data)
{
    i2c_master_transfer_t transfer;
    uint8_t buff[2];

    buff[0] = regAddr;
    buff[1] = data;

    transfer.flags = kI2C_TransferDefaultFlag;
    transfer.slaveAddress = deviceAddr;
    transfer.direction = kI2C_Write;
    transfer.subaddress = 0;
    transfer.subaddressSize = 0;
    transfer.data = buff;
    transfer.dataSize = 2;

    if (I2C_MasterTransferBlocking(I2C1, &transfer) != kStatus_Success)
	{
		return kStatus_Fail;
	}

	return kStatus_Success;
}


//!*************************************************************************************************
//! status_t i2c_read(uint8_t deviceAddr, uint8_t regAddr, uint8_t *data, uint32_t dataLen)
//!
//! @description
//! Function calls api function for reading data (one byte) from the slave device.
//!
//! @note	Buffer data has to be initialize before function is called.
//!
//! @param    uint8_t deviceAddr	Address of device we want to communicate.
//! @param    uint8_t regAddr		Slave register address, if needed to get data from specific reg.
//! @param    uint8_t data			Data buffer, where should be data stored after received.
//! @param    uint32_t dataLen		Size of how much data do we want.
//!
//! @code
//! uint8_t deviceAddr = 0x52;
//! uint8_t regAddr = 0;	// If we don't want to read data from specific register
//! uint8_t data[5] = {0,};
//! i2cRead(deviceAddr, regAddr, data, sizeof(data));
//!
//! @return   kStatus_Fail if data was received unsuccessfully, kStatus_Success otherwise.
//!*************************************************************************************************
status_t i2cRead(uint8_t deviceAddr, uint8_t regAddr, uint8_t *data, uint32_t dataLen)
{
	if (!data)
	{
		return kStatus_Fail;
	}

    i2c_master_transfer_t transfer;
    /* Send register address to read from */
    transfer.flags = kI2C_TransferDefaultFlag;
    transfer.slaveAddress = deviceAddr;
    transfer.direction = kI2C_Write;
    transfer.subaddress = regAddr;
    transfer.subaddressSize = 1;
    transfer.data = NULL;
    transfer.dataSize = 0;
    if (I2C_MasterTransferBlocking(I2C1, &transfer) != kStatus_Success)
    {
        return kStatus_Fail;
    }

    /* Read data from device */
    transfer.direction = kI2C_Read;
    transfer.subaddress = 0;
    transfer.subaddressSize = 0;
    transfer.data = data;
    transfer.dataSize = dataLen;
    if (I2C_MasterTransferBlocking(I2C1, &transfer) != kStatus_Success)
    {
        return kStatus_Fail;
    }

    return kStatus_Success;
}


