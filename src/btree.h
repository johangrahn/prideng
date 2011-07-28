#ifndef __BTREE_H__
#define __BTREE_H__


#define BTREE_KEY_MAX 40

typedef struct btree btree_t;

struct btree
{
	/* The searchable key */
	char 	key[BTREE_KEY_MAX];
	
	/* Index pointer to the external data storage */
	int 	index;
	
	
	/* Pointer to left and right directions */
	btree_t *left,
			*right;
};

/* Inserts the index data with the given key into the tree */
int 
btree_insert( btree_t **tree, char *key, int index );

/* Returns the index from the given key */
int 
btree_search( btree_t *tree, char *key );
	
#endif
