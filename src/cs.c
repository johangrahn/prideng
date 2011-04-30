#include "cs.h"
#include <stdlib.h>
#include <stdio.h>

cs_t *
cs_new( int gen_size, int replicas ) 
{
	cs_t *cs;
	int i;

	cs = malloc( sizeof( cs_t ) );

	cs->max_gen = cs->min_gen = -1;
	
	/* Create space for the generations */
	cs->gens = malloc( sizeof( gen_t ) * gen_size );
	
	/* Create memory for each replica in each generation */
	for( i = 0; i < gen_size; i++ )
	{
		cs->gens[i] = gen_new( replicas );
	}

	cs->num_gen = gen_size;
	return cs;
}

int 
cs_insert( cs_t *cs, int up )
{
	gen_t *g;

	cs->max_gen++;

	/* Fetch the current generation */
	g = cs->gens[ cs->max_gen ];
	
	g->data[0] = up;

	/* Associate the generation number to the generation */
	g->num = cs->max_gen;

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
	int i;

	/* Remove the memory from each generation */
	for( i = 0; i < cs->num_gen; i++ )
	{
		gen_free( cs->gens[i] );
	}

	free( cs->gens );
	free( cs );
	cs = NULL;
}
