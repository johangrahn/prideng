#include "resolve.h"
#include "png.h"
#include "object.h"
#include "btree.h"
#include "method_list.h"
#include "gen.h"

#include <stdlib.h>

/* Default conflict resolution function 
 * Takes the first update in the replica set as update */
mc_t*
resolve( gen_t *g );
	

void* 
resolve_thread( void *data )
{
	method_list_t 		*m_list;
	mc_t 				*mc;
	cs_t				*cs;
	method_prototype 	method;
	char				*cs_dboid;
	resolve_function 	res_func;
	ev_queue_t			*res_queue;
	png_t 				*conf;
	gen_t 				*g;
	obj_t 				*obj;
	
	conf = (png_t*) data;
	res_queue = &conf->res_queue;
	m_list = conf->m_list;
	
	/* Set default resolve function */
	res_func = &resolve;
	
	printf( "[Resolve Thread] Started.\n" );
	
	while( 1 )
	{
		/* locks the signal variable and wait */
		/*
		pthread_mutex_lock( conf->resolve_sig_lock );
		pthread_cond_wait( conf->resolve_sig, conf->resolve_sig_lock  );
		pthread_mutex_unlock( conf->resolve_sig_lock );
		*/
		
		ev_queue_listen( res_queue );
		
		/* Get what conflict set that send the signal */
		cs_dboid = ev_queue_pop( res_queue );
		
		cs = cs_list_find( &((png_t*)data)->cs_list, cs_dboid );
		
		/* Fetch a generation that needs to be resolved */
		g = cs_pop( cs );
		if( g != NULL )
		{
			/* Fetch the object from the storage */
			imdb_fetch( &conf->stable_db, "obj", (void*)&obj );
			
			/* Perform some conflict resolution */
			mc = res_func( g );
			
			/* Fetch the function pointer */
			method = method_list_find( m_list, mc->method_name );
			
			/* Perform the method on the object */
			method( obj, mc->params, mc->num_param );
			
			/* Put the object back */
			imdb_store( &conf->stable_db, "obj", obj, sizeof( obj_t ) );
			
			printf( "Conflict resolved generation %d\n", g->num );
			
			gen_free( g );
		}		
	}
	
	return NULL;
}

mc_t*
resolve( gen_t *g )
{
	int it;
	
	for(it = 0; it < g->size; it++ )
	{
		if( g->data[it].type == GEN_UPDATE )
		{
			return &g->data[it].data;
		}
	}
	
	return NULL;
}
