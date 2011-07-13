#ifndef __CS_H_
#define __CS_H_

#include "gen.h"
#include "mc.h"

typedef struct c 
{

	int min_gen, min_pos;
	int max_gen, max_pos;
	int prop_gen, prop_pos;

	gen_t **gens;
	
	int num_gen;
} cs_t;

/* Creates a new conflict set with a defined 
 * number of generations and replicas */
cs_t *
cs_new( int gen_size, int replicas ); 

/* Insert the update defined in up var
 *
 * If there are no room in the conflict set, -1 is returned 
 */
int 
cs_insert( cs_t *cs, mc_t *up, int rep_id );

/* Stores the propagated update from another replica into the conflict set */
int 
cs_insert_remote(cs_t *cs, mc_t *up, int rep_id, int own_id);

/* 
 * Returns 1 if the conflict set is empty
 */
int
cs_is_empty( cs_t *cs );

/*
 * Returns 1 if the conflict set is full 
 */
int
cs_is_full( cs_t *cs );

/* Fetches the oldest generation from the conflict set
 *
 * If no generation is availble or cs is empty, it return NULL
 */
gen_t *
cs_pop( cs_t *cs );

/* Prints information about the conflict set */
void 
cs_show( cs_t *cs );

/* Removes the memory that have been allocated */
void 
cs_free( cs_t *cs );

#endif

