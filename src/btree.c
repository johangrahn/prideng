#include "btree.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

btree_t *
btree_create(  )
{
	btree_t *tree;
	tree = malloc( sizeof( btree_t ) );
	
	tree->key[0]	= '\0';
	tree->index 	= -1;
	tree->left 		= NULL;
	tree->right 	= NULL;
	
	return tree;
}

int 
btree_insert( btree_t **tree, char *key, int index )
{
	int 		cmp;
	btree_t 	*curr;
	
	if( *tree == NULL )
	{
		*tree = btree_create();
		(*tree)->index = index;
	}
	else
	{
		curr = *tree;
		
		while( curr )
		{
			/* See if we create a node on the left or right side */
			cmp = strcmp( (*tree)->key, key );

			if ( cmp > 0 )
			{
				if( curr->right == NULL )
				{
					curr->right = btree_create();
					curr->right->index = index;
				
					return 1;
				}
			
				curr = curr->right;
			}
			else if( cmp < 0 )
			{
				if( curr->left == NULL )
				{
					curr->left = btree_create();
					curr->left->index = index;
				
					return 1;
				}
			
				curr = curr->left;
			}
		}	
	}
	
	return 1;
}