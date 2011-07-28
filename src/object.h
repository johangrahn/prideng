#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "mc.h"

typedef struct 
{
	int a;
} obj_t;

#endif

void 
obj_inc( obj_t *o, int num );

void
obj_inc_res( obj_t *o, param_t *params, int num_params );
