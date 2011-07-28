#include "resolve.h"
#include "png.h"
#include "object.h"
#include "btree.h"
#include "method_list.h"
#include "gen.h"

#include <stdlib.h>

mc_t*
resolve( gen_t *g );
	

void* 
resolve_thread( void *data )
{
	method_list_t 		m_list;
	mc_t 				*mc;
	method_prototype 	method;
	png_t 				*conf;
	gen_t 				*g;
	obj_t 				*obj;
	
	conf = (png_t*) data;
	
	method_list_init( &m_list );
	
	method_list_insert( &m_list, "obj_inc", &obj_inc_res );

	printf( "[Resolve Thread] Started.\n" );
	
	while( 1 )
	{
		/* locks the signal variable and wait */
		pthread_mutex_lock( conf->resolve_sig_lock );
		pthread_cond_wait( conf->resolve_sig, conf->resolve_sig_lock  );
		pthread_mutex_unlock( conf->resolve_sig_lock );
		
		/* Fetch a generation that needs to be resolved */
		g = cs_pop( conf->cs );
		if( g != NULL )
		{
			/* Fetch the object from the storage */
			imdb_fetch( &conf->stable_db, "obj", (void*)&obj );
			
			/* Perform some conflict resolution */
			mc = resolve( g );
			
			printf( "Fetched method name: %s\n", mc->method_name );
			
			/* Fetch the function pointer */
			method = method_list_find( &m_list, mc->method_name );
			
			/* Perform the method on the object */
			method( obj, mc->params, mc->num_param );
			
			/* Put the object back */
			imdb_store( &conf->stable_db, "obj", obj, sizeof( obj_t ) );
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
