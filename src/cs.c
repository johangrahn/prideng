#include "cs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* 
 * Creates a new generation at the top
 */
gen_t *
cs_create_gen( cs_t *cs );

/* Returns the position in the array where the given generation exists */
int 
cs_get_pos( cs_t *cs, int gen );


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
cs_insert( cs_t *cs, mc_t *up, int rep_id )
{
	gen_t *g;
	
	if( cs_is_full( cs ) )
	{
		return -1;
	}
	
	g = cs_create_gen( cs );

	up->gen = g->num;
	g->data[rep_id].data = *up;
	strncpy( g->data[rep_id].data.method_name, up->method_name, MC_METHOD_SIZE );
	g->data[rep_id].type = GEN_UPDATE;


	printf( "Inserted <%s> into generation %d\n", up->method_name, up->gen );
	return 1;

}

int 
cs_insert_remote( cs_t *cs, mc_t *up, int rep_id, int own_id)
{
	int 	g_pos;
	gen_t 	*g;

	if( cs_is_empty( cs ) || up->gen > cs->max_gen )
	{
		/* Create generation until the proper generation for the remote update
		 * have been created and inserted */
		while( 1 )
		{
			g = cs_create_gen( cs );
		
			/* Check that we are at the generation that is retreived */
			if( g->num == up->gen )
			{	
				g->data[rep_id].data = *up;
				strncpy(g->data[rep_id].data.method_name, up->method_name, MC_METHOD_SIZE );
				g->data[rep_id].type = GEN_UPDATE;
				
				g->data[own_id].type = GEN_NO_UP;

				/* No need to create any more generations */
				break;
			}
			else
			{
				g->data[rep_id].type = GEN_NO_UP;
				g->data[own_id].type = GEN_NO_UP;
			}
		}
		
		printf( "Inserting data into generation %d\n", g->num );	

		return 1;
	}
	else
	{
		printf( "Conflict set not empty, inserting data...\n" );

		/* Find the generation position where the update is going to be stored
		 * into */
		g_pos = cs_get_pos( cs, up->gen  );
		if( g_pos == -1 )
		{
			printf( "Failed to find the position for generation %d in cs_insert_remote()", up->gen );
		}
		
		printf( "Found position: %d", g_pos );	
		g = cs->gens[g_pos];

		printf( "Inserting data into generation %d\n", g->num );	
	}

	return 0;
}

void
cs_set_stab( cs_t *cs, int rep_id, int gen )
{
	int g_pos;

	g_pos = cs_get_pos( cs, gen );
	if( g_pos != -1 )
	{
		gen_t *g;

		g = cs->gens[g_pos];
		
		/* Check if there have been any updates, if not, set to no update */
		if( g->data[rep_id].type == GEN_NONE )
		{
			g->data[rep_id].type = GEN_NO_UP;
		}
		
		printf( "Updated stabilization info on gen %d on replica %d\n", gen, rep_id );
	}
	else
	{
		printf( "Failed to fetch generation position for cs_set_stab() for gen %d \n", gen );
	}
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
	if( ( cs->max_gen - cs->min_gen + 1 ) <  cs->num_gen  )
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
	
	/* Check so that the generation have already been propagated, 
	 * or it is a remote update */
	if( g->num <= cs->prop_gen )
	{	
		cs->min_gen++;
		cs->min_pos = cs_inc_pos( cs->min_pos, cs->num_gen );

		/* Return a copy of the data so that it 
		 * doesn't get overriten 
		 */
		return gen_copy( g );
	}
	else
	{
		printf( "Failed to stablize, generation %d is not propagated\n ", g->num );
		
		return NULL;
	}
	
	

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
		printf( "Propagated: %d\n", cs->prop_gen );
		
		/* Iterate trough each generation */
		for( g_it = cs->min_gen; g_it <= cs->max_gen; g_it++, g_pos++ )
		{
			g = cs->gens[ g_pos ];
			printf("%d:[ ", g->num );
		
			/* Iterate through all replica information 
			 * on the given generation */
			for( rep_it = 0; rep_it < g->size; rep_it++ )
			{
				switch( g->data[rep_it].type )
				{
					case GEN_UPDATE:
						printf(" U");
					break;
					case GEN_NO_UP:
						printf(" O");
					break;

					case GEN_NONE:
						printf(" N");
					break;
				}
				
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

gen_t *
cs_create_gen( cs_t *cs )
{
	/* Check if there is any generations previously created */
	if( cs->max_pos == -1 )
	{
		cs->max_pos = 0;
		cs->min_pos = 0;
		cs->max_gen = 0;
		cs->min_gen = 0;
	}
	else
	{
		/* Just create a new position at the highest generation */
		cs->max_pos = cs_inc_pos( cs->max_pos, cs->num_gen ); 	
		cs->max_gen++;
	}

	gen_reset( cs->gens[cs->max_pos] );
	cs->gens[cs->max_pos]->num = cs->max_gen;


	return cs->gens[cs->max_pos];
}

int 
cs_get_pos( cs_t *cs, int gen )
{
	/* Check if the generation exists in the array */
	if( gen < cs->min_gen || gen > cs->max_gen )
	{
		return -1;
	}
	
	/* increase the pos with the difference between the min and current
	 * generation */
	return (cs->min_pos + gen - cs->min_gen) % cs->num_gen;
}
