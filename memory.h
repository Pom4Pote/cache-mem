
#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
* Structure memory
* @param data données
* @param size taille de la mémoire
* @param data_size taille des données
* @typedef struct memory
*/
struct memory
{
    void* data;
    int size;
    int data_size;
};

struct memory* memory_create(int size, int data_size);
void memory_destroy(struct memory* mem);
void memory_set(struct memory* mem, int index, void* value);
void* memory_get(struct memory* mem, int index);
void* memory_get_block(struct memory* mem, int index, int block_size);
void print_memory(struct memory* mem);


#endif // __MEMORY_H__