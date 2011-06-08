set( SOURCES
	prideng.c
	cs.c
	gen.c
	prop.c
	rep_list.c
)

add_executable( prideng ${SOURCES} )
target_link_libraries( prideng pthread )
