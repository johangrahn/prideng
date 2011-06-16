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

    if( p == NULL )  {
        return -1;
    }

    freeaddrinfo(servinfo); 

    return connectSocket;

}
