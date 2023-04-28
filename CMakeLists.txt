cmake_minimum_required(VERSION 3.24)

project(Tog)

################################################################
#flags
set(CMAKE_CXX_STANDARD 17)
################################################################
#directories
include_directories(lib)
################################################################
#sources
# Set source files for RELEASE target
set(RELEASE_SOURCE_FILES
        main.cpp

        )

# Set source files for DEBUG target
set(DEBUG_SOURCE_FILES

        testing/testing.cpp

        )
################################################################
# Create RELEASE target
add_executable(Tog ${RELEASE_SOURCE_FILES} ${TA-LIB} )

# Create DEBUG target
add_executable(testing_tool ${DEBUG_SOURCE_FILES} ${TA-LIB})
################################################################