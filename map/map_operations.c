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
#include <stdbool.h>

//**************************************************************************************************
//* EXTERN VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* PRIVATE MACROS AND DEFINES
//**************************************************************************************************

//**************************************************************************************************
//* PRIVATE TYPEDEFS
//**************************************************************************************************

//* Directions for creating new block
typedef enum _block_direction
{
	newBlock_up = 0U, newBlock_down, newBlock_left, newBlock_right
} block_direction;

//**************************************************************************************************
//* STATIC VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* GLOBAL VARIABLES
//**************************************************************************************************
map_block *currentBlockInMap;
uint8_t currentPosRows = MAP_ROWS / 2;
uint8_t currentPosCols = MAP_COLUMNS / 2;

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
//! Also sets all neighbour pointers to null.
//!
//! @param    map_block *newBlock Pointer to a new map block, which should be alocated
//!
//! @return   None
//!*************************************************************************************************
static void initNewBlock(struct map_blk *newBlock)
{
	map_object_t **block;
	block = (enum _map_object**) malloc(MAP_ROWS * sizeof(enum _map_object*));

	for (int i = 0; i < MAP_ROWS; i++)
	{
		block[i] = (enum _map_object*) malloc(
				MAP_COLUMNS * sizeof(enum _map_object));
	}

	newBlock->currentBlock = block;

	for (int i = 0; i < MAP_ROWS; i++)
	{
		for (int j = 0; j < MAP_COLUMNS; j++)
		{
			newBlock->currentBlock[i][j] = map_Empty;
		}
	}

	newBlock->blockDown = NULL;
	newBlock->blockLeft = NULL;
	newBlock->blockRight = NULL;
	newBlock->blockUp = NULL;
}

static bool checkExistingBlock(block_direction direction)
{
	bool ret = false;
	switch (direction)
	{
		case newBlock_up:
			if (currentBlockInMap->blockUp != NULL)
			{
				ret = true;
			}
			break;
		case newBlock_down:
			if (currentBlockInMap->blockDown != NULL)
			{
				ret = true;
			}
			break;
		case newBlock_left:
			if (currentBlockInMap->blockLeft != NULL)
			{
				ret = true;
			}
			else
			break;
		case newBlock_right:
			if (currentBlockInMap->blockRight != NULL)
			{
				ret = true;
			}
			break;

		default:
			break;
	}
	return ret;
}

//!*************************************************************************************************
//! static void createNewBlock(map_block *current, map_block *newBlock, block_direction direction)
//!
//! @description
//! Function connects new map block to existing one if there is NOT.
//! If block already exists, function just move us there.
//!
//! @param    map_block *current  Pointer to a existing map block
//! @param    map_block *newBlock Pointer to a new map block
//! @param    block_direction direction Direction The direction in which the new block is allocated
//!
//! @return   None
//!*************************************************************************************************
static void createNewBlock(block_direction direction)
{
	map_block *newBlock = malloc(sizeof(*newBlock));

	initNewBlock(newBlock);
	switch (direction)
	{
		case newBlock_up:
			currentBlockInMap->blockUp = newBlock;
			newBlock->blockDown = currentBlockInMap;
			break;
		case newBlock_down:
			currentBlockInMap->blockDown = newBlock;
			newBlock->blockUp = currentBlockInMap;
			break;
		case newBlock_left:
			currentBlockInMap->blockLeft = newBlock;
			newBlock->blockRight = currentBlockInMap;
			break;
		case newBlock_right:
			currentBlockInMap->blockRight = newBlock;
			newBlock->blockLeft = currentBlockInMap;
			break;

		default:
			break;
	}

	// Move to new block
	currentBlockInMap = newBlock;

	// TODO: connect block to existing neighbors
}

static void moveInBlocks(block_direction direction)
{
	bool blockExists = checkExistingBlock(direction);

	if (!blockExists)
	{
		createNewBlock(direction);
	}
	else
	{
		switch (direction)
		{
			case newBlock_up:
				currentBlockInMap = currentBlockInMap->blockUp;
				break;
			case newBlock_down:
				currentBlockInMap = currentBlockInMap->blockDown;
				break;
			case newBlock_left:
				currentBlockInMap = currentBlockInMap->blockLeft;
				break;
			case newBlock_right:
				currentBlockInMap = currentBlockInMap->blockRight;
				break;

			default:
				break;
		}
	}
}

//**************************************************************************************************
//* GLOBAL FUNCTIONS
//**************************************************************************************************

//!*************************************************************************************************
//! void createMap(void)
//!
//! @description
//! Function creates first block of a map.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
void createMap()
{
	currentBlockInMap = malloc(sizeof(*currentBlockInMap));
	initNewBlock(currentBlockInMap);
	currentBlockInMap->currentBlock[currentPosRows][currentPosCols] = map_CurrentPosition;
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
//! void deleteMap(void)
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
	// TODO: Go through all map blocks.
	free(currentBlockInMap->currentBlock);
	free(currentBlockInMap);
}


//!*************************************************************************************************
//! void moveInMap(void)
//!
//! @description
//! Movement in the map based on a certain direction.
//!
//! If we can move in current map block, we just simply move in the field.
//! But if we are on the border and want to move across it, we have to allocate a new block and 
//! update our position in the field.
//!
//! IMPORTANT! If we move up relative to our previous position, we have to reduce currentPosRow
//! by one. That is due 2D field indexing in C!
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
void moveInMap(map_move_direction direction)
{
	currentBlockInMap->currentBlock[currentPosRows][currentPosCols] = map_Track;
	switch (direction)
	{
		case move_up:
			// If we can move in block, move up
			if (currentPosRows != MAP_BLOCK_MIN_ROW)
			{
				currentPosRows--;
			}
			else
			{
				// Create new block up and move to its last row.
				moveInBlocks(newBlock_up);
				currentPosRows = MAP_BLOCK_MAX_ROW;
			}
			break;

		case move_upLeft:
			// If we can move in block, move up and left
			if (currentPosCols != MAP_BLOCK_MIN_COL
					&& currentPosRows != MAP_BLOCK_MIN_ROW)
			{
				currentPosCols--;
				currentPosRows--;
			}
			else
			{
				// Move to the block on the left
				if (currentPosCols == MAP_BLOCK_MIN_COL &&
						currentPosRows != MAP_BLOCK_MIN_ROW)
				{
					moveInBlocks(newBlock_left);
					currentPosRows--;
					currentPosCols = MAP_BLOCK_MAX_COL;
				}
				// Move to the block above us
				else if (currentPosCols != MAP_BLOCK_MIN_COL &&
						currentPosRows == MAP_BLOCK_MIN_ROW)
				{
					moveInBlocks(newBlock_up);
					currentPosRows = MAP_BLOCK_MAX_ROW;
					currentPosCols--;
				}
				// Move to the upper left block
				else
				{
					moveInBlocks(newBlock_up);
					moveInBlocks(newBlock_left);
					currentPosCols = MAP_BLOCK_MAX_COL;
					currentPosRows = MAP_BLOCK_MAX_ROW;
				}
			}
			break;

		case move_left:
			// If we can move in block, move left
			if (currentPosCols != MAP_BLOCK_MIN_COL)
			{
				currentPosCols--;
			}
			else
			{
				createNewBlock(newBlock_left);
				currentPosCols = MAP_BLOCK_MAX_COL;
			}
			break;

		case move_downLeft:
			if (currentPosCols != MAP_BLOCK_MIN_COL
					&& currentPosRows != MAP_BLOCK_MAX_ROW)
			{
				currentPosCols--;
				currentPosRows++;
			}
			else
			{
				// Move to the block on the left
				if (currentPosCols == MAP_BLOCK_MIN_COL &&
						currentPosRows != MAP_BLOCK_MAX_ROW)
				{
					moveInBlocks(newBlock_left);
					currentPosRows--;
					currentPosCols = MAP_BLOCK_MAX_COL;
				}
				// Move to the block below us
				else if (currentPosCols != MAP_BLOCK_MIN_COL &&
						currentPosRows == MAP_BLOCK_MAX_ROW)
				{
					moveInBlocks(newBlock_down);
					currentPosRows = MAP_BLOCK_MIN_ROW;
					currentPosCols--;
				}
				// Move to the block to the left below us
				else
				{
					createNewBlock(newBlock_down);
					createNewBlock(newBlock_left);
					currentPosCols = MAP_BLOCK_MAX_COL;
					currentPosRows = MAP_BLOCK_MIN_ROW;
				}
			}
			break;

		case move_down:
			if (currentPosRows != MAP_BLOCK_MAX_ROW)
			{
				currentPosRows++;
			}
			else
			{
				createNewBlock(newBlock_down);
				currentPosRows = MAP_BLOCK_MIN_ROW;
			}
			break;

		case move_downRight:
			if (currentPosCols != MAP_BLOCK_MAX_COL
					&& currentPosRows != MAP_BLOCK_MAX_ROW)
			{
				currentPosCols++;
				currentPosRows++;
			}
			else
			{
				// Move to the block on the right
				if (currentPosCols == MAP_BLOCK_MAX_COL &&
						currentPosRows != MAP_BLOCK_MAX_ROW)
				{
					moveInBlocks(newBlock_right);
					currentPosCols = MAP_BLOCK_MIN_COL;
					currentPosRows--;
				}
				// Move to the block below us
				else if (currentPosCols != MAP_BLOCK_MAX_COL &&
						currentPosRows == MAP_BLOCK_MAX_ROW)
				{
					moveInBlocks(newBlock_down);
					currentPosRows = MAP_BLOCK_MIN_ROW;
					currentPosCols--;
				}
				// Move to the block to the right below us
				else
				{
					createNewBlock(newBlock_down);
					createNewBlock(newBlock_right);
					currentPosCols = MAP_BLOCK_MIN_COL;
					currentPosRows = MAP_BLOCK_MIN_ROW;
				}
			}
			break;

		case move_right:
			if (currentPosCols != MAP_BLOCK_MAX_COL)
			{
				currentPosCols++;
			}
			else
			{
				createNewBlock(newBlock_right);
				currentPosCols = MAP_BLOCK_MIN_COL;
			}
			break;

		case move_upRight:
			if (currentPosCols != MAP_BLOCK_MAX_COL
					&& currentPosRows != MAP_BLOCK_MIN_ROW)
			{
				currentPosCols++;
				currentPosRows--;
			}
			else
			{
				// Move to the block on the right
				if (currentPosCols == MAP_BLOCK_MAX_COL &&
						currentPosRows != MAP_BLOCK_MIN_ROW)
				{
					moveInBlocks(newBlock_right);
					currentPosCols = MAP_BLOCK_MIN_COL;
					currentPosRows--;
				}
				// Move to the block above us
				else if (currentPosCols != MAP_BLOCK_MAX_COL &&
						currentPosRows == MAP_BLOCK_MIN_ROW)
				{
					moveInBlocks(newBlock_up);
					currentPosRows = MAP_BLOCK_MAX_ROW;
					currentPosCols++;
				}
				// Move to the upper left block
				else
				{
					moveInBlocks(newBlock_up);
					moveInBlocks(newBlock_right);
					currentPosCols = MAP_BLOCK_MIN_COL;
					currentPosRows = MAP_BLOCK_MAX_ROW;
				}
			}
			break;

		default:
			break;
	}
	currentBlockInMap->currentBlock[currentPosRows][currentPosCols] = map_CurrentPosition;
}
