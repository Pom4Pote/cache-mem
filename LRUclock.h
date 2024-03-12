#ifndef __LRUclock_H__
#define __LRUclock_H__

#include "cache.h"


/*
* Structure LRUclock
* @param bits tableau de bits, 0 = pas utilisé, 1 = utilisé
* @param cache structure de cache
* @param pointer pointeur vers le prochain bloc à remplacer
* @param miss compteur de miss
* @param hit compteur de hit
* @typedef struct LRUclock
*/
struct LRUclock{
    char* bits;
    struct cache cache;
    int pointer;

    int miss;
    int hit;
};

/*
* Crée une structure LRUclock
* @param block_size taille d'un bloc
* @param num_blocks nombre de blocs
* @param data_size taille des données
* @return la structure LRUclock
*/
struct LRUclock* LRUclock_create(int block_size, int num_blocks, int data_size);

/*
* Détruit une structure LRUclock
* @param lru la structure à détruire
*/
void LRUclock_destroy(struct LRUclock* lru);

/*
* Lit une valeur dans le cache
* @param lru la structure LRUclock
* @param mem la mémoire
* @param address l'adresse à lire
* @return la valeur lue
*/
void* LRUclock_read(struct LRUclock* lru, struct memory* mem, int address);

/*
* Ecrit une valeur dans le cache
* @param lru la structure LRUclock
* @param mem la mémoire
* @param address l'adresse à écrire
* @param value la valeur à écrire
*/
void LRUclock_write(struct LRUclock* lru, struct memory* mem, int address, void* value);

/*
* Affiche le cache
* @param lru la structure LRUclock
*/
void print_cacheclock(struct LRUclock* lru);

/*
* Récupère le nombre de miss
* @param lru la structure LRUclock
*/
int LRUclock_get_miss(struct LRUclock* lru);

/*
* Récupère le nombre de hit
* @param lru la structure LRUclock
*/
int LRUclock_get_hit(struct LRUclock* lru);

#endif // __LRUclock_H__