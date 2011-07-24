set( SOURCES
	prideng.c
	cs.c
	gen.c
	prop.c
	rep_list.c
	network.c
	receiver.c
	pack.c
	mc.c
	dboid.c
	imdb.c
	resolve.c
	btree.c
)

# This is for ossp-uuid 
link_directories (
	"/opt/local/lib"
	"/opt/local/lib/db47"
)

add_executable( prideng ${SOURCES} )
target_link_libraries( prideng pthread uuid db)
