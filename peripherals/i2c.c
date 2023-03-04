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
#define I2C_MASTER_CLOCK_FREQUENCY (12000000)
#define I2C_MASTER_SLAVE_ADDR_7BIT (0x52U)
#define I2C_MASTER_TX_CMD (0x01U)

//**************************************************************************************************
//* PRIVATE TYPEDEFS
//**************************************************************************************************

//**************************************************************************************************
//* STATIC VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* GLOBAL VARIABLES
//**************************************************************************************************
i2c_master_config_t masterConfig;

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
//! void function(void)
//!
//! @description
//! Function
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************



void i2c_init_my(void)
{
    i2c_master_transfer_t masterXfer;

    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = 400000;
    I2C_MasterInit(I2C1, &masterConfig, I2C_MASTER_CLOCK_FREQUENCY);

    masterXfer.flags = kI2C_TransferDefaultFlag;
    masterXfer.slaveAddress = I2C_MASTER_SLAVE_ADDR_7BIT;
    masterXfer.direction = kI2C_Write;
    masterXfer.subaddress = I2C_MASTER_TX_CMD;
    masterXfer.subaddressSize = 1;
    masterXfer.data = NULL;
    masterXfer.dataSize = 0;

    I2C_MasterTransferBlocking(I2C1, &masterXfer);
}

void I2C_WriteByte(uint8_t reg, uint8_t data)
{
    uint8_t buf[2];
    i2c_master_transfer_t masterXfer;

    buf[0] = reg;
    buf[1] = data;

    masterXfer.slaveAddress = I2C_MASTER_SLAVE_ADDR_7BIT;
    masterXfer.direction = kI2C_Write;
    masterXfer.subaddress = 0U;
    masterXfer.subaddressSize = 0U;
    masterXfer.data = buf;
    masterXfer.dataSize = 2U;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    I2C_MasterTransferBlocking(I2C1, &masterXfer);
}

void i2c_write_reg(uint8_t deviceAddr, uint8_t regAddr, uint8_t value)
{
    i2c_master_transfer_t transfer;
    uint8_t txBuff[2];

    txBuff[0] = regAddr;
    txBuff[1] = value;

    transfer.flags = kI2C_TransferDefaultFlag;
    transfer.slaveAddress = deviceAddr;
    transfer.direction = kI2C_Write;
    transfer.subaddress = 0;
    transfer.subaddressSize = 0;
    transfer.data = txBuff;
    transfer.dataSize = 2;

    I2C_MasterTransferBlocking(I2C1, &transfer);
}


uint8_t I2C_ReadByte(uint8_t reg)
{
    uint8_t buf[1];
    i2c_master_transfer_t masterXfer;

    masterXfer.slaveAddress = I2C_MASTER_SLAVE_ADDR_7BIT;
    masterXfer.direction = kI2C_Write;
    masterXfer.subaddress = reg;
    masterXfer.subaddressSize = 1;
    masterXfer.data = NULL;
    masterXfer.dataSize = 0;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    I2C_MasterTransferBlocking(I2C1, &masterXfer);

    masterXfer.slaveAddress = I2C_MASTER_SLAVE_ADDR_7BIT;
    masterXfer.direction = kI2C_Read;
    masterXfer.subaddress = 0U;
    masterXfer.subaddressSize = 0U;
    masterXfer.data = buf;
    masterXfer.dataSize = 1U;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    I2C_MasterTransferBlocking(I2C1, &masterXfer);

    return buf[0];
}

status_t i2c_read_reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint32_t data_len)
{
    i2c_master_transfer_t transfer = {0};

    /* Send register address to read from */
    transfer.flags = kI2C_TransferDefaultFlag;
    transfer.slaveAddress = dev_addr;
    transfer.direction = kI2C_Write;
    transfer.subaddress = reg_addr;
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
    transfer.dataSize = data_len;
    if (I2C_MasterTransferBlocking(I2C1, &transfer) != kStatus_Success)
    {
        return kStatus_Fail;
    }

    return kStatus_Success;
}





