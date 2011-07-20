#include "pack.h"

#include <stdlib.h>
#include <string.h>

ppack_t *
pack_create_prop( int size )
{
	ppack_t *p;
	int it;

	p = malloc( sizeof( ppack_t ) + (sizeof(mc_t) * ( size )) );

	p->size = sizeof( ppack_t ) + (sizeof(mc_t) * ( size ));
	p->type = PROPAGATION;
	
	p->num_up = -1;
	p->rep_id = -1;

	for( it = 0; it < size; it++ )
	{
		mc_init( &p->updates[it] );
	}

	return p;
}
