/**
 ********************************************************************************
 * @file    map_init.c
 * @author  user
 * @date    Dec 5, 2022
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "map_init.h"
#include "stdlib.h"

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * PRIVATE TYPEDEFS
 ************************************/
typedef enum _block_direction
{
  Up = 0U,
  Down,
  Left,
  Right
}block_direction;

/************************************
 * STATIC VARIABLES
 ************************************/

/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/************************************
 * STATIC FUNCTIONS
 ************************************/
// static void initNewBlock(map_block *newBlock)
// {
//   map_object_t field = malloc(BLK_ROWS * sizeof(map_object_t));
// 	for (int i = 0; i < BLK_ROWS; i++)
// 	{
// 	  block->currentBlock->field[i] = malloc(BLK_COLS * sizeof(int));
// 	}
//   for(int i = 0; i < BLK_ROWS; i++)
//   {
//     for (int j = 0; j < BLK_COLS; j++)
//     {
//       block->currentBlock->field[i][j] = Empty;
//     }
//   }
// }

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

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void initMap()
{
  map_block currentBlock;
  //initNewBlock(&currentBlock);
  currentBlock.currentBlock->field[0][0] = Wall;
  for (int i = 0; i < BLK_ROWS; i++)
	  free(currentBlock.currentBlock->field[i]);
}
