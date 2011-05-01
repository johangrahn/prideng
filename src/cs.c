#include "cs.h"
#include <stdlib.h>
#include <stdio.h>


/* Increases the position of the generation pointer 
 * based on its current position and cs size */
int 
cs_inc_pos( int gen_pos, int cs_size );

cs_t *
cs_new( int gen_size, int replicas ) 
{
	cs_t *cs;
	int i;

	cs = malloc( sizeof( cs_t ) );

	/* Reset variables */
	cs->min_gen = cs->min_pos = -1;
	cs->max_gen = cs->max_pos = -1;

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

	/* Increase generation pointer to point to 
	 * the new generation 
	 */
	cs->max_pos = cs_inc_pos( cs->max_pos, cs->num_gen );
	cs->max_gen++;

	/* Fetch the current generation */
	g = cs->gens[ cs->max_pos ];

	/* Remove old data from the generation */
	gen_reset( g );

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
cs_show( cs_t *cs )
{
	if( cs_is_empty( cs ) )
	{
		printf( "Conflict set is empty\n");
	}
	else
	{
		printf( "[ ]");
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

int
cs_inc_pos( int gen_pos, int cs_size )
{
	return ( gen_pos + 1 ) % cs_size;
}

