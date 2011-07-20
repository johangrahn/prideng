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
)

add_executable( prideng ${SOURCES} )
target_link_libraries( prideng pthread )
