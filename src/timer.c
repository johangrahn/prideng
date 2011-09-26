#include "timer.h"

/* Marks the time in the timeval structure using gettimeofday() */
void
timer_mark( struct timeval *t )
{
	gettimeofday( t, NULL );
}

/* Returns the difference in ms between the start time and the end time */
double 
timer_get_diff( struct timeval *start, struct timeval *end )
{
	double ms;
	
	/* Calculate the number of seconds */
	ms = ( end->tv_sec - start->tv_sec ) * 1000.0;
	
	/* Add the number of microseconds */
	ms += ( end->tv_usec - start->tv_usec ) / 1000.0;
	
	return ms;
}