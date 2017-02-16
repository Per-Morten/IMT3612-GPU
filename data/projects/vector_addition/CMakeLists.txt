cmake_minimum_required (VERSION 2.6)
project (vector_addition)

# The version number.
set (vector_addition_VERSION_MAJOR 1)
set (vector_addition_VERSION_MINOR 0)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}"
                    "/opt/intel/opencl-1.2-sdk-6.3.0.1904/include/"
                    "../common/")

set(GCC_FLAGS "-Wall -O2 -std=c11 -fopenmp")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_FLAGS}")


add_executable(vector_addition 
    vector_addition.c
)
target_link_libraries(vector_addition OpenCL pms_common)
