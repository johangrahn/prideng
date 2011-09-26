cmake_minimum_required( VERSION 2.6)

project( prideng )

# Sets where the compiled executable should be placed 
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR} )

# set( CMAKE_C_FLAGS "-D_POSIX_C_SOURCE=200112L")
set( CMAKE_C_FLAGS "")
set( CMAKE_C_FLAGS_DEBUG "-g -Wall  -Wextra -pedantic " )

set(TIMER_TEST "1" CACHE TYPE BOOLEAN)

if(TIMER_TEST)
	add_definitions( "-DTIMER_TEST=1")
endif()

include_directories( "/opt/local/include/db47" "/usr/include/ossp" "/opt/local/include/ossp" "/opt/local/include" )
add_subdirectory( src )
