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
	cs->prop_gen = cs->prop_pos = -1;

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
	
	if( cs_is_full( cs ) )
	{
		return -1;
	}
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

int
cs_is_full( cs_t *cs )
{
	if( ( cs->max_gen - cs->min_gen ) < ( cs->num_gen ) )
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

gen_t *
cs_pop( cs_t *cs )
{
	gen_t *g;

	if( cs_is_empty( cs ) )
	{
		return NULL;
	}

	if( cs->min_gen == -1 )
	{
		cs->min_gen = 0;
		cs->min_pos = 0;
	}

	g = cs->gens[ cs->min_pos ];
	cs->min_gen++;
	cs->min_pos = cs_inc_pos( cs->min_pos, cs->num_gen );
	
	/* Return a copy of the data so that it 
	 * doesn't get overriten 
	 */
	return gen_copy( g );
}

void 
cs_show( cs_t *cs )
{
	int g_it, g_pos, rep_it;
	gen_t *g;
	if( cs_is_empty( cs ) )
	{
		printf( "Conflict set is empty\n");
	}
	else
	{

		/* Set the first position if the conflict set */
		if(cs->min_gen == -1 )
			g_pos = 0;
		else
			g_pos = cs->min_pos;

		printf( "Full: %s\n", cs_is_full( cs ) == 1 ? "Yes" : "No" );
		
		/* Iterate trough each generation */
		for( g_it = cs->min_gen; g_it < cs->max_gen; g_it++, g_pos++ )
		{
			g = cs->gens[ g_pos ];
			printf("%d:[ ", g->num );
		
			/* Iterate through all replica information 
			 * on the given generation */
			for( rep_it = 0; rep_it < g->size; rep_it++ )
			{
				printf("%d", g->data[rep_it] );
				
				if( rep_it != (g->size - 1 ) )
				{
					printf( " |");
				}
			}

			printf( " ]\n");
		}
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

