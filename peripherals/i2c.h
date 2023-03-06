/**
 ***************************************************************************************************
 * @file    i2c.h
 * @author  user
 * @date    Mar 4, 2023
 * @brief   
 ***************************************************************************************************
 */

#ifndef I2C_H_
#define I2C_H_

#ifdef __cplusplus
extern "C" {
#endif

//**************************************************************************************************
//* INCLUDES
//**************************************************************************************************

//**************************************************************************************************
//* MACROS AND DEFINES
//**************************************************************************************************

//**************************************************************************************************
//* TYPEDEFS
//**************************************************************************************************

//**************************************************************************************************
//* EXPORTED VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* GLOBAL FUNCTION PROTOTYPES
//**************************************************************************************************
void i2cInit(uint8_t deviceAddr, uint8_t *data, size_t dataSize);
status_t i2cWrite(uint8_t deviceAddr, uint8_t regAddr, uint8_t data);
status_t i2cRead(uint8_t deviceAddr, uint8_t regAddr, uint8_t *data, uint32_t dataLen);


#ifdef __cplusplus
}
#endif

#endif /* I2C_H_ */
