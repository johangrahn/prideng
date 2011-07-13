#include "pack.h"

#include <stdlib.h>

ppack_t *
pack_create_prop( int size )
{
	ppack_t *p;

	p = malloc( sizeof( ppack_t ) + sizeof(int) * (size - 1 ) );

	p->size = sizeof( ppack_t ) + sizeof(int) * (size - 1 );
	p->type = PROPAGATION;

	return p;
}
