cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project(gg)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Process Abseil's CMake build system
add_subdirectory(third_party/abseil-cpp)
add_subdirectory(third_party/googletest)

add_executable(graph main.cpp graph.cpp)
target_link_libraries(graph PRIVATE
        absl::container_memory
        absl::hash_function_defaults
        absl::raw_hash_map
        absl::algorithm_container
        absl::memory)

add_executable(graph_test test_main.cpp graph_test.cpp graph.cpp)
target_link_libraries(graph_test gtest_main
        absl::container_memory
        absl::hash_function_defaults
        absl::raw_hash_map
        absl::algorithm_container
        absl::memory)

