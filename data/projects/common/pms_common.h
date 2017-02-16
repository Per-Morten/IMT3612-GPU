#pragma once
#include <CL/cl.h>
#define PMS_CHECK_ERROR(E, S) pms_check_error(E,S,__FILE__,__LINE__)

#define PMS_FAILURE -1
#define PMS_SUCCESS 0

const char* 
pms_stringify_error(const cl_int status_code);

void 
pms_check_error(const cl_int status,
                const char* operation,
                const char* filename,
                const int32_t line);

int32_t 
pms_output_device_info(const cl_device_id device_id);

int32_t
pms_read_kernel(const char* filename, 
                char* buffer, 
                size_t size_of_buffer);
