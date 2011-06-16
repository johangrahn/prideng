#ifndef __REP_LIST_H_
#define __REP_LIST_H_

typedef struct 
{
	char 	host[40];
	int 	port;
	int		id;
	int		sock;
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

#endif
