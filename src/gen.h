#ifndef __GEN_H_
#define __GEN_H_

#include "mc.h"

/* Enum that represents different type of 
 * update in a generation 
 */ 
typedef enum 
{
	GEN_UPDATE,
	GEN_NO_UP,
	GEN_NONE
} gen_update_t;

/* Structure for storing data about an update */
typedef struct
{
	mc_t data;
	gen_update_t type;
} gen_data_t;

typedef struct
{
	gen_data_t *data; /* Data for the given generation */
	int num; /* Generation number */

	int size; /* Stores how many replicas there are */
} gen_t;

/* Creates memory for the generation and the replica information */
gen_t *
gen_new( int replicas );

/* Copy all memory that have been allocated */
gen_t *
gen_copy( gen_t *g );


/* Reset all variables that have been used 
 * in the gen_t structure
 */
void 
gen_reset( gen_t *g );

/* Returns 1 if the generation is complete, 0 otherwise */
int 
gen_is_complete( gen_t *g );

/* Removes the memory allocated for the generation */
void
gen_free( gen_t *gen );

#endif

