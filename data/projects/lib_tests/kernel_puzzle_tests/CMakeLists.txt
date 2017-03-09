cmake_minimum_required(VERSION 2.6)
project(kernel_puzzle_tests)

set(kernel_puzzle_tests_VERSION_MAJOR 1)
set(kernel_puzzle_tests_VERSION_MINOR 0)

add_executable(kernel_puzzle_tests 
    kernel_puzzle_tests.c
)
target_link_libraries(kernel_puzzle_tests OpenCL pms_common)
