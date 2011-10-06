#include "hash_table.h"

#include <stdio.h>
void
h_table_init( h_table_t *table, size_t size, size_t data_size )
{
	table->data_arr = malloc( size * data_size );
	table->curr = -1;
	table->tree = NULL;
}

void 
h_table_insert( h_table_t *table, char *key, void *data )
{
	/* Move the pointer to a new position */
	table->curr++;
	
	/* Store the data */
	table->data_arr[ table->curr ] = data;
	
	table->tree = btree_insert( table->tree, key, table->curr );
}

void *
h_table_find( h_table_t *table, char *key )
{
	int index;
	
	index = btree_search( table->tree, key );
	
	if( index != -1 )
	{
		return table->data_arr[ index ];
	}
	else
	{
		return NULL;
	}
}

/* Removes the entry from the table */
void
h_table_remove( h_table_t *table, char *key )
{
	btree_remove( table->tree, key );
}

void 
h_table_free( h_table_t *table )
{
	btree_free( table->tree );
	free( table->data_arr );
	
}
