#ifndef __RESOLVE_H_
#define __RESOLVE_H_

#include "mc.h"
#include "gen.h"

/* Function prototype for conflict resolution function */
typedef mc_t* (*resolve_function)( gen_t* );

void* 
resolve_thread( void *data );
	
#endif
