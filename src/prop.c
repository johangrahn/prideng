#include "prop.h"
#include "png.h"

#include <stdio.h>
#include <pthread.h>

void* 
prop_thread( void* data )
{
	pthread_cond_t *prop_sig;
	pthread_mutex_t *prop_sig_lock;
	cs_t 			*cs;
	gen_t 			*g;
	int 			it;
	int start_gen, start_pos, end_gen;
	
	prop_sig = ((png_t*)data)->prop_sig;
	prop_sig_lock = ((png_t*)data)->prop_sig_lock;
	cs = ((png_t*)data)->cs;

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
	
		for( it = start_gen; it <= end_gen; it++ )
		{
			/* Fetch the generation */
			g = cs->gens[ it ];
		
			/* Check if it needs to be propagated 
			 * TODO: Add correct checkup 
			*/
			if( g->data->data != -1 )
			{
				printf( "[Prop Thread] Propagating generation %d\n", g->num );
			}
		
			/* TODO: Increase with modulus function */
			start_pos++;
		}
		
		cs->prop_gen = end_gen;
		cs->prop_pos = start_pos;
		
	}
	
	pthread_exit( NULL );
}
