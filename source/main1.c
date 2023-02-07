/*
 * main1.c
 *
 *  Created on: Feb 6, 2023
 *      Author: user
 */


#include <stdio.h>
#include "fsl_i2c.h"
#include "fsl_debug_console.h"

#define M5_I2C_ADDRESS 0x57

i2c_master_transfer_t masterXfer;
uint8_t data[2];

void i2c_init(void)
{
    i2c_master_config_t masterConfig;
    I2C_MasterGetDefaultConfig(&masterConfig);
    I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetFreq(I2C0_CLK_SRC));
}

void i2c_read(void)
{
    masterXfer.slaveAddress = M5_I2C_ADDRESS;
    masterXfer.direction = kI2C_Read;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = data;
    masterXfer.dataSize = 2;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    I2C_MasterTransferBlocking(I2C0, &masterXfer);
}

int main1(void)
{
    i2c_init();

    while (1)
    {
        i2c_read();
        PRINTF("Data: %d\n", (data[0] << 8) | data[1]);
    }

    return 0;
}
