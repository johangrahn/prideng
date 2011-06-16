#ifndef __NETWORK_H_
#define __NETWORK_H_

int 
net_create_tcp_socket( char *host, int port );

int
net_create_tcp_server( int port );

#endif 
