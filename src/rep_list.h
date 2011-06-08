#ifndef __REP_LIST_H_
#define __REP_LIST_H_

typedef struct 
{
	char 	host[40];
	int 	port;
	int		id;
} rep_t;

typedef struct 
{
	rep_t *reps;
	int size;
} rep_list_t;


void
rep_list_init( rep_list_t *repl );

void
rep_list_add( rep_list_t *repl, rep_t *rep );

void
rep_list_free( rep_list_t *repl );


/* Parses a string to get the replica info */
int
rep_parse_str( char *str, rep_t *rep );

#endif
