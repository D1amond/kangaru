cmake_minimum_required(VERSION 2.6)
project(example4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")
add_executable(example4 example4.cpp)
include_directories(${PROJECT_SOURCE_DIR}/../../include/kangaru)

install(TARGETS example4 RUNTIME DESTINATION bin)
