#include <pms_common.h>

int32_t
pms_build_program(cl_program program, 
                  cl_device_id device_id,
                  const char* arguments)
{
    cl_int error = clBuildProgram(program, 0, NULL, arguments, NULL, NULL);
    if (error != CL_SUCCESS)
    {
        size_t length = 0;
        char buffer[2048];

        error = clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 
                                      sizeof(buffer), buffer, &length); 

        PMS_CHECK_CL_ERROR(error, "getting_build_info");

        PMS_ERROR("Failed to build program!\nError: %s\nBuild info: %s\n",
                  pms_stringify_error(error),
                  buffer);

        return PMS_FAILURE;
    }
    return PMS_SUCCESS;
}
