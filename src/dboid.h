#ifndef __DBOID_H__
#define __DBOID_H__

#ifndef DBOID_SIZE 
	#define	DBOID_SIZE 40
#endif 

/* Generates a correct id into the memory pointed by id */
void
dboid_gen( char *id );

#endif 

