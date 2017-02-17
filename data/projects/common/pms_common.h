#pragma once
#include <CL/cl.h>
#include <stdarg.h>
#include <stdio.h>

#define PMS_CHECK_CL_ERROR(E, S) pms_check_cl_error(E,S,__FILE__,__LINE__)

#define PMS_ERROR(fmt, ...) \
pms_log(stderr, "ERROR" ,__func__, __LINE__, fmt, #__VA_ARGS__); \
exit(EXIT_FAILURE);

#define PMS_INFO(fmt, ...) \
pms_log(stdout, "INFO", __func__, __LINE__, fmt, #__VA_ARGS__);

#define PMS_FAILURE -1
#define PMS_SUCCESS 0

///////////////////////////////////////////////////////////////////////////////
/// \brief Do not use this function, use macros PMS_LOG_INFO and PMS_LOG_ERROR
///        instead.
/// \todo Integrate proper filename, i.e. remove all parts except the actual
///       filename.
///////////////////////////////////////////////////////////////////////////////
void
pms_log(FILE* file,
        const char* type,
        //const char* filename,
        const char* func,
        const int line,
        const char* fmt,
        ...);

const char* 
pms_stringify_error(const cl_int status_code);

void 
pms_check_cl_error(const cl_int status,
                   const char* operation,
                   const char* filename,
                   const int32_t line);

int32_t 
pms_output_device_info(const cl_device_id device_id);

int32_t
pms_read_kernel(const char* filename, 
                char* buffer, 
                size_t size_of_buffer);
