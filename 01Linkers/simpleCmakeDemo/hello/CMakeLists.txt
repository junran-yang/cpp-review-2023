cmake_minimum_required(VERSION 3.12)
project(hello)
set(CMAKE_CXX_STANDARD 11)
add_executable(hello hello.cc helper.cc)
