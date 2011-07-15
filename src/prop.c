#include "prop.h"
#include "png.h"
#include "network.h"
#include "pack.h"
#include "cs.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void* 
prop_thread( void* data )
{
	pthread_cond_t *prop_sig;
	pthread_mutex_t *prop_sig_lock;
	cs_t 			*cs;
	gen_t 			*g;
	png_t 			*png;
	int 			it,
					i;
	int start_gen, start_pos, end_gen, end_pos;
	int				prop_fail;
	rep_list_t		*rlist;
	ppack_t			*p_pack;
	prop_sig 		= ((png_t*)data)->prop_sig;
	prop_sig_lock 	= ((png_t*)data)->prop_sig_lock;
	cs 				= ((png_t*)data)->cs;
	rlist 			= &((png_t*)data)->rlist;
		
	
	png = (png_t*) data;

	printf( "[Prop Thread] Thread started\n" );

	while( 1 )
	{
		/* locks the signal variable and waits */
		pthread_mutex_lock( prop_sig_lock );
		pthread_cond_wait( prop_sig, prop_sig_lock  );
		pthread_mutex_unlock( prop_sig_lock );

		printf( "[Prop Thread] Recevied signal \n" );

		/* Check if propagation has already been performed */
		if( cs->prop_gen == -1 )
		{	
			/* Start at the begining */
			start_gen = 0;
			start_pos = 0;
		
		}	
		else if( cs->prop_gen == cs->max_gen )
		{
			/* No propagation is needed */
			printf( "[Prop Thread] No propagation is needed\n" );
			continue; 
		}
		else
		{
			start_gen = cs->prop_gen;
			start_pos = cs->prop_pos;
		}
		
		/* Fetch the newest generaton that have been inserted */
		end_gen = cs->max_gen;
		end_pos = cs->max_pos;
	
		/* Create a propagation package to send */
		p_pack = pack_create_prop( end_gen - start_gen + 1 );
		p_pack->rep_id = png->id;
		p_pack->num_up = end_gen - start_gen + 1;
		
		printf( "Creating prop package for %d updates\n", end_gen - start_gen + 1);
		
		i = 0;

		/* Fetches each generation that needs to be propagated. It check if the
		 * generation has the UPDATE status, otherwise it should not be
		 * propagated
		 */
		
		printf( "start_pos: %d\n", start_pos );
		printf( "end_pos: %d\n", end_pos );
		for( it = start_pos; 
				it <= end_pos; 
				it = cs_inc_pos( it, cs->num_gen ) )
		{
			printf( "Running\n");
			/* Fetch the generation */
			g = cs->gens[ it ];
			
			/* Check if it needs to be propagated */
			if( g->data[png->id].type != GEN_NONE && 
				g->data[png->id].type != GEN_NO_UP )
			{
				p_pack->updates[i].gen = g->data[png->id].data.gen;
				strncpy( p_pack->updates[i].method_name, g->data[png->id].data.method_name, MC_METHOD_SIZE );
				/* Increase the array pointer to the package struct only when
				 * we find a generation that needs propagation
				 */
				i++;
				
				printf( "Added update in package\n" );			
			}
			

		}

		cs->prop_gen = end_gen;
		cs->prop_pos = it;
		
		
		if( i == 0 )
		{
			printf( "No generations needs to be propagated!\n" );
			continue;
		}
		
		/* Correct the number of updates that are valid */
		p_pack->num_up = i;
		
		printf( "The corrected number of updates is: %d\n", i );
		
		prop_fail = 0;
		
		for( it = 0; it < rlist->size; it++ )
		{
			rep_t *rep;
			rep = &rlist->reps[it];

			printf( "[Prop Thread] Propagating generations to %s:%d\n",
					rlist->reps[it].host, 
					rlist->reps[it].port );

		

			/* Send the data to the replica */
			net_send_pack( rep->sock, (pack_t*)p_pack );

		}

		/* Remove the allocated package */
		free( p_pack );
	
		
	}
	
	pthread_exit( NULL );
}
