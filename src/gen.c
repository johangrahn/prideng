#include "gen.h"

#include <stdlib.h>

gen_t *
gen_new( int replicas )
{
	gen_t *g;

	g = malloc( sizeof( gen_t ) );
	g->data = malloc( sizeof( gen_data_t ) * replicas );
	
	g->size = replicas;

	gen_reset( g );

	return g;
}

void 
gen_reset( gen_t *g )
{
	int i;

	g->num = -1;

	for( i = 0; i < g->size; i++ )
	{
		g->data[i].type = GEN_NONE;
		mc_init( &g->data[i].data );
	}
}

gen_t *
gen_copy( gen_t *g )
{
	gen_t *g_new;
	int it;

	g_new = malloc( sizeof( gen_t ) );

	g_new->size = g->size;
	g_new->num = g->num;

	g_new->data = malloc(sizeof( gen_data_t ) * g->size );
	
	for( it = 0; it < g->size; it++ )
	{
		g_new->data[it] = g->data[it];
		mc_copy( &g->data[it].data , &g_new->data[it].data );
	}

	return g_new;
}

void
gen_free( gen_t *gen )
{
	free( gen->data );
	free( gen );
	gen = NULL;
}
