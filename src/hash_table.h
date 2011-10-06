#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include "btree.h"
#include <stdlib.h>

typedef struct 
{
	/* Stores the key and the index to the data */
	btree_t 	*tree; 
	
	/* Stores the data */
	void 		**data_arr;
	
	/* Points to the current position in the static array that is used */
	int 		curr; 
} h_table_t;

/* Creates a new static hash table with a defined size */
void
h_table_init( h_table_t *table, size_t size, size_t data_size );

/* Inserts data into the hash table  */
void 
h_table_insert( h_table_t *table, char *key, void *data );

/* Finds the data from the key */
void *
h_table_find( h_table_t *table, char *key );

/* Removes the entry from the table */
void
h_table_remove( h_table_t *table, char *key );

/* Free all memory used by the hash table */
void 
h_table_free( h_table_t *table );

#endif