#include "btree.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

btree_t *
btree_create( char *key  )
{
	btree_t *tree;
	tree = malloc( sizeof( btree_t ) );
	
	strcpy( tree->key, key );
	tree->index 	= -1;
	tree->left 		= NULL;
	tree->right 	= NULL;
	
	return tree;
}

btree_t * 
btree_insert( btree_t *tree, char *key, int index )
{
	int 		cmp;
	btree_t 	*curr,
				*t;
	
	if( tree == NULL )
	{
		t = btree_create( key );
		t->index = index;
		t->left = NULL;
		t->right = NULL;
		return t;
	}
	else
	{
		curr = tree;
		
		while( curr )
		{
			/* See if we create a node on the left or right side */
			cmp = strcmp( tree->key, key );

			if ( cmp > 0 )
			{
				if( curr->right == NULL )
				{
					curr->right = btree_create( key );
					curr->right->index = index;
				
					return tree;
				}
			
				curr = curr->right;
			}
			else if( cmp < 0 )
			{
				if( curr->left == NULL )
				{
					curr->left = btree_create( key );
					curr->left->index = index;
				
					return tree;
				}
			
				curr = curr->left;
			}
		}	
	}
	
	return tree;
}

int 
btree_search( btree_t *tree, char *key )
{
	btree_t *curr;
	int 	cmp;
	
	curr = tree;
	
	while( curr )
	{
		cmp = strcmp( curr->key, key );
		
		if( cmp == 0)
		{
			return curr->index;
		}
		else if( cmp < 0 )
		{
			curr = curr->right;
		}
		else
		{
			curr = curr->left;
		}
	}
	
	return -1;
}
