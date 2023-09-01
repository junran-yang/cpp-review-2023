cmake_minimum_required(VERSION 3.12)
project(revenue)

find_package(ConfigureBbTarget REQUIRED)
find_package(CompilerHelpers REQUIRED)
configure_compile_options( 
  COMPILER_ID GNU
  LANGUAGES CXX
  OPTIONS -Wall -Werror
)

add_subdirectory(librevenue)
add_subdirectory(main)
