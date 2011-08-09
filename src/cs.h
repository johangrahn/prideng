#ifndef __CS_H_
#define __CS_H_

#include "gen.h"
#include "mc.h"
#include "dboid.h"
#include "ev_queue.h"

#include <pthread.h>
typedef struct c 
{

	int min_gen, min_pos;
	int max_gen, max_pos;
	int prop_gen, prop_pos;

	gen_t **gens;
	
	int num_gen;
	
	/* The unique database id */
	char dboid[ DBOID_SIZE ];
	
	/* Lock for accessing the structure */
	pthread_mutex_t lock;
	
	ev_queue_t *prop_queue;
	ev_queue_t *res_queue;
	
} cs_t;

/* Creates a new conflict set with a defined 
 * number of generations and replicas */
cs_t *
cs_new( int gen_size, 
		int replicas, 
		ev_queue_t *prop_queue,
		ev_queue_t *res_queue );
			

/* Insert the update defined in up var
 *
 * If there are no room in the conflict set, -1 is returned 
 */
int 
cs_insert( cs_t *cs, mc_t *up, int rep_id );

/* Stores the propagated update from another replica into the conflict set */
int 
cs_insert_remote(cs_t *cs, mc_t *up, int rep_id, int own_id);

/* Sets the stabilization information for a given generation and replica */
void
cs_set_stab( cs_t *cs, int rep_id, int gen );

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


/* Creates a copy of the conflict set and marks this conflict set as locked */
cs_t *
cs_create_trans_obj( cs_t *cs );


/* Locks / unlocks access */
void
cs_lock( cs_t *cs );

void
cs_unlock( cs_t *cs );
	
/* Prints information about the conflict set */
void 
cs_show( cs_t *cs );

/* Removes the memory that have been allocated */
void 
cs_free( cs_t *cs );

/* Increases the position of the generation pointer 
 * based on its current position and cs size */
int 
cs_inc_pos( int gen_pos, int cs_size );

#endif

