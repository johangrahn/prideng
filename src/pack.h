#ifndef __PACK_H_
#define __PACK_H_

typedef enum 
{
	prop
} pack_type

typedef struct 
{
	int 		size;
	pack_type 	type;
	
	int 		rep_id;
	int 		num_up;
	int 		updates[];
	
} prop_pack_t

/* Allocates memory for a new propagation package 
 * based on the number of updates that needs 
 * to be send 
 */
prop_pack_t *
pack_create_prop(int size);

#endif

