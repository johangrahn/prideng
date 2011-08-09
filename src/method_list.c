#include "method_list.h"

#include <stdlib.h>

void 
method_list_init( method_list_t *list )
{
	list->tree = NULL;
	list->curr = -1;
}

void
method_list_insert( method_list_t *list, char *key, method_prototype method )
{
	/* Move the pointer to a new position */
	list->curr++;
	
	/* Store the method */
	list->methods[ list->curr ] = method;
	
	list->tree = btree_insert( list->tree, key, list->curr );
}

method_prototype
method_list_find( method_list_t *list, char *key )
{
	int index;
	
	index = btree_search( list->tree, key );
	
	if( index != -1 )
	{
		return list->methods[ index ];
	}
	else
	{
		return NULL;
	}
}

void
method_list_free( method_list_t *list )
{
	btree_free( list->tree );
}
