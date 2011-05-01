#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cs.h"

typedef struct
{
	cs_t *cs;
} png_t;

void 
png_show_prompt();

int
png_handle_cmd( png_t *png, char *cmd );

int main( int argc, char **argv )
{
	char cmd[40];
	int res;
	png_t png;

	/* Create confligt set that will be used in
	 * the application 
	 */
	png.cs = cs_new( 10, 1 );

	/* Start taking in user commands */
	while( 1 )
	{

		png_show_prompt();

		/* Get the command from the prompt */
		fgets( cmd, 40, stdin );
		cmd[ strlen( cmd ) -1 ] = '\0';

		res = png_handle_cmd( &png, cmd );

		/* Check if the quit command was issued */
		if( res == 0 )
		{
			break;
		}

	}

	cs_free( png.cs );
	return 0;
}

void png_show_prompt()
{
	printf( "Command> ");
}

int png_handle_cmd( png_t *png,  char *cmd )
{
	/* Check what command that was issued */
	if( strcmp( cmd, "q" ) == 0 ) 
	{
		printf( "Quiting..\n" );
		return 0;
	}
	else if( strcmp( cmd, "add" ) == 0 )
	{
		if( cs_insert( png->cs, 3 ) == -1 )
		{
			printf( "Conflict set is full!" );
		}
		
		return 1;
	}
	else if( strcmp( cmd, "s") == 0 )
	{
		cs_show( png->cs );

		return 1;
	}
	else
	{
		return -1;
	}
}
