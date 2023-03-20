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

static bool checkID(int id)
{
	bool idFound = false;
	/*for(int i = 0; i < pathFieldCapacity; i++)
	{
		if (path[i] == id)
		{
			idFound = true;
			break;
		}
	}*/
	return idFound;
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

