/**
 ***************************************************************************************************
 * @file    hash_table.c
 * @author  user
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
static HashTable *ht;

//**************************************************************************************************
//* GLOBAL VARIABLES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTION PROTOTYPES
//**************************************************************************************************

//**************************************************************************************************
//* STATIC FUNCTIONS
//**************************************************************************************************

//**************************************************************************************************
//* GLOBAL FUNCTIONS
//**************************************************************************************************


// Function implementations
void createHashTable(int size)
{
	ht = (HashTable *)malloc(sizeof(HashTable));
    ht->size = size;
    ht->count = 0;
    ht->table = (Node **)calloc(size, sizeof(Node *));
}

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

void insertToHashTable(int key, map_block value)
{
    if (ht->count >= ht->size * 0.7) {
        resizeHashTable(ht);
    }
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) return;
    new_node->key = key;
    new_node->value = &value;
    new_node->next = NULL;

    int index = hash(key, ht->size);
    Node *node = ht->table[index];

    if (!node) {
        ht->table[index] = new_node;
    } else {
        while (node->next) {
            node = node->next;
        }
        node->next = new_node;
    }

    free(new_node);

    ht->count++;
}

map_block *searchItemInHT(int key, int x, int y)
{
    int index = hash(key, ht->size);
    Node *node = ht->table[index];
    while (node) {
        if (node->key == key && node->value->corX == x && node->value->corY == y) {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}

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
            insertToHashTable(node->key, *(node->value));
            Node *temp = node;
            node = node->next;
            free(temp);
        }
    }

    free(old_table);
}

int hash(int key, int size) {
    return key % size;
}
