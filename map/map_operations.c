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

// LEFT
#define	GO_LEFT_X	--(currentBlockInMap->corX)
#define GO_LEFT_Y	   currentBlockInMap->corY

// UP
#define GO_UP_X		   currentBlockInMap->corX
#define GO_UP_Y		++(currentBlockInMap->corY)

// RIGHT
#define GO_RIGHT_X	++(currentBlockInMap->corX)
#define GO_RIGHT_Y	   currentBlockInMap->corY

// DOWN
#define GO_DOWN_X	   currentBlockInMap->corX
#define GO_DOWN_Y	--(currentBlockInMap->corY)

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
    int unique_id = (y * MAP_ROWS * MAP_COLUMNS) + x;
    unique_id *= 31;
    return unique_id;
}

//!*************************************************************************************************
//! static bool checkExistingBlock(block_direction direction)
//!
//! @description
//! Function looks at the block next to current in the direction we want to move
//! if block already exists or not.
//!
//! @param    block_direction direction	The direction we want to go
//!
//! @return   True if block already exists, False otherwise
//!*************************************************************************************************
static bool doesBlockExists(map_block *block, int ID, int X, int Y)
{
	bool ret = false;

	block = searchItemInHT(ID, X, Y);
	if (block)
	{
		ret = true;
	}

	return ret;
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
static void createNewBlock(map_block *block, int ID, int X, int Y)
{
	block = malloc(sizeof(*block));

	initNewBlock(block);
	block->corX = X;
	block->corY = Y;
	block->id = ID;

	insertToHashTable(ID, block);
}

void getCoordinates(block_direction direction, int *x, int *y)
{
	switch (direction)
	{
		case block_up:
			*x = GO_UP_X;
			*y = GO_UP_Y;
			break;
		case block_down:
			*x = GO_DOWN_X;
			*y = GO_DOWN_Y;
			break;
		case block_left:
			*x = GO_LEFT_X;
			*y = GO_LEFT_Y;
			break;
		case block_right:
			*x = GO_RIGHT_X;
			*y = GO_RIGHT_Y;
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
	map_block *pBlockToMove = NULL;
	int x = 0;
	int y = 0;

	// Based on direction we get x, y coordinates
	getCoordinates(direction, &x, &y);

	// Get ID from coordinates
	int ID = getUniqueID(x, y);

	if (!doesBlockExists(pBlockToMove, ID, x, y))
	{
		createNewBlock(pBlockToMove, ID, x, y);

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
	currentBlockInMap = malloc(sizeof(*currentBlockInMap));
	initNewBlock(currentBlockInMap);
	currentBlockInMap->currentBlock[currPosInBlk.Row][currPosInBlk.Col] = map_CurrentPosition;
	currentBlockInMap->corX = 0;
	currentBlockInMap->corY = 0;
	createHashTable(200);
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
//! IMPORTANT! If we move up relative to our previous position, we have to reduce currPosInBlkRow
//! by one. That is due 2D field indexing in C!
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
void moveInMap(map_move_direction direction)
{
	currentBlockInMap->currentBlock[currPosInBlk.Row][currPosInBlk.Col] = map_Track;
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
	currentBlockInMap->currentBlock[currPosInBlk.Row][currPosInBlk.Col] = map_CurrentPosition;
}
