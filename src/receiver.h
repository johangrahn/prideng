#ifndef __RECEIVER_H_
#define __RECEIVER_H_

#include <stdlib.h>
#include "png.h"


/* 
 * Main receiver thread that receives data from the 
 * other replicas 
 */

void *
receiver_thread( void *data );

/* Fetches the package in the socket into the buffer var.
 * 
 * Returns the number of bytes that have been fetched. If nothing where
 * fetched, the functions returns 0
 */
int
receiver_get_package( int socket, char *buffer );

/* Takes the package and processes it into the conflict set */
void
receiver_process_pack( char *data, size_t size, png_t *png );

#endif 


