#include <pms_common.h>

int32_t
pms_setup_opencl_cpu(cl_device_id* out_device_id,
                     cl_context* out_context,
                     cl_command_queue* out_command_queue)
{
    size_t count;
    pms_get_device_id(CL_DEVICE_TYPE_CPU,
                      out_device_id,
                      &count);

    cl_int error = 0;
    *out_context = clCreateContext(NULL, 1, out_device_id,
                                   NULL, NULL, &error);

    PMS_CHECK_CL_ERROR(error, "create context");

    *out_command_queue = clCreateCommandQueueWithProperties((*out_context), 
                                                            (*out_device_id), 
                                                            NULL, 
                                                            &error);
    PMS_CHECK_CL_ERROR(error, "create command queue");

    return PMS_SUCCESS;
}
