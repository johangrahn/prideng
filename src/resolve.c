#include "resolve.h"
#include "png.h"

#include <stdlib.h>

void* 
resolve_thread( void *data )
{
	png_t *conf;
	gen_t *g;
	
	conf = (png_t*) data;
	
	printf( "[Resolve Thread] Started.\n" );
	
	while( 1 )
	{
		/* locks the signal variable and wait */
		pthread_mutex_lock( conf->resolve_sig_lock );
		pthread_cond_wait( conf->resolve_sig, conf->resolve_sig_lock  );
		pthread_mutex_unlock( conf->resolve_sig_lock );
		
		/* Fetch a generation that needs to be resolved */
		g = cs_pop( conf->cs );
		if( g != NULL )
		{
			/* Get objet from the storage */
			
			/* Perform the update on the object */
			
			/* Put the object back to the store */
		}		
	}
	
	return NULL;
}
