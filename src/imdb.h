#ifndef __IMDB_H__
#define __IMDB_H__

#include <db.h>

typedef struct 
{
	DB_ENV 	*envp; 
	DB		*db;
} imdb_t;


/* Create the connection to the storage */ 
int
imdb_init( imdb_t *imdb );

/* Stores the data pointed out in data with a given size into the key */
int
imdb_store( imdb_t *imdb, char *key, void *data, size_t size );

/* Fetches the data pointed out by the key, allocates new memory for it and
 * stores it into the data pointer */
int 
imdb_fetch( imdb_t *imdb, char *key, void **data );

#endif
