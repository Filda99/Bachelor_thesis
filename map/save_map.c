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
extern int maxTopBlock;
extern int maxBottomBlock;
extern int maxLeftBlock;
extern int maxRightBlock;

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
static FIL	file;

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

static bool openFile()
{
	FATFS 	fs;
	FRESULT fr;

	fr = f_mount(&fs, file_name, 0);
	if(fr)
	{
		PRINTF("\nError mounting file system\r\n");
		return false;
	}
	fr = f_open(&file, file_name, FA_CREATE_NEW | FA_WRITE);
	if(fr)
	{
		PRINTF("\nError opening text file\r\n");
		return false;
	}

	return true;
}

static bool closeFile()
{
	FRESULT fr;
	fr = f_close(&file);
	if(fr)
	{
		PRINTF("\nError close text file\r\n");
		return 0;
	}

	return 1;
}

static bool writeToFile(uint8_t data)
{
	FRESULT fr;
	UINT 	bw;
	char charToBeSend;

	memcpy(&charToBeSend, &data, 1);

	//fr = f_write(&file, &charToBeSend, 1, &bw);
	PRINTF("Saving: %i\r\n", data);
	if(fr)
	{
		PRINTF("\nError write text file\r\n");
		return false;
	}
	return true;
}

static void saveBlocks(map_block* blockBuffer, int bufferCount)
{
	int startIndexBuffer = 0;				// Starting index of block row in block field of a row we are processing
	int processingRow = blockBuffer->corY;	// Stores number of row which we are currently processing
	int prevProcessedBlockColumn = blockBuffer->corX;	// Stores previously processed column which is helpful for filling gaps
	uint8_t processedRows = 0;

	// Go through all blocks in the buffer
	for (int indexBuffer = startIndexBuffer; indexBuffer < bufferCount;)
	{
		// First block only, print unnecessarily gaps
		while (blockBuffer[indexBuffer].corX - 1 - indexBuffer > maxLeftBlock)
		{
			// Print the gap
			for (int columnBlock = 0; columnBlock < MAP_COLUMNS; columnBlock++)
			{
				writeToFile(0);
			}
		}
		// While we are processing the subset of the blocks which are on the same row
		while (processingRow == blockBuffer[indexBuffer].corY)
		{
			// If there is some gap, fill it
			while (blockBuffer[indexBuffer].corX - 1 > prevProcessedBlockColumn)
			{
				// Print the gap
				for (int columnBlock = 0; columnBlock < MAP_COLUMNS; columnBlock++)
				{
					writeToFile(0);
				}
			}
			// Print the current processing row
			for (int columnBlock = 0; columnBlock < MAP_COLUMNS; columnBlock++)
			{
				writeToFile(blockBuffer[indexBuffer].block[processedRows][columnBlock]);
			}
			// Move to the next block
			prevProcessedBlockColumn = blockBuffer[indexBuffer].corX;
			indexBuffer++;
			processingRow++;
			indexBuffer = startIndexBuffer;
			processedRows++;
		}
		processingRow = blockBuffer[indexBuffer].corY;
		if (processedRows < MAP_ROWS)
		{
			indexBuffer = startIndexBuffer;
		}
		else
		{
			processedRows = 0;
			startIndexBuffer = ++indexBuffer;
		}
	}
}

static void sort_coordinates(map_block* block, int num_coords) {
    int i, j;
    map_block temp;

    // Bubble sort by Y coordinate
    for (i = 0; i < num_coords - 1; i++) {
        for (j = 0; j < num_coords - i - 1; j++) {
            if (block[j].corY < block[j+1].corY) {
                temp = block[j];
                block[j] = block[j+1];
                block[j+1] = temp;
            }
        }
    }

    // Bubble sort by X coordinate within each Y group
    int start_idx = 0;
    for (i = 0; i < num_coords - 1; i++) {
        if (block[i].corY != block[i+1].corY) {
            for (j = start_idx; j < i; j++) {
                for (int k = start_idx; k < i - (j - start_idx); k++) {
                    if (block[k].corX > block[k+1].corX) {
                        temp = block[k];
                        block[k] = block[k+1];
                        block[k+1] = temp;
                    }
                }
            }
            start_idx = i+1;
        }
    }

    // Sort last Y group by X coordinate
    for (j = start_idx; j < num_coords - 1; j++) {
        for (int k = start_idx; k < num_coords - 1 - (j - start_idx); k++) {
            if (block[k].corX > block[k+1].corX) {
                temp = block[k];
                block[k] = block[k+1];
                block[k+1] = temp;
            }
        }
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
	// dynamically allocate field to store map blocks in the buffer
	map_block *blockBuffer = (map_block *)malloc(ht->count * sizeof(map_block));
	if (blockBuffer == NULL) {
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
			blockBuffer[index] = *(node->value);
			index++;
			node = node->next;
		}
	}

	sort_coordinates(blockBuffer, ht->count);
	saveBlocks(blockBuffer, ht->count);
}
