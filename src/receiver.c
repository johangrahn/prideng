#include "receiver.h"

#include "png.h"

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
			}
		}
	}

	return NULL;
}
