#ifndef __CS_LIST_H__
#define __CS_LIST_H__

#include "btree.h"
#include "cs.h"

typedef struct
{
	btree_t *tree;
	cs_t 	*clist[10];
	int 	curr;
} cs_list_t;

/* Sets default values */
void 
cs_list_init( cs_list_t *list );

/* Inserts a conflict set into the binary tree */ 
void
cs_list_insert( cs_list_t *list, cs_t *cs );

/* Finds a conflict set based on its dboid */
cs_t *
cs_list_find( cs_list_t *list, char *dboid );

void
cs_list_free( cs_list_t *list );

#endif
