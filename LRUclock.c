#include "LRUclock.h"

struct LRUclock* LRUclock_create(int block_size, int num_blocks, int data_size){
    struct LRUclock* lru = malloc(sizeof(struct LRUclock));
    lru->cache.block_size = block_size;
    lru->cache.num_blocks = num_blocks;
    lru->cache.blocks = malloc(sizeof(struct block) * num_blocks);
    lru->bits = malloc(sizeof(char) * num_blocks);
    int i;
    for(i = 0; i < num_blocks; i++){
        lru->cache.blocks[i].data = malloc(data_size);
        lru->cache.blocks[i].valid = 0;
        lru->cache.blocks[i].dirty = 0;
        lru->bits[i] = 0;
    }
    lru->pointer = 0;
    lru->miss = 0;
    lru->hit = 0;
    return lru;
}

void LRUclock_destroy(struct LRUclock* lru){
    int i;
    for(i = 0; i < lru->cache.num_blocks; i++){
        free(lru->cache.blocks[i].data);
    }
    free(lru->cache.blocks);
    free(lru->bits);
    free(lru);
}

void* LRUclock_read(struct LRUclock* lru, struct memory* mem, int address){
    int block_size = lru->cache.block_size;
    int num_blocks = lru->cache.num_blocks;
    int data_size = mem->data_size;
    int tag = address - (address % block_size);
    int index = 0;
    int offset = address % block_size;
    struct block* block = NULL;
    //printf("Address : %d\n", address);
    //printf("Tag : %d\n", tag);
    for(int i = 0; i < num_blocks; i++){
        if(lru->cache.blocks[i].valid && lru->cache.blocks[i].tag == tag){
            index = i;
            block = &lru->cache.blocks[i];
            break;
        }
    }
    //printf("Index : %d\n", index);
    //printf("Block : %d\n", block==NULL);
    if(block == NULL){
        lru->miss++;
        while(lru->bits[lru->pointer]){
            lru->bits[lru->pointer] = 0;
            lru->pointer = (lru->pointer + 1) % num_blocks;
        }
        if(lru->cache.blocks[lru->pointer].dirty){
            int old_address = lru->cache.blocks[lru->pointer].tag * block_size * num_blocks + lru->pointer * block_size;
            void* old_block = memory_get_block(mem, old_address, block_size);
            memcpy(lru->cache.blocks[lru->pointer].data, old_block, block_size * data_size);
        }
        lru->cache.blocks[lru->pointer].valid = 1;
        lru->cache.blocks[lru->pointer].dirty = 0;
        lru->cache.blocks[lru->pointer].tag = tag;
        void* new_block = memory_get_block(mem, address, block_size);
        memcpy(lru->cache.blocks[lru->pointer].data, new_block, block_size * data_size);
        lru->bits[lru->pointer] = 0;
        int old_pointer = lru->pointer;
        lru->pointer = (lru->pointer + 1) % num_blocks;
        block = &lru->cache.blocks[old_pointer];
        return block->data + offset * data_size;
    }
    lru->hit++;
    lru->bits[index] = 1;
    return block->data + offset * data_size;
}

void LRUclock_write(struct LRUclock* lru, struct memory* mem, int address, void* value){
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
        while(lru->bits[lru->pointer]){
            lru->bits[lru->pointer] = 0;
            lru->pointer = (lru->pointer + 1) % num_blocks;
        }
        if(lru->cache.blocks[lru->pointer].dirty){
            int old_address = lru->cache.blocks[lru->pointer].tag * block_size * num_blocks + lru->pointer * block_size;
            void* old_block = memory_get_block(mem, old_address, block_size);
            memcpy(lru->cache.blocks[lru->pointer].data, old_block, block_size * data_size);
        }
        lru->cache.blocks[lru->pointer].valid = 1;
        lru->cache.blocks[lru->pointer].dirty = 0;
        lru->cache.blocks[lru->pointer].tag = tag;
        void* new_block = memory_get_block(mem, address, block_size);
        memcpy(lru->cache.blocks[lru->pointer].data, new_block, block_size * data_size);
        lru->bits[lru->pointer] = 0;
        lru->pointer = (lru->pointer + 1) % num_blocks;
        block = &lru->cache.blocks[lru->pointer];
    }
    lru->hit++;
    lru->cache.blocks[index].dirty = 1;
    memcpy(block->data + offset * data_size, value, data_size);
}

int LRUclock_get_miss(struct LRUclock* lru){
    return lru->miss;
}

int LRUclock_get_hit(struct LRUclock* lru){
    return lru->hit;
}

void print_cacheclock(struct LRUclock* lru){
    int i;
    for(i = 0; i < lru->cache.num_blocks; i++){
        printf("Block %d: ", i);
        for(int j = 0; j < lru->cache.block_size; j++){
            printf("%d ", *((int*)lru->cache.blocks[i].data + j));
        }
    }
    printf("\n");
    for(i = 0; i < lru->cache.num_blocks; i++){
        printf("Bit %d: %d ", i, lru->bits[i]);
    }
    printf("\n");
}