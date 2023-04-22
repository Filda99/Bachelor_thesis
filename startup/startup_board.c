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
#include "startup_board.h"

#include "common/delay.h"
#include "startup_peripherals.h"
#include "MKL25Z4.h"
#include "fsl_tpm.h"
#include "fsl_i2c.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_port.h"
#include "global_macros.h"
#include "fsl_adc16.h"

#include "routine.h"

//**************************************************************************************************
//* EXTERN VARIABLES
//**************************************************************************************************
extern dma_handle_t dmaHandle;

//**************************************************************************************************
//* PRIVATE MACROS AND DEFINES
//**************************************************************************************************
#define I2C_MASTER_CLOCK_FREQUENCY CLOCK_GetFreq(kCLOCK_CoreSysClk)

#define PORT_PCR_ODE_MASK                   0x00000200u

// Get source clock for TPM driver
#define TPM_SOURCE_CLOCK 		CLOCK_GetFreq(kCLOCK_PllFllSelClk)
#define GPIO_HALL_IRQn			PORTD_IRQn



#define EXAMPLE_I2C_MASTER_BASEADDR I2C0
#define EXAMPLE_I2C_SLAVE_BASEADDR I2C1
#define I2C_MASTER_CLK_SRC I2C0_CLK_SRC
#define I2C_MASTER_CLK_FREQ CLOCK_GetFreq(I2C0_CLK_SRC)
#define I2C_SLAVE_CLK_SRC I2C1_CLK_SRC
#define I2C_SLAVE_CLK_FREQ CLOCK_GetFreq(I2C1_CLK_SRC)
#define EXAMPLE_I2C_DMAMUX_BASEADDR DMAMUX0
#define EXAMPLE_I2C_DMA_BASEADDR DMA0
#define I2C_DMA_CHANNEL 0U

#define DMA_REQUEST_SRC kDmaRequestMux0I2C0
#define I2C_MASTER_SLAVE_ADDR_7BIT 0x7EU
#define I2C_BAUDRATE 100000U

#define DEMO_ADC16_USER_CHANNEL 0U /*PTE20, ADC0_SE0 */


//**************************************************************************************************
//* PRIVATE TYPEDEFS
//**************************************************************************************************

//**************************************************************************************************
//* STATIC VARIABLES
//**************************************************************************************************
i2c_slave_handle_t g_s_handle;

//**************************************************************************************************
//* GLOBAL VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTION PROTOTYPES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTIONS
//**************************************************************************************************


static void init_adc(void)
{
	adc16_config_t adc16ConfigStruct;
	adc16_channel_config_t adc16ChannelConfigStruct;

	 /*
	 * adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceVref;
	 * adc16ConfigStruct.clockSource = kADC16_ClockSourceAsynchronousClock;
	 * adc16ConfigStruct.enableAsynchronousClock = true;
	 * adc16ConfigStruct.clockDivider = kADC16_ClockDivider8;
	 * adc16ConfigStruct.resolution = kADC16_ResolutionSE12Bit;
	 * adc16ConfigStruct.longSampleMode = kADC16_LongSampleDisabled;
	 * adc16ConfigStruct.enableHighSpeed = false;
	 * adc16ConfigStruct.enableLowPower = false;
	 * adc16ConfigStruct.enableContinuousConversion = false;
	 */
	ADC16_GetDefaultConfig(&adc16ConfigStruct);

	ADC16_Init(DEMO_ADC16_BASE, &adc16ConfigStruct);
	ADC16_EnableHardwareTrigger(DEMO_ADC16_BASE, false); /* Make sure the software trigger is used. */

	adc16ChannelConfigStruct.channelNumber = DEMO_ADC16_USER_CHANNEL;
	adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
    adc16ChannelConfigStruct.enableDifferentialConversion = false;

	ADC16_SetChannelConfig(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP, &adc16ChannelConfigStruct);
}

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
	tpmParamSer.dutyCyclePercent = 7.375;

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
    I2C_MasterInit(SAVING_I2C, &masterConfig, I2C_MASTER_CLOCK_FREQUENCY);
}

void i2cInitSlave()
{
	i2c_slave_config_t slaveConfig;

	DMAMUX_Init(EXAMPLE_I2C_DMAMUX_BASEADDR);
	DMA_Init(EXAMPLE_I2C_DMA_BASEADDR);

	PRINTF("\r\nI2C example -- MasterDMA_SlaveInterrupt.\r\n");

	I2C_SlaveGetDefaultConfig(&slaveConfig);

	slaveConfig.addressingMode = kI2C_Address7bit;
	slaveConfig.slaveAddress = I2C_MASTER_SLAVE_ADDR_7BIT;
	slaveConfig.upperAddress = 0;

	I2C_SlaveInit(DATA_I2C, &slaveConfig, I2C_SLAVE_CLK_FREQ);

	memset(&g_s_handle, 0, sizeof(g_s_handle));

	I2C_SlaveTransferCreateHandle(DATA_I2C, &g_s_handle, i2c_slave_callback, NULL);
	I2C_SlaveTransferNonBlocking(DATA_I2C, &g_s_handle, kI2C_SlaveCompletionEvent);

	DMAMUX_SetSource(EXAMPLE_I2C_DMAMUX_BASEADDR, I2C_DMA_CHANNEL, DMA_REQUEST_SRC);
	DMAMUX_EnableChannel(EXAMPLE_I2C_DMAMUX_BASEADDR, I2C_DMA_CHANNEL);
	DMA_CreateHandle(&dmaHandle, EXAMPLE_I2C_DMA_BASEADDR, I2C_DMA_CHANNEL);
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
	i2cInitSlave();
	init_adc();

	PRINTF("Startup board and peripherals complete.\r\n");
}
