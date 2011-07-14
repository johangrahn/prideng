#include "pack.h"

#include <stdlib.h>
#include <string.h>

ppack_t *
pack_create_prop( int size )
{
	ppack_t *p;
	int it;

	p = malloc( sizeof( ppack_t ) + sizeof(mc_t) * (size - 1 ) );

	p->size = sizeof( ppack_t ) + sizeof(mc_t) * (size - 1 );
	p->type = PROPAGATION;
	
	p->num_up = -1;
	p->rep_id = -1;

	for( it = 0; it < size; it++ )
	{
		p->updates[it].gen = - 1;
		memset( p->updates[it].method_name, 0, MC_METHOD_SIZE );
	}

	return p;
}
