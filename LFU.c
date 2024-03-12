#include "LFU.h"

struct LFU* LFU_create(int block_size, int num_blocks, int data_size){
    struct LFU* lfu = (struct LFU*)malloc(sizeof(struct LFU));
    lfu->frequencies = (int*)malloc(num_blocks * sizeof(int));
    for(int i = 0; i < num_blocks; i++){
        lfu->frequencies[i] = 0;
    }
    lfu->cache.block_size = block_size;
    lfu->cache.num_blocks = num_blocks;
    lfu->cache.blocks = (struct block*)malloc(num_blocks * sizeof(struct block));
    for(int i = 0; i < num_blocks; i++){
        lfu->cache.blocks[i].data = malloc(data_size * block_size);
        lfu->cache.blocks[i].valid = 0;
        lfu->cache.blocks[i].dirty = 0;
    }
    lfu->miss = 0;
    lfu->hit = 0;
    return lfu;
}

void LFU_destroy(struct LFU* lfu){
    int i;
    for(i = 0; i < lfu->cache.num_blocks; i++){
        free(lfu->cache.blocks[i].data);
    }
    free(lfu->cache.blocks);
    free(lfu->frequencies);
    free(lfu);
}

void* LFU_read(struct LFU* lfu, struct memory* mem, int address){
    int block_size = lfu->cache.block_size;
    int num_blocks = lfu->cache.num_blocks;
    int data_size = mem->data_size;
    int tag = address - (address % block_size);
    int index = 0;
    int offset = address % block_size;
    struct block* block = NULL;
    for(int i = 0; i < num_blocks; i++){
        if(lfu->cache.blocks[i].valid && lfu->cache.blocks[i].tag == tag){
            index = i;
            block = &lfu->cache.blocks[i];
            break;
        }
    }

    if(block == NULL){
        lfu->miss++;
        int min = lfu->frequencies[0];
        int min_index = 0;
        for(int i = 1; i < lfu->cache.num_blocks; i++){
            if(lfu->frequencies[i] < min){
                min = lfu->frequencies[i];
                min_index = i;
            }
        }
        index = min_index;
        if(lfu->cache.blocks[index].dirty){
            int old_address = lfu->cache.blocks[index].tag * block_size * num_blocks + index * block_size;
            void* old_block = memory_get_block(mem, old_address, block_size);
            memcpy(lfu->cache.blocks[index].data, old_block, block_size * data_size);
        }

        lfu->cache.blocks[index].valid = 1;
        lfu->cache.blocks[index].dirty = 0;
        lfu->cache.blocks[index].tag = tag;
        void* new_block = memory_get_block(mem, tag, block_size);
        memcpy(lfu->cache.blocks[index].data, new_block, block_size * data_size);
        lfu->frequencies[index] = 1;
    }else{
        lfu->hit++;
        lfu->frequencies[index]++;
    }
    return lfu->cache.blocks[index].data + offset * data_size;

}

void LFU_write(struct LFU* lfu, struct memory* mem, int address, void* value){
    int block_size = lfu->cache.block_size;
    int num_blocks = lfu->cache.num_blocks;
    int data_size = mem->data_size;
    int tag = address - (address % block_size);
    int index = 0;
    int offset = address % block_size;
    struct block* block = NULL;
    for(int i = 0; i < num_blocks; i++){
        if(lfu->cache.blocks[i].valid && lfu->cache.blocks[i].tag == tag){
            index = i;
            block = &lfu->cache.blocks[i];
            break;
        }
    }

    if(block == NULL){
        lfu->miss++;
        int min = lfu->frequencies[0];
        int min_index = 0;
        for(int i = 1; i < lfu->cache.num_blocks; i++){
            if(lfu->frequencies[i] < min){
                min = lfu->frequencies[i];
                min_index = i;
            }
        }
        index = min_index;
        if(lfu->cache.blocks[index].dirty){
            int old_address = lfu->cache.blocks[index].tag * block_size * num_blocks + index * block_size;
            void* old_block = memory_get_block(mem, old_address, block_size);
            memcpy(lfu->cache.blocks[index].data, old_block, block_size * data_size);
        }

        lfu->cache.blocks[index].valid = 1;
        lfu->cache.blocks[index].dirty = 0;
        lfu->cache.blocks[index].tag = tag;
        void* new_block = memory_get_block(mem, tag, block_size);
        memcpy(lfu->cache.blocks[index].data, new_block, block_size * data_size);
        lfu->frequencies[index] = 1;
    }else{
        lfu->hit++;
        lfu->frequencies[index]++;
    }
    memcpy(lfu->cache.blocks[index].data + offset * data_size, value, data_size);
}

void print_cacheLFU(struct LFU* lfu){
    int i;
    for(i = 0; i < lfu->cache.num_blocks; i++){
        printf("Block %d: ", i);
        for(int j = 0; j < lfu->cache.block_size; j++){
            printf("%d ", *((int*)lfu->cache.blocks[i].data + j));
        }
    }
    printf("\n");

    for(i = 0; i < lfu->cache.num_blocks; i++){
        printf("Frequency %d: %d\n", i, lfu->frequencies[i]);
    }
    printf("\n");
}

int LFU_get_miss(struct LFU* lfu){
    return lfu->miss;
}

int LFU_get_hit(struct LFU* lfu){
    return lfu->hit;
}