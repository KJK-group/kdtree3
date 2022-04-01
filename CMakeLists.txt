cmake_minimum_required(VERSION 3.14)
project(kdtree3)


set(CMAKE_CXX_COMPILER "/usr/bin/clang++-12")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(Eigen3 REQUIRED)



file(GLOB src_files ./src/*.cpp)
file(GLOB header_files ./include/**/*.hpp)
add_library(kdtree3 ${src_files} ${header_files})
target_compile_features(kdtree3 PUBLIC cxx_std_17)


add_executable(mytest tests/kdtree3.test.cpp)

# add_executable(mytest)
# target_compile_options(mytest
# -Wall
# -Wextra
# -Wconversion
# -Wsign-conversion
# )
target_compile_features(mytest PUBLIC cxx_std_17)
target_link_libraries(mytest kdtree3)

# target_sources(mytest PUBLIC tests/kdtree3.test.cpp src/kdtree3.cpp)
