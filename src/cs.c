#include "cs.h"
#include <stdlib.h>

cs_t *
cs_new( int size )
{
	cs_t *cs;

	cs = malloc( sizeof( cs_t ) );

	cs->max_gen = cs->min_gen = -1;

	return cs;
}
