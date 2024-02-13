cmake_minimum_required(VERSION 3.21)
project(median_filter C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

macro(add_coverage_flags target)
    if (ENABLE_TESTING)
        target_compile_options(${target} PRIVATE --coverage)
        target_link_options(${target} PRIVATE --coverage)
    endif ()
endmacro()

if (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(INSTALL_LIB ON)
else ()
    set(INSTALL_LIB OFF)
endif ()

option(ENABLE_TESTING "Enable library tests" OFF)
option(ENABLE_BENCHMARK "Enable example benchmarks" OFF)
option(BUILD_EXAMPLES "Enable examples building" OFF)

if (ENABLE_TESTING)
    include(FetchContent)

    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.5.2 # or a later release
    )

    FetchContent_MakeAvailable(Catch2)

    enable_testing()
    add_custom_target(coverage
            COMMAND gcovr -r ${CMAKE_CURRENT_LIST_DIR} -e ".*_test\.cpp.*" -e ".*/examples/.*" -x coverage.info
    )
endif ()

if (ENABLE_EXAMPLES)
    add_subdirectory(examples/)
endif ()

add_subdirectory(src/)

add_library(median_filter::median_filter ALIAS median_filter)

if (INSTALL_LIB)
    include(GNUInstallDirs)
    configure_file(${PROJECT_SOURCE_DIR}/cmake/pkg-config.pc.in ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc @ONLY)
    include(CMakePackageConfigHelpers)
    configure_package_config_file(${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake.in
            ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
            INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
            PATH_VARS CMAKE_INSTALL_LIBDIR)

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h")
    install(TARGETS median_filter EXPORT ${PROJECT_NAME}-targets INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install(EXPORT ${PROJECT_NAME}-targets NAMESPACE ${PROJECT_NAME}:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME})
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc" DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake" DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME})
endif ()
