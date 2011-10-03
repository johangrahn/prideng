#ifndef __PNG_H_
#define __PNG_H_

#include "cs.h"
#include "rep_list.h"
#include "imdb.h"
#include "method_list.h"
#include "cs_list.h"
#include "ev_queue.h"
#include "hash_table.h"

#include <pthread.h>

typedef struct
{
	rep_list_t rlist;
	
	/* Deprecated: Use the cs list instead */
	cs_t *cs;
	
	cs_list_t cs_list;
	
	/* Queue for all propagation events */
	ev_queue_t prop_queue;
	
	/* Queue for all resolve events */
	ev_queue_t res_queue;
	
	/* Signal for when propagation is needed */
	pthread_mutex_t *prop_sig_lock;
	pthread_cond_t *prop_sig;
	
	/* Signal for when a conflict resolution is needed */
	pthread_mutex_t *resolve_sig_lock;
	pthread_cond_t *resolve_sig;
	
	/* Method list for all objects that is replicated */
	method_list_t *m_list;
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
	
	/* Table for timers */
	h_table_t 	timers;
	
} png_t;

png_t __conf;

#endif

