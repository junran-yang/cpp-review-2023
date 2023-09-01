add_executable(revenue.tsk main.cpp sales.cpp licensing.cpp sumof.cpp)
target_link_libraries(revenue.tsk PRIVATE bsl)
configure_bb_target(revenue.tsk V2 HEADER_DIRS .)
