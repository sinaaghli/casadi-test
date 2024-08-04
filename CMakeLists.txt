cmake_minimum_required(VERSION 3.0)
project(casadi_example)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add optimization flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# Manually set the paths to CasADi include and library directories
set(CASADI_INCLUDE_DIR "/usr/local/include")
set(CASADI_LIBRARY_DIR "/usr/local/lib")

# Include CasADi directories
include_directories(${CASADI_INCLUDE_DIR})

# Add executable target
add_executable(casadi_example main.cpp)

# Link against CasADi libraries
find_library(CASADI_LIB casadi HINTS ${CASADI_LIBRARY_DIR})
target_link_libraries(casadi_example ${CASADI_LIB})

