/**
 ***************************************************************************************************
 * @file    startup_board.c
 * @author  user
 * @date    Nov 25, 2022
 * @brief   
 ***************************************************************************************************
 */


//**************************************************************************************************
//* INCLUDES
//**************************************************************************************************
#include "startup_peripherals.h"
#include "MKL25Z4.h"
#include "fsl_tpm.h"
#include "fsl_i2c.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "peripherals/isl29125.h"
#include "fsl_port.h"
#include "global_macros.h"

//**************************************************************************************************
//* EXTERN VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* PRIVATE MACROS AND DEFINES
//**************************************************************************************************

// Get source clock for TPM driver
#define TPM_SOURCE_CLOCK 		CLOCK_GetFreq(kCLOCK_PllFllSelClk)
#define I2C_MASTER_CLK_FREQ 	CLOCK_GetFreq(I2C0_CLK_SRC)
#define GPIO_HALL_IRQn			PORTD_IRQn
#define GPIO_COLOR_MAIN_IRQn	PORTA_IRQn

#define LEFT_TPM_IC				kTPM_Chnl_0
#define RIGHT_TPM_IC			kTPM_Chnl_4
#define CENTER_TPM_IC			kTPM_Chnl_3

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

//!*************************************************************************************************
//! static void init_tsi(void)
//!
//! @description
//! Function enables capacitive touch sensor on the MCU.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
static void init_tsi(void)
{
	SIM->SCGC5 |= SIM_SCGC5_TSI_MASK;
	TSI0->GENCS = TSI_GENCS_TSIEN_MASK | TSI_GENCS_MODE(0) | TSI_GENCS_REFCHRG(4) | TSI_GENCS_DVOLT(0) | TSI_GENCS_EXTCHRG(7);
	TSI0->DATA = TSI_DATA_TSICH(9) | TSI_DATA_TSICNT_MASK | TSI_DATA_SWTS_MASK;
}

//!*************************************************************************************************
//! static void init_leds()
//!
//! @description
//! Function enables leds on the MCU.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
static void init_leds(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;
	PORTB->PCR[18] = PORT_PCR_MUX(1);
	PORTB->PCR[19] = PORT_PCR_MUX(1);
	PORTD->PCR[1] = PORT_PCR_MUX(1);
	GPIOB->PDDR |= (1 << 18) | (1 << 19);
	GPIOD->PDDR |= (1<<1);
}


//!*************************************************************************************************
//! static void startupPWM(void)
//!
//! @description
//! Function sets appropriate parameters and starts pwm with duty cycle for stopping.
//! This starts both motors and servo.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
static void startupPWM(void)
{
	PRINTF("\t- Motor and servo.\r\n");
	// Motors
	tpm_config_t tpmInfo;
	tpm_chnl_pwm_signal_param_t tpmParam[2];

	// Configure tpm params with frequency 24kHZ
	tpmParam[0].chnlNumber = kTPM_Chnl_0;
	tpmParam[0].level = kTPM_HighTrue;
	tpmParam[0].dutyCyclePercent = 7.365000;

	tpmParam[1].chnlNumber = kTPM_Chnl_1;
	tpmParam[1].level = kTPM_HighTrue;
	tpmParam[1].dutyCyclePercent = 7.365000;

	// Select the clock source for the TPM counter as kCLOCK_PllFllSelClk
	CLOCK_SetTpmClock(1U);

	TPM_GetDefaultConfig(&tpmInfo);
	tpmInfo.prescale = kTPM_Prescale_Divide_32;
	TPM_Init(TPM1, &tpmInfo);

	TPM_SetupPwm(TPM1, tpmParam, 2U, kTPM_EdgeAlignedPwm, 50U,
			TPM_SOURCE_CLOCK);
	TPM_StartTimer(TPM1, kTPM_SystemClock);

	// Servo
	tpm_chnl_pwm_signal_param_t tpmParamSer;

	// Configure tpm params with frequency 24kHZ
	tpmParamSer.chnlNumber = kTPM_Chnl_5;
	tpmParamSer.level = kTPM_HighTrue;
	tpmParamSer.dutyCyclePercent = 50.0000;

	// Select the clock source for the TPM counter as kCLOCK_PllFllSelClk
	CLOCK_SetTpmClock(1U);

	TPM_GetDefaultConfig(&tpmInfo);
	tpmInfo.prescale = kTPM_Prescale_Divide_32;
	TPM_Init(TPM0, &tpmInfo);

	TPM_SetupPwm(TPM0, &tpmParamSer, 1U, kTPM_EdgeAlignedPwm, 1000U,
			TPM_SOURCE_CLOCK);
	TPM_StartTimer(TPM0, kTPM_SystemClock);

}

/*
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
*/

//!*************************************************************************************************
//! static void startupInterrupts(void)
//!
//! @description
//! Function starts interrupts for HALL sensor on one wheel.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
static void startupInterrupts(void)
{
	PRINTF("\t- Interrupts.\r\n");
	port_pin_config_t config;
	port_interrupt_t portInterrupt;

	// HALL sensors interrupt
	config.mux = kPORT_MuxAsGpio;
	config.pullSelect = kPORT_PullUp;
	portInterrupt = kPORT_InterruptFallingEdge;

	PORT_SetPinConfig(GPIO_HALL, HALL_PIN, &config);
	PORT_SetPinInterruptConfig(GPIO_HALL, HALL_PIN, portInterrupt);

	EnableIRQ(GPIO_HALL_IRQn);
}


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
	PRINTF("\t- Color sensor.\r\n");
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

	// todo: GPIO C4 set to HIGH, GPIO C5 set to LOW
}


//**************************************************************************************************
//* GLOBAL FUNCTIONS
//**************************************************************************************************

void startupInit(void)
{
	init_leds();
	init_tsi();
}

//!*************************************************************************************************
//! void startupBoard(void)
//!
//! @description
//! Function starts all used functionalities, which have not yet been launched in pin_mux.c.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
void startupBoard(void)
{
	PRINTF("Startup:\r\n");
	startupPWM();
	startupInterrupts();
	startupSensorCapture();

	//startupI2C();
}
