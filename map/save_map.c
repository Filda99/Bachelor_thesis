/**
 ***************************************************************************************************
 * @file    save_map.c
 * @author  xjahnf00
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
#include "SD.h"
#include "diskio.h"
#include "ff.h"
#include "utilities/fsl_debug_console.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "board.h"
#include "common/delay.h"

//**************************************************************************************************
//* EXTERN VARIABLES
//**************************************************************************************************
extern HashTable *ht;
extern int maxBottomBlock;
extern int maxLeftBlock;

//**************************************************************************************************
//* PRIVATE MACROS AND DEFINES
//**************************************************************************************************
#define EXAMPLE_SPI_MASTER SPI0
#define EXAMPLE_SPI_MASTER_SOURCE_CLOCK kCLOCK_BusClk
#define EXAMPLE_SPI_MASTER_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)

#define BUFFER_SIZE 15

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

//**************************************************************************************************
//* GLOBAL VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTION PROTOTYPES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTIONS
//**************************************************************************************************

static void insertValue(int newValue, char **stringPtr, size_t *currentSizePtr) {
    // Convert uint8_t value to string and add ", "
    char valueStr[3] = " , ";
    //snprintf(valueStr, sizeof(valueStr), "%d, ", newValue);
    if (newValue == 0)
    	valueStr[0] = '-';	// Pass zero's, because they are nothing.
    else if (newValue == 1)
    	valueStr[0] = '1';
    else if (newValue == 2)
    	valueStr[0] = '2';
    else if (newValue == 3)
    	valueStr[0] = '3';
    else if (newValue == 4)
    	valueStr[0] = '4';
    else if (newValue == 5)
    	valueStr[0] = '5';
    else if (newValue == -1)
    	valueStr[1] = '\n';

    if (newValue != -1)
    	PRINTF("%c", valueStr[0]);
    else if (newValue == 0)
    	PRINTF("-");
    else
    	PRINTF("\r\n");

    // Calculate new string size and reallocate memory
    size_t valueSize = strlen(valueStr);
    size_t newSize = *currentSizePtr + valueSize;
    *stringPtr = (char*)realloc(*stringPtr, newSize + 1);
    if (*stringPtr == NULL) {
		PRINTF("\nError reallocating string\r\n");
    	LED_BLUE_OFF();
    	LED_GREEN_OFF();
		while(1)
		{
			LED_RED_TOGGLE();
			delay_ms(100);
		}
    }

    // Append new value to string and update size
    strcat(*stringPtr, valueStr);
    *currentSizePtr += valueSize;
}


static char *saveBlocks(map_block* blockBuffer, int bufferCount, size_t *currentSizePtr)
{
	static char *stringPtr = NULL;

	int rowIndexBuffer = 0;			// Starting index of block row in block field of a row we are processing
	int processingRowInBlock = 0;	// Stores number of row which we are currently processing

	int firstBlockInSubField = blockBuffer->corX;		// Number of first block on the row (for printing gaps at the beginning)
	int prevProcessedBlockColumn = blockBuffer->corX;	// Stores previously processed column which is helpful for filling gaps between blocks
	int processingBlockRow = blockBuffer->corY;			// Stores the number of block row, the subset of all blocks

	// Go through all blocks in the buffer
	for (int indexBuffer = rowIndexBuffer; indexBuffer < bufferCount;)
	{
		// If we are processing the subset of the blocks which are on the same row
		if (processingBlockRow == blockBuffer[indexBuffer].corY)
		{

			int fillingBlockGap = blockBuffer[indexBuffer].corX;
			// First block only, print unnecessarily gaps
			while (fillingBlockGap > maxLeftBlock && blockBuffer[indexBuffer].corX == firstBlockInSubField)
			{
				// Print the gap
				for (int columnInBlock = 0; columnInBlock < MAP_COLUMNS; columnInBlock++)
				{
					insertValue(0, &stringPtr, currentSizePtr);
				}

				fillingBlockGap--;
			}

			fillingBlockGap = blockBuffer[indexBuffer].corX - 1;
			// If there is some gap, fill it
			while (fillingBlockGap > prevProcessedBlockColumn)
			{
				// Print the gap
				for (int columnBlock = 0; columnBlock < MAP_COLUMNS; columnBlock++)
				{
					insertValue(0, &stringPtr, currentSizePtr);
				}

				fillingBlockGap--;
			}
			prevProcessedBlockColumn = blockBuffer[indexBuffer].corX;

			// Print the current processing row
			for (int columnBlock = 0; columnBlock < MAP_COLUMNS; columnBlock++)
			{
				insertValue(blockBuffer[indexBuffer].block[processingRowInBlock][columnBlock], &stringPtr, currentSizePtr);
			}
		}

		if (processingRowInBlock + 1 < MAP_ROWS && processingBlockRow != blockBuffer[indexBuffer].corY)
		{
			processingRowInBlock++;
			insertValue(-1, &stringPtr, currentSizePtr);
			indexBuffer = rowIndexBuffer;
			continue;
		}
		else if (processingBlockRow != blockBuffer[indexBuffer].corY)
		{
			processingRowInBlock = 0;
			processingBlockRow--;

			insertValue(-1, &stringPtr, currentSizePtr);

			rowIndexBuffer = indexBuffer;

			if (processingBlockRow >= maxBottomBlock)
			{
				firstBlockInSubField = blockBuffer[indexBuffer].corX;
				continue;
			}
			else
				break;
		}
		else
		{
			indexBuffer++;
			if (indexBuffer >= bufferCount && processingRowInBlock + 1 < MAP_ROWS)
			{
				processingRowInBlock++;
				insertValue(-1, &stringPtr, currentSizePtr);
				indexBuffer = rowIndexBuffer;
			}
		}

	}

	return stringPtr;
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

    /*for (int id = 0; id < num_coords; id++)
    {
		PRINTF("Block: %i, %i\r\n", block[id].corX, block[id].corY);
		for (int i = 0; i < MAP_ROWS; i++)
		{
			for (int j = 0; j < MAP_COLUMNS; j++)
			{
				PRINTF(" %d |", block[id].block[i][j]);
			}
			PRINTF("\r\n");
		}
    }*/
}

static void writeStringToSD(char **stringPtr, size_t *currentSizePtr)
{
    spi_master_config_t masterConfig = {0};
    uint32_t sourceClock = 0U;
    FATFS 	fs;
	FRESULT fr;
	FIL		fil;
	UINT 	bw;

	char file_name[]="Map.csv";

     /*Init SPI master
     * masterConfig.enableStopInWaitMode = false;
     * masterConfig.polarity = kSPI_ClockPolarityActiveHigh;
     * masterConfig.phase = kSPI_ClockPhaseFirstEdge;
     * masterConfig.direction = kSPI_MsbFirst;
     * masterConfig.dataMode = kSPI_8BitMode;
     * masterConfig.txWatermark = kSPI_TxFifoOneHalfEmpty;
     * masterConfig.rxWatermark = kSPI_RxFifoOneHalfFull;
     * masterConfig.pinMode = kSPI_PinModeNormal;
     * masterConfig.outputMode = kSPI_SlaveSelectAutomaticOutput;
     * masterConfig.baudRate_Bps = 500000U;*/
    SPI_MasterGetDefaultConfig(&masterConfig);
    sourceClock = EXAMPLE_SPI_MASTER_CLK_FREQ;
    SPI_MasterInit(EXAMPLE_SPI_MASTER, &masterConfig, sourceClock);

	fr = f_mount(&fs, file_name, 0);
	if(fr)
	{
		PRINTF("\nError mounting file system\r\n");
		LED_GREEN_OFF();
		LED_BLUE_OFF();
		while(1)
		{
			LED_RED_TOGGLE();
			delay_ms(100);
		}
	}
	PRINTF("Opening file.\r\n");

	LED_GREEN_TOGGLE();
	delay_ms(150);
	LED_GREEN_ON();

	fr = f_open(&fil, file_name, FA_CREATE_ALWAYS | FA_WRITE);
	if(fr)
	{
		fr = f_open(&fil, file_name, FA_OPEN_EXISTING | FA_WRITE);
		if(fr)
		{
			PRINTF("\nError opening text file\r\n");
			LED_GREEN_OFF();
			LED_BLUE_OFF();
			while(1)
			{
				LED_RED_TOGGLE();
				delay_ms(100);
			}
		}
	}

	LED_GREEN_TOGGLE();
	delay_ms(150);
	LED_GREEN_ON();

	PRINTF("Writing to file...\r\n");
	fr = f_write(&fil, *stringPtr, *currentSizePtr, &bw);
	if(fr)
	{
		PRINTF("\nError write text file\r\n");
		LED_GREEN_OFF();
		LED_BLUE_OFF();
		while(1)
		{
			LED_RED_TOGGLE();
			delay_ms(100);
		}
	}

	LED_GREEN_TOGGLE();
	delay_ms(150);
	LED_GREEN_ON();

	PRINTF("Closing file.\r\n");
	fr = f_close(&fil);
	if(fr)
	{
		PRINTF("\nError close text file\r\n");
		LED_GREEN_OFF();
		LED_BLUE_OFF();
		while(1)
		{
			LED_RED_TOGGLE();
			delay_ms(100);
		}
	}
	else
	{
		PRINTF("File closed.\r\n");
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
	static char *fieldsBuffer = NULL;
	static size_t currentSize = 0;

	//initSPI();
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

	PRINTF("Saving map...\r\n");
	fieldsBuffer = saveBlocks(blockBuffer, ht->count, &currentSize);

	LED_RED_ON();
	LED_GREEN_ON();
	writeStringToSD(&fieldsBuffer, &currentSize);

	LED_RED_TOGGLE();
	while(1);
}




