#ifndef __EV_QUEUE_H__
#define __EV_QUEUE_H__

typedef struct 
{
	char* 	q_data[10];
	int 	head,
			tail;	
} ev_queue_t;


void
ev_queue_init( ev_queue_t *q );

void
ev_queue_push( ev_queue_t *q, char *data );

char *
ev_queue_pop( ev_queue_t *q );

#endif
