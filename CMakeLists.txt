cmake_minimum_required(VERSION 3.2)
project(connect4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c11")

add_executable(connect4 game.c)