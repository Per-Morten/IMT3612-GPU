#include <pms_common.h>
#include <CL/cl.h>

int32_t
pms_get_device_id(cl_device_type device_type,
                  cl_device_id* out_device_id,
                  size_t* out_device_count)
{
    cl_uint id_count = 0;
    cl_int error = clGetPlatformIDs(0, NULL, &id_count);

    PMS_CHECK_CL_ERROR(error, "get platform ids");
    if (id_count == 0)
    {
        return PMS_FAILURE;
    }

    cl_platform_id platform_ids[id_count]; 
    error = clGetPlatformIDs(id_count, platform_ids, NULL);

    *out_device_count = 0;
    for (size_t i = 0; i < id_count; ++i)
    {
        error = clGetDeviceIDs(platform_ids[i], 
                               device_type, 
                               1,
                               &out_device_id[*out_device_count],
                               NULL);

        if (error == CL_SUCCESS)
        {
            (*out_device_count)++;
        }
    }

    if ((*out_device_id) == NULL)
    {
        PMS_CHECK_CL_ERROR(error, "get device id");
    }

    return PMS_SUCCESS;
}
