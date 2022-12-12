/**
 ********************************************************************************
 * @file    map_init.h
 * @author  user
 * @date    Dec 5, 2022
 * @brief   
 ********************************************************************************
 */

#ifndef MAP_INIT_H_
#define MAP_INIT_H_

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
typedef enum _map_object {
  Empty = 0U,  // We don't know, what is here yet
  Road,		   // Road (between lines)
  Line,        // Line detected on current field of the map
  Wall         // Wall detected on field
}map_object_t;

typedef struct block{
  map_object_t field[BLK_ROWS][BLK_COLS];
}block;

typedef struct map_blk
{
  struct block   *currentBlock;
  struct map_blk *blockUp;
  struct map_blk *blockDown;
  struct map_blk *blockLeft;
  struct map_blk *blockRight;
}map_block;


/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void initMap(void);

#ifdef __cplusplus
}
#endif

#endif 
