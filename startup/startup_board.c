/**
 ********************************************************************************
 * @file    startup_board.c
 * @author  user
 * @date    Nov 25, 2022
 * @brief   
 ********************************************************************************
 */


/************************************
 * INCLUDES
 ************************************/
#include "startup_peripherals.h"
#include "MKL25Z4.h"
#include "fsl_tpm.h"
#include "fsl_i2c.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "peripherals/isl29125.h"
#include "fsl_port.h"
#include "global_macros.h"

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
/* Get source clock for TPM driver */
#define TPM_SOURCE_CLOCK 		CLOCK_GetFreq(kCLOCK_PllFllSelClk)
#define I2C_MASTER_CLK_FREQ 	CLOCK_GetFreq(I2C0_CLK_SRC)
#define GPIO_HALL_IRQn			PORTA_IRQn
#define GPIO_COLOR_MAIN_IRQn	PORTD_IRQn

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
static void startupPWM(void)
{
	tpm_config_t tpmInfo;
	tpm_chnl_pwm_signal_param_t tpmParam[2];

	// Configure tpm params with frequency 24kHZ
	tpmParam[0].chnlNumber = kTPM_Chnl_0;
	tpmParam[0].level = kTPM_HighTrue;
	tpmParam[0].dutyCyclePercent = 7.365;

	tpmParam[1].chnlNumber = kTPM_Chnl_1;
	tpmParam[1].level = kTPM_HighTrue;
	tpmParam[1].dutyCyclePercent = 7.365;

	// Select the clock source for the TPM counter as kCLOCK_PllFllSelClk
	CLOCK_SetTpmClock(1U);

	TPM_GetDefaultConfig(&tpmInfo);
	tpmInfo.prescale = kTPM_Prescale_Divide_32;
	// Initialize TPM module
	TPM_Init(TPM1, &tpmInfo);

	TPM_SetupPwm(TPM1, tpmParam, 2U, kTPM_EdgeAlignedPwm, 50U,
			TPM_SOURCE_CLOCK);
	TPM_StartTimer(TPM1, kTPM_SystemClock);
}

static void startupI2C(void)
{
    // ULTRASONIC masterXfer.slaveAddress = 0x57U;
	i2c_master_config_t config;

	I2C_MasterGetDefaultConfig(&config);

	uint32_t srcClk = I2C_MASTER_CLK_FREQ;
	I2C_MasterInit(I2C0, &config, srcClk);

	bool ColorSensor = ISL_ColorSensorInit();

	if(ColorSensor)
	{
		uint8_t redColor[2];
		ISL_readRed(redColor);
		redColor[1]++;
	}
}

static void startupInterrupts(void)
{
	port_pin_config_t config;
	port_interrupt_t portInterrupt;

	// HALL sensors interrupt
	config.mux = kPORT_MuxAsGpio;
	config.pullSelect = kPORT_PullDisable;
	portInterrupt = kPORT_InterruptFallingEdge;

	PORT_SetMultiplePinsConfig(GPIO_HALL, HALL_IRQ_MASK, &config);
	PORT_SetPinInterruptConfig(GPIO_HALL, HALL_IRQ_MASK, portInterrupt);

	EnableIRQ(GPIO_HALL_IRQn);
	// Color sensor interrupt
	/*port_pin_config_t configSen;
	configSen.mux = kPORT_MuxAsGpio;
	configSen.pullSelect = kPORT_PullDown;*/
	/*port_pin_config_t config = {
	       kPORT_PullUp ,
	       kPORT_PullEnable,
	       kPORT_FastSlewRate,
	       kPORT_PassiveFilterDisable,
	       kPORT_OpenDrainDisable,
	       kPORT_LowDriveStrength,
	       kPORT_MuxAsGpio,
	       kPORT_UnlockRegister,
	  };
	portInterrupt = kPORT_InterruptEitherEdge;

	//PORT_SetPinConfig(GPIO_COLOR_MAIN_SEN, 0, &config);
	PORT_SetMultiplePinsConfig(GPIO_COLOR_MAIN_SEN, COLOR_MAIN_IRQ_MASK, &configSen);
	PORT_SetPinInterruptConfig(GPIO_COLOR_MAIN_SEN, COLOR_MAIN_IRQ_MASK, portInterrupt);

	EnableIRQ(GPIO_COLOR_MAIN_IRQn);*/
}

static void startupSensorCapture()
{
	// Select the clock source for the TPM counter as kCLOCK_PllFllSelClk
	/*CLOCK_SetTpmClock(1U);

	tpm_config_t tpmInfo;

	TPM_GetDefaultConfig(&tpmInfo);
	tpmInfo.prescale = kTPM_Prescale_Divide_32;
	// Initialize TPM module
	TPM_Init(TPM0, &tpmInfo);

	TPM_SetupInputCapture(TPM0, kTPM_Chnl_0, kTPM_RiseAndFallEdge);*/
	//TPM_SetupInputCapture(TPM0, kTPM_Chnl_2, kTPM_RiseAndFallEdge);
	//TPM_SetupInputCapture(TPM0, kTPM_Chnl_3, kTPM_RiseAndFallEdge);
}

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void startupBoard(void)
{
	//startupI2C();
	//startupPWM();
	//startupSensorCapture();
	startupInterrupts();
}
