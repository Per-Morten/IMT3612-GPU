/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Taken from: 
/// https://github.com/HandsOnOpenCL/Exercises-Solutions/blob/master/Exercises/C_common/device_info.c
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>
#include <pms_common.h>
//
//  define VERBOSE if you want to print info about work groups sizes
//#define  VERBOSE 1
#ifdef VERBOSE
     extern int pms_stringify_error(const cl_int);
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
        printf("Error: Failed to access device name!\n");
        return PMS_FAILURE;
    }
    printf(" \n Device is  %s ",device_name);

    error = clGetDeviceInfo(device_id, CL_DEVICE_TYPE, sizeof(device_type), &device_type, NULL);
    if (error != CL_SUCCESS)
    {
        printf("Error: Failed to access device type information!\n");
        return PMS_FAILURE;
    }
    if(device_type  == CL_DEVICE_TYPE_GPU)
       printf(" GPU from ");

    else if (device_type == CL_DEVICE_TYPE_CPU)
       printf("\n CPU from ");

    else 
       printf("\n non  CPU or GPU processor from ");

    error = clGetDeviceInfo(device_id, CL_DEVICE_VENDOR, sizeof(vendor_name), &vendor_name, NULL);
    if (error != CL_SUCCESS)
    {
        printf("Error: Failed to access device vendor name!\n");
        return PMS_FAILURE;
    }
    printf(" %s ",vendor_name);

    error = clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &comp_units, NULL);
    if (error != CL_SUCCESS)
    {
        printf("Error: Failed to access device number of compute units !\n");
        return PMS_FAILURE;
    }
    printf(" with a max of %d compute units \n",comp_units);

#ifdef VERBOSE
//
// Optionally print information about work group sizes
//
    error = clGetDeviceInfo( device_id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), 
                               &max_work_itm_dims, NULL);
    if (error != CL_SUCCESS)
    {
        printf("Error: Failed to get device Info (CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS)!\n",
                                                                            error_code(error));
        return PMS_FAILURE;
    }
    
    max_loc_size = (size_t*)malloc(max_work_itm_dims * sizeof(size_t));
    if(max_loc_size == NULL){
       printf(" malloc failed\n");
       return PMS_FAILURE;
    }
    error = clGetDeviceInfo( device_id, CL_DEVICE_MAX_WORK_ITEM_SIZES, max_work_itm_dims* sizeof(size_t), 
                               max_loc_size, NULL);
    if (error != CL_SUCCESS)
    {
        printf("Error: Failed to get device Info (CL_DEVICE_MAX_WORK_ITEM_SIZES)!\n",error_code(error));
        return PMS_FAILURE;
    }
    error = clGetDeviceInfo( device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), 
                               &max_wrkgrp_size, NULL);
    if (error != CL_SUCCESS)
    {
        printf("Error: Failed to get device Info (CL_DEVICE_MAX_WORK_GROUP_SIZE)!\n",error_code(error));
        return PMS_FAILURE;
    }
   printf("work group, work item information");
   printf("\n max loc dim ");
   for(int i=0; i< max_work_itm_dims; i++)
     printf(" %d ",(int)(*(max_loc_size+i)));
   printf("\n");
   printf(" Max work group size = %d\n",(int)max_wrkgrp_size);
#endif

    return PMS_SUCCESS;

}
