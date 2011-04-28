#ifndef __CS_H_
#define __CS_H_

typedef struct c 
{
	int min_gen;
	int max_gen;
} cs_t;

/* Creates a new conflict set with a defined size */
cs_t *
cs_new( int size ); 

#endif

