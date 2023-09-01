add_executable(revenue main.cpp)
include_directories("${CMAKE_SOURCE_DIR}/librevenue")
target_link_libraries(revenue PRIVATE librevenue)
