/**
 ***************************************************************************************************
 * @file    save_map.c
 * @author  user
 * @date    Apr 13, 2023
 * @brief   
 ***************************************************************************************************
 */

//**************************************************************************************************
//* INCLUDES
//**************************************************************************************************
#include "save_map.h"
#include "map_operations.h"
#include "fsl_spi.h"
#include <stdlib.h>
#include "SD.h"
#include "diskio.h"
#include "ff.h"
#include "utilities/fsl_debug_console.h"

//**************************************************************************************************
//* EXTERN VARIABLES
//**************************************************************************************************
extern HashTable *ht;

//**************************************************************************************************
//* PRIVATE MACROS AND DEFINES
//**************************************************************************************************
#define EXAMPLE_SPI_MASTER SPI0
#define EXAMPLE_SPI_MASTER_SOURCE_CLOCK kCLOCK_BusClk
#define EXAMPLE_SPI_MASTER_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)

//**************************************************************************************************
//* PRIVATE TYPEDEFS
//**************************************************************************************************
typedef struct {
    int x;
    int y;
} Coordinate;

//**************************************************************************************************
//* STATIC VARIABLES
//**************************************************************************************************
static const char file_name[] = "Map.csv";

//**************************************************************************************************
//* GLOBAL VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTION PROTOTYPES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTIONS
//**************************************************************************************************

static void initSPI()
{
	spi_master_config_t masterConfig = {0};
	uint32_t sourceClock = 0U;
	/*
	 * masterConfig.enableStopInWaitMode = false;
	 * masterConfig.polarity = kSPI_ClockPolarityActiveHigh;
	 * masterConfig.phase = kSPI_ClockPhaseFirstEdge;
	 * masterConfig.direction = kSPI_MsbFirst;
	 * masterConfig.dataMode = kSPI_8BitMode;
	 * masterConfig.txWatermark = kSPI_TxFifoOneHalfEmpty;
	 * masterConfig.rxWatermark = kSPI_RxFifoOneHalfFull;
	 * masterConfig.pinMode = kSPI_PinModeNormal;
	 * masterConfig.outputMode = kSPI_SlaveSelectAutomaticOutput;
	 * masterConfig.baudRate_Bps = 500000U;
	 */
	SPI_MasterGetDefaultConfig(&masterConfig);
	sourceClock = EXAMPLE_SPI_MASTER_CLK_FREQ;
	SPI_MasterInit(EXAMPLE_SPI_MASTER, &masterConfig, sourceClock);
}

static void writeToFile()
{
	FATFS 	fs;
	FRESULT fr;
	FIL		fil;
	UINT 	bw;

	fr = f_mount(&fs, file_name, 0);
	if(fr)
	{
		PRINTF("\nError mounting file system\r\n");
		for(;;){}
	}
	fr = f_open(&fil, file_name, FA_CREATE_NEW | FA_WRITE);
	if(fr)
	{
		PRINTF("\nError opening text file\r\n");
		for(;;){}
	}
	fr = f_write(&fil, "Test1 ,Test2 ,Test3 ,Test4 \r\n", 29, &bw);
	if(fr)
	{
		PRINTF("\nError write text file\r\n");
		for(;;){}
	}
	fr = f_close(&fil);
	if(fr)
	{
		PRINTF("\nError close text file\r\n");
		for(;;){}
	}
}

//**************************************************************************************************
//* GLOBAL FUNCTIONS
//**************************************************************************************************

//!*************************************************************************************************
//! void function(void)
//!
//! @description
//! Function
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
void saveMap()
{
	// dynamically allocate field to store coordinates
	Coordinate **coordinates = (Coordinate **)malloc(ht->count * sizeof(Coordinate *));
	if (coordinates == NULL) {
		// handle allocation error
		return;
	}
	// iterate over all allocated blocks and save their coordinates
	int index = 0;
	for (int i = 0; i < ht->size; i++)
	{
		Node *node = ht->table[i];
		while (node != NULL)
		{
			coordinates[index] = (Coordinate *)malloc(sizeof(Coordinate));
			coordinates[index]->x = node->value->corX;
			coordinates[index]->y = node->value->corY;
			index++;
			node = node->next;
		}
	}


}
