cmake_minimum_required(VERSION 3.27)
project(median_filter)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

option(ENABLE_TESTING "Enable library tests" OFF)
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
endif ()

if (ENABLE_EXAMPLES)
    add_subdirectory(example/)
endif ()

add_subdirectory(src/)

add_library(median_filter::median_filter ALIAS median_filter)
add_library(median_filter::median_filter_cpp ALIAS median_filter_cpp)

include(GNUInstallDirs)
configure_file(${PROJECT_SOURCE_DIR}/cmake/pkg-config.pc.in ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc @ONLY)
include(CMakePackageConfigHelpers)
configure_package_config_file(${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
        PATH_VARS CMAKE_INSTALL_LIBDIR)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h")
install(TARGETS median_filter EXPORT ${PROJECT_NAME}-targets INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS median_filter_cpp EXPORT ${PROJECT_NAME}-targets INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT ${PROJECT_NAME}-targets NAMESPACE ${PROJECT_NAME}:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME})
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc" DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake" DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME})
