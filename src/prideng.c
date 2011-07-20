#include "cs.h"
#include "prop.h"
#include "png.h"
#include "network.h"
#include "receiver.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <getopt.h>
#include <unistd.h>



void 
png_show_prompt();

int
png_handle_cmd( png_t *png, char *cmd );

int 
png_handle_args( png_t *png, int argc, char **argv );

int main( int argc, char **argv )
{
	char 		cmd[40];
	int 		res;
	int			lsock;
	int 		it;

	pthread_t 	p_thread,
				r_thread;

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
	__conf.cs = cs_new( 10, 2 );
	__conf.prop_sig = &prop_signal;
	__conf.prop_sig_lock = &prop_sig_lock;
	rep_list_init( &__conf.rlist );

	png_handle_args( &__conf, argc, argv );

	if( __conf.id == -1 )
	{
		printf( "You must set a replica id!\n" );
		exit( 1 );
	}

	if( __conf.lport != -1 )
	{
		/* Create a TCP server socket */
		lsock = net_create_tcp_server( __conf.lport );
		__conf.lsock = lsock;
	}
	else
	{
		printf( "You must set a listen port!\n" );
		exit( 1 );
	}

	/* Create threads for propagation, 
	 * and receving  
	 */
	pthread_create( &p_thread, NULL, prop_thread, &__conf );
	pthread_create( &r_thread, NULL, receiver_thread, &__conf );

	for( it = 0; it < __conf.rlist.size; it++ )
	{
		int rep_sock;
		rep_t *rep;

		rep = &__conf.rlist.reps[it];
		while( 1 )
		{
			/* Connect to the replica */
			rep_sock = net_create_tcp_socket( rep->host, rep->port );

			if(	rep_sock == -1 )
			{
				printf( "Failed to connect to replica on %s:%d\n", rep->host, rep->port );
				sleep( 1 );

			}
			else
			{
				rep->sock = rep_sock;
				break;
			}
		}
	}
		

	/* Start taking in user commands */
	while( 1 )
	{

		png_show_prompt();

		/* Get the command from the prompt */
		fgets( cmd, 40, stdin );
		cmd[ strlen( cmd ) -1 ] = '\0';

		res = png_handle_cmd( &__conf, cmd );

		/* Check if the quit command was issued */
		if( res == 0 )
		{
			break;
		}

	}

	/* Stops the propagation thread */
	pthread_cancel( p_thread );
	pthread_join( p_thread, NULL );

	pthread_cond_destroy( &prop_signal );
	pthread_mutex_destroy( &prop_sig_lock );
	
	cs_free( __conf.cs );
	rep_list_free( &__conf.rlist );
	return 0;
}

void png_show_prompt()
{
	printf( "Command> ");
}

int png_handle_cmd( png_t *png,  char *cmd )
{
	char *curr_cmd;
	cs_t *cs;
	
	curr_cmd = strtok( cmd, " " );
		
	if( curr_cmd == NULL )
		return 1;

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
		/*	
		cs = cs_create_trans_obj( png->cs );
		*/
		for( it = 0; it < num_updates; it++ )
		{
			mc_t update;
			strcpy( update.dboid, "id" );
			strcpy(update.method_name, "metod" );
			update.num_param 				= 1;
			update.params[0].type 			= TYPE_INT;
			update.params[0].data.int_data 	= 1;
				
			cs_lock( png->cs );
			
			if( cs_insert( png->cs, &update, png->id ) == -1 )
			{
				printf( "Conflict set is full!\n" );
				break;
			}
			
			cs_unlock( png->cs );
		}
		
		cs_unlock( png->cs );
		
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
	
	else if( strcmp( curr_cmd, "rep" ) == 0 )
	{
		rep_t rep;
		char *host;
		int id;
		int port;
	
		id = atoi( strtok( NULL, " " ) );
		host = strtok( NULL, " " );
		port = atoi( strtok( NULL, " " ) );
	
		rep.id = id;
		strncpy( rep.host, host, strlen( host ) + 1 );
		rep.port = port;

		/* 
		 * Reseting the socket so that we know that there
		 * is no connection at first
		 */
		rep.sock = -1;
	
		
		/* Add the replica to the replica list */
		rep_list_add( &png->rlist, &rep );
		
		printf( "Replica %d:%s:%d was added\n", rep.id, rep.host, rep.port );

	
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

int 
png_handle_args(png_t *png, int argc, char **argv )
{
	char *filename, line[50];
	FILE *fp;
	int arg;
	
	png->lport = -1;
	png->id = -1;
	png->lsock = -1;

	while( ( arg = getopt( argc, argv, "c:l:r:") ) != -1 )
	{
		switch( arg )
		{
			case 'c':
			{
				filename = optarg;

				printf( "Reading data from file %s\n", filename );
				
				fp = fopen( filename, "r" );
				while( fgets( line, 50, fp ) )
				{

					png_handle_cmd( png, line );
				}

				fclose( fp );
			}
			break;
			
			case 'r':
				png->id = atoi( optarg );
			break;

			case 'l':
				
				printf( "Listens for connections on port %s\n", optarg );
				png->lport = atoi( optarg );
			break;
		}
	}
	
	return 1;
	
}
