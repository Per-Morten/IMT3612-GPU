cmake_minimum_required (VERSION 2.6)
project (kernel_tests)

# The version number.
set (kernel_tests_VERSION_MAJOR 1)
set (kernel_tests_VERSION_MINOR 0)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}"
                    "/opt/intel/opencl-1.2-sdk-6.3.0.1904/include/"
		    "${CMAKE_SOURCE_DIR}/common")

set(GCC_FLAGS "-Wall -O3 -std=c11 -fopenmp")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_FLAGS}")


add_executable(kernel_tests 
    kernel_tests.c
)
target_link_libraries(kernel_tests OpenCL pms_common)
