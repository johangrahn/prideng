#ifndef __PNG_H_
#define __PNG_H_

#include "cs.h"
#include "rep_list.h"

#include <pthread.h>

typedef struct
{
	rep_list_t rlist;
	cs_t *cs;
	pthread_mutex_t *prop_sig_lock;
	pthread_cond_t *prop_sig;
} png_t;


#endif

