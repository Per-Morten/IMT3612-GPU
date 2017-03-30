/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Taken from: 
/// https://github.com/HandsOnOpenCL/Exercises-Solutions/blob/master/Exercises/C_common/device_info.c
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <CL/cl.h>
#include <pms_common.h>

// not supported: CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE
// not supported: CL_DEVICE_PARENT_DEVICE  
// not supported: CL_DEVICE_PARTITION_PROPERTIES
// not supported: CL_DEVICE_PARTITION_TYPE
// 

void
print_device_info_string(cl_device_id device_id,
                         cl_device_info info, 
                         const char* info_name)
{
    char buffer[1024];
    cl_int error = clGetDeviceInfo(device_id, info, sizeof(buffer), buffer, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Could not access %s", info_name);
        return;
    }
    PMS_INFO("%s: %s", info_name, buffer);
}

void
print_device_info_bool(cl_device_id device_id,
                       cl_device_info info,
                       const char* info_name)
{
    cl_bool buffer;
    cl_int error = clGetDeviceInfo(device_id, info, sizeof(buffer), &buffer, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Could not access %s", info_name);
        return;
    }
    PMS_INFO("%s: %d", info_name, buffer);
}

void
print_device_info_ulong(cl_device_id device_id,
                        cl_device_info info,
                        const char* info_name)
{
    cl_ulong buffer;
    cl_int error = clGetDeviceInfo(device_id, info, sizeof(buffer), &buffer, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Could not access %s", info_name);
        return;
    }
    PMS_INFO("%s: %u", info_name, buffer);
}

void
print_device_info_uint(cl_device_id device_id,
                       cl_device_info info,
                       const char* info_name)
{
    cl_uint buffer;
    cl_int error = clGetDeviceInfo(device_id, info, sizeof(buffer), &buffer, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Could not access %s", info_name);
        return;
    }
    PMS_INFO("%s: %u", info_name, buffer);
}

void
print_device_info_device_mem_cache_type(cl_device_id device_id,
                                        cl_device_info info,
                                        const char* info_name)
{
    cl_device_mem_cache_type buffer;
    cl_int error = clGetDeviceInfo(device_id, info, sizeof(buffer), &buffer, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Could not access %s", info_name);
        return;
    }
    char string[128] = {0};
    if (buffer & CL_NONE)
    {
        strcat(string, "CL_NONE");
    }
    if (buffer & CL_READ_ONLY_CACHE)
    {
        strcat(string, "CL_READ_ONLY_CACHE");
    }
    if (buffer & CL_READ_WRITE_CACHE)
    {
        strcat(string, "CL_READ_WRITE_CACHE");
    }
    PMS_INFO("%s: %s", info_name, string);
}

void
print_device_info_device_fp_config(cl_device_id device_id,
                                   cl_device_info info,
                                   const char* info_name)
{
    cl_device_fp_config buffer;
    cl_int error = clGetDeviceInfo(device_id, info, sizeof(buffer), &buffer, NULL);

    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Could not access %s", info_name);
        return;
    }

    char string[128] = {0};
    if (buffer & CL_FP_DENORM)
    {
        strcat(string, "CL_FP_DENORM ");
    }
    if (buffer & CL_FP_INF_NAN)
    {
        strcat(string, "CL_FP_INF_NAN ");
    }
    if (buffer & CL_FP_ROUND_TO_NEAREST)
    {
        strcat(string, "CL_FP_ROUND_TO_NEAREST ");
    }
    if (buffer & CL_FP_ROUND_TO_ZERO)
    {
        strcat(string, "CL_FP_ROUND_TO_ZERO ");
    }
    if (buffer & CL_FP_ROUND_TO_INF)
    {
        strcat(string, "CL_FP_ROUND_TO_INF ");
    }
    if (buffer & CL_FP_FMA)
    {
        strcat(string, "CP_FP_FMA ");
    }
    if (buffer & CL_FP_SOFT_FLOAT)
    {
        strcat(string, "CL_FP_SOFT_FLOAT ");
    }
    PMS_INFO("%s: %s", info_name, string);
}

void
print_device_info_size_t(cl_device_id device_id,
                         cl_device_info info,
                         const char* info_name)
{
    size_t buffer;
    cl_int error = clGetDeviceInfo(device_id, info, sizeof(buffer), &buffer, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Could not access %s", info_name);
        return;
    }
    PMS_INFO("%s: %zu", info_name, buffer);
}

void
print_device_info_device_local_mem_type(cl_device_id device_id,
                                        cl_device_info info,
                                        const char* info_name)
{
    cl_device_local_mem_type buffer;
    cl_int error = clGetDeviceInfo(device_id, info, sizeof(buffer), &buffer, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Could not access %s", info_name);
        return;
    }
    char string[128] = {0};
    if (buffer & CL_NONE)
    {
        strcat(string, "CL_NONE");
    }
    if (buffer & CL_LOCAL)
    {
        strcat(string, "CL_LOCAL");
    }
    if (buffer & CL_GLOBAL)
    {
        strcat(string, "CL_GLOBAL");
    }
    PMS_INFO("%s: %s", info_name, string);
}

void
print_device_info_device_exec_capabilities(cl_device_id device_id,
                                           cl_device_info info,
                                           const char* info_name)
{
    cl_device_exec_capabilities buffer;
    cl_int error = clGetDeviceInfo(device_id, info, sizeof(buffer), &buffer, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Could not access %s", info_name);
    }

    char string[128] = {0};
    if (buffer & CL_EXEC_KERNEL)
    {
        strcat(string, "CL_EXEC_KERNEL ");
    }
    if (buffer & CL_EXEC_NATIVE_KERNEL)
    {
        strcat(string, "CL_EXEC_NATIVE_KERNEL ");
    }

    PMS_INFO("%s: %s", info_name, string);
}

void
print_device_info_size_t_array(cl_device_id device_id,
                               cl_device_info info,
                               const char* info_name)
{
    size_t count = 0;
    cl_int error = clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(count), &count, NULL);
    size_t buffer[1024];
    error = clGetDeviceInfo(device_id, info, sizeof(buffer), buffer, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Could not access %s", info_name);
        return;
    }

    char string[2048] = {0};
    for (size_t i = 0; i < count; ++i)
    {
        char buf[2048] = {0};
        sprintf(buf, "%zu ", buffer[i]);
        strcat(string, buf);
    }


    PMS_INFO("%s: %s", info_name, string);
}

void
print_device_info_device_partition_property(cl_device_id device_id,
                                            cl_device_info info,
                                            const char* info_name)
{

}

void
print_device_info_device_affinity_domain(cl_device_id device_id,
                                         cl_device_info info,
                                         const char* info_name)
{    
    cl_device_affinity_domain buffer;
    cl_int error = clGetDeviceInfo(device_id, info, sizeof(buffer), &buffer, NULL);

    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Could not access %s", info_name);
        return;
    }

    char string[128] = {0};
    if (buffer & CL_DEVICE_AFFINITY_DOMAIN_NUMA)
    {
        strcat(string, "CL_DEVICE_AFFINITY_DOMAIN_NUMA ");
    }
    if (buffer & CL_DEVICE_AFFINITY_DOMAIN_L4_CACHE)
    {
        strcat(string, "CL_DEVICE_AFFINITY_DOMAIN_L4_CACHE ");
    }
    if (buffer & CL_DEVICE_AFFINITY_DOMAIN_L3_CACHE)
    {
        strcat(string, "CL_DEVICE_AFFINITY_DOMAIN_L3_CACHE ");
    }
    if (buffer & CL_DEVICE_AFFINITY_DOMAIN_L2_CACHE)
    {
        strcat(string, "CL_DEVICE_AFFINITY_DOMAIN_L2_CACHE ");
    }
    if (buffer & CL_DEVICE_AFFINITY_DOMAIN_L1_CACHE)
    {
        strcat(string, "CL_DEVICE_AFFINITY_DOMAIN_L1_CACHE ");
    }
    if (buffer & CL_DEVICE_AFFINITY_DOMAIN_NEXT_PARTITIONABLE)
    {
        strcat(string, "CL_DEVICE_AFFINITY_DOMAIN_NEXT_PARTITIONABLE ");
    }
    
    PMS_INFO("%s: %s", info_name, string);
}

void
print_device_info_command_queue_properties(cl_device_id device_id,
                                           cl_device_info info,
                                           const char* info_name)
{
    cl_command_queue_properties buffer;
    cl_int error = clGetDeviceInfo(device_id, info, sizeof(buffer), &buffer, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Could not access %s", info_name);
    }

    char string[128] = {0};
    if (buffer & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE)
    {
        strcat(string, "CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE ");
    }
    if (buffer & CL_QUEUE_PROFILING_ENABLE)
    {
        strcat(string, "CL_QUEUE_PROFILING_ENABLE ");
    }

    PMS_INFO("%s: %s", info_name, string);
}

void
print_device_info_device_type(cl_device_id device_id,
                              cl_device_info info,
                              const char* info_name)
{
    cl_device_type buffer;
    cl_int error = clGetDeviceInfo(device_id, info, sizeof(buffer), &buffer, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Could not access %s", info_name);
    }

    char string[128] = {0};
    if (buffer & CL_DEVICE_TYPE_CPU)
    {
        strcat(string, "CL_DEVICE_TYPE_CPU ");
    }
    if (buffer & CL_DEVICE_TYPE_GPU)
    {
        strcat(string, "CL_DEVICE_TYPE_GPU ");
    }
    if (buffer & CL_DEVICE_TYPE_ACCELERATOR)
    {
        strcat(string, "CL_DEVICE_TYPE_ACCELERATOR ");
    }
    if (buffer & CL_DEVICE_TYPE_DEFAULT)
    {
        strcat(string, "CL_DEVICE_TYPE_DEFAULT ");
    }
    if (buffer & CL_DEVICE_TYPE_CUSTOM)
    {
        strcat(string, "CL_DEVICE_TYPE_CUSTOM ");
    }

    PMS_INFO("%s: %s", info_name, string);
}

void
print_device_info_memory_size_ulong(cl_device_id device_id,
                                    cl_device_info info,
                                    const char* info_name)
{
    cl_ulong buffer = 0;
    cl_int error = clGetDeviceInfo(device_id, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(buffer), &buffer, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Could not access %s", info_name);
        return;
    }

    PMS_INFO("%s: 2 ^ %zu bytes", info_name, (size_t)log2f(buffer));
}

void
print_device_info_memory_size_uint(cl_device_id device_id,
                                   cl_device_info info,
                                   const char* info_name)
{
    cl_uint buffer = 0;
    cl_int error = clGetDeviceInfo(device_id, info, sizeof(buffer), &buffer, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Could not access %s", info_name);
        return;
    }

    PMS_INFO("%s: 2 ^ %zu bytes", info_name, (size_t)log2f(buffer));
}

void
print_device_info_memory_size_size_t(cl_device_id device_id,
                                     cl_device_info info,
                                     const char* info_name)
{
    size_t buffer = 0;
    cl_int error = clGetDeviceInfo(device_id, info, sizeof(buffer), &buffer, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Could not access %s", info_name);
        return;
    }

    PMS_INFO("%s: 2 ^ %zu bytes", info_name, (size_t)log2f(buffer));
}

int32_t
pms_output_device_info(const cl_device_id device_id)
{
    // Function starts:
    print_device_info_uint(device_id, CL_DEVICE_ADDRESS_BITS, "CL_DEVICE_ADDRESS_BITS");
    print_device_info_string(device_id, CL_DEVICE_BUILT_IN_KERNELS, "CL_DEVICE_BUILT_IN_KERNELS");
    print_device_info_bool(device_id, CL_DEVICE_COMPILER_AVAILABLE, "CL_DEVICE_COMPILER_AVAILABLE");
    print_device_info_device_fp_config(device_id, CL_DEVICE_DOUBLE_FP_CONFIG, "CL_DEVICE_DOUBLE_FP_CONFIG");
    print_device_info_bool(device_id, CL_DEVICE_ENDIAN_LITTLE, "CL_DEVICE_ENDIAN_LITTLE");
    print_device_info_bool(device_id, CL_DEVICE_ERROR_CORRECTION_SUPPORT, "CL_DEVICE_ERROR_CORRECTION_SUPPORT");
    print_device_info_device_exec_capabilities(device_id, CL_DEVICE_EXECUTION_CAPABILITIES, "CL_DEVICE_EXECUTION_CAPABILITIES");
    print_device_info_string(device_id, CL_DEVICE_EXTENSIONS, "CL_DEVICE_EXTENSIONS");
    print_device_info_memory_size_ulong(device_id, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, "CL_DEVICE_GLOBAL_MEM_CACHE_SIZE");
    print_device_info_device_mem_cache_type(device_id, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, "CL_DEVICE_GLOBAL_MEM_CACHE_TYPE");
    print_device_info_memory_size_uint(device_id, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, "CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE");
    print_device_info_memory_size_ulong(device_id, CL_DEVICE_GLOBAL_MEM_SIZE, "CL_DEVICE_GLOBAL_MEM_SIZE");
    print_device_info_memory_size_size_t(device_id, CL_DEVICE_GLOBAL_VARIABLE_PREFERRED_TOTAL_SIZE, "CL_DEVICE_GLOBAL_VARIABLE_PREFERRED_TOTAL_SIZE");
    print_device_info_size_t(device_id, CL_DEVICE_IMAGE2D_MAX_HEIGHT, "CL_DEVICE_IMAGE2D_MAX_HEIGHT");
    print_device_info_size_t(device_id, CL_DEVICE_IMAGE2D_MAX_WIDTH, "CL_DEVICE_IMAGE2D_MAX_WIDTH");
    print_device_info_size_t(device_id, CL_DEVICE_IMAGE3D_MAX_DEPTH, "CL_DEVICE_IMAGE3D_MAX_DEPTH");
    print_device_info_size_t(device_id, CL_DEVICE_IMAGE3D_MAX_HEIGHT, "CL_DEVICE_IMAGE3D_MAX_HEIGHT");
    print_device_info_size_t(device_id, CL_DEVICE_IMAGE3D_MAX_WIDTH, "CL_DEVICE_IMAGE3D_MAX_WIDTH");
    print_device_info_uint(device_id, CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT, "CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT");
    print_device_info_memory_size_size_t(device_id, CL_DEVICE_IMAGE_MAX_ARRAY_SIZE, "CL_DEVICE_IMAGE_MAX_ARRAY_SIZE");
    print_device_info_memory_size_size_t(device_id, CL_DEVICE_IMAGE_MAX_BUFFER_SIZE, "CL_DEVICE_IMAGE_MAX_BUFFER_SIZE");
    print_device_info_uint(device_id, CL_DEVICE_IMAGE_PITCH_ALIGNMENT, "CL_DEVICE_IMAGE_PITCH_ALIGNMENT");
    print_device_info_bool(device_id, CL_DEVICE_IMAGE_SUPPORT, "CL_DEVICE_IMAGE_SUPPORT");
    print_device_info_bool(device_id, CL_DEVICE_LINKER_AVAILABLE, "CL_DEVICE_LINKER_AVAILABLE");
    print_device_info_memory_size_ulong(device_id, CL_DEVICE_LOCAL_MEM_SIZE, "CL_DEVICE_LOCAL_MEM_SIZE");
    print_device_info_device_local_mem_type(device_id, CL_DEVICE_LOCAL_MEM_TYPE, "CL_DEVICE_LOCAL_MEM_TYPE");
    print_device_info_uint(device_id, CL_DEVICE_MAX_CLOCK_FREQUENCY, "CL_DEVICE_MAX_CLOCK_FREQUENCY");
    print_device_info_uint(device_id, CL_DEVICE_MAX_COMPUTE_UNITS, "CL_DEVICE_MAX_COMPUTE_UNITS");
    print_device_info_uint(device_id, CL_DEVICE_MAX_CONSTANT_ARGS, "CL_DEVICE_MAX_CONSTANT_ARGS");
    print_device_info_memory_size_ulong(device_id, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, "CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE");
    print_device_info_memory_size_size_t(device_id, CL_DEVICE_MAX_GLOBAL_VARIABLE_SIZE, "CL_DEVICE_MAX_GLOBAL_VARIABLE_SIZE");
    print_device_info_memory_size_ulong(device_id, CL_DEVICE_MAX_MEM_ALLOC_SIZE, "CL_DEVICE_MAX_MEM_ALLOC_SIZE");
    print_device_info_uint(device_id, CL_DEVICE_MAX_ON_DEVICE_EVENTS, "CL_DEVICE_MAX_ON_DEVICE_EVENTS");
    print_device_info_uint(device_id, CL_DEVICE_MAX_ON_DEVICE_QUEUES, "CL_DEVICE_MAX_ON_DEVICE_QUEUES");
    print_device_info_memory_size_size_t(device_id, CL_DEVICE_MAX_PARAMETER_SIZE, "CL_DEVICE_MAX_PARAMETER_SIZE");
    print_device_info_uint(device_id, CL_DEVICE_MAX_PIPE_ARGS, "CL_DEVICE_MAX_PIPE_ARGS");
    print_device_info_uint(device_id, CL_DEVICE_MAX_READ_IMAGE_ARGS, "CL_DEVICE_MAX_READ_IMAGE_ARGS");
    print_device_info_uint(device_id, CL_DEVICE_MAX_READ_WRITE_IMAGE_ARGS, "CL_DEVICE_MAX_READ_WRITE_IMAGE_ARGS");
    print_device_info_uint(device_id, CL_DEVICE_MAX_SAMPLERS, "CL_DEVICE_MAX_SAMPLERS");
    print_device_info_size_t(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, "CL_DEVICE_MAX_WORK_GROUP_SIZE");
    print_device_info_uint(device_id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, "CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS");
    print_device_info_size_t_array(device_id, CL_DEVICE_MAX_WORK_ITEM_SIZES, "CL_DEVICE_MAX_WORK_ITEM_SIZES");
    print_device_info_uint(device_id, CL_DEVICE_MAX_WRITE_IMAGE_ARGS, "CL_DEVICE_MAX_WRITE_IMAGE_ARGS");
    print_device_info_uint(device_id, CL_DEVICE_MEM_BASE_ADDR_ALIGN, "CL_DEVICE_MEM_BASE_ADDR_ALIGN");
    print_device_info_string(device_id, CL_DEVICE_NAME, "CL_DEVICE_NAME");
    print_device_info_uint(device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR, "CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR");
    print_device_info_uint(device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT, "CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT");
    print_device_info_uint(device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_INT, "CL_DEVICE_NATIVE_VECTOR_WIDTH_INT");
    print_device_info_uint(device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG, "CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG");
    print_device_info_uint(device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT, "CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT");
    print_device_info_uint(device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE, "CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE");
    print_device_info_uint(device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF, "CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF");
    print_device_info_string(device_id, CL_DEVICE_OPENCL_C_VERSION, "CL_DEVICE_OPENCL_C_VERSION");
    print_device_info_uint(device_id, CL_DEVICE_PARTITION_MAX_SUB_DEVICES, "CL_DEVICE_PARTITION_MAX_SUB_DEVICES");
    print_device_info_device_affinity_domain(device_id, CL_DEVICE_PARTITION_AFFINITY_DOMAIN, "CL_DEVICE_PARTITION_AFFINITY_DOMAIN");
    print_device_info_uint(device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, "CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR");
    print_device_info_uint(device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, "CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT");
    print_device_info_uint(device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, "CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT");
    print_device_info_uint(device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, "CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG");
    print_device_info_uint(device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, "CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT");
    print_device_info_uint(device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, "CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE");
    print_device_info_uint(device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF, "CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF");
    print_device_info_memory_size_size_t(device_id, CL_DEVICE_PRINTF_BUFFER_SIZE, "CL_DEVICE_PRINTF_BUFFER_SIZE");
    print_device_info_bool(device_id, CL_DEVICE_PREFERRED_INTEROP_USER_SYNC, "CL_DEVICE_PREFERRED_INTEROP_USER_SYNC");
    print_device_info_string(device_id, CL_DEVICE_PROFILE, "CL_DEVICE_PROFILE");
    print_device_info_size_t(device_id, CL_DEVICE_PROFILING_TIMER_RESOLUTION, "CL_DEVICE_PROFILING_TIMER_RESOLUTION");
    print_device_info_command_queue_properties(device_id, CL_DEVICE_QUEUE_PROPERTIES, "CL_DEVICE_QUEUE_PROPERTIES");
    print_device_info_uint(device_id, CL_DEVICE_REFERENCE_COUNT, "CL_DEVICE_REFERENCE_COUNT");
    print_device_info_device_fp_config(device_id, CL_DEVICE_SINGLE_FP_CONFIG, "CL_DEVICE_SINGLE_FP_CONFIG");
    print_device_info_device_type(device_id, CL_DEVICE_TYPE, "CL_DEVICE_TYPE");
    print_device_info_string(device_id, CL_DEVICE_VENDOR, "CL_DEVICE_VENDOR");
    print_device_info_uint(device_id, CL_DEVICE_VENDOR_ID, "CL_DEVICE_VENDOR_ID");
    print_device_info_string(device_id, CL_DEVICE_VERSION, "CL_DEVICE_VERSION");
    print_device_info_string(device_id, CL_DRIVER_VERSION, "CL_DRIVER_VERSION");

    return PMS_SUCCESS;

}
