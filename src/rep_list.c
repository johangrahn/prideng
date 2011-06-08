#include "rep_list.h"

#include <stdlib.h>
#include <string.h>

void
rep_list_init( rep_list_t *repl )
{
	repl->reps = NULL;
	repl->size = 0;
}

void
rep_list_add( rep_list_t *repl, rep_t *rep )
{
	int size;

	repl->size++;
	
	if( repl->size == 0)
		size = 1;
	else
		size = repl->size;


	/* Expand the memory used for the list */
	repl->reps = realloc( repl->reps, sizeof ( rep_t ) * size );
	
	/* Add the new replica */
	repl->reps[size - 1] = *rep;
	strncpy( repl->reps[size - 1 ].host, rep->host, strlen( rep->host ) + 1  );

}

void
rep_list_free( rep_list_t *repl )
{
	free( repl->reps );
}

int
rep_parse_str( char *str, rep_t *rep )
{
	char *host;
	int id;
	int port;
	
	id = atoi( strtok( NULL, " " ) );
	host = strtok( NULL, " " );
	port = atoi( strtok( NULL, " " ) );
	
	rep->id = id;
	strncpy( rep->host, host, strlen( host ) + 1 );
	rep->port = port;

	return 1;
}
