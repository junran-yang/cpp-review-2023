add_library(librevenue STATIC licensing.cpp sales.cpp sumof.cpp)
target_link_libraries(librevenue PUBLIC bsl)
configure_bb_target(librevenue V2 HEADER_DIRS .)
