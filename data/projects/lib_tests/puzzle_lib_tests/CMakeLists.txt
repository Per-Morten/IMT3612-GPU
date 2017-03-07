cmake_minimum_required (VERSION 2.6)
project (puzzle_lib_tests)

# The version number.
set (puzzle_lib_tests_VERSION_MAJOR 1)
set (puzzle_lib_tests_VERSION_MINOR 0)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}"
                    "/opt/intel/opencl-1.2-sdk-6.3.0.1904/include/"
		    "${CMAKE_SOURCE_DIR}/common"
		    "${CMAKE_SOURCE_DIR}/puzzle_lib")

set(GCC_FLAGS "-Wall -O3 -std=c11 -fopenmp")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_FLAGS}")


add_executable(puzzle_lib_tests 
    string_twist_test.c
)

target_link_libraries(puzzle_lib_tests
       	OpenCL
       	pms_common
	pms_puzzle_lib
)
