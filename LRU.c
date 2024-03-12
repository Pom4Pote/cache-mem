#include "LRU.h"

struct LRU* LRU_create(int block_size, int num_blocks, int data_size){
    struct LRU* lru = malloc(sizeof(struct LRU));
    lru->cache.block_size = block_size;
    lru->cache.num_blocks = num_blocks;
    lru->cache.blocks = malloc(sizeof(struct block) * num_blocks);
    lru->timestamps = malloc(sizeof(time_t) * num_blocks);
    int i;
    for(i = 0; i < num_blocks; i++){
        lru->cache.blocks[i].data = malloc(data_size);
        lru->cache.blocks[i].valid = 0;
        lru->cache.blocks[i].dirty = 0;
        lru->timestamps[i] = 0;
    }
    lru->miss = 0;
    lru->hit = 0;
    return lru;
}

void LRU_destroy(struct LRU* lru){
    int i;
    for(i = 0; i < lru->cache.num_blocks; i++){
        free(lru->cache.blocks[i].data);
    }
    free(lru->cache.blocks);
    free(lru->timestamps);
    free(lru);
}

void* LRU_read(struct LRU* lru, struct memory* mem, int address){
    int block_size = lru->cache.block_size;
    int num_blocks = lru->cache.num_blocks;
    int data_size = mem->data_size;
    int tag = address - (address % block_size);
    int index = 0;
    int offset = address % block_size;
    struct block* block = NULL;
    for(int i = 0; i < num_blocks; i++){
        if(lru->cache.blocks[i].valid && lru->cache.blocks[i].tag == tag){
            index = i;
            block = &lru->cache.blocks[i];
            break;
        }
    }
    if(block == NULL){
        lru->miss++;
        int oldest = 0;
        for(int i = 0; i < num_blocks; i++){
            if(lru->timestamps[i] < lru->timestamps[oldest]){
                oldest = i;
            }
        }
        if(lru->cache.blocks[oldest].dirty){
            int old_address = lru->cache.blocks[oldest].tag * block_size * num_blocks + oldest * block_size;
            void* old_block = memory_get_block(mem, old_address, block_size);
            memcpy(lru->cache.blocks[oldest].data, old_block, block_size * data_size);
        }

        lru->cache.blocks[oldest].valid = 1;
        lru->cache.blocks[oldest].dirty = 0;
        lru->cache.blocks[oldest].tag = tag;
        void* new_block = memory_get_block(mem, address, block_size);
        memcpy(lru->cache.blocks[oldest].data, new_block, block_size * data_size);
        lru->timestamps[oldest] = time(NULL);
        return lru->cache.blocks[oldest].data + offset * data_size;
    }
    lru->hit++;
    lru->timestamps[index] = time(NULL);
    return block->data + offset * data_size;
}

void LRU_write(struct LRU* lru, struct memory* mem, int address, void* value){
    int block_size = lru->cache.block_size;
    int num_blocks = lru->cache.num_blocks;
    int data_size = mem->data_size;
    int tag = address - (address % block_size);
    int index = 0;
    int offset = address % block_size;
    struct block* block = NULL;
    for(int i = 0; i < num_blocks; i++){
        if(lru->cache.blocks[i].valid && lru->cache.blocks[i].tag == tag){
            index = i;
            block = &lru->cache.blocks[i];
            break;
        }
    }
    if(block == NULL){
        lru->miss++;
        int oldest = 0;
        for(int i = 0; i < num_blocks; i++){
            if(lru->timestamps[i] < lru->timestamps[oldest]){
                oldest = i;
            }
        }
        if(lru->cache.blocks[oldest].dirty){
            int old_address = lru->cache.blocks[oldest].tag * block_size * num_blocks + oldest * block_size;
            void* old_block = memory_get_block(mem, old_address, block_size);
            memcpy(lru->cache.blocks[oldest].data, old_block, block_size * data_size);
        }

        lru->cache.blocks[oldest].valid = 1;
        lru->cache.blocks[oldest].dirty = 0;
        lru->cache.blocks[oldest].tag = tag;
        void* new_block = memory_get_block(mem, address, block_size);
        memcpy(lru->cache.blocks[oldest].data, new_block, block_size * data_size);
        lru->timestamps[oldest] = time(NULL);
        block = &lru->cache.blocks[oldest];
    }
    lru->hit++;
    lru->timestamps[index] = time(NULL);
    memcpy(block->data + offset * data_size, value, data_size);
    block->dirty = 1;
}

void print_cache(struct LRU* lru){
    int i;
    for(i = 0; i < lru->cache.num_blocks; i++){
        printf("Block %d: ", i);
        for(int j = 0; j < lru->cache.block_size; j++){
            printf("%d ", *((int*)lru->cache.blocks[i].data + j));
        }
    }
    printf("\n");
}

int LRU_get_miss(struct LRU* lru){
    return lru->miss;
}

int LRU_get_hit(struct LRU* lru){
    return lru->hit;
}
