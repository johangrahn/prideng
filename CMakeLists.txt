cmake_minimum_required( VERSION 2.6)

project( prideng )

# Sets where the compiled executable should be placed 
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR} )


set( CMAKE_C_FLAGS_DEBUG "-g -Wall  -Wextra -pedantic " )

add_subdirectory( src )
