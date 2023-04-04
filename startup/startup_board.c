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
#include "common/delay.h"
#include "startup_peripherals.h"
#include "MKL25Z4.h"
#include "fsl_tpm.h"
#include "fsl_i2c.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_port.h"
#include "global_macros.h"

//**************************************************************************************************
//* EXTERN VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* PRIVATE MACROS AND DEFINES
//**************************************************************************************************
#define I2C_MASTER_CLOCK_FREQUENCY CLOCK_GetFreq(kCLOCK_CoreSysClk)

#define PORT_PCR_ODE_MASK                   0x00000200u

// Get source clock for TPM driver
#define TPM_SOURCE_CLOCK 		CLOCK_GetFreq(kCLOCK_PllFllSelClk)
#define GPIO_HALL_IRQn			PORTD_IRQn


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


/*static void cleanUpBoard()
{
	uint32_t* sram_start_address = (uint32_t*)0x1FFFF000; // Start address of SRAM
	uint32_t sram_size = 0x4000; // Size of SRAM
	uint32_t i;


	// Erase SRAM memory
	for (i = 0; i < sram_size/4; i++)
	{
		*(sram_start_address + i) = 0;
	}

}*/

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
	PRINTF("\t- Motor and servo pwm initialization.\r\n");
	// Motors
	// todo: Motor Configuration
	tpm_config_t tpmInfo;
	tpm_chnl_pwm_signal_param_t tpmParam[2];

	// Configure tpm params with frequency 24kHZ
	tpmParam[0].chnlNumber = kTPM_Chnl_0;
	tpmParam[0].level = kTPM_HighTrue;

	tpmParam[1].chnlNumber = kTPM_Chnl_1;
	tpmParam[1].level = kTPM_HighTrue;

	// Select the clock source for the TPM counter as kCLOCK_PllFllSelClk
	CLOCK_SetTpmClock(1U);

	TPM_GetDefaultConfig(&tpmInfo);
	tpmInfo.prescale = kTPM_Prescale_Divide_16;
	TPM_Init(TPM1, &tpmInfo);

	TPM_SetupPwm(TPM1, tpmParam, 2U, kTPM_CenterAlignedPwm, 50U,
			TPM_SOURCE_CLOCK);
	TPM_StartTimer(TPM1, kTPM_SystemClock);

	// Servo
	tpm_chnl_pwm_signal_param_t tpmParamSer;

	// Configure tpm params with frequency 24kHZ
	tpmParamSer.chnlNumber = kTPM_Chnl_5;
	tpmParamSer.level = kTPM_HighTrue;
	tpmParamSer.dutyCyclePercent = 7.37;

	// Select the clock source for the TPM counter as kCLOCK_PllFllSelClk
	CLOCK_SetTpmClock(1U);

	TPM_GetDefaultConfig(&tpmInfo);
	tpmInfo.prescale = kTPM_Prescale_Divide_32;
	TPM_Init(TPM0, &tpmInfo);

	TPM_SetupPwm(TPM0, &tpmParamSer, 1U, kTPM_EdgeAlignedPwm, 50U,
			TPM_SOURCE_CLOCK);
	TPM_StartTimer(TPM0, kTPM_SystemClock);
}


//!*************************************************************************************************
//! static void startupInterrupts(void)
//!
//! @description
//! Function starts interrupts for HALL sensor on the left wheel.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
static void startupInterrupts(void)
{
	PRINTF("\t- Hall interrupt initialization.\r\n");
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
//! void i2c_init(void)
//!
//! @description
//! Function initialize i2c master device.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
void i2cInit(void)
{
	i2c_master_config_t masterConfig;
    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = 200000;
    I2C_MasterInit(USING_I2C, &masterConfig, I2C_MASTER_CLOCK_FREQUENCY);
    //I2C0->F = 0b01000001;
    //I2C0->F = 0x23;
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
	PRINTF("Startup board and peripherals.\r\n");

	//cleanUpBoard();
	startupPWM();
	startupInterrupts();
	i2cInit();

	PRINTF("Startup board and peripherals complete.\r\n");
}
