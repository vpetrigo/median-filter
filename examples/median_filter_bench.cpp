/**
 * \file
 * \brief
 * \author
 */

#include "naive_median_filter.h"

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <random>

std::vector<unsigned> generate_random_data()
{
    std::random_device rnd_device;
    std::mt19937 mersenne_engine{rnd_device()};
    std::uniform_int_distribution<unsigned> dist{std::numeric_limits<unsigned>::min(),
                                                 std::numeric_limits<unsigned>::max()};
    std::vector<unsigned> random_data(4096);

    std::generate(std::begin(random_data), std::end(random_data),
                  [&dist, &mersenne_engine] { return dist(mersenne_engine); });

    return random_data;
}

TEST_CASE("Naive median filter", "[naive_median_filter]")
{
    BENCHMARK_ADVANCED("Window size = 3")(Catch::Benchmark::Chronometer meter)
    {
        const auto random_data = generate_random_data();
        struct median_filter filter;
        unsigned buffer[6];

        mf_init(&filter, buffer, buffer + 3, 3);

        meter.measure([&random_data, &filter] {
            for (const auto &e : random_data) {
                mf_insert_value(&filter, e);
            }
        });
    };

    BENCHMARK_ADVANCED("Window size = 16")
    (Catch::Benchmark::Chronometer meter)
    {
        const auto random_data = generate_random_data();
        struct median_filter filter;
        unsigned buffer[32];

        mf_init(&filter, buffer, buffer + 16, 16);

        meter.measure([&random_data, &filter] {
            for (const auto &e : random_data) {
                mf_insert_value(&filter, e);
            }
        });
    };

    BENCHMARK_ADVANCED("Window size = 128")
    (Catch::Benchmark::Chronometer meter)
    {
        const auto random_data = generate_random_data();
        struct median_filter filter;
        unsigned buffer[256];

        mf_init(&filter, buffer, buffer + 128, 128);

        meter.measure([&random_data, &filter] {
            for (const auto &e : random_data) {
                mf_insert_value(&filter, e);
            }
        });
    };

    BENCHMARK_ADVANCED("Window size = 1024")
    (Catch::Benchmark::Chronometer meter)
    {
        const auto random_data = generate_random_data();
        struct median_filter filter;
        unsigned buffer[2048];

        mf_init(&filter, buffer, buffer + 1024, 1024);

        meter.measure([&random_data, &filter] {
            for (const auto &e : random_data) {
                mf_insert_value(&filter, e);
            }
        });
    };
}