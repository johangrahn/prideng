#ifndef __MC_H__
#define __MC_H__

#define MC_METHOD_SIZE 40
#define MC_PARAM_MAX 5

#include "dboid.h"


/* Different parameter types that can be used */
typedef enum
{
	TYPE_NONE, 
	TYPE_INT
} param_type_t;

typedef struct 
{
	param_type_t type;
	
	/* Different data containers for the data type */
	union 
	{
		int int_data;
	} data;
} param_t;

typedef struct 
{
	char 	method_name[MC_METHOD_SIZE];
	
	/* This is the unique identifier for each update */
	char 	dboid[DBOID_SIZE];

	/* Stores the generation number to what generation it belongs to */ 
	int 	gen;
	
	/* Stores the parameters that is associated with the update */
	int 	num_param;
	param_t params[MC_PARAM_MAX];
	 
} mc_t;

/* Sets the default values for each property in the method update */
void
mc_init( mc_t *mc );

/* Copies the data stored in mc1 into mc2 */
void
mc_copy( mc_t *mc1, mc_t *mc2 );

#endif 

