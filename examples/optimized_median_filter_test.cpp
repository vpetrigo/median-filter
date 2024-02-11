/**
 * \file
 * \brief
 * \author
 */

#include "optimized_median_filter.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/internal/catch_run_context.hpp>

#include <algorithm>
#include <array>
#include <random>

std::array<unsigned, 1024> generate_random_data()
{
    std::mt19937 mersenne_engine{Catch::rngSeed()};
    std::uniform_int_distribution<unsigned> dist{std::numeric_limits<unsigned>::min(),
                                                 std::numeric_limits<unsigned>::max()};
    std::array<unsigned, 1024> random_data{};

    std::generate(std::begin(random_data), std::end(random_data),
                  [&dist, &mersenne_engine] { return dist(mersenne_engine); });

    return random_data;
}

TEST_CASE("Optimized median filter", "[optimized_median_filter]")
{
    struct optimized_median_filter filter;
    struct median_filter_node buffer[3];

    optimized_mf_init(&filter, buffer, 3);

    SECTION("First simple test")
    {
        optimized_mf_insert_value(&filter, 1);
        optimized_mf_debug(&filter);
        REQUIRE(optimized_mf_get_median(&filter) == 1);
        optimized_mf_insert_value(&filter, 2);
        optimized_mf_debug(&filter);
        REQUIRE(optimized_mf_get_median(&filter) == 2);
        optimized_mf_insert_value(&filter, 3);
        optimized_mf_debug(&filter);
        REQUIRE(optimized_mf_get_median(&filter) == 2);
        optimized_mf_insert_value(&filter, 4);
        optimized_mf_debug(&filter);
        REQUIRE(optimized_mf_get_median(&filter) == 3);
    }

    SECTION("Insert same value")
    {
        const unsigned value = 1;
        optimized_mf_insert_value(&filter, value);
        REQUIRE(optimized_mf_get_median(&filter) == value);
        optimized_mf_insert_value(&filter, value);
        REQUIRE(optimized_mf_get_median(&filter) == value);
        optimized_mf_insert_value(&filter, value);
        REQUIRE(optimized_mf_get_median(&filter) == value);
        optimized_mf_insert_value(&filter, value);
        REQUIRE(optimized_mf_get_median(&filter) == value);
    }

    SECTION("Large input")
    {
        std::array<unsigned, 1024> input{generate_random_data()};

        for (const auto &e : input) {
            optimized_mf_insert_value(&filter, e);
        }

        std::sort(std::begin(input) + 1021, std::end(input));
        const auto expected = *(std::begin(input) + 1022);
        REQUIRE(optimized_mf_get_median(&filter) == expected);
    }
}
