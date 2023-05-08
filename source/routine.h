/**
 ********************************************************************************
 * @file    routine.h
 * @author  xjahnf00
 * @date    Aug 2, 2022
 * @brief   
 ********************************************************************************
 */

#ifndef ROUTINE_H_
#define ROUTINE_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include "fsl_i2c_dma.h"
#include "fsl_i2c.h"

/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void routine(void);
void i2c_slave_callback(I2C_Type *base, i2c_slave_transfer_t *xfer, void *userData);

#ifdef __cplusplus
}
#endif

#endif 
