#include "cs.h"
#include <stdlib.h>
#include <stdio.h>
cs_t *
cs_new( int size )
{
	cs_t *cs;

	cs = malloc( sizeof( cs_t ) );

	cs->max_gen = cs->min_gen = -1;

	return cs;
}

int 
cs_insert( cs_t *cs, int up )
{
	/* TODO: Check if conflict set is empty */


	printf( "Inserting update %d", up );

	return 1;

}
