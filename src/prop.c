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
	int 			prop_gen;
	int 			prop_pos;

	prop_sig = ((png_t*)data)->prop_sig;
	prop_sig_lock = ((png_t*)data)->prop_sig_lock;
	cs = ((png_t*)data)->cs;

	printf( "[Prop Thread] Thread started\n" );

	/* locks the signal variable and waits */
	pthread_mutex_lock( prop_sig_lock );
	pthread_cond_wait( prop_sig, prop_sig_lock  );
	pthread_mutex_unlock( prop_sig_lock );

	printf( "[Prop Thread] Recevied signal \n" );

	

	pthread_exit( NULL );
}
