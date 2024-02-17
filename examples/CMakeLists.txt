add_executable(c_example c_example.c)
target_link_libraries(c_example PRIVATE median_filter::median_filter)

add_library(naive_median_filter naive_median_filter.c naive_median_filter.h)
add_library(optimized_median_filter optimized_median_filter.c optimized_median_filter.h)

if (ENABLE_TESTING)
    add_executable(optimized_median_filter_test optimized_median_filter_test.cpp)
    target_link_libraries(optimized_median_filter_test PRIVATE optimized_median_filter Catch2::Catch2WithMain)
endif ()

if (ENABLE_BENCHMARK)
    add_executable(median_filter_bench median_filter_bench.cpp)
    target_link_libraries(median_filter_bench PRIVATE
            optimized_median_filter naive_median_filter Catch2::Catch2WithMain)
endif ()
