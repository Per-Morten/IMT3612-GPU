cmake_minimum_required(VERSION 2.6)
project(kernel_string_tests)

set(kernel_string_tests_VERSION_MAJOR 1)
set(kernel_string_tests_VERSION_MINOR 0)

add_executable(kernel_string_tests 
    kernel_string_tests.c
)
target_link_libraries(kernel_string_tests OpenCL pms_common)
