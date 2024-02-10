/**
 * \file
 * \brief
 * \author
 */

#include "optimized_median_filter.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Optimized median filter", "[optimized_median_filter]")
{
    struct optimized_median_filter filter;
    struct median_filter_node buffer[3];

    optimized_mf_init(&filter, buffer, 3);

    optimized_mf_insert_value(&filter, 1);
    optimized_mf_debug(&filter);
    REQUIRE(optimized_mf_get_median(&filter) == 1);
    optimized_mf_insert_value(&filter, 2);
    optimized_mf_debug(&filter);
    REQUIRE(optimized_mf_get_median(&filter) == 1);
    optimized_mf_insert_value(&filter, 3);
    optimized_mf_debug(&filter);
    REQUIRE(optimized_mf_get_median(&filter) == 2);
    optimized_mf_insert_value(&filter, 4);
    optimized_mf_debug(&filter);
}
