/**
 ********************************************************************************
 * @file    pulseIn.c
 * @author  user
 * @date    Nov 25, 2022
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
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

/************************************
 * STATIC FUNCTIONS
 ************************************/

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void pulseIn(PORT_Type port, uint8_t pin)
{
	/*// Pockat dokud na danem pinu nenarazime na vzestupnou hranu
	while(!(GPIOD->PDOR & pin));
	// Merit do te doby, dokud neni sestupna hrana
	time_t start, end;
	time
	time(&start);
	while(!(~(GPIOD->PDOR) & pin));
	time(&end);
	time_t res = end - start;
	PRINTF("Time: %i\r\n", res);*/

}

