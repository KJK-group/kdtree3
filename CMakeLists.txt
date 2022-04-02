cmake_minimum_required(VERSION 3.14)
project(kdtree3)

# global configuration
set(CMAKE_CXX_COMPILER "/usr/bin/clang++-12")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# dependencies
find_package(Eigen3 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB src_files ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB header_files ${CMAKE_SOURCE_DIR}/include/**/*.hpp)

# library
add_library(${PROJECT_NAME} STATIC ${src_files} ${header_files})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# Set the public header property to the one with the actual API.
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER
                                                 include/kdtree3.hpp)

# test
add_executable(mytest ${CMAKE_SOURCE_DIR}/test/kdtree3.test.cpp)
target_compile_features(mytest PUBLIC cxx_std_17)
target_link_libraries(mytest kdtree3)
