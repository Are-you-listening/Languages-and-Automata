cmake_minimum_required(VERSION 3.24)

project(Tog)

################################################################
#flags
set(CMAKE_CXX_STANDARD 17)
################################################################
#directories
include_directories(JSON_Parser TA-lib)
################################################################
#sources
#set()
################################################################
#exec
add_executable(Tog main.cpp ${TA-LIB} )

################################################################
#library

################################################################

# Set source files for RELEASE target
set(RELEASE_SOURCE_FILES
        main.cpp

        )

# Set source files for DEBUG target
set(DEBUG_SOURCE_FILES

        testing/testing.cpp

        )

# Create RELEASE target
add_executable(main_executable ${RELEASE_SOURCE_FILES})

# Create DEBUG target
add_executable(testing_tool ${DEBUG_SOURCE_FILES})
