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
	printf( "Inserting update %d by creating generation 0\n", up );
	
	cs->min_gen = cs->max_gen = 0;
	return 1;

}

int
cs_is_empty( cs_t *cs )
{
	if( cs->max_gen == -1 || 
		cs->min_gen > cs->max_gen )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
