#include "ev_queue.h"

void
ev_queue_init( ev_queue_t *q )
{
	q->head = -1;
	q->tail = -1;
}

void
ev_queue_push(ev_queue_t *q, char *data )
{
	if( q->head == -1 )
	{
		q->head = q->tail = 0;
		q->q_data[ q->head ] = data;
	}
	else
	{
		q->tail++;
		q->q_data[ q->tail ] = data;
	}
}

char *
ev_queue_pop( ev_queue_t *q )
{
	return q->q_data[q->head++];
}

