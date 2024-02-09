add_executable(c_example c_example.c)
target_link_libraries(c_example PRIVATE median_filter::median_filter)

add_executable(temp temp.c)
target_link_libraries(temp PRIVATE naive_median_filter)

add_library(naive_median_filter naive_median_filter.c naive_median_filter.h)

add_executable(median_filter_bench median_filter_bench.cpp)
target_link_libraries(median_filter_bench PRIVATE naive_median_filter Catch2::Catch2WithMain)