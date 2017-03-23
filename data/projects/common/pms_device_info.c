/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Taken from: 
/// https://github.com/HandsOnOpenCL/Exercises-Solutions/blob/master/Exercises/C_common/device_info.c
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <CL/cl.h>
#include <pms_common.h>
//
//  define VERBOSE if you want to print info about work groups sizes
#define  VERBOSE 1
#ifdef VERBOSE
     extern const char* pms_stringify_error(const cl_int);
#endif

int32_t 
pms_output_device_info(const cl_device_id device_id)
{
    cl_int error;                            // error code returned from OpenCL calls
    cl_device_type device_type;         // Parameter defining the type of the compute device
    cl_uint comp_units;                 // the max number of compute units on a device
    cl_char vendor_name[1024] = {0};    // string to hold vendor name for compute device
    cl_char device_name[1024] = {0};    // string to hold name of compute device

#ifdef VERBOSE
    cl_uint          max_work_itm_dims;
    size_t           max_wrkgrp_size;
    size_t          *max_loc_size;
#endif


    error = clGetDeviceInfo(device_id, CL_DEVICE_NAME, sizeof(device_name), &device_name, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Failed to access device name!");
        return PMS_FAILURE;
    }
    PMS_INFO("Device is %s ",device_name);

    char type[1024] = {0};
    error = clGetDeviceInfo(device_id, CL_DEVICE_TYPE, sizeof(device_type), &device_type, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Failed to access device type information!");
        return PMS_FAILURE;
    }
    if (device_type  == CL_DEVICE_TYPE_GPU)
    {
       strcat(type, "GPU from ");
    }
    else if (device_type == CL_DEVICE_TYPE_CPU)
    {
       strcat(type, "CPU from ");
    }
    else 
    {
        strcat(type, "Non CPU or GPU processor from ");
    }

    error = clGetDeviceInfo(device_id, CL_DEVICE_VENDOR, sizeof(vendor_name), &vendor_name, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Failed to access device vendor name!");
        return PMS_FAILURE;
    }
    PMS_INFO("%s %s ",type, vendor_name);

    cl_char opencl_c_version[1024] = {0};
    error = clGetDeviceInfo(device_id, CL_DEVICE_OPENCL_C_VERSION, sizeof(opencl_c_version), &opencl_c_version, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Failed to access opencl c version!");
        return PMS_FAILURE;
    }
    PMS_INFO("Running opencl c version: %s", opencl_c_version);

    cl_char driver_version[1024] = {0};
    error = clGetDeviceInfo(device_id, CL_DRIVER_VERSION, sizeof(driver_version), &driver_version, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Failed to access opencl driver version!");
        return PMS_FAILURE;
    }
    PMS_INFO("Driver version: %s", driver_version);

    error = clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &comp_units, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Failed to access device number of compute units!");
        return PMS_FAILURE;
    }
    PMS_INFO("Max of %u compute units",comp_units);

    cl_ulong global_memory_size = 0;
    error = clGetDeviceInfo(device_id, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &global_memory_size, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Failed to access device global memory size!");
        return PMS_FAILURE;
    }
    PMS_INFO("Global memory size: 2 ^ %zu bytes", (size_t)log2f(global_memory_size));

    cl_ulong global_cache_size = 0;
    error = clGetDeviceInfo(device_id, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(cl_ulong), &global_cache_size, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Failed to access device global cache size!");
        return PMS_FAILURE;
    }
    PMS_INFO("Global cache size: 2 ^ %zu bytes", (size_t)log2f(global_cache_size));

    cl_uint global_cacheline_size = 0;
    error = clGetDeviceInfo(device_id, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, sizeof(cl_uint), &global_cacheline_size, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Failed to access device global cacheline size!");
        return PMS_FAILURE;
    }
    PMS_INFO("Global cache line size: 2 ^ %zu bytes", (size_t)log2f(global_cacheline_size));

    cl_ulong local_memory_size = 0;
    error = clGetDeviceInfo(device_id, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &local_memory_size, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Failed to access device local memory size!");
        return PMS_FAILURE;
    }
    PMS_INFO("Local memory size: 2 ^ %zu bytes", (size_t)log2f(local_memory_size));

#ifdef VERBOSE
//
// Optionally print information about work group sizes
//
    PMS_INFO("Work group sizes:");
    error = clGetDeviceInfo( device_id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), 
                               &max_work_itm_dims, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Failed to get device Info (CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS) %s!",
                                                                            pms_stringify_error(error));
        return PMS_FAILURE;
    }
    
    max_loc_size = (size_t*)malloc(max_work_itm_dims * sizeof(size_t));
    if(max_loc_size == NULL){
       PMS_INFO("malloc failed");
       return PMS_FAILURE;
    }
    error = clGetDeviceInfo( device_id, CL_DEVICE_MAX_WORK_ITEM_SIZES, max_work_itm_dims* sizeof(size_t), 
                               max_loc_size, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Failed to get device Info (CL_DEVICE_MAX_WORK_ITEM_SIZES)! %s",pms_stringify_error(error));
        return PMS_FAILURE;
    }
    error = clGetDeviceInfo( device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), 
                               &max_wrkgrp_size, NULL);
    if (error != CL_SUCCESS)
    {
        PMS_WARN("Error: Failed to get device Info (CL_DEVICE_MAX_WORK_GROUP_SIZE)! %s",pms_stringify_error(error));
        return PMS_FAILURE;
    }
   PMS_INFO("work group, work item information");
   PMS_INFO("max loc dim ");
   for(size_t i=0; i < max_work_itm_dims; i++)
     PMS_INFO(" %zu ",(size_t)(*(max_loc_size+i)));
   PMS_INFO("Max work group size = %d",(size_t)max_wrkgrp_size);
   free(max_loc_size);



#endif

    return PMS_SUCCESS;

}
