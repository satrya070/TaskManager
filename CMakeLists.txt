cmake_minimum_required(VERSION 3.15)

project(TaskManager VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set output directories for builds
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(TaskManager src/main.cpp "include/Task.h" "src/Task.cpp")

# include headers
target_include_directories(TaskManager PRIVATE include)

# cross platform settings
#[[if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Compiling on Windows")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Compiling on Linux")
endif
]]#