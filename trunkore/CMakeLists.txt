cmake_minimum_required(VERSION 2.8)

#Suppress ZERO_CHECK prject generation
set(CMAKE_SUPPRESS_REGENERATION true)

#There is no way to suppress ALL_BUILD project generation (for "make all")

set(PROJECT_NAME trunkore)

if(WIN32)
    set(PLATFORM_ID PLATFORM_WINDOWS)
elseif(UNIX)
    set(PLATFORM_ID PLATFORM_NIX)
elseif(APPLE)
    set(PLATFORM_ID PLATFORM_MACOS)
endif()
add_compile_definitions(${PLATFORM_ID})

include_directories(./src)

file(GLOB SRC_FILES ./src/*.cpp ./src/ModuleContainer/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})