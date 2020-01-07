cmake_minimum_required(VERSION 2.8)

#Suppress ZERO_CHECK prject generation
set(CMAKE_SUPPRESS_REGENERATION true)

#Build trunkore
add_subdirectory(trunkore)
#Build TestJobLib
add_subdirectory(TestJobLib)