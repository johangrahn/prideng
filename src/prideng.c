#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cs.h"

void png_show_prompt();
int png_handle_cmd( char *cmd );

int main( int argc, char **argv )
{
	char cmd[40];
	int res;

	/* Start taking in user commands */
	
	while( 1 )
	{

		png_show_prompt();

		/* Get the command from the prompt */
		fgets( cmd, 40, stdin );
		cmd[ strlen( cmd ) -1 ] = '\0';

		res = png_handle_cmd( cmd );

		/* Check if the quit command was issued */
		if( res == 0 )
		{
			break;
		}

	}
	return 0;
}

void png_show_prompt()
{
	printf( "Command> ");
}

int png_handle_cmd( char *cmd )
{
	cs_t *cs;
	/* Check what command that was issued */
	if( strcmp( cmd, "q" ) == 0 ) 
	{
		printf( "Quiting..\n" );
		return 0;
	}
	else if( strcmp( cmd, "add" ) == 0 )
	{
		cs = cs_new( 10 );
		free( cs );
		return 1;
	}
	else
	{
		return -1;
	}
}
