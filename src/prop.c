#include "prop.h"

#include <stdio.h>
#include <pthread.h>

void* 
prop_thread( void* data )
{
	printf( "[Prop Thread] Thread started\n" );

	
	pthread_exit( NULL );
}
