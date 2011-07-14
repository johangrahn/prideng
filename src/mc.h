#ifndef __MC_H__
#define __MC_H__

#define MC_METHOD_SIZE 40

typedef struct 
{
	char 	method_name[MC_METHOD_SIZE];

	/* Stores the generation number to what generation it belongs to */ 
	int 	gen; 
} mc_t;

#endif 

