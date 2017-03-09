cmake_minimum_required (VERSION 2.6)
project (puzzle_lib_tests)

set (puzzle_lib_tests_VERSION_MAJOR 1)
set (puzzle_lib_tests_VERSION_MINOR 0)

add_executable(puzzle_lib_tests 
    string_twist_test.c
)

target_link_libraries(puzzle_lib_tests
       	OpenCL
       	pms_common
	pms_puzzle_lib
)
