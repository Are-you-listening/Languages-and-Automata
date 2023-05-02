cmake_minimum_required(VERSION 3.24)

project(Tog)

################################################################
#flags
set(CMAKE_CXX_STANDARD 17)
################################################################
#directories
include_directories(lib)
include_directories(lib/Toi_Anas)
include_directories(lib/Toi_Emil)
include_directories(lib/Toi_Kars)
include_directories(lib/Toi_Tibo)
################################################################
#sources
# Set source files for RELEASE target

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# List the source files with the correct directory prefixes
set(MIDIPASER_SRC
        src/midiparser/MidiParser.h
        src/midiparser/MidiParser.cpp
        src/midiparser/ByteX.h
        src/midiparser/ByteX.cpp
        )

set(MAIN_SRC
        src/Genre.h
        src/Genre.cpp
        src/Note.h
        src/Note.cpp
        src/Song.h
        src/Song.cpp
        ${MIDIPASER_SRC}
        src/NFA.cpp
        src/NFA.h
        src/RE.cpp
        src/RE.h
        src/DFA.cpp
        src/DFA.h
        src/ENFA.cpp
        src/ENFA.h
        lib/Toi_Tibo/DFAT.cpp
        lib/Toi_Tibo/DFAT.h

        lib/Toi_Emil/REE.cpp
        lib/Toi_Emil/REE.h

        lib/Toi_Emil/DFAE.cpp
        lib/Toi_Emil/DFAE.h
        lib/Toi_Emil/automaat.cpp
        lib/Toi_Emil/automaat.h
        lib/Toi_Emil/Node.cpp
        lib/Toi_Emil/Node.h
        )

set(RELEASE_SOURCE_FILES ${MAIN_SRC} main.cpp)


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