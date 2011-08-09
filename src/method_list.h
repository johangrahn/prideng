#ifndef __METHOD_LIST_H__
#define __METHOD_LIST_H__

#include "btree.h"
#include "object.h"

/* Prototype for all methods that are replicated */
typedef void (*method_prototype)( void*, param_t*, int );

typedef struct 
{
	/* Stores the key and the index to the function pointer */
	btree_t 			*tree; 
	
	/* Stores the function pointers */
	method_prototype 	methods[10];
	
	/* Points to the current position in the static array that is used */
	int 				curr; 
} method_list_t;

/* Sets default values to the structure */
void 
method_list_init( method_list_t *list );

void
method_list_insert( method_list_t *list, char *key, method_prototype method );

/* Finds the method for the given key */
method_prototype
method_list_find( method_list_t *list, char *key );

void
method_list_free( method_list_t *list );

#endif 
