cmake_minimum_required(VERSION 3.22)
project(ringbuf)

set(CMAKE_CXX_STANDARD 20)

set(COMPILE_FLAGS "-Wall -Werror -g -pthread")

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${COMPILE_FLAGS}")

set(BOOST_INCLUDE_DIRS "/opt/boost_1_79_0")

include_directories(${BOOST_INCLUDE_DIRS})

add_executable(ringbuf main.cpp DataQueue.cpp DataQueue.hpp)
