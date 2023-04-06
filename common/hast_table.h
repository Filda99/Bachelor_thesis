/**
 ***************************************************************************************************
 * @file    hast_table.h
 * @author  user
 * @date    Mar 20, 2023
 * @brief   
 ***************************************************************************************************
 */

#ifndef HAST_TABLE_H_
#define HAST_TABLE_H_

#ifdef __cplusplus
extern "C" {
#endif

//**************************************************************************************************
//* INCLUDES
//**************************************************************************************************
#include "map/map_operations.h"
//**************************************************************************************************
//* MACROS AND DEFINES
//**************************************************************************************************

//**************************************************************************************************
//* TYPEDEFS
//**************************************************************************************************
typedef struct Node {
	int key;
	map_block *value;
	struct Node *next;
} Node;

typedef struct HashTable {
	int size;
	int count;
	Node **table;
} HashTable;

//**************************************************************************************************
//* EXPORTED VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* GLOBAL FUNCTION PROTOTYPES
//**************************************************************************************************
void createHashTable(int size);
void deleteHashTable();
void insertToHashTable(int key, map_block *value);
map_block *searchItemInHT(int key, int x, int y);
void deleteItemInHT(int key);
void resizeHashTable();
int hash(int key, int size);

#ifdef __cplusplus
}
#endif

#endif /* HAST_TABLE_H_ */
