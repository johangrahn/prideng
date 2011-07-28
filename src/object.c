#include "object.h"

void 
obj_inc( obj_t *o, int num )
{
	o->a += num;
}

void
obj_inc_res( obj_t *o, param_t *params, int num_params )
{
	obj_inc( o, params[0].data.int_data );
}
