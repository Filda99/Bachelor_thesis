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
#include "utilities/fsl_debug_console.h"
#include "common/hast_table.h"

//**************************************************************************************************
//* EXTERN VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* PRIVATE MACROS AND DEFINES
//**************************************************************************************************

#define	GET_COORDINATES_X	currentBlockInMap->corX
#define GET_COORDINATES_Y	currentBlockInMap->corY


//**************************************************************************************************
//* PRIVATE TYPEDEFS
//**************************************************************************************************

//**************************************************************************************************
//* STATIC VARIABLES
//**************************************************************************************************
static map_block *firstBlockToStartSaving = NULL;

//**************************************************************************************************
//* GLOBAL VARIABLES
//**************************************************************************************************
map_block *currentBlockInMap;

curr_pos_map currPosInBlk = {
		MAP_ROWS / 2,
		MAP_COLUMNS / 2
};

//**************************************************************************************************
//* STATIC FUNCTION PROTOTYPES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTIONS
//**************************************************************************************************

static int getUniqueID(int x, int y)
{
    int unique_id = (y * MAP_ROWS * MAP_COLUMNS) + x + 1;
    unique_id *= 31;
    return unique_id;
}

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
static map_object_t** initNewBlock()
{
	map_object_t** newBlock;
	//map_object_t** field;
	newBlock = (map_object_t **)malloc(MAP_ROWS * sizeof(map_object_t*));
	if (!newBlock)
	{
		//return NULL;  // error: calloc() failed
	}

	for (int i = 0; i < MAP_ROWS; i++)
	{
		newBlock[i] = (map_object_t*)malloc(MAP_COLUMNS * sizeof(map_object_t));
		if (!newBlock[i]) {
			// error: calloc() failed, free memory allocated so far
			for (int j = 0; j < i; j++) {
				free(newBlock[j]);
			}
			free(newBlock);
			//return NULL;
		}
	}


	for (int i = 0; i < MAP_ROWS; i++)
	{
		for (int j = 0; j < MAP_COLUMNS; j++)
		{
			newBlock[i][j] = map_Empty;
		}
	}

	return newBlock;
}


//!*************************************************************************************************
//! static void createNewBlock(map_block *current, map_block *newBlock, block_direction direction)
//!
//! @description
//! Function creates new block to move to.
//!
//! @param    block_direction direction The direction in which the new block will be created
//!
//! @return   None
//!*************************************************************************************************
static map_block *createNewBlock(int ID, int X, int Y)
{
	map_block *block = (map_block*)malloc(sizeof(map_block));
	if (block != NULL)
	{
		block->block = initNewBlock();
		block->corX = X;
		block->corY = Y;
	}

	return block;
}

void getCoordinates(block_direction direction, int *x, int *y)
{
	*x = GET_COORDINATES_X;
	*y = GET_COORDINATES_Y;
	switch (direction)
	{
		case block_up:
			(*y)++;
			break;
		case block_down:
			(*y)--;
			break;
		case block_left:
			(*x)--;
			break;
		case block_right:
			(*x)++;
			break;

		default:
			break;
	}
}

//!*************************************************************************************************
//! static void moveInBlocks(block_direction direction)
//!
//! @description
//! Function ensures movement between blocks.
//!
//! @param    block_direction direction The direction in which we want to go
//!
//! @return   None
//!*************************************************************************************************
static void moveBetweenBlocks(block_direction direction)
{
	int oldID = getUniqueID(currentBlockInMap->corX, currentBlockInMap->corY);
	if(searchItemInHT(oldID, currentBlockInMap->corX, currentBlockInMap->corY) == NULL)
	{
		insertToHashTable(oldID, currentBlockInMap);
	}

 	int X = 0;
	int Y = 0;

	// Based on direction we get x, y coordinates
	getCoordinates(direction, &X, &Y);

	// Get ID from coordinates
	int ID = getUniqueID(X, Y);

	map_block *pBlockToMove = searchItemInHT(ID, X, Y);

	if (!pBlockToMove)
	{
		pBlockToMove = createNewBlock(ID, X, Y);
	}

	currentBlockInMap = pBlockToMove;

	// For saving purpose, store the most top-left block
	if (currentBlockInMap->corX < firstBlockToStartSaving->corX && currentBlockInMap->corY > firstBlockToStartSaving->corY)
	{
		firstBlockToStartSaving = currentBlockInMap;
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
	createHashTable(50);

	map_block *pBlockToMove;
	int ID = getUniqueID(0, 0);
	pBlockToMove = createNewBlock(ID, 0, 0);

	currentBlockInMap = pBlockToMove;

	currentBlockInMap->block[currPosInBlk.Row][currPosInBlk.Col] = map_CurrentPosition;
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
	free(currentBlockInMap->block);
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
//! IMPORTANT! If we move up relative to our previous position, we have to reduce currPosInBlkRow
//! by one. That is due 2D field indexing in C!
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
void moveInMap(map_move_direction direction)
{
	currentBlockInMap->block[currPosInBlk.Row][currPosInBlk.Col] = map_Track;
	switch (direction)
	{
		case move_up:
			// If we can move in block, move up
			if (currPosInBlk.Row != MAP_BLOCK_MIN_ROW)
			{
				currPosInBlk.Row--;
			}
			else
			{
				// Create new block up and move to its last row.
				moveBetweenBlocks(block_up);
				currPosInBlk.Row = MAP_BLOCK_MAX_ROW;
			}
			break;

		case move_upLeft:
			// If we can move in block, move up and left
			if (currPosInBlk.Col != MAP_BLOCK_MIN_COL
					&& currPosInBlk.Row != MAP_BLOCK_MIN_ROW)
			{
				currPosInBlk.Col--;
				currPosInBlk.Row--;
			}
			else
			{
				// Move to the block on the left
				if (currPosInBlk.Col == MAP_BLOCK_MIN_COL &&
						currPosInBlk.Row != MAP_BLOCK_MIN_ROW)
				{
					moveBetweenBlocks(block_left);
					currPosInBlk.Row--;
					currPosInBlk.Col = MAP_BLOCK_MAX_COL;
				}
				// Move to the block above us
				else if (currPosInBlk.Col != MAP_BLOCK_MIN_COL &&
						currPosInBlk.Row == MAP_BLOCK_MIN_ROW)
				{
					moveBetweenBlocks(block_up);
					currPosInBlk.Row = MAP_BLOCK_MAX_ROW;
					currPosInBlk.Col--;
				}
				// Move to the upper left block
				else
				{
					moveBetweenBlocks(block_up);
					moveBetweenBlocks(block_left);
					currPosInBlk.Col = MAP_BLOCK_MAX_COL;
					currPosInBlk.Row = MAP_BLOCK_MAX_ROW;
				}
			}
			break;

		case move_left:
			// If we can move in block, move left
			if (currPosInBlk.Col != MAP_BLOCK_MIN_COL)
			{
				currPosInBlk.Col--;
			}
			else
			{
				moveBetweenBlocks(block_left);
				currPosInBlk.Col = MAP_BLOCK_MAX_COL;
			}
			break;

		case move_downLeft:
			if (currPosInBlk.Col != MAP_BLOCK_MIN_COL
					&& currPosInBlk.Row != MAP_BLOCK_MAX_ROW)
			{
				currPosInBlk.Col--;
				currPosInBlk.Row++;
			}
			else
			{
				// Move to the block on the left
				if (currPosInBlk.Col == MAP_BLOCK_MIN_COL &&
						currPosInBlk.Row != MAP_BLOCK_MAX_ROW)
				{
					moveBetweenBlocks(block_left);
					currPosInBlk.Row--;
					currPosInBlk.Col = MAP_BLOCK_MAX_COL;
				}
				// Move to the block below us
				else if (currPosInBlk.Col != MAP_BLOCK_MIN_COL &&
						currPosInBlk.Row == MAP_BLOCK_MAX_ROW)
				{
					moveBetweenBlocks(block_down);
					currPosInBlk.Row = MAP_BLOCK_MIN_ROW;
					currPosInBlk.Col--;
				}
				// Move to the block to the left below us
				else
				{
					moveBetweenBlocks(block_down);
					moveBetweenBlocks(block_left);
					currPosInBlk.Col = MAP_BLOCK_MAX_COL;
					currPosInBlk.Row = MAP_BLOCK_MIN_ROW;
				}
			}
			break;

		case move_down:
			if (currPosInBlk.Row != MAP_BLOCK_MAX_ROW)
			{
				currPosInBlk.Row++;
			}
			else
			{
				moveBetweenBlocks(block_down);
				currPosInBlk.Row = MAP_BLOCK_MIN_ROW;
			}
			break;

		case move_downRight:
			if (currPosInBlk.Col != MAP_BLOCK_MAX_COL
					&& currPosInBlk.Row != MAP_BLOCK_MAX_ROW)
			{
				currPosInBlk.Col++;
				currPosInBlk.Row++;
			}
			else
			{
				// Move to the block on the right
				if (currPosInBlk.Col == MAP_BLOCK_MAX_COL &&
						currPosInBlk.Row != MAP_BLOCK_MAX_ROW)
				{
					moveBetweenBlocks(block_right);
					currPosInBlk.Col = MAP_BLOCK_MIN_COL;
					currPosInBlk.Row--;
				}
				// Move to the block below us
				else if (currPosInBlk.Col != MAP_BLOCK_MAX_COL &&
						currPosInBlk.Row == MAP_BLOCK_MAX_ROW)
				{
					moveBetweenBlocks(block_down);
					currPosInBlk.Row = MAP_BLOCK_MIN_ROW;
					currPosInBlk.Col--;
				}
				// Move to the block to the right below us
				else
				{
					moveBetweenBlocks(block_down);
					moveBetweenBlocks(block_right);
					currPosInBlk.Col = MAP_BLOCK_MIN_COL;
					currPosInBlk.Row = MAP_BLOCK_MIN_ROW;
				}
			}
			break;

		case move_right:
			if (currPosInBlk.Col != MAP_BLOCK_MAX_COL)
			{
				currPosInBlk.Col++;
			}
			else
			{
				moveBetweenBlocks(block_right);
				currPosInBlk.Col = MAP_BLOCK_MIN_COL;
			}
			break;

		case move_upRight:
			if (currPosInBlk.Col != MAP_BLOCK_MAX_COL
					&& currPosInBlk.Row != MAP_BLOCK_MIN_ROW)
			{
				currPosInBlk.Col++;
				currPosInBlk.Row--;
			}
			else
			{
				// Move to the block on the right
				if (currPosInBlk.Col == MAP_BLOCK_MAX_COL &&
						currPosInBlk.Row != MAP_BLOCK_MIN_ROW)
				{
					moveBetweenBlocks(block_right);
					currPosInBlk.Col = MAP_BLOCK_MIN_COL;
					currPosInBlk.Row--;
				}
				// Move to the block above us
				else if (currPosInBlk.Col != MAP_BLOCK_MAX_COL &&
						currPosInBlk.Row == MAP_BLOCK_MIN_ROW)
				{
					moveBetweenBlocks(block_up);
					currPosInBlk.Row = MAP_BLOCK_MAX_ROW;
					currPosInBlk.Col++;
				}
				// Move to the upper left block
				else
				{
					moveBetweenBlocks(block_up);
					moveBetweenBlocks(block_right);
					currPosInBlk.Col = MAP_BLOCK_MIN_COL;
					currPosInBlk.Row = MAP_BLOCK_MAX_ROW;
				}
			}
			break;

		default:
			break;
	}
	currentBlockInMap->block[currPosInBlk.Row][currPosInBlk.Col] = map_CurrentPosition;
}
