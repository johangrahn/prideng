#ifndef __PACK_H_
#define __PACK_H_

#include "mc.h"
/*
 * Represents the different types of packages that can be 
 * send to a replica 
 */
typedef enum 
{
	PROPAGATION
} pack_type;

typedef struct
{
	int 		size;
	pack_type 	type;

} pack_t;

typedef struct 
{
	int 		size;
	pack_type 	type;
	
	int 		rep_id;
	int 		num_up;
	mc_t 		updates[1];
	
} ppack_t;

/* Allocates memory for a new propagation package 
 * based on the number of updates that needs 
 * to be send 
 */
ppack_t *
pack_create_prop(int size);

#endif

