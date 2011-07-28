#include "method_list.h"

void 
method_list_init( method_list_t *list )
{
	list->curr = -1;
}

void
method_list_insert( method_list_t *list, char *key, method_prototype method )
{
	/* Move the pointer to a new position */
	list->curr++;
	
	/* Store the method */
	list->methods[ list->curr ] = method;
	
	btree_insert( &list->tree, key, list->curr );
}

