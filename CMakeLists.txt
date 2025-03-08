cmake_minimum_required(VERSION 3.15)

project(TaskManager VERSION 1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set output directories for builds
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB SOURCES "src/*.cpp")

add_library(sqlite3 STATIC external/sqlite/lib/sqlite3.c)

add_executable(TaskManager src/main.cpp ${SOURCES})

# include headers
target_include_directories(TaskManager PRIVATE include/ external/sqlite/include)
target_link_libraries(TaskManager PRIVATE sqlite3)

# cross platform settings
#[[if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Compiling on Windows")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Compiling on Linux")
endif
]]#