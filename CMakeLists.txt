cmake_minimum_required(VERSION 3.27)
project(median-filter)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

option(ENABLE_TESTING "Enable library tests" OFF)

if (ENABLE_TESTING)
    include(FetchContent)

    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.5.2 # or a later release
    )

    FetchContent_MakeAvailable(Catch2)

    enable_testing()
endif ()

add_subdirectory(src/)

add_library(median_filter::median_filter ALIAS median_filter)
add_library(median_filter::median_filter_cpp ALIAS median_filter_cpp)
