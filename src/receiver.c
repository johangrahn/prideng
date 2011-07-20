#include "receiver.h"
#include "pack.h"
#include "png.h"
#include "network.h"

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

/* Sends stabilization message to the other replicas */
int
receiver_send_stab(int rep_id, int gen, png_t *png );

void *
receiver_thread( void *data )
{
	png_t 	*png;
	fd_set 	master, 
			read_fds;
	int		fdmax;
	int i;
	socklen_t addr_len;
	struct sockaddr_storage their_addr;
	
	int 	bytes;
	char 	buffer[2048];

	png = (png_t *)data;
	
	addr_len = sizeof( their_addr );
	
	FD_ZERO( &read_fds );
	FD_ZERO( &master );

	printf( "[Receiver Thread] Starting thread\n" );

	FD_SET( png->lsock, &master );
	fdmax = png->lsock;

	while( 1 )
	{
		read_fds = master;

		/* Listens to the sockets connected and see 
		 * when there is data there
		 */
		if( select( fdmax + 1, &read_fds, NULL, NULL, NULL) == -1 ) 
		{
			printf( "select: %s", strerror(errno) );
		}
		
		/*
		 * Iterate through each connection 
		 */
		for( i = 0; i <= fdmax; i++ )
		{
			if( FD_ISSET( i, &read_fds ) ) 
			{
				/* We have found a new connection */
				if( i == png->lsock )
				{
					int rt_sock;
					rt_sock = accept( png->lsock, (struct sockaddr *)&their_addr, &addr_len );

					if( rt_sock == -1 ) 
					{
						perror( "accept " );
					}
					else 
					{
						FD_SET( rt_sock, &master );
						if( rt_sock > fdmax ) {
							fdmax = rt_sock;
						}
						
						printf( "Got a connection!\n" );

						/*
						networkSetNonBlockingMode( remote_socket );
						*/
					}
				}
				else 
				{
					/* This is not a new connection, 
					 * we fetch data from it instead
					 */
					
					bytes = receiver_get_package( i, buffer );
					if( bytes > 0 )
					{
						/* Package have been received, do something with it */
						printf( "[Receiver Thread] %d bytes received\n", bytes );
						
						/* Integrate the package into the conflict set */
						receiver_process_pack( buffer, bytes, png );
					}
					else if( bytes == 0 )
					{
						/* No data on the socket, remove it from the master list */
						FD_CLR(i, &master );
					}
					else
					{
						printf( "[Receiver Thread] %s", strerror( errno ) );
					}
				}
			}
		}
	}

	return NULL;
}

int
receiver_get_package( int socket, char *buffer )
{
	int len,
		bytes, 
		received;

	/* Reads the package length from the package definition */
	if( recv( socket, &len, sizeof(len), MSG_PEEK ) == -1 )
	{
		printf( "Failed to read the length of the package\n" );
		return -1;
	}

	/* Try to fetch the full package from the stream */
	received = recv( socket, buffer, len, 0 );
	if( received == -1 )
	{
		printf( "Failed to read the package from the stream\n" );
		return -1;
	}

	/* Check if the full package have been retrieved, if not, fetch the rest */
	if( received < len )
	{
		while( received < len )
		{
			bytes = recv( socket, buffer + received, len, 0);
			if( bytes == -1 )
			{
				printf( "Failed to read a part of the package\n" );
				return -1;
			}

			/* Increase the counter for how many bytes we have fetched from the
			 * package 
			 */
			received += bytes;
		}
	}

	/* Returns the number of bytes that we have fetched */
	return len;
}

void
receiver_process_pack( char *data, size_t size, png_t *png )
{
	pack_t 		*pack;
	ppack_t 	*prop_pack;
	spack_t		*spack;
	cs_t 		*cs;
	int			it;

	if( size < sizeof( pack_t ) )
	{
		printf( "[Receiver Thread] Package data is to smal, ignoring the package\n" );
		return;
	}

	/* Convert the data to a correct package structure so that we can detect
	 * the package type */
	pack = (pack_t*) data;

	/* Fetch conflict set that is affected */
	cs = png->cs;
			

	switch( pack->type )
	{
		case PROPAGATION:
			prop_pack = (ppack_t*) data; 
			printf( "Detected a propagation package from replica %d with %d updates \n", prop_pack->rep_id, prop_pack->num_up );	
			
			cs_lock( cs );	
					
			for( it = 0; it < prop_pack->num_up; it++ )
			{
				/* If the insert adds generations, we need to send
				 * stabilization messages to the other replicas 
				 */
				
				if( cs_insert_remote( cs, &prop_pack->updates[it], prop_pack->rep_id, png->id ) )
				{
					receiver_send_stab( png->id, prop_pack->updates[it].gen, png );
				}
			}
			
			cs_unlock( cs );
			
		break;

		case STABILIZATION:
			spack = (spack_t*) data;
			
			printf("Detected a stabilization package from replica %d on generation %d\n", spack->rep_id, spack->gen );	
			cs_lock( cs );
			cs_set_stab( cs, spack->rep_id, spack->gen );
			cs_unlock( cs );	
		break;
	}
}

int
receiver_send_stab( int rep_id, int gen, png_t *png )
{
	int 		it;
	spack_t 	pack;
	rep_t 		*rep;
	
	pack.size = sizeof( spack_t );
	pack.type = STABILIZATION;
	pack.gen = gen;
	pack.rep_id = rep_id;

	for( it = 0; it < png->rlist.size; it++ )
	{
		rep = &png->rlist.reps[it];

		printf( "[Receiver Thread] Sending stabilization message to %s:%d\n",
				png->rlist.reps[it].host, 
				png->rlist.reps[it].port );

		
		
		/* Send the data to the replica */
		net_send_pack( rep->sock, (pack_t*)&pack );

	}

	return 1;

}
