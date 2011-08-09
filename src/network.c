#include "network.h"

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
#include <stdio.h>
#include <time.h>

int 
net_create_tcp_socket( char *host, int port )
{

    int             connectSocket, rv;
    struct addrinfo hints,
                    *servinfo,
                    *p;
    char            port_str[10];


    memset( &hints, 0, sizeof hints );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; 

    snprintf( port_str, sizeof(port_str), "%d", port );

    if ((rv = getaddrinfo( host, port_str, &hints, &servinfo)) != 0) {
         fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
         return -1;
    }

     /* loop through all the results and bind to the first we can */
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((connectSocket = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        /*
        if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }
        */

        if (connect( connectSocket, p->ai_addr, p->ai_addrlen) == -1) {
            close( connectSocket );
            /*__DEBUG( "network_create_tcp_socket: %s", strerror(errno)); */
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); 
    
	if( p == NULL )  {
        return -1;
    }


    return connectSocket;

}

int 
net_create_tcp_server( int port )
{

	int             listenSocket;
    struct addrinfo hints,
                    *servinfo,
                    *p;
    char            port_str[10];
    int             rv,
                    yes=1;


    memset( &hints, 0, sizeof hints );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; 

    snprintf( port_str, sizeof(port_str), "%d", port );
    if ((rv = getaddrinfo(NULL, port_str, &hints, &servinfo)) != 0) {
         fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
         exit( 1 );
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((listenSocket = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

		
        if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }
		
		
        if (bind(listenSocket, p->ai_addr, p->ai_addrlen) == -1) {
            close( listenSocket );
            /* __DEBUG( "bind: %s", strerror(errno)); */
            continue;
        }
		
        break;
    }

    if( p == NULL )  {
        printf( "Failed to bind to port: %s", port_str );
        return -1;
    }
    
    freeaddrinfo(servinfo); 



    if (listen(listenSocket, 10) == -1) {
        perror("listen");
        return -1;
    }


	return listenSocket;
}

int
net_send_pack( int socket, void *data, int size )
{
	ppack_t *p_pack;
	int sent,
		left, 
		total_sent,
		plen;

	p_pack = (ppack_t*) data;
	total_sent = 0;
	left = plen = size;
	
	/* Tries to send the full package to the socket. If only a part of it is
	 * send, we send the rest until the full package have been send
	 */
	while( total_sent < plen )
	{
		sent = send( socket, p_pack + total_sent, left, 0 );
		if( sent == -1 )
		{
			printf( "Failed to send %d bytes\n", left );
			continue;
		}
		
		total_sent += sent;
		left -= sent;
	}

	return 1;
}
