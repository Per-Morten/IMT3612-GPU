#include <string.h>
#include <CL/cl.h>
#include <pms_common.h>

int32_t
setup_opencl(cl_device_id* out_device_id,
             cl_context* out_context)
{
    // not bothering with more devices, only aiming for CPU on this PC anyway, and 
    // I know I only have one device.
    size_t device_count;
    pms_get_device_id(CL_DEVICE_TYPE_CPU,
                      out_device_id,
                      &device_count);

    cl_int error = 0;
    *out_context = clCreateContext(NULL, 1, out_device_id,
                                   NULL, NULL, &error);

    PMS_CHECK_CL_ERROR(error, "create context");

    return PMS_SUCCESS;
}

int32_t
cleanup_opencl(cl_context context)
{
    clReleaseContext(context);

    return PMS_SUCCESS;
}

int32_t
setup_kernel(cl_device_id device_id,
             cl_context context,
             char* filepath,
             char* arguments)
{
    cl_program program;
    int32_t result = pms_create_program(context,
                                        filepath,
                                        &program,
                                        2048);

    if (result != PMS_SUCCESS)
    {
        PMS_ERROR("Could not create program");
    }

    // Build program:
    cl_int compilation_error = clBuildProgram(program, 0, NULL, arguments, NULL, NULL);
    size_t length = 0;
    char buffer[2048];
    cl_int error = clGetProgramBuildInfo(program, 
                                         device_id, 
                                         CL_PROGRAM_BUILD_LOG, 
                                         sizeof(buffer), 
                                         buffer,
                                         &length);

    PMS_CHECK_CL_ERROR(error, "getting build info");

    PMS_WARN("Failed to build program!");
    PMS_WARN("Build info: %s", buffer);

    if (compilation_error != CL_SUCCESS)
    {
        return PMS_FAILURE;
    }

    return PMS_SUCCESS;
}

///////////////////////////////////////////////////////////
/// Also adds 1 per space.
///////////////////////////////////////////////////////////
size_t
calculate_total_size(const char** strings, size_t count)
{
    size_t total_length = 0;
    for (size_t i = 0; i < count; ++i)
    {
        total_length += strlen(strings[i]) + 1;
    }
    return total_length;
}

char*
cat_strings(char* buffer, const char** strings, size_t count)
{
    for (size_t i = 0; i < count; ++i)
    {
        strcat(buffer, strings[i]);
        strcat(buffer, " ");
    }
    return buffer;
}

int
main(int argc, char** argv)
{
    char* compiler_args = NULL;
    if (argc < 2)
    {
        PMS_ERROR("File not specified, argc < 2.\nUsage: <file.cl> <compiler_args>");
    }
    if (argc >= 3)
    {
        const char** arguments_start = (const char**)&argv[2];
        size_t length = calculate_total_size(arguments_start, argc - 2) + 1;

        compiler_args = malloc(length * sizeof(char));        
        compiler_args = cat_strings(compiler_args, arguments_start, argc - 2);
        
        PMS_INFO("Building with arguments: %s", compiler_args);
    }

    cl_device_id device_id;
    cl_context context;
    setup_opencl(&device_id, &context);
    int32_t result = setup_kernel(device_id, context, argv[1], compiler_args);
    free(compiler_args);
    if (result == PMS_SUCCESS)
    {
        PMS_INFO("Program build successfully");
    }
    cleanup_opencl(context);

    return 0;
}
