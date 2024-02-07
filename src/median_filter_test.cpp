/**
 * \file
 * \brief
 * \author Vladimir Petrigo
 */

#include "median_filter.h"

#include <catch2/catch_test_macros.hpp>

#include <array>

TEST_CASE("Median filter", "[median filter (unsigned) - small buffer]")
{
    using TestType = std::array<unsigned, 3>;
    using TestCase = std::pair<TestType, unsigned>;
    struct median_filter_unsigned filter {};
    unsigned buffer[3];

    std::vector<TestCase> tests{
        {
            {0, 0, 0},
            0,
        },
        {
            {0, 0, 1},
            0,
        },
        {
            {0, 1, 1},
            1,
        },
        {
            {1, 1, 1},
            1,
        },
        {
            {1, 2, 3},
            2,
        },
        {
            {3, 2, 1},
            2,
        },
        {
            {10, 220, 1},
            10,
        },
    };

    for (const auto &test : tests) {
        REQUIRE(median_filter_init_unsigned(&filter, buffer, test.first.size()));

        for (const auto &sample : test.first) {
            REQUIRE(median_filter_insert_number_unsigned(&filter, sample));
        }

        REQUIRE(filter.size == filter.buffer_size);
        REQUIRE(median_filter_get_median_unsigned(&filter) == test.second);
    }
}

TEST_CASE("Median filter", "[median filter (int) - small buffer]")
{
    using TestType = std::array<int, 3>;
    using TestCase = std::pair<TestType, int>;
    struct median_filter_unsigned filter {};
    unsigned buffer[3];

    std::vector<TestCase> tests{
        {
            {0, 0, 0},
            0,
        },
        {
            {0, 0, 1},
            0,
        },
        {
            {0, 1, 1},
            1,
        },
        {
            {1, 1, 1},
            1,
        },
        {
            {1, 2, 3},
            2,
        },
        {
            {3, 2, 1},
            2,
        },
        {
            {10, 220, 1},
            10,
        },
    };

    for (const auto &test : tests) {
        median_filter_init(&filter, buffer, test.first.size());
        REQUIRE(median_filter_init_unsigned(&filter, buffer, test.first.size()));

        for (const auto &sample : test.first) {
            REQUIRE(median_filter_insert_number_unsigned(&filter, sample));
        }

        REQUIRE(filter.size == filter.buffer_size);
        REQUIRE(median_filter_get_median_unsigned(&filter) == test.second);
    }
}
