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


//**************************************************************************************************
//* GLOBAL FUNCTIONS
//**************************************************************************************************

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
bool_t doesBlockExists(block_direction direction)
{
	bool_t ret = false;
	map_block* neighbor;

	// Calculate id's for neighbors
	switch (direction)
	{
		case block_left:
			int leftNeigborID = getUniqueID(--(currentBlockInMap->corX), currentBlockInMap->corY);
			if (searchItemInHT(leftNeigborID, --(currentBlockInMap->corX), currentBlockInMap->corY))
			{
				ret = true;
			}
			break;
		case block_up:
			int upNeigborID = getUniqueID(currentBlockInMap->corX, ++(currentBlockInMap->corY));
			if (searchItemInHT(upNeigborID, currentBlockInMap->corX, ++(currentBlockInMap->corY)))
			{
				ret = true;
			}
			break;
		case block_right:
			int rightNeigborID = getUniqueID(++(currentBlockInMap->corX), currentBlockInMap->corY);
			if (searchItemInHT(rightNeigborID, ++(currentBlockInMap->corX), currentBlockInMap->corY))
			{
				ret = true;
			}
			break;
		case block_down:
			int downNeigborID = getUniqueID(currentBlockInMap->corX, --(currentBlockInMap->corY));
			if (searchItemInHT(downNeigborID, currentBlockInMap->corX, --(currentBlockInMap->corY)))
			{
				ret = true;
			}
			break;

		default:
			break;
	}
	return ret;
}

