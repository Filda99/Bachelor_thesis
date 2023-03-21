/**
 ********************************************************************************
 * @file    map_init.h
 * @author  user
 * @date    Dec 5, 2022
 * @brief   
 ********************************************************************************
 */

#ifndef MAP_OPERATIONS_H_
#define MAP_OPERATIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include "global_macros.h"

/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/

//* Map possible objects
typedef enum _map_object {
  map_Empty = 0U,	///< We don't know, what is here yet
  map_CurrentPosition,
  map_Track,		///< I was there in prev. steps
  map_Road,		   	///< Road (between lines)
  map_Line,        	///< Line detected on current field of the map
  map_Wall         	///< Wall detected on field
}map_object_t;

//* One block in a map
typedef struct map_blk
{
	map_object_t 	**currentBlock; // 2D field of map objects
	int	corX;
	int corY;
	int id;
}map_block;

//* Directions for moving in the map
typedef enum
{
	move_unknown = 0U,
	move_upRight,
	move_up = 2,
	move_downRight,
	move_right = 4,
	move_downLeft,
	move_down = 8,
	move_upLeft,
	move_left = 16,
}map_move_direction;

//* Directions for blocks
typedef enum _block_direction
{
	block_up = 0U, block_down, block_left, block_right
} block_direction;

//* Current position in the map block
typedef struct
{
	int Row;
	int Col;
}curr_pos_map;

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void createMap(void);
void saveMap(void);
void deleteMap(void);
void moveInMap(map_move_direction direction);

#ifdef __cplusplus
}
#endif

#endif 
