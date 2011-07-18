#ifndef __MC_H__
#define __MC_H__

#define MC_METHOD_SIZE 40
#define MC_PARAM_MAX 5

/* Different parameter types that can be used */
typedef enum
{
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

	/* Stores the generation number to what generation it belongs to */ 
	int 	gen;
	
	/* Stores the parameters that is associated with the update */
	int 	num_param;
	param_t params[MC_PARAM_MAX];
	 
} mc_t;

#endif 

