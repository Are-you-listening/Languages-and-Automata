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
set(TA-LIB TA-lib/DFA.cpp TA-lib/DFA.h TA-lib/ENFA.cpp
        TA-lib/ENFA.h TA-lib/RE.cpp TA-lib/RE.h)
################################################################
#exec
add_executable(Tog main.cpp ${TA-LIB})
################################################################
#library

################################################################
