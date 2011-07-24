#ifndef __BTREE_H__
#define __BTREE_H__


#define BTREE_KEY_MAX 40

typedef struct btree btree_t;

struct btree
{
	/* The searchable key */
	char 	key[BTREE_KEY_MAX];
	
	/* Index pointer to the external data storage */
	void 	*index;
	
	
	/* Pointer to left and right directions */
	btree_t *left,
			*right;
};

/* Allocates memory for the first position */
btree_t *
btree_create( );

/* Inserts the data with the given key into the tree */
int 
btree_insert( btree_t **tree, char *key, void *data );

#endif
