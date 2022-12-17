/**
 ***************************************************************************************************
 * @file    map_init.c
 * @author  user
 * @date    Dec 5, 2022
 * @brief   
 ***************************************************************************************************
 */

//**************************************************************************************************
//* INCLUDES
//**************************************************************************************************
#include "map/map_operations.h"
#include "stdlib.h"

//**************************************************************************************************
//* EXTERN VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* PRIVATE MACROS AND DEFINES
//**************************************************************************************************

//**************************************************************************************************
//* PRIVATE TYPEDEFS
//**************************************************************************************************
typedef enum _block_direction
{
  Up = 0U,
  Down,
  Left,
  Right
}block_direction;


//**************************************************************************************************
//* STATIC VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* GLOBAL VARIABLES
//**************************************************************************************************
map_block *currentBlock;

//**************************************************************************************************
//* STATIC FUNCTION PROTOTYPES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTIONS
//**************************************************************************************************

//!*************************************************************************************************
//! static void initNewBlock(map_block *newBlock)
//!
//! @description
//! Function alocates memory for the new block and sets all fields to empty.
//!
//! @param    map_block *newBlock Pointer to a new map block, which should be alocated
//!
//! @return   None
//!*************************************************************************************************
static void initNewBlock(map_block *newBlock)
{
	map_object_t **block;
	block = (enum _map_object**)malloc(MAP_BLOCK_ROWS * sizeof(enum _map_object*));

	for (int i = 0; i < MAP_BLOCK_ROWS; i++)
	{
		block[i] = (enum _map_object*)malloc(MAP_BLOCK_COLS * sizeof(enum _map_object));
	}

	newBlock->currentBlock = block;

	for(int i = 0; i < MAP_BLOCK_ROWS; i++)
	{
		for (int j = 0; j < MAP_BLOCK_COLS; j++)
		{
			newBlock->currentBlock[i][j] = map_Empty;
		}
	}

}

//!*************************************************************************************************
//! static void createNewBlock(map_block *current, map_block *newBlock, block_direction direction)
//!
//! @description
//! Function connects new map block to existing one.
//!
//! @param    map_block *current  Pointer to a existing map block
//! @param    map_block *newBlock Pointer to a new map block
//! @param    block_direction direction Direction The direction in which the new block is allocated
//!
//! @return   None
//!*************************************************************************************************
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

//**************************************************************************************************
//* GLOBAL FUNCTIONS
//**************************************************************************************************

//!*************************************************************************************************
//! void createMap(void)
//!
//! @description
//! Function creates first map block.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
void createMap()
{
  initNewBlock(&currentBlock);
}


//!*************************************************************************************************
//! void saveMap(void)
//!
//! @description
//! Function goes through all map blocks and saves them to a file.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
void saveMap()
{

}


//!*************************************************************************************************
//! void saveMap(void)
//!
//! @description
//! Function goes through all map blocks and dealocates them.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
void deleteMap()
{
  //free(currentBlock.currentBlock);

}


//!*************************************************************************************************
//! void saveMap(void)
//!
//! @description
//! TBD
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
void moveInMap()
{

}



