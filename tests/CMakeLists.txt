include(FetchContent)

# fetch google test for unit testing
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/6910c9d9165801d8827d628cb72eb7ea9dd538c5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(test_task_manager test_task_manager.cpp test_task.cpp)
target_include_directories(test_task_manager PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_task_manager PRIVATE GTest::gtest_main)

enable_testing()