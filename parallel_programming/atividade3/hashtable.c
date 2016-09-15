#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

#include "hashtable.h"

#define HASH_SIZE 9987

struct entry {
	int key;
	struct entry *next;
};

typedef struct entry entry_t;

struct hash_table {
/* defina os campos da sua estrutura principal aqui 
 * Note que o tipo de dado do elemento a ser armazenado deve
 * ser "int".
 * */
  int total_elements; 
  struct entry **table;
};


/*
 * Initializes the hash table. Needs to be called only once at the
 * beginning.
 * Does not need to be thread safe.
 */
void ht_init(hash_table_t **ht)
{
	int i;

	*ht = malloc(sizeof(hash_table_t));

	(*ht)->total_elements = 0;

	(*ht)->table = malloc(sizeof(entry_t *) * HASH_SIZE);

	for(i = 0; i < HASH_SIZE; i++) {
		(*ht)->table[i] = NULL;
	}
}

/*
 * Hash function.
 * Returns index of item.
 */
int ht_hash(int item) 
{
	return (abs(item) % HASH_SIZE);
}

/*
 * Returns the number of elements in the hash table.
 * Does not need to be thread safe.
 */
size_t ht_size(hash_table_t *ht)
{
	return ht->total_elements;
}

/*
 * Adds an item to the given list.
 * Return 1 if successful or 0 otherwise.
 */
int ht_add(hash_table_t *ht, int item)
{
	int position = ht_hash(item);
	int inserted = 0;

	// Allocate memory for the next element in hash table
	entry_t *new_entry = malloc(sizeof(entry_t));
	new_entry->key = item;
	new_entry->next = NULL;

	// Check if position is empty
	if(ht->table[position] == NULL) {	
		ht->table[position] = new_entry;
		inserted = 1;
	} else {	// Pass all elements in chain to add new entry
		entry_t *next_entry;
		next_entry = ht->table[position];

		while(next_entry->next != NULL) next_entry = next_entry->next;

		next_entry->next = new_entry;
		inserted = 1;
	}

	ht->total_elements++;

	return inserted;
}

/*
 * Removes an item from the given list.
 * Return 1 if successful or 0 otherwise.
 */
int ht_remove(hash_table_t *ht, int item) 
{
	int position = ht_hash(item);
	int removed = 0;

	if(ht->table[position] != NULL) {
		entry_t *next_entry, *previous;
		// Check if item to be removed is the first of a chain list
		if(ht->table[position]->key == item) {
			// Remove the first element of chain
			next_entry = ht->table[position];
			ht->table[position] = next_entry->next;
			
			free(next_entry);
			removed = 1;
		} else {		// Remove the others elements of chain
			previous = ht->table[position];
			next_entry = previous->next;

			while(next_entry != NULL) {
				if(next_entry->key == item) {
					previous->next = next_entry->next;
					free(next_entry);

					removed = 1;
					break;
				}
				previous = next_entry;
				next_entry = next_entry->next;
			}
		}
	}

	if(removed) ht->total_elements--;

	return removed;
}

/*
 * Search for an item in the given list.
 * Return 1 if successful or 0 otherwise.
 */
int ht_contain(hash_table_t *ht, int item)
{
	int position = ht_hash(item);
	int found = 0;

	if(ht->table[position] != NULL) {
		entry_t *next_entry;
		next_entry = ht->table[position];

		while(next_entry != NULL) {
			if(next_entry->key == item) {
				found = 1;
				break;
			}
			next_entry = next_entry->next;
		}
	}
	
	return found;
}

/*
 * Voces podem executar o programa abaixo para garantir que
 * o basico esteja funcionando antes de prosseguir.
 */

#ifdef TEST_HASH
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

hash_table_t *hasht;

int main()
{
  ht_init(&hasht);

  ht_add(hasht, 0);
  ht_add(hasht, -10);
  ht_add(hasht, 10);

  assert(ht_contain(hasht, 10));
  assert(ht_contain(hasht, 0));
  assert(ht_contain(hasht, -10));
  
  ht_remove(hasht, 0);
  assert(!ht_contain(hasht, 0));

  ht_remove(hasht, 10);
  assert(!ht_contain(hasht, 10));

  ht_remove(hasht, -10);
  assert(!ht_contain(hasht, -10));

  assert(ht_contain(hasht, 0));

  return 0;
}
#endif
