#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "mc.h"
#include "dboid.h"

typedef struct 
{
	int a;
	
	char dboid[ DBOID_SIZE ];
} obj_t;

#endif

void 
obj_inc( obj_t *o, int num );

void
obj_inc_res( void *o, param_t *params, int num_params );
