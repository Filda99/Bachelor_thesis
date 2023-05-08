/**
 ***************************************************************************************************
 * @file    hash_table.c
 * @author  xjahnf00
 * @date    Mar 20, 2023
 * @brief   
 ***************************************************************************************************
 */

//**************************************************************************************************
//* INCLUDES
//**************************************************************************************************
#include "utilities/fsl_debug_console.h"
#include "hast_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//**************************************************************************************************
//* EXTERN VARIABLES
//**************************************************************************************************

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
HashTable *ht;

//**************************************************************************************************
//* STATIC FUNCTION PROTOTYPES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTIONS
//**************************************************************************************************

//**************************************************************************************************
//* GLOBAL FUNCTIONS
//**************************************************************************************************


//!*************************************************************************************************
//! void createHashTable(int size)
//!
//! @description
//! Function allocates the hash table.
//!
//! @param    int	size	Number of rows in the table
//!
//! @return   None
//!*************************************************************************************************
void createHashTable(int size)
{
	ht = (HashTable *)malloc(sizeof(HashTable));
    ht->size = size;
    ht->count = 0;
    ht->table = (Node **)calloc(size, sizeof(Node *));
}


//!*************************************************************************************************
//! void deleteHashTable(void)
//!
//! @description
//! Function will delete(deallocates) all items and rows in the hash table.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
void deleteHashTable()
{
    for (int i = 0; i < ht->size; i++) {
        Node *node = ht->table[i];
        while (node) {
            Node *temp = node;
            node = node->next;
            free(temp);
        }
    }
    free(ht->table);
    free(ht);
}


//!*************************************************************************************************
//! void insertToHashTable(int key, map_block *value)
//!
//! @description
//! Function will insert block to the row of a table.
//!
//! @param    int		key		Key of item to be inserted
//! @param    map_block	value	Block of map to be inserted into the hash table
//!
//! @return   None
//!*************************************************************************************************
void insertToHashTable(int key, map_block *value)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) return;
    new_node->key = key;
    new_node->value = value;
    new_node->next = NULL;

    int index = abs(hash(key, ht->size));
    Node *node = ht->table[index];

    if (!node) {
        ht->table[index] = new_node;
    } else {
        while (node->next) {
            node = node->next;
        }
        node->next = new_node;
    }

    ht->count++;
}


//!*************************************************************************************************
//! map_block* searchItemInHT(int key, int x, int y)
//!
//! @description
//! Function will try to find block of map by given ID and x/y axis.
//!
//! @param    int	key	Key of the block
//! @param    int	x	X-axis of the block
//! @param    int	y	Y-axis of the block
//!
//! @return   If found, return block of the map. Otherwise return null.
//!*************************************************************************************************
map_block *searchItemInHT(int key, int x, int y)
{
    int index = abs(hash(key, ht->size));
    Node *node = ht->table[index];
    while (node != NULL) {
        if (node->key == key && node->value->corX == x && node->value->corY == y) {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}


//!*************************************************************************************************
//! void deleteItemInHT(int key)
//!
//! @description
//! Function will delete item from hash table.
//!
//! @param    int	key	Key of a block which should be deleted.
//!
//! @return   None
//!*************************************************************************************************
void deleteItemInHT(int key)
{
    int index = hash(key, ht->size);
    Node *node = ht->table[index];
    Node *prev = NULL;

    while (node) {
        if (node->key == key) {
            if (prev) {
                prev->next = node->next;
            } else {
                ht->table[index] = node->next;
            }

            free(node);
            ht->count--;
            return;
        }

        prev = node;
        node = node->next;
    }
}


//!*************************************************************************************************
//! void resizeHashTable()
//!
//! @description
//! Function will reallocates hash table to be big enough for the items.
//!
//! @param    None
//!
//! @return   None
//!*************************************************************************************************
void resizeHashTable()
{
    int old_size = ht->size;
    Node **old_table = ht->table;

    ht->size += 25;
    ht->count = 0;
    ht->table = (Node **)calloc(ht->size, sizeof(Node *));

    for (int i = 0; i < old_size; i++) {
        Node *node = old_table[i];
        while (node) {
            insertToHashTable(node->key, node->value);
            Node *temp = node;
            node = node->next;
            free(temp);
        }
    }

    free(old_table);
}


//!*************************************************************************************************
//! int hash(int key, int size)
//!
//! @description
//! Function calculate row so that item can be inserted into hash table.
//!
//! @param    int	key		Key of the item
//! @param    int	size	Size of the hast table (number of rows)
//!
//! @return   Number of row
//!*************************************************************************************************
int hash(int key, int size)
{
    return key % size;
}
