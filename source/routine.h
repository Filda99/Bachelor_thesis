/**
 ********************************************************************************
 * @file    routine.h
 * @author  user
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
#include "stdint.h"

/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/
typedef struct
{
	uint32_t x;
	uint32_t y;
}point;

typedef struct
{
	point initial;
	point final;
}vector;

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void routine(void);

#ifdef __cplusplus
}
#endif

#endif 
