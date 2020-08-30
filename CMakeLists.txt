cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS "-O3")

## This should be commented out
#set(CMAKE_CXX_FLAGS "-g")
#set(CMAKE_CXX_FLAGS "-fsanitize=address")
#set(CMAKE_CXX_FLAGS "--coverage")

#set(CMAKE_CXX_FLAGS_DEBUG "-g")
#set(CMAKE_CXX_FLAGS_DEBUG "-fsanitize=address")
#set(CMAKE_CXX_FLAGS_DEBUG "--coverage")

project(gg)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(third_party/abseil-cpp)
add_subdirectory(third_party/gflags)
add_subdirectory(third_party/glog)
SET(WITH_GFLAGS_OFF)
#add_subdirectory(third_party/leveldb)
find_library(JEMALLOC_LIB jemalloc)

add_executable(graph main.cpp graph.cpp)

target_link_libraries(graph PRIVATE
        glog::glog
        gflags::gflags
        absl::container_memory
        absl::hash_function_defaults
        absl::raw_hash_map
        absl::algorithm_container
        absl::memory
#        leveldb
        ${JEMALLOC_LIB}
        )

option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()
