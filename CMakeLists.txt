cmake_minimum_required(VERSION 3.24)

project(Tog)

################################################################
#flags
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-std=c++17 -Ofast") #-Wall -Werror -02
################################################################
#directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(lib)
#include_directories(lib/Toi_Anas)
#include_directories(lib/Toi_Emil)
#include_directories(lib/Toi_Kars)
#include_directories(lib/Toi_Tibo)
################################################################
#sources
# Set source files for RELEASE target

# List the source files with the correct directory prefixes
set(MIDIPASER_SRC

        src/midiparser/MidiParser.h
        src/midiparser/MidiParser.cpp
        src/midiparser/ByteX.h
        src/midiparser/ByteX.cpp
        src/midiparser/midiExporter.cpp
        src/midiparser/midiExporter.h)

set(GUI_SRC

        src/gui/Gui.cpp
        src/gui/Gui.h
        src/gui/SongListWidget.cpp
        src/gui/SongListWidget.h
        src/gui/Button.cpp
        src/gui/Button.h
        src/gui/ButtonCompare.cpp
        src/gui/ButtonCompare.h
        src/gui/SongListWidgetSingle.cpp
        src/gui/SongListWidgetSingle.h
        src/gui/SongWidget.cpp
        src/gui/SongWidget.h)

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
        src/ENFAT.cpp
        src/ENFAT.h
        src/WNFA.cpp
        src/WNFA.h
        src/WDFA.cpp
        src/WDFA.h

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
        lib/Toi_Emil/weightedautomaat.cpp
        lib/Toi_Emil/weightedautomaat.h
        lib/Toi_Emil/weightedNode.cpp
        lib/Toi_Emil/weightedNode.h

        lib/Toi_Kars/Utils.cpp
        lib/Toi_Kars/Utils.h
        lib/Toi_Kars/State.h
        lib/Toi_Kars/State.cpp

        lib/Utils.h
        lib/Utils.cpp)

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

target_link_libraries(Tog -lX11)
target_link_libraries(testing_tool -lX11)