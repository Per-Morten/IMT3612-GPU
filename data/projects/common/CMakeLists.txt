cmake_minimum_required (VERSION 2.6)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(pms_common 
    pms_build_program.c
    pms_cleanup_opencl.c
    pms_create_program.c
    pms_device_info.c 
    pms_error.c 
    pms_get_device_id.c
    pms_log.c
    pms_read_kernel.c
    pms_setup_opencl_cpu.c
)

target_link_libraries(pms_common OpenCL)
