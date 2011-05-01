#ifndef __GEN_H_
#define __GEN_H_

typedef struct
{
	int *data; /* Data for the given generation */
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

/* Removes the memory allocated for the generation */
void
gen_free( gen_t *gen );

#endif

