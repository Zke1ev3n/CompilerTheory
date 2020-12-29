cmake_minimum_required(VERSION 3.9)
project(LALR1)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "-static-libstdc++")
add_executable(LALR1 main.cc lalr1.cc)
