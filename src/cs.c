#include "cs.h"
#include <stdlib.h>
#include <stdio.h>
cs_t *
cs_new( int size )
{
	cs_t *cs;

	cs = malloc( sizeof( cs_t ) );

	cs->max_gen = cs->min_gen = -1;
	cs->gens = malloc( sizeof(gen_t) * size );

	return cs;
}

int 
cs_insert( cs_t *cs, int up )
{
	gen_t g;
	g.data = up;
	cs->max_gen++;
	g.num = cs->max_gen;
	cs->gens[ cs->max_gen ] = g;

	printf( "Inserted %d into generation %d\n", up, cs->max_gen );
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

void 
cs_free( cs_t *cs )
{
	free( cs->gens );
	free( cs );
	cs = NULL;
}
