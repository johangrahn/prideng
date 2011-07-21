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
)

# This is for ossp-uuid 
link_directories (
	"/opt/local/lib"
)

add_executable( prideng ${SOURCES} )
target_link_libraries( prideng pthread uuid )
