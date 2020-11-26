cmake_minimum_required(VERSION 3.9)
project(LL1)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "-static-libstdc++")
add_executable(LL1 main.cc lexer.cc ll1.cc)
