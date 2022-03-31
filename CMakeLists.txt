cmake_minimum_required(VERSION 3.14)

project(kdtree3)

include_directories(${CMAKE_SOURCE_DIR}/include/kdtree3)

add_executable(test tests/kdtree3.test.cpp)
target_compile_features(test PUBLIC cxx_std_17)
