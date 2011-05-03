#include "cs.h"
#include "prop.h"
#include "png.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

void 
png_show_prompt();

int
png_handle_cmd( png_t *png, char *cmd );

int main( int argc, char **argv )
{
	char 		cmd[40];
	int 		res;
	png_t 		png;
	pthread_t 	p_thread;
	
	/* Signal that happends when conflict set 
	 * needs to propagate a generation 
	 */
	pthread_cond_t prop_signal;
	pthread_mutex_t prop_sig_lock;

	pthread_cond_init( &prop_signal, NULL );
	pthread_mutex_init( &prop_sig_lock, NULL );


	/* Create confligt set that will be used in
	 * the application 
	 */
	png.cs = cs_new( 10, 1 );
	png.prop_sig = &prop_signal;
	png.prop_sig_lock = &prop_sig_lock;

	/* Create threads for propagation, 
	 * stabilization and conflict resolution 
	 */
	pthread_create( &p_thread, NULL, prop_thread, &png );
	

	/* Start taking in user commands */
	while( 1 )
	{

		png_show_prompt();

		/* Get the command from the prompt */
		fgets( cmd, 40, stdin );
		cmd[ strlen( cmd ) -1 ] = '\0';

		res = png_handle_cmd( &png, cmd );

		/* Check if the quit command was issued */
		if( res == 0 )
		{
			break;
		}

	}

	/* Stops the propagation thread */
	pthread_cancel( p_thread );

	cs_free( png.cs );
	return 0;
}

void png_show_prompt()
{
	printf( "Command> ");
}

int png_handle_cmd( png_t *png,  char *cmd )
{
	char *curr_cmd;

	curr_cmd = strtok( cmd, " ");
		
	/* Check what command that was issued */
	if( strcmp( curr_cmd, "q" ) == 0 ) 
	{
		printf( "Quiting..\n" );
		return 0;
	}
	else if( strcmp( curr_cmd, "add" ) == 0 )
	{
		int num_updates, it;
		char *updates;
		
		updates = strtok( NULL, " " );
		if( updates != NULL)
			num_updates = atoi( updates );
		else
			num_updates = 1;

		for( it = 0; it < num_updates; it++ )
		{
			if( cs_insert( png->cs, 3 ) == -1 )
			{
				printf( "Conflict set is full!" );
				break;
			}
		}
		return 1;
	}
	else if( strcmp( curr_cmd, "prop") == 0 )
	{
		pthread_mutex_lock( png->prop_sig_lock );
		pthread_cond_signal( png->prop_sig );
		pthread_mutex_unlock( png->prop_sig_lock );

		return 1;

	}
	else if( strcmp( curr_cmd, "s") == 0 )
	{
		cs_show( png->cs );

		return 1;
	}
	else if( strcmp( curr_cmd, "stab") == 0)
	{
		gen_t *g;

		/* Fetches the oldest generation */
		g = cs_pop( png->cs );
		if( g != NULL )
		{
			printf( "Generation %d stabilized\n", g->num );
			gen_free( g );
		}
		else
		{
			printf( "No generation to stabilize is found\n");
		}

		return 1;

	}
	else
	{
		return -1;
	}
}
