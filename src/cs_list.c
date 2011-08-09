#include "cs_list.h"

#include <stdlib.h>

void 
cs_list_init( cs_list_t *list )
{
	list->tree = NULL;
	list->curr = -1;
}
 
void
cs_list_insert( cs_list_t *list, cs_t *cs )
{
	/* Move the pointer to a new position */
	list->curr++;
	
	/* Store the method */
	list->clist[ list->curr ] = cs;
	
	/* Store the data inside the binary tree */
	list->tree = btree_insert( list->tree, cs->dboid, list->curr );
	
	
}

cs_t *
cs_list_find( cs_list_t *list, char *dboid )
{
	int index;
	
	index = btree_search( list->tree, dboid );
	
	if( index != -1 )
	{
		return list->clist[ index ];
	}
	else
	{
		return NULL;
	}
}
