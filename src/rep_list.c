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
	repl->size++;
	
	/* Expand the memory used for the list */
	repl->reps = realloc( repl->reps, sizeof ( rep_t ) * repl->size );
	
	/* Add the new replica */
	repl->reps[repl->size] = *rep;
	strncpy( repl->reps[repl->size].host, rep->host, strlen( rep->host ) + 1  );

}