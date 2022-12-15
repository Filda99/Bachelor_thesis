/**
 ********************************************************************************
 * @file    map_init.c
 * @author  user
 * @date    Dec 5, 2022
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "map/map_operations.h"
#include "stdlib.h"

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * PRIVATE TYPEDEFS
 ************************************/
typedef enum _block_direction
{
  Up = 0U,
  Down,
  Left,
  Right
}block_direction;

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
static void initNewBlock(map_block *newBlock)
{
	map_object_t **block;
	block = (enum _map_object**)malloc(BLK_ROWS * sizeof(enum _map_object*));

	for (int i = 0; i < BLK_ROWS; i++)
	{
		block[i] = (enum _map_object*)malloc(BLK_COLS * sizeof(enum _map_object));
	}

	newBlock->currentBlock = block;

	for(int i = 0; i < BLK_ROWS; i++)
	{
		for (int j = 0; j < BLK_COLS; j++)
		{
			newBlock->currentBlock[i][j] = map_Empty;
		}
	}

}

static void createNewBlock(map_block *current, map_block *newBlock, block_direction direction)
{
  initNewBlock(newBlock);
  switch (direction)
  {
  case Up:
    current->blockUp = newBlock;
    newBlock->blockDown = current;
    break;
  case Down:
    current->blockDown = newBlock;
    newBlock->blockUp = current;
    break;
  case Left:
    current->blockLeft = newBlock;
    newBlock->blockRight = current;
    break;
  case Right:
    current->blockRight = newBlock;
    newBlock->blockLeft = current;
    break;
  
  default:
    break;
  }
}

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void initMap()
{
  map_block currentBlock;
  initNewBlock(&currentBlock);
}


void saveMap()
{

}


void deinitMap()
{
  //free(currentBlock.currentBlock);

}
