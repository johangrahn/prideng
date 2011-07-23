#ifndef __PNG_H_
#define __PNG_H_

#include "cs.h"
#include "rep_list.h"
#include "imdb.h"

#include <pthread.h>

typedef struct
{
	rep_list_t rlist;
	cs_t *cs;
	
	/* Signal for when propagation is needed */
	pthread_mutex_t *prop_sig_lock;
	pthread_cond_t *prop_sig;
	
	/* Signal for when a conflict resolution is needed */
	pthread_mutex_t *resolve_sig_lock;
	pthread_cond_t *resolve_sig;
	
	
	/* 
		Port number where the application listens to 
		for incoming connections
	*/
	int lport;
	
	/* 
	 * Unique replica id to identify the replica 
	 */
	int id;

	/* ID for the listen socket */
	int lsock;
	
	/* Stable storage for objects */
	imdb_t stable_db;
	
} png_t;

png_t __conf;

#endif

