/**
 * \file
 * \brief
 * \author Vladimir Petrigo
 */

#include "median_filter.h"

#include <catch2/catch_template_test_macros.hpp>

#include <algorithm>
#include <array>
#include <random>
#include <iostream>

template <typename T>
using InputType = std::vector<T>;

template <typename T>
auto get_filter();

template <>
auto get_filter<std::uint8_t>()
{
    return median_filter_uint8_t{};
}

template <>
auto get_filter<std::uint16_t>()
{
    return median_filter_uint16_t{};
}

template <>
auto get_filter<std::uint32_t>()
{
    return median_filter_uint32_t{};
}

template <>
auto get_filter<std::uint64_t>()
{
    return median_filter_uint64_t{};
}

template <>
auto get_filter<std::int8_t>()
{
    return median_filter_int8_t{};
}

template <>
auto get_filter<std::int16_t>()
{
    return median_filter_int16_t{};
}

template <>
auto get_filter<std::int32_t>()
{
    return median_filter_int32_t{};
}

template <>
auto get_filter<std::int64_t>()
{
    return median_filter_int64_t{};
}

template <>
auto get_filter<float>()
{
    return median_filter_float{};
}

template <>
auto get_filter<double>()
{
    return median_filter_double{};
}

TEMPLATE_TEST_CASE("Median filter (unsigned)", "[median_filter]", std::uint8_t, std::uint16_t, std::uint32_t,
                   std::uint64_t)
{
    auto filter = get_filter<TestType>();
    using InputType = std::vector<TestType>;
    using TestCase = std::pair<InputType, TestType>;
    auto buffer = InputType{};

    SECTION("small buffer")
    {
        buffer.resize(3);
        std::vector<TestCase> tests{
            {
                {TestType{0}, TestType{0}, TestType{0}},
                TestType{0},
            },
            {
                {TestType{0}, TestType{0}, TestType{1}},
                TestType{0},
            },
            {
                {TestType{0}, TestType{1}, TestType{1}},
                TestType{1},
            },
            {
                {TestType{1}, TestType{1}, TestType{1}},
                TestType{1},
            },
            {
                {TestType{1}, TestType{2}, TestType{3}},
                TestType{2},
            },
            {
                {TestType{3}, TestType{2}, TestType{1}},
                TestType{2},
            },
            {
                {TestType{10}, TestType{220}, TestType{1}},
                TestType{10},
            },
        };

        for (const auto &test : tests) {
            REQUIRE(median_filter_init(&filter, buffer.data(), test.first.size()));
            for (const auto &sample : test.first) {
                REQUIRE(median_filter_insert_number(&filter, sample));
            }
            REQUIRE(filter.size == filter.buffer_size);
            REQUIRE(median_filter_get_median(&filter) == test.second);
        }
    }

    SECTION("invalid input")
    {
        CHECK_FALSE(median_filter_init(nullptr, buffer.data(), buffer.size()));
        CHECK_FALSE(median_filter_init(&filter, static_cast<TestType *>(nullptr), buffer.size()));
        CHECK_FALSE(median_filter_init(&filter, buffer.data(), 0));
        CHECK_FALSE(median_filter_init(&filter, buffer.data(), 1));
    }

    SECTION("invalid insert")
    {
        CHECK_FALSE(median_filter_insert_number(nullptr, TestType{0}));
    }

    SECTION("large buffer")
    {
        buffer.resize(1024);
        std::random_device rnd_device;
        std::mt19937 mersenne_engine{rnd_device()};
        std::uniform_int_distribution<TestType> dist{std::numeric_limits<TestType>::min(),
                                                     std::numeric_limits<TestType>::max()};
        std::vector<TestType> random_data(1024);

        std::generate(random_data.begin(), random_data.end(),
                      [&mersenne_engine, &dist]() { return dist(mersenne_engine); });

        REQUIRE(median_filter_init(&filter, buffer.data(), buffer.size()));

        for (const auto &e : random_data) {
            median_filter_insert_number(&filter, e);
        }

        std::sort(random_data.begin(), random_data.end());
        const auto &expected = random_data[random_data.size() / 2];

        REQUIRE(median_filter_get_median(&filter) == expected);
    }

    SECTION("large input with small filter buffer")
    {
        buffer.resize(4);
        std::random_device rnd_device;
        std::mt19937 mersenne_engine{rnd_device()};
        std::uniform_int_distribution<TestType> dist{std::numeric_limits<TestType>::min(),
                                                     std::numeric_limits<TestType>::max()};
        std::vector<TestType> random_data(1024);

        std::generate(random_data.begin(), random_data.end(),
                      [&mersenne_engine, &dist]() { return dist(mersenne_engine); });

        REQUIRE(median_filter_init(&filter, buffer.data(), buffer.size()));
        std::vector<TestType> random_data_input(buffer.size());
        std::size_t begin = 0;
        std::size_t counter = 0;

        for (const auto &e : random_data) {
            median_filter_insert_number(&filter, e);
            ++counter;

            std::cout << "inserted " << e << '\n';

            if (counter % buffer.size() == 0) {
                std::copy_n(std::cbegin(random_data) + begin, random_data_input.size(), std::begin(random_data_input));
                REQUIRE(random_data_input.size() == 4);
                begin = counter;
                std::sort(std::begin(random_data_input), std::end(random_data_input));
                const auto &expected = random_data_input[random_data_input.size() / 2];
                REQUIRE(median_filter_get_median(&filter) == expected);
            }
        }
    }
}

TEMPLATE_TEST_CASE("Median filter (signed)", "[median_filter]", std::int8_t, std::int16_t, std::int32_t, std::int64_t)
{
    auto filter = get_filter<TestType>();
    using TestCase = std::pair<InputType<TestType>, TestType>;
    auto buffer = InputType<TestType>{};

    SECTION("small buffer")
    {
        buffer.resize(3);
        std::vector<TestCase> tests{
            {
                {TestType{0}, TestType{0}, TestType{0}},
                TestType{0},
            },
            {
                {TestType{0}, TestType{0}, TestType{1}},
                TestType{0},
            },
            {
                {TestType{0}, TestType{1}, TestType{1}},
                TestType{1},
            },
            {
                {TestType{1}, TestType{1}, TestType{1}},
                TestType{1},
            },
            {
                {TestType{1}, TestType{2}, TestType{3}},
                TestType{2},
            },
            {
                {TestType{3}, TestType{2}, TestType{1}},
                TestType{2},
            },
            {
                {TestType{10}, TestType{127}, TestType{1}},
                TestType{10},
            },
        };

        for (const auto &test : tests) {
            REQUIRE(median_filter_init(&filter, buffer.data(), test.first.size()));
            for (const auto &sample : test.first) {
                REQUIRE(median_filter_insert_number(&filter, sample));
            }
            REQUIRE(filter.size == filter.buffer_size);
            REQUIRE(median_filter_get_median(&filter) == test.second);
        }
    }

    SECTION("invalid input")
    {
        buffer.resize(3);
        CHECK_FALSE(median_filter_init(nullptr, buffer.data(), buffer.size()));
        CHECK_FALSE(median_filter_init(&filter, static_cast<TestType *>(nullptr), buffer.size()));
        CHECK_FALSE(median_filter_init(&filter, buffer.data(), 0));
        CHECK_FALSE(median_filter_init(&filter, buffer.data(), 1));
    }

    SECTION("invalid insert")
    {
        CHECK_FALSE(median_filter_insert_number(nullptr, TestType{0}));
    }

    SECTION("large buffer")
    {
        buffer.resize(1024);
        std::random_device rnd_device;
        std::mt19937 mersenne_engine{rnd_device()};
        std::uniform_int_distribution<TestType> dist{std::numeric_limits<TestType>::min(),
                                                     std::numeric_limits<TestType>::max()};
        std::vector<TestType> random_data(1024);

        std::generate(random_data.begin(), random_data.end(),
                      [&mersenne_engine, &dist]() { return dist(mersenne_engine); });

        REQUIRE(median_filter_init(&filter, buffer.data(), buffer.size()));

        for (const auto &e : random_data) {
            median_filter_insert_number(&filter, e);
        }

        std::sort(random_data.begin(), random_data.end());
        const auto &expected = random_data[random_data.size() / 2];

        REQUIRE(median_filter_get_median(&filter) == expected);
    }
}

TEMPLATE_TEST_CASE("Median filter (float)", "[median_filter]", float, double)
{
    auto filter = get_filter<TestType>();
    using TestCase = std::pair<InputType<TestType>, TestType>;
    auto buffer = InputType<TestType>{};

    SECTION("small buffer")
    {
        buffer.resize(3);

        std::vector<TestCase> tests{
            {
                {TestType{0}, TestType{0}, TestType{0}},
                TestType{0},
            },
            {
                {TestType{0}, TestType{0}, TestType{1}},
                TestType{0},
            },
            {
                {TestType{0}, TestType{1}, TestType{1}},
                TestType{1},
            },
            {
                {TestType{1}, TestType{1}, TestType{1}},
                TestType{1},
            },
            {
                {TestType{1}, TestType{2}, TestType{3}},
                TestType{2},
            },
            {
                {TestType{3}, TestType{2}, TestType{1}},
                TestType{2},
            },
            {
                {TestType{10}, TestType{127}, TestType{1}},
                TestType{10},
            },
        };

        for (const auto &test : tests) {
            REQUIRE(median_filter_init(&filter, buffer.data(), test.first.size()));
            for (const auto &sample : test.first) {
                REQUIRE(median_filter_insert_number(&filter, sample));
            }
            REQUIRE(filter.size == filter.buffer_size);
            REQUIRE(median_filter_get_median(&filter) == test.second);
        }
    }

    SECTION("invalid input")
    {
        buffer.resize(3);
        CHECK_FALSE(median_filter_init(nullptr, buffer.data(), buffer.size()));
        CHECK_FALSE(median_filter_init(&filter, static_cast<TestType *>(nullptr), buffer.size()));
        CHECK_FALSE(median_filter_init(&filter, buffer.data(), 0));
        CHECK_FALSE(median_filter_init(&filter, buffer.data(), 1));
    }

    SECTION("invalid insert")
    {
        CHECK_FALSE(median_filter_insert_number(nullptr, TestType{0}));
    }

    SECTION("large buffer")
    {
        buffer.resize(1024);
        std::random_device rnd_device;
        std::mt19937 mersenne_engine{rnd_device()};
        std::uniform_real_distribution<TestType> dist{std::numeric_limits<TestType>::min(),
                                                      std::numeric_limits<TestType>::max()};
        std::vector<TestType> random_data(1024);

        std::generate(random_data.begin(), random_data.end(),
                      [&mersenne_engine, &dist]() { return dist(mersenne_engine); });

        REQUIRE(median_filter_init(&filter, buffer.data(), buffer.size()));

        for (const auto &e : random_data) {
            median_filter_insert_number(&filter, e);
        }

        std::sort(random_data.begin(), random_data.end());
        const auto &expected = random_data[random_data.size() / 2];

        REQUIRE(median_filter_get_median(&filter) == expected);
    }
}
