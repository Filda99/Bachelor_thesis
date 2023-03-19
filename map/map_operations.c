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

//! Field of directions, where we went
static map_move_direction *path = NULL;
static uint16_t pathFieldCapacity = 0;

static int maxBlockX = 0;
static int minBlockX = 0;
static int maxBlockY = 0;
static int minBlockY = 0;

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

static bool checkID(int id)
{
	bool idFound = false;
	for(int i = 0; i < pathFieldCapacity; i++)
	{
		if (path[i] == id)
		{
			idFound = true;
			break;
		}
	}
	return idFound;
}

static void connectToNeighbors()
{
	// Calculate id's for neighbors
	int leftNeigborID = getUniqueID(--(currentBlockInMap->corX), currentBlockInMap->corY);
	int upNeigborID = getUniqueID(currentBlockInMap->corX, ++(currentBlockInMap->corY));
	int rightNeigborID = getUniqueID(++(currentBlockInMap->corX), currentBlockInMap->corY);
	int downNeigborID = getUniqueID(currentBlockInMap->corX, --(currentBlockInMap->corY));

	// Check id's if exist, if yes, connect them to existing block
	if (checkID(leftNeigborID))
	{
		connectBlock(leftNeigborID);
	}
	if (checkID(upNeigborID))
	{
		connectBlock(leftNeigborID);
	}
	if (checkID(rightNeigborID))
	{
		connectBlock(leftNeigborID);
	}
	if (checkID(downNeigborID))
	{
		connectBlock(leftNeigborID);
	}

}

//!*************************************************************************************************
//! static void add_elements(map_move_direction** array, int* capacity, int num_elements)
//!
//! @description
//! Function dynamically allocates given field.
//!
//! @param	map_move_direction** array	Pointer to array to be allocated
//! @param	int* capacity				Number of allocated items in the array
//! @param	int num_elements			Number for how much should we allocate
//!
//! @return   None
//!*************************************************************************************************
static void add_elements(map_move_direction** array, int* capacity, int num_elements)
{
    int new_capacity = *capacity + num_elements;
    map_move_direction* new_array = realloc(*array, new_capacity * sizeof(int));

    if (new_array == NULL) {
        PRINTF("Error: Unable to allocate enough memory\r\n");
        exit(1);
    }

    *array = new_array;
    *capacity = new_capacity;
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

	newBlock->blockDown = NULL;
	newBlock->blockLeft = NULL;
	newBlock->blockRight = NULL;
	newBlock->blockUp = NULL;
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
//! Function connects new map block to existing one.
//!
//! @param    block_direction direction The direction in which the new block will be created
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
			newBlock->corY = ++currentBlockInMap->corY;
			break;
		case newBlock_down:
			currentBlockInMap->blockDown = newBlock;
			newBlock->blockUp = currentBlockInMap;
			newBlock->corY = --currentBlockInMap->corY;
			break;
		case newBlock_left:
			currentBlockInMap->blockLeft = newBlock;
			newBlock->blockRight = currentBlockInMap;
			newBlock->corX = --currentBlockInMap->corX;
			break;
		case newBlock_right:
			currentBlockInMap->blockRight = newBlock;
			newBlock->blockLeft = currentBlockInMap;
			newBlock->corX = ++currentBlockInMap->corX;
			break;

		default:
			break;
	}

	// Move to new block
	currentBlockInMap = newBlock;

	// TODO: connect block to existing neighbors
	// Get id of the new block
	currentBlockInMap->id = getUniqueID(currentBlockInMap->corX, currentBlockInMap->corY);
	connectToNeighbours();
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
	currentBlockInMap->currentBlock[currPosInBlk.Row][currPosInBlk.Col] = map_CurrentPosition;
	currentBlockInMap->corX = 0;
	currentBlockInMap->corY = 0;
	add_elements(&path, &pathFieldCapacity, 10);
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
				moveInBlocks(newBlock_up);
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
					moveInBlocks(newBlock_left);
					currPosInBlk.Row--;
					currPosInBlk.Col = MAP_BLOCK_MAX_COL;
				}
				// Move to the block above us
				else if (currPosInBlk.Col != MAP_BLOCK_MIN_COL &&
						currPosInBlk.Row == MAP_BLOCK_MIN_ROW)
				{
					moveInBlocks(newBlock_up);
					currPosInBlk.Row = MAP_BLOCK_MAX_ROW;
					currPosInBlk.Col--;
				}
				// Move to the upper left block
				else
				{
					moveInBlocks(newBlock_up);
					moveInBlocks(newBlock_left);
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
				createNewBlock(newBlock_left);
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
					moveInBlocks(newBlock_left);
					currPosInBlk.Row--;
					currPosInBlk.Col = MAP_BLOCK_MAX_COL;
				}
				// Move to the block below us
				else if (currPosInBlk.Col != MAP_BLOCK_MIN_COL &&
						currPosInBlk.Row == MAP_BLOCK_MAX_ROW)
				{
					moveInBlocks(newBlock_down);
					currPosInBlk.Row = MAP_BLOCK_MIN_ROW;
					currPosInBlk.Col--;
				}
				// Move to the block to the left below us
				else
				{
					createNewBlock(newBlock_down);
					createNewBlock(newBlock_left);
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
				createNewBlock(newBlock_down);
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
					moveInBlocks(newBlock_right);
					currPosInBlk.Col = MAP_BLOCK_MIN_COL;
					currPosInBlk.Row--;
				}
				// Move to the block below us
				else if (currPosInBlk.Col != MAP_BLOCK_MAX_COL &&
						currPosInBlk.Row == MAP_BLOCK_MAX_ROW)
				{
					moveInBlocks(newBlock_down);
					currPosInBlk.Row = MAP_BLOCK_MIN_ROW;
					currPosInBlk.Col--;
				}
				// Move to the block to the right below us
				else
				{
					createNewBlock(newBlock_down);
					createNewBlock(newBlock_right);
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
				createNewBlock(newBlock_right);
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
					moveInBlocks(newBlock_right);
					currPosInBlk.Col = MAP_BLOCK_MIN_COL;
					currPosInBlk.Row--;
				}
				// Move to the block above us
				else if (currPosInBlk.Col != MAP_BLOCK_MAX_COL &&
						currPosInBlk.Row == MAP_BLOCK_MIN_ROW)
				{
					moveInBlocks(newBlock_up);
					currPosInBlk.Row = MAP_BLOCK_MAX_ROW;
					currPosInBlk.Col++;
				}
				// Move to the upper left block
				else
				{
					moveInBlocks(newBlock_up);
					moveInBlocks(newBlock_right);
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
