add_executable(revenue.tsk main.cpp)
target_link_libraries(revenue.tsk PRIVATE librevenue)
configure_bb_target(revenue.tsk V2 HEADER_DIRS .)
