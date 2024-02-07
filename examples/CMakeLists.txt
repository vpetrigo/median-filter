add_executable(c_example c_example.c)
target_link_libraries(c_example PRIVATE median_filter::median_filter)
