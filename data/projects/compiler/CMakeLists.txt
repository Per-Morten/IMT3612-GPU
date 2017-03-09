cmake_minimum_required(VERSION 2.6)
project(compiler)

set(compiler_VERSION_MAJOR 1)
set(compiler_VERSION_MINOR 0)

add_executable(compiler 
    compiler.c
)

target_link_libraries(compiler OpenCL pms_common)
