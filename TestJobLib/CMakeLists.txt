cmake_minimum_required(VERSION 2.8)

#Suppress ZERO_CHECK prject generation
set(CMAKE_SUPPRESS_REGENERATION true)

set(PROJECT_NAME trunkore_testlib)

include_directories(./src)

file(GLOB SRC_FILES ./src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})