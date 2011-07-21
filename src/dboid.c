#include "dboid.h"

#include <ossp/uuid.h>
#include <stdlib.h>

void
dboid_gen( char *name, char *id )
{
	uuid_t *uuid; 
	uuid_t *uuid_ns;
	char *str;
	
	/* Create the ID */
 	uuid_create(&uuid); 
	uuid_create(&uuid_ns);
    uuid_load(uuid_ns, "ns:URL");
	uuid_make(uuid, UUID_MAKE_V3, uuid_ns, name );
	str = NULL; 
	uuid_export(uuid, UUID_FMT_STR,&str, NULL); 
	
	uuid_destroy(uuid_ns); 
	uuid_destroy(uuid); 
	
	strncpy(id, str, DBOID_SIZE );
	
	free( str );
}
