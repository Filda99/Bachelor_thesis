/**
 ********************************************************************************
 * @file    startup_peripherals.c
 * @author  user
 * @date    Jul 30, 2022
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "common/delay.h"
#include "fsl_tpm.h"
#include "fsl_debug_console.h"
#include "global_macros.h"

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

#define I2C_MASTER_CLK_FREQ 	CLOCK_GetFreq(I2C0_CLK_SRC)
#define GPIO_COLOR_MAIN_IRQn	PORTA_IRQn

#define LEFT_TPM_IC				kTPM_Chnl_0
#define RIGHT_TPM_IC			kTPM_Chnl_4
#define CENTER_TPM_IC			kTPM_Chnl_3

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


//!*************************************************************************************************
//! static void startupSensorCapture()
//!
//! @description
//! Function sets input compare(input capture) function for color sensors.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
static void startupSensorCapture()
{
	PRINTF("\t- Color sensor initialization.\r\n");
	tpm_config_t tpmInfo;

	// Select the clock source for the TPM counter as kCLOCK_PllFllSelClk
	CLOCK_SetTpmClock(1U);

	TPM_GetDefaultConfig(&tpmInfo);
	TPM_Init(MAIN_SEN_TPM_BASE, &tpmInfo);

	TPM_SetupInputCapture(MAIN_SEN_TPM_BASE, LEFT_TPM_IC, kTPM_RisingEdge);
	TPM_SetupInputCapture(MAIN_SEN_TPM_BASE, RIGHT_TPM_IC, kTPM_RisingEdge);
	TPM_SetupInputCapture(MAIN_SEN_TPM_BASE, CENTER_TPM_IC, kTPM_RisingEdge);

    TPM_EnableInterrupts(MAIN_SEN_TPM_BASE, kTPM_Chnl0InterruptEnable);
    TPM_EnableInterrupts(MAIN_SEN_TPM_BASE, kTPM_Chnl4InterruptEnable);
    TPM_EnableInterrupts(MAIN_SEN_TPM_BASE, kTPM_Chnl3InterruptEnable);
    EnableIRQ(MAIN_SEN_TPM_IRQ);


	TPM_StartTimer(MAIN_SEN_TPM_BASE, kTPM_SystemClock);

	delay_ms(100);
}

//!*************************************************************************************************
//! static void initMotors()
//!
//! @description
//! Function initialize motors.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
static void initMotors()
{
	PRINTF("\t\t- Motors initialization started.");
	static float initDutyCycleStep = 0.06;

	float initDutyCycle = 2.88;

	delay_ms(100);

	for (int i = 0; i < 100; i++)
	{
		TPM_UpdatePwmDutycycle(TPM1, kTPM_Chnl_0, kTPM_CenterAlignedPwm, initDutyCycle);
		TPM_UpdatePwmDutycycle(TPM1, kTPM_Chnl_1, kTPM_CenterAlignedPwm, initDutyCycle);
		delay_ms(2);

		initDutyCycle += initDutyCycleStep;
		if( (i % 10) == 0) PRINTF(".");
	}

	// After inicialization, stop motors
	TPM_UpdatePwmDutycycle(TPM1, kTPM_Chnl_0, kTPM_CenterAlignedPwm, 7.365000);
	TPM_UpdatePwmDutycycle(TPM1, kTPM_Chnl_1, kTPM_CenterAlignedPwm, 7.365000);
	delay_ms(50);
	PRINTF("\r\n\t\t- Motors initialization complete.\r\n");
}

//!*************************************************************************************************
//! static void initServo()
//!
//! @description
//! Function initialize servomotor.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
static void initServo()
{
	PRINTF("\t\t- Servo initialization started.\r\n");
	TPM_UpdatePwmDutycycle(TPM0, kTPM_Chnl_5, kTPM_CenterAlignedPwm, 7.5);
	delay_ms(100);
	TPM_UpdatePwmDutycycle(TPM0, kTPM_Chnl_5, kTPM_CenterAlignedPwm, 3.735);
	delay_ms(100);
	TPM_UpdatePwmDutycycle(TPM0, kTPM_Chnl_5, kTPM_CenterAlignedPwm, 11.205);
	delay_ms(100);
	TPM_UpdatePwmDutycycle(TPM0, kTPM_Chnl_5, kTPM_CenterAlignedPwm, 7.5);
	delay_ms(100);
	PRINTF("\t\t- Servo initialization complete.\r\n");
}

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void startupPeripherals()
{
	// Servo and motors
	startupSensorCapture();

	initMotors();
	initServo();
}
