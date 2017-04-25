#include_directories(include)
#include_directories(src)

# file( GLOB SOURCES src/frontend/ast/*.cpp )

cmake_minimum_required (VERSION 3.1)

project(DaedalusCompiler)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-Wall)

include_directories(./include)

# find_package(Boost REQUIRED COMPONENTS program_options)
# include_directories(${Boost_INCLUDE_DIRS})


add_executable(daedalusc src/daedalusc.cpp)
