#include "pack.h"

#include <stdlib.h>

pack_t *
pack_create_prop( int size )
{
	pack_t *p;

	p = malloc( sizeof( pack_t ) + sizeof(int) * (size - 1 ) );

	p->size = sizeof( pack_t ) + sizeof(int) * (size - 1 );
	p->type = PROPAGATION;

	return p;
}
