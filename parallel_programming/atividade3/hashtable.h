

#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H


typedef struct hash_table hash_table_t;

/* Basic API for operations on a hash table */
void ht_init(hash_table_t **ht);
size_t ht_size(hash_table_t *ht);
int ht_add(hash_table_t *ht, int item);
int ht_remove(hash_table_t *ht, int item);
int ht_contain(hash_table_t *ht, int item);



#endif
