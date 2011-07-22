#include "imdb.h"

#include <stdlib.h>
#include <string.h>

/* Opens the database */
int
imdb_open( imdb_t *imdb );
	
int
imdb_init( imdb_t *imdb )
{
	int				ret;
	DB_ENV 			*envp;
	u_int32_t 		envFlags;
		
	ret = db_env_create( &envp, 0 );
	if( ret != 0 ) 
	{
		printf( "Failed to create anvironment for database store\n" );
		return -1;
	}
	
	envFlags = 
		DB_CREATE |
		DB_INIT_LOCK |
		DB_INIT_LOG | 
		DB_INIT_TXN |
		DB_INIT_MPOOL |
		DB_PRIVATE /* Don't put region files on disk */
	;
 
    /* Store database logs entirely in memory */
    envp->log_set_config( envp, DB_LOG_IN_MEMORY, 1 ); 

    /* Increase the cache size  */
    envp->set_cachesize( envp, 0, 100 * 1024 * 1024, 1 ); 

    envp->set_errfile( envp, stderr );
	
	ret = envp->open( envp, NULL, envFlags, 0 );
	if( ret != 0 ) {
		printf( "Failed to open environment for database store\n");
		return -1;
	}
	
	imdb->envp = envp;
	
	/* Open a database */
	imdb_open( imdb );
	

	
	return 1;
}

int
imdb_store( imdb_t *imdb, char *key_name, void *data, size_t size )
{
	DBT key,
		value;
	int ret;
	
	memset( &key, 0, sizeof( DBT ) );
	memset( &value, 0, sizeof( DBT ) );
	
	key.data = key_name;
	key.size = strlen( key_name ) + 1;
	
	value.data = data;
	value.size = size;
	
	ret = imdb->db->put( imdb->db, NULL, &key, &value, 0 );
	if( ret != 0 ) 
	{
		printf( "Failed to store data on key %s on optimstic storage\n", key_name );
		return -1;
	}
	
	return 1;
}

int 
imdb_fetch( imdb_t *imdb, char *key_name, void **data )
{
	DBT			key,
				value;
	int 		ret;	

	memset( &key, 0, sizeof( DBT ) );
	memset( &value, 0, sizeof( DBT ) );
	
	key.data = key_name;
	key.size = strlen(key_name) + 1;

	value.flags = DB_DBT_MALLOC;
	
	ret = imdb->db->get( imdb->db, NULL, &key, &value, 0 );
	if( ret != 0 ) 
	{
		printf( "Failed to fetch object with key %s\n", key_name );
		return -1;
	}
	
	*data = value.data;
	
	return 1;
}



int
imdb_open( imdb_t *imdb )
{
	DB_MPOOLFILE 	*mpf;
	DB 				*dbp;
	u_int32_t 		open_flags;
	int 			ret;

	/* Creates the database handler */
	ret = db_create( &dbp, imdb->envp, 0 );
	if( ret != 0 ) {
		printf( "Can't create database for optimistic storage\n" );
		exit( 1 );
	}

	mpf = dbp->get_mpf( dbp );
	mpf->set_flags( mpf, DB_MPOOL_NOFILE, 1 );
	
	/* Opens the database file */
	open_flags = DB_CREATE | DB_EXCL;

	ret = dbp->open( dbp, NULL, NULL, NULL, DB_BTREE, open_flags, 0 );
	if( ret != 0 ) 
	{
		printf( "Failed to open database for optimistic storage\n" );
		return -1;
	}
	
	imdb->db = dbp;
	
	return 1;
}
