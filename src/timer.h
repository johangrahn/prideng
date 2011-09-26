#ifndef __TIMER_H__
#define __TIMER_H__

#include <sys/time.h>

/* Marks the time in the timeval structure using gettimeofday() */
void
timer_mark( struct timeval *t ); 

/* Returns the difference in ms between the start time and the end time */
double 
timer_get_diff( struct timeval *start, struct timeval *end );

#endif