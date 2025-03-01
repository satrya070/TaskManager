cmake_minimum_required(VERSION 3.15)

project(TaskManager VERSION 1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set output directories for builds
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#set_target_properties(TaskManager PROPERTIES LINKER_LANGUAGE CXX)


add_library(sqlite3 STATIC external/sqlite/lib/sqlite3.c)
target_include_directories(sqlite3 PUBLIC external/sqlite/include/)

add_executable(TaskManager src/main.cpp "include/Task.h" "src/Task.cpp")

# include headers
target_include_directories(TaskManager PRIVATE include/)
target_link_libraries(TaskManager PRIVATE sqlite3)

# cross platform settings
#[[if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Compiling on Windows")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Compiling on Linux")
endif
]]#