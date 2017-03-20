cmake_minimum_required (VERSION 2.6)
project (string_puzzle)

set (string_puzzle_VERSION_MAJOR 1)
set (string_puzzle_VERSION_MINOR 0)

add_executable(string_puzzle 
    string_puzzle.c
)

target_link_libraries(string_puzzle
       	OpenCL
       	pms_common
	    pms_puzzle_lib
)
