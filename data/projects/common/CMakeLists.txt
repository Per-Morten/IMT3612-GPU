cmake_minimum_required (VERSION 2.6)
include_directories(${PROJECT_SOURCE_DIR}/common)

add_library(pms_common 
    pms_device_info.c 
    pms_error.c 
    pms_read_kernel.c
)
