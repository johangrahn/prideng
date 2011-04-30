#ifndef __GEN_H_
#define __GEN_H_

typedef struct
{
	int *data; /* Data for the given generation */
	int num; /* Generation number */
} gen_t;

/* Creates memory for the generation and the replica information */
gen_t *
gen_new( int replicas );

#endif

