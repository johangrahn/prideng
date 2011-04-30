#include "gen.h"

#include <stdlib.h>

gen_t *
gen_new( int replicas )
{
	gen_t *g;

	g = malloc( sizeof( gen_t ) );
	g->data = malloc( sizeof(int) * replicas );
	return g;
}
