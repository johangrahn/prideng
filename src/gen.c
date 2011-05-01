#include "gen.h"

#include <stdlib.h>

gen_t *
gen_new( int replicas )
{
	gen_t *g;

	g = malloc( sizeof( gen_t ) );
	g->data = malloc( sizeof(int) * replicas );
	
	g->size = replicas;

	return g;
}

void 
gen_reset( gen_t *g )
{
	int i;

	g->num = -1;

	for( i = 0; i < g->size; i++ )
	{
		g->data[i] = -1;
	}
}

void
gen_free( gen_t *gen )
{
	free( gen->data );
	free( gen );
	gen = NULL;
}
