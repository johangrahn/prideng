#include "btree.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

btree_t *
btree_create()
{
	btree_t *tree;
	
	tree = malloc( sizeof( btree_t ) );
	
	tree->key[0]	= '\0';
	tree->index 	= NULL;
	tree->left 		= NULL;
	tree->right 	= NULL;
	
	return tree;
}

int 
btree_insert( btree_t **tree, char *key, void *data)
{
	int cmp;
	btree_t *curr;
	
	if( *tree == NULL )
	{
		*tree 			= malloc( sizeof( btree_t ) );
		(*tree)->left 	= NULL;
		(*tree)->right 	= NULL;
		
		(*tree)->index = data;
	}
	else
	{
		curr = *tree;
		while( !curr )
		{
			
			/* See if we create a node on the left or right side */
			cmp = strcmp( (*tree)->key, key );

			if( cmp == 0 )
			{
				printf( "Equal\n" );
			}
			else if ( cmp > 0 )
			{
				/* Higher */
			}
			else
			{
				/* Lower */
			}
		}
	
	}
	
	return 1;
}