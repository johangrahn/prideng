#include "mc.h"

#include <string.h>
#include <stdio.h>

#include "gen.h"

void
mc_init( mc_t *mc )
{
	int it;

	mc->gen 			= -1;
	mc->method_name[0] 	= '\0';
	mc->dboid[0] 		= '\0';
	mc->num_param 		= -1;

	for( it = 0; it < MC_PARAM_MAX; it++ )
	{
		mc->params[it].type = TYPE_NONE;
		mc->params[it].data.int_data = -1;
	}

}

void
mc_copy( mc_t *mc1, mc_t *mc2 )
{
	int it;

	if( mc1 == NULL || mc2 == NULL )
	{
		printf( "Failed to copy mc1 to mc2: mc1 or mc2 is null\n" );
	}

	/* Copy the static data in the structure */
	*mc2 = *mc1;

	/* Copy the strings separatly */
	strncpy( mc2->dboid, mc1->dboid, DBOID_SIZE );
	strncpy( mc2->method_name, mc1->method_name, MC_METHOD_SIZE );	

	for( it = 0; it < mc1->num_param; it++ )
	{
		mc2->params[it] = mc1->params[it];
	}
}

