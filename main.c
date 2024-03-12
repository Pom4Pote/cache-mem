#include "LRU.h"
#include "LRUclock.h"
#include "LFU.h"

#include<unistd.h>

// Fonction pour générer une liste aléatoire
int* generate_list(int size){
    srand(time(NULL));
    int* list = malloc(sizeof(int) * size);
    int i;
    for(i = 0; i < size; i++){
        list[i] = rand() % 32;
    }
    return list;
}

// Fonction pour generer une liste de test
int* string_to_tab(){
    //2 5 10 1 2 2 6 9 1 2 10 2 6 1 2 1 6 9 5 1
    int* tab = malloc(sizeof(int) * 20);
    tab[0] = 2;
    tab[1] = 5;
    tab[2] = 10;
    tab[3] = 1;
    tab[4] = 2;
    tab[5] = 2;
    tab[6] = 6;
    tab[7] = 9;
    tab[8] = 1;
    tab[9] = 2;
    tab[10] = 10;
    tab[11] = 2;
    tab[12] = 6;
    tab[13] = 1;
    tab[14] = 2;
    tab[15] = 1;
    tab[16] = 6;
    tab[17] = 9;
    tab[18] = 5;
    tab[19] = 1;

    // 0 4 1 4 2 4 3 4 2 4 0 4 1 4 2 4 3 4
    /*int* tab = malloc(sizeof(int) * 18);
    tab[0] = 0;
    tab[1] = 4;
    tab[2] = 1;
    tab[3] = 4;
    tab[4] = 2;
    tab[5] = 4;
    tab[6] = 3;
    tab[7] = 4;
    tab[8] = 2;
    tab[9] = 4;
    tab[10] = 0;
    tab[11] = 4;
    tab[12] = 1;
    tab[13] = 4;
    tab[14] = 2;
    tab[15] = 4;
    tab[16] = 3;
    tab[17] = 4;*/
    return tab;
    
}

// Fonction main
int main(){
    struct LRU* lru = LRU_create(4, 4, sizeof(int));
    struct LRUclock* lruclock = LRUclock_create(4, 4, sizeof(int));
    struct LFU* lfu = LFU_create(4, 4, sizeof(int));

    struct memory* mem = memory_create(32, sizeof(int));


    int i;
    for(i = 0; i < 32; i++){
        int value = i;
        memory_set(mem, i, &value);
    }
    int* list = generate_list(64);
    //int* list = string_to_tab();
    for(i = 0; i < 64; i++){
        printf("Tour : %d\n", i+1);
        int* value = LRU_read(lru, mem, list[i]);
        printf("LRU index : %d, valeur : %d\n", list[i], *value);
        print_cache(lru);
        printf("\n");

        int* value2 = LRUclock_read(lruclock, mem, list[i]);
        printf("LRUclock index : %d, valeur : %d\n", list[i], *value2);
        print_cacheclock(lruclock);
        printf("\n");

        int* value3 = LFU_read(lfu, mem, list[i]);
        printf("LFU index : %d, valeur : %d\n", list[i], *value3);
        print_cacheLFU(lfu);

        sleep(1);
    }

    printf("LRU miss : %d\n", LRU_get_miss(lru));
    printf("LRU hit : %d\n", LRU_get_hit(lru));
    printf("LRUclock miss : %d\n", LRUclock_get_miss(lruclock));
    printf("LRUclock hit : %d\n", LRUclock_get_hit(lruclock));
    printf("LFU miss : %d\n", LFU_get_miss(lfu));
    printf("LFU hit : %d\n", LFU_get_hit(lfu));

    LRU_destroy(lru);
    LRUclock_destroy(lruclock);
    LFU_destroy(lfu);
    memory_destroy(mem);
    return 0;
}
