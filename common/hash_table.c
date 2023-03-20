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
HashTable *create_hash_table(int size) {
    HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
    ht->size = size;
    ht->count = 0;
    ht->table = (Node **)calloc(size, sizeof(Node *));
return ht;
}

void delete_hash_table(HashTable *ht) {
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

void insert(HashTable *ht, int key, map_block *value) {
    if (ht->count >= ht->size * 0.7) {
        resize_hash_table(ht);
    }
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->key = key;
    new_node->value = value;
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

    ht->count++;
}

map_block *search(HashTable *ht, int key) {
    int index = hash(key, ht->size);
    Node *node = ht->table[index];
    while (node) {
        if (node->key == key) {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}

void delete(HashTable *ht, int key) {
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

void resize_hash_table(HashTable *ht) {
    int old_size = ht->size;
    Node **old_table = ht->table;

    ht->size *= 2;
    ht->count = 0;
    ht->table = (Node **)calloc(ht->size, sizeof(Node *));

    for (int i = 0; i < old_size; i++) {
        Node *node = old_table[i];
        while (node) {
            insert(ht, node->key, node->value);
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
