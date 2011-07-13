#include "prop.h"
#include "png.h"
#include "network.h"
#include "pack.h"

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
	int 			it;
	int start_gen, start_pos, end_gen;
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
			start_gen = cs->prop_gen + 1;
			start_pos = cs->prop_pos + 1;
		}
		
		/* Fetch the newest generaton that have been inserted */
		end_gen = cs->max_gen;
	
		/* Create a propagation package to send */
		p_pack = pack_create_prop( end_gen - start_gen + 1 );
		p_pack->rep_id = png->id;
		p_pack->num_up = end_gen - start_gen + 1;
		
		int i;
		i = 0;
		for( it = start_gen; it <= end_gen; it++)
		{
			/* Fetch the generation */
			g = cs->gens[ it ];
		
			
			/* Check if it needs to be propagated 
			 * TODO: Add correct checkup 
			*/
			if( g->data->type != NONE || g->data->type != NO_UP )
			{
				p_pack->updates[i].gen = g->data->data.gen;
				strncpy( p_pack->updates[i].method_name, g->data->data.method_name, MC_METHOD_SIZE );
				i++;			
			}
			

			/* TODO: Increase with modulus function */
			start_pos++;
		}

		/* Correct the number of updates that are valid */
		p_pack->num_up = i;
		
		prop_fail = 0;
		
		for( it = 0; it < rlist->size; it++ )
		{
			rep_t *rep;
			int rep_sock;
			rep = &rlist->reps[it];

			printf( "[Prop Thread] Propagating generations to %s:%d\n",
					rlist->reps[it].host, 
					rlist->reps[it].port );

			/* Check if a connection already exists */
			if( rep->sock == -1 )
			{


				/* Connect to the replica */
				rep_sock = net_create_tcp_socket( rep->host, rep->port );

				if(	rep_sock == -1 )
				{
					printf( "Failed to connect to replica on %s:%d\n", rep->host, rep->port );
					prop_fail = 1;
					break;
				}
				else
				{
					rep->sock = rep_sock;
				}
			}


			/* Send the data to the replica */
			net_send_pack( rep->sock, (pack_t*)p_pack );

		}

		/* Remove the allocated package */
		free( p_pack );
	
		cs->prop_gen = end_gen;
		cs->prop_pos = start_pos;
		
	}
	
	pthread_exit( NULL );
}
