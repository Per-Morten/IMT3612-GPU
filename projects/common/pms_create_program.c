#include <pms_common.h>
#include <CL/cl.h>
#include <stdio.h>

int32_t
pms_create_program(cl_context context, 
                   const char* kernel_filepath,
                   cl_program* program,
                   size_t size_of_file)
{
    // For some reason it crashes if I try to use a character array o.O?
    char* kernel_source = calloc(size_of_file, sizeof(char));

    const int32_t pms_error = pms_read_kernel(kernel_filepath, 
                                              kernel_source, 
                                              size_of_file);
    if (pms_error == PMS_FAILURE)
    {
        PMS_ERROR("Could not read file %s", kernel_filepath);
        free(kernel_source);

        return PMS_FAILURE;
    }

    cl_int error;
    *program = clCreateProgramWithSource(context, 1,
                                         (const char**)&kernel_source, 
                                         NULL,
                                         &error);

    free(kernel_source);
    PMS_CHECK_CL_ERROR(error, "create_command_with_source");
    return PMS_SUCCESS;
}
