#ifndef __CACHE_H__
#define __CACHE_H__

#include "memory.h"

struct block{
    int tag;
    int dirty;
    int valid;

    void* data;
};

struct cache
{
    int block_size;
    struct block* blocks;
    int num_blocks;
};


#endif // __CACHE_H__