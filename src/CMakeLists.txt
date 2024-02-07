add_library(median_filter median_filter.c median_filter.c)
target_include_directories(median_filter PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>)

add_library(median_filter_cpp INTERFACE)
target_include_directories(median_filter_cpp INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>)

if (ENABLE_TESTING)
    add_executable(median_filter_test median_filter_test.cpp)
    target_link_libraries(median_filter_test PRIVATE Catch2::Catch2WithMain median_filter::median_filter)
    add_test(NAME median_filter_test COMMAND median_filter_test)
endif ()