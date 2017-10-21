cmake_minimum_required(VERSION 2.6)
project(exercise02)

set(exercise02_VERSION_MAJOR 1)
set(exercise02_VERSION_MINOR 0)

add_executable(exercise02 
    exercise02.c
)
target_link_libraries(exercise02 OpenCL pms_common m)
