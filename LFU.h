#ifndef __LFU_H__
#define __LFU_H__

#include "cache.h"

struct LFU{
    int* frequencies;
    struct cache cache;

    int miss;
    int hit;
};

struct LFU* LFU_create(int block_size, int num_blocks, int data_size);
void LFU_destroy(struct LFU* lfu);
void* LFU_read(struct LFU* lfu, struct memory* mem, int address);
void LFU_write(struct LFU* lfu, struct memory* mem, int address, void* value);
void print_cacheLFU(struct LFU* lfu);

int LFU_get_miss(struct LFU* lfu);
int LFU_get_hit(struct LFU* lfu);

#endif // __LFU_H__