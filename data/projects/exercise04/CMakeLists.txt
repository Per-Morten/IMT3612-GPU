cmake_minimum_required(VERSION 2.6)
project(exercise04)

set(exercise04_VERSION_MAJOR 1)
set(exercise04_VERSION_MINOR 0)

add_executable(exercise04 
    exercise04.c
)
target_link_libraries(exercise04 OpenCL pms_common m)
