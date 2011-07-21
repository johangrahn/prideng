#include "dboid.h"

#include <ossp/uuid.h>
#include <stdlib.h>

void
dboid_gen( char *id )
{
	uuid_t *uuid; char *str;

	/* Create the ID */
 	uuid_create(&uuid); 
	uuid_make(uuid, UUID_MAKE_V1); 
 	str = NULL; 
	uuid_export(uuid, UUID_FMT_STR,&str, NULL); 
	uuid_destroy(uuid); 
	
	strncpy(id, str, DBOID_SIZE );
	
	free( str );
}
