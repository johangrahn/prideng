#ifndef __NETWORK_H_
#define __NETWORK_H_

#include "pack.h"

int 
net_create_tcp_socket( char *host, int port );

int
net_create_tcp_server( int port );

/* Sends the package to the specified socket */
int
net_send_pack( int socket,  void* data, int size );

#endif 

