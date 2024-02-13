/**
 * \file
 * \brief
 * \author Vladimir Petrigo
 */

#include "median_filter.h"

#include <catch2/catch_template_test_macros.hpp>

#include <algorithm>
#include <array>
#include <iostream>
#include <random>

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

template <typename T>
constexpr auto get_filter_node();

#define GET_FILTER_NODE(ty)                                                                                            \
    template <>                                                                                                        \
    constexpr auto get_filter_node<ty>()                                                                               \
    {                                                                                                                  \
        return median_filter_node_##ty{};                                                                              \
    }

GET_FILTER_NODE(float)
GET_FILTER_NODE(double)
GET_FILTER_NODE(uint8_t)
GET_FILTER_NODE(uint16_t)
GET_FILTER_NODE(uint32_t)
GET_FILTER_NODE(uint64_t)
GET_FILTER_NODE(int8_t)
GET_FILTER_NODE(int16_t)
GET_FILTER_NODE(int32_t)
GET_FILTER_NODE(int64_t)

template <typename T, typename std::enable_if<std::is_integral<T>::value>::type * = nullptr>
auto get_distribution()
{
    return std::uniform_int_distribution<T>{std::numeric_limits<T>::min(), std::numeric_limits<T>::max()};
}

template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type * = nullptr>
auto get_distribution()
{
    return std::uniform_real_distribution<T>{std::numeric_limits<T>::min(), std::numeric_limits<T>::max()};
}

TEMPLATE_TEST_CASE("Median filter", "[median_filter]", std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t,
                   std::int8_t, std::int16_t, std::int32_t, std::int64_t, float, double)
{
    auto filter = get_filter<TestType>();
    using InputType = std::vector<decltype(get_filter_node<TestType>())>;
    using TestCase = std::pair<std::vector<TestType>, TestType>;
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
                {TestType{3}, TestType{3}, TestType{3}, TestType{3}, TestType{1}, TestType{3}},
                TestType{3},
            },
            {
                {TestType{0}, TestType{1}, TestType{3}, TestType{2}, TestType{1}, TestType{2}},
                TestType{2},
            },
            {
                {TestType{10}, TestType{127}, TestType{1}},
                TestType{10},
            },
        };

        for (const auto &test : tests) {
            REQUIRE(median_filter_init(&filter, buffer.data(), buffer.size()));
            for (const auto &sample : test.first) {
                REQUIRE(median_filter_insert_value(&filter, sample));
            }

            REQUIRE(median_filter_get_median(&filter) == test.second);
        }
    }

    SECTION("invalid input")
    {
        CHECK_FALSE(median_filter_init(nullptr, buffer.data(), buffer.size()));
        CHECK_FALSE(
            median_filter_init(&filter, static_cast<decltype(get_filter_node<TestType>()) *>(nullptr), buffer.size()));
        CHECK_FALSE(median_filter_init(&filter, buffer.data(), 0));
        CHECK_FALSE(median_filter_init(&filter, buffer.data(), 1));
        CHECK_FALSE(median_filter_init(&filter, static_cast<decltype(get_filter_node<TestType>()) *>(nullptr), 2));
    }

    SECTION("invalid insert")
    {
        CHECK_FALSE(median_filter_insert_value(nullptr, TestType{0}));
    }

    SECTION("large buffer")
    {
        buffer.resize(1024);
        std::random_device rnd_device;
        std::mt19937 mersenne_engine{rnd_device()};
        auto dist = get_distribution<TestType>();
        std::vector<TestType> random_data(1024);

        std::generate(random_data.begin(), random_data.end(),
                      [&mersenne_engine, &dist]() { return dist(mersenne_engine); });

        REQUIRE(median_filter_init(&filter, buffer.data(), buffer.size()));

        for (const auto &e : random_data) {
            median_filter_insert_value(&filter, e);
        }

        std::sort(random_data.begin(), random_data.end());
        const auto &expected = random_data[random_data.size() / 2];

        REQUIRE(median_filter_get_median(&filter) == expected);
    }

    SECTION("large input with small filter buffer (even buffer size)")
    {
        buffer.resize(4);
        std::random_device rnd_device;
        std::mt19937 mersenne_engine{rnd_device()};
        auto dist = get_distribution<TestType>();
        std::vector<TestType> random_data(1024);

        std::generate(random_data.begin(), random_data.end(),
                      [&mersenne_engine, &dist]() { return dist(mersenne_engine); });

        REQUIRE(median_filter_init(&filter, buffer.data(), buffer.size()));
        std::vector<TestType> random_data_input(buffer.size());
        std::size_t begin = 0;
        std::size_t counter = 0;

        for (const auto &e : random_data) {
            median_filter_insert_value(&filter, e);
            ++counter;

            if (counter % buffer.size() == 0) {
                std::copy_n(std::cbegin(random_data) + begin, random_data_input.size(), std::begin(random_data_input));
                REQUIRE(random_data_input.size() == buffer.size());
                begin = counter;
                std::sort(std::begin(random_data_input), std::end(random_data_input));
                const auto &expected = random_data_input[random_data_input.size() / 2];
                REQUIRE(median_filter_get_median(&filter) == expected);
            }
        }
    }

    SECTION("large input with small filter buffer (odd buffer size)")
    {
        buffer.resize(5);
        std::random_device rnd_device;
        std::mt19937 mersenne_engine{rnd_device()};
        auto dist = get_distribution<TestType>();
        std::vector<TestType> random_data(1024);

        std::generate(random_data.begin(), random_data.end(),
                      [&mersenne_engine, &dist]() { return dist(mersenne_engine); });

        REQUIRE(median_filter_init(&filter, buffer.data(), buffer.size()));
        std::vector<TestType> random_data_input(buffer.size());
        std::size_t begin = 0;
        std::size_t counter = 0;

        for (const auto &e : random_data) {
            median_filter_insert_value(&filter, e);
            ++counter;

            if (counter % buffer.size() == 0) {
                std::copy_n(std::cbegin(random_data) + begin, random_data_input.size(), std::begin(random_data_input));
                REQUIRE(random_data_input.size() == buffer.size());
                begin = counter;
                std::sort(std::begin(random_data_input), std::end(random_data_input));
                const auto &expected = random_data_input[random_data_input.size() / 2];
                REQUIRE(median_filter_get_median(&filter) == expected);
            }
        }
    }
}
