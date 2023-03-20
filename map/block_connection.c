/**
 ***************************************************************************************************
 * @file    block_connection.c
 * @author  user
 * @date    Mar 20, 2023
 * @brief   
 ***************************************************************************************************
 */
#include "global_macros.h"
#include "map_operations.h"
#include "common/hast_table.h"

//**************************************************************************************************
//* INCLUDES
//**************************************************************************************************

//**************************************************************************************************
//* EXTERN VARIABLES
//**************************************************************************************************
extern map_block *currentBlockInMap;

//**************************************************************************************************
//* PRIVATE MACROS AND DEFINES
//**************************************************************************************************

//**************************************************************************************************
//* PRIVATE TYPEDEFS
//**************************************************************************************************

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
static int getUniqueID(int x, int y)
{
    int unique_id = (y * MAP_ROWS * MAP_COLUMNS) + x;
    unique_id *= 31;
    return unique_id;
}

static void connectBlock(map_block *neighbor, block_direction direction)
{
	if (direction == block_left)
	{
		currentBlockInMap->blockLeft = neighbor;
		neighbor->blockRight = currentBlockInMap;
	}
	else if (direction == block_right)
	{
		currentBlockInMap->blockRight = neighbor;
		neighbor->blockLeft = currentBlockInMap;
	}
	else if (direction == block_up)
	{
		currentBlockInMap->blockUp = neighbor;
		neighbor->blockDown = currentBlockInMap;
	}
	else if (direction == block_down)
	{
		currentBlockInMap->blockDown = neighbor;
		neighbor->blockUp = currentBlockInMap;
	}
}


//**************************************************************************************************
//* GLOBAL FUNCTIONS
//**************************************************************************************************

void connectToNeighbors()
{
	// Calculate id's for neighbors
	int leftNeigborID = getUniqueID(--(currentBlockInMap->corX), currentBlockInMap->corY);
	int upNeigborID = getUniqueID(currentBlockInMap->corX, ++(currentBlockInMap->corY));
	int rightNeigborID = getUniqueID(++(currentBlockInMap->corX), currentBlockInMap->corY);
	int downNeigborID = getUniqueID(currentBlockInMap->corX, --(currentBlockInMap->corY));

	// Check id's if exist, if yes, connect them to existing block
	map_block* neighbor;
	neighbor = searchItemInHT(leftNeigborID, --(currentBlockInMap->corX), currentBlockInMap->corY);
	if (neighbor)
	{
		connectBlock(neighbor, block_left);
	}

	neighbor = searchItemInHT(upNeigborID, currentBlockInMap->corX, ++(currentBlockInMap->corY));
	if (neighbor)
	{
		connectBlock(neighbor, block_up);
	}

	neighbor = searchItemInHT(rightNeigborID, ++(currentBlockInMap->corX), currentBlockInMap->corY);
	if (neighbor)
	{
		connectBlock(neighbor, block_right);
	}

	neighbor = searchItemInHT(downNeigborID, currentBlockInMap->corX, --(currentBlockInMap->corY));
	if (neighbor)
	{
		connectBlock(neighbor, block_down);
	}
}

