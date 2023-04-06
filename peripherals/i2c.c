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
#include "global_macros.h"
#include "../utilities/fsl_debug_console.h"

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
void i2cInit(uint32_t baudRate)
{
    i2c_master_config_t masterConfig;
    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = baudRate;
    I2C_MasterInit(CAM_I2C, &masterConfig, I2C_MASTER_CLOCK_FREQUENCY);

    I2C_Enable(CAM_I2C, true);
    PRINTF("I2C initialized---------\r\n");
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
    //uint8_t buff[2];
    //uint32_t tmp;
    //buff[0] = regAddr;
    //buff[1] = data;

    uint8_t buff[1];
    buff[0] = data;

    transfer.flags = kI2C_TransferDefaultFlag;
    transfer.slaveAddress = deviceAddr;
    transfer.direction = kI2C_Write;
    transfer.subaddress = 0;
    transfer.subaddressSize = 0;
    //transfer.data = buff;
    transfer.data = buff;
    //transfer.dataSize = 2;
    transfer.dataSize = 1;

    i2c_master_transfer_callback_t callback;
    i2c_master_handle_t g_m_handle;
    //memset(&g_m_handle, 0, sizeof(g_m_handle));
    I2C_MasterTransferCreateHandle(CAM_I2C, &g_m_handle, callback, NULL);

    I2C_MasterTransferBlocking(CAM_I2C, &transfer);
   /* if (I2C_MasterTransferNonBlocking(CAM_I2C,&g_m_handle, &transfer) != kStatus_Success)
	{
		return kStatus_Fail;
	}
    I2C_MasterTransferHandleIRQ(CAM_I2C, &g_m_handle);*/

    while(g_m_handle.state != 0x0U){ //idle state
    	I2C_MasterTransferHandleIRQ(CAM_I2C, &g_m_handle);
    }
    /*
    if(){
    	PRINTF("I2C data send fail---------\r\n");
    }else{
    	PRINTF("I2C data send done---------\r\n");
    }
	*/
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
	PRINTF("I2C receive byte---------\r\n");
    i2c_master_transfer_t transfer;
    /* Send register address to read from */
    transfer.flags = kI2C_TransferDefaultFlag;
    transfer.slaveAddress = deviceAddr;
    //transfer.direction = kI2C_Write;
    //transfer.subaddress = regAddr;
    //transfer.subaddressSize = 1;
    //transfer.data = NULL;
    //transfer.dataSize = 0;

    transfer.direction = kI2C_Read;
    transfer.subaddress = 0;
    transfer.subaddressSize = 0;
    transfer.data = data;
    transfer.dataSize = dataLen;

    i2c_master_transfer_callback_t callback;
    i2c_master_handle_t g_m_handle;
        //memset(&g_m_handle, 0, sizeof(g_m_handle));
    I2C_MasterTransferCreateHandle(CAM_I2C, &g_m_handle, callback, NULL);



    if (I2C_MasterTransferNonBlocking(CAM_I2C,&g_m_handle, &transfer) != kStatus_Success)
    {
        return kStatus_Fail;
    }
    I2C_MasterTransferHandleIRQ(CAM_I2C, &g_m_handle);
    /*
    if(){
    	PRINTF("I2C transfer check fail---------\r\n");
    }else{
    	PRINTF("I2C transfer check done---------\r\n");
    }
	*/
    /* Read data from device */

    /*
    transfer.direction = kI2C_Read;
    transfer.subaddress = 0;
    transfer.subaddressSize = 0;
    transfer.data = data;
    transfer.dataSize = dataLen;
	*/
    /*
    if (I2C_MasterTransferNonBlocking(CAM_I2C,&g_m_handle, &transfer) != kStatus_Success)
    {
        return kStatus_Fail;
    }
    */
    I2C_MasterTransferHandleIRQ(CAM_I2C, &g_m_handle);
    /*
    if(){
    	PRINTF("I2C data recv fail---------\r\n");
    }else{
    	PRINTF("I2C data recv done---------\r\n");
    }
	*/
    return kStatus_Success;
}


