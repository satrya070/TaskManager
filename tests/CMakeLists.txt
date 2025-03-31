include(FetchContent)

# fetch google test for unit testing
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/6910c9d9165801d8827d628cb72eb7ea9dd538c5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(test_task_manager
    ${CMAKE_SOURCE_DIR}/src/AddCommand.cpp
    test_task_manager.cpp
    test_task.cpp
    test_add_command.cpp
)

#file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/AddCommand.cpp)

target_include_directories(test_task_manager PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/external/sqlite/include)
target_link_libraries(test_task_manager PRIVATE GTest::gtest_main sqlite3)

if(TARGET sqlite3)
    message(STATUS "static sqlite3 exist in tests")
else()
    message(STATUS "static sqlite3 does NOT exist here in tests!")
endif()

enable_testing()