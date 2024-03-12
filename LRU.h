#ifndef __LRU_H__
#define __LRU_H__

#include "cache.h"
#include <time.h>

struct LRU{
    time_t* timestamps;
    struct cache cache;

    int miss;
    int hit;
};

struct LRU* LRU_create(int block_size, int num_blocks, int data_size);
void LRU_destroy(struct LRU* lru);
void* LRU_read(struct LRU* lru, struct memory* mem, int address);
void LRU_write(struct LRU* lru, struct memory* mem, int address, void* value);
void print_cache(struct LRU* lru);

int LRU_get_miss(struct LRU* lru);
int LRU_get_hit(struct LRU* lru);

#endif // __LRU_H__