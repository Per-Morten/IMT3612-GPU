cmake_minimum_required(VERSION 2.6)
project(exercise05)

set(exercise05_VERSION_MAJOR 1)
set(exercise05_VERSION_MINOR 0)

add_executable(exercise05 
    exercise05.c
)
target_link_libraries(exercise05 OpenCL pms_common m)
