add_executable(hello hello.cc)
target_link_libraries(hello PUBLIC helper)
target_include_directories(hello PUBLIC "${CMAKE_SOURCE_DIR}/lib")