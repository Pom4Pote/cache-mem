#include "memory.h"


struct memory* memory_create(int size, int data_size){
    struct memory* mem = malloc(sizeof(struct memory));
    mem->data = malloc(size * data_size);
    mem->size = size;
    mem->data_size = data_size;
    return mem;
}

void memory_destroy(struct memory* mem){
    free(mem->data);
    free(mem);
}

void memory_set(struct memory* mem, int index, void* value){
    memcpy(mem->data + index * mem->data_size, value, mem->data_size);
}

void* memory_get(struct memory* mem, int index){
    return mem->data + index * mem->data_size;
}

void* memory_get_block(struct memory* mem, int index, int block_size){
    return mem->data + (index - (index % block_size)) * mem->data_size;
}

void print_memory(struct memory* mem){
    printf("Memory : ");
    int i;
    for(i = 0; i < mem->size; i++){
        printf("%d : %d, ", i, *(int*)memory_get(mem, i));
    }
    printf("\n");
}