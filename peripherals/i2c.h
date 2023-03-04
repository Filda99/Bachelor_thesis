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

status_t i2c_read_reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint32_t data_len);
void i2c_write_reg(uint8_t deviceAddr, uint8_t regAddr, uint8_t value);
void i2c_init_my(void);


#ifdef __cplusplus
}
#endif

#endif /* I2C_H_ */
