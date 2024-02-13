add_library(median_filter median_filter.c median_filter.h)
target_include_directories(median_filter PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>)
add_coverage_flags(median_filter)

if (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(median_filter PRIVATE -Wall -Wextra -Werror -Wconversion)
elseif (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(median_filter PRIVATE /W3 /WX)
endif ()

if (ENABLE_TESTING)
    add_executable(median_filter_test median_filter_test.cpp)
    target_link_libraries(median_filter_test PRIVATE Catch2::Catch2WithMain median_filter::median_filter)
    add_coverage_flags(median_filter_test)
    add_test(NAME median_filter_test COMMAND median_filter_test)
endif ()
