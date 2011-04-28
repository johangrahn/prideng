#ifndef __CS_H_
#define __CS_H_

typedef struct c 
{
	int min_gen;
	int max_gen;
} cs_t;

/* Creates a new conflict set with a defined size */
cs_t *
cs_new( int size ); 

/* Insert the update defined in up var
 *
 * If there are no room in the conflict set, -1 is returned 
 */
int 
cs_insert( cs_t *cs, int up );

/* 
 * Returns 0 if the conflict set is empty
 */
int
cs_is_empty( cs_t *cs );

#endif

