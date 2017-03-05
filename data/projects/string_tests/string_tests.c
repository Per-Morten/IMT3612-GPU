#include <string.h>
#include <stdint.h>
#include <CL/cl.h>
#include <pms_common.h>

int32_t
setup_kernel(cl_device_id device_id,
             cl_context context,
             cl_program* out_program,
             cl_kernel* out_kernel,
             const char* kernel_name,
             const char* filepath,
             const char* arguments)
{
    int32_t result = pms_create_program(context,
                                        filepath,
                                        out_program,
                                        2048);
    if (result != PMS_SUCCESS)
    {
        PMS_ERROR("Could not create program");
    }

    result = pms_build_program((*out_program), device_id, arguments);
    if (result != PMS_SUCCESS)
    {
        PMS_ERROR("Could not build program");
    }

    cl_int error = 0;
    (*out_kernel) = clCreateKernel((*out_program), 
                                   kernel_name, 
                                   &error);
    PMS_CHECK_CL_ERROR(error, "create kernel");

    return PMS_SUCCESS;
}

int32_t
cleanup_kernel(cl_program program,
               cl_kernel kernel)
{
    clReleaseProgram(program);
    clReleaseKernel(kernel);

    return PMS_SUCCESS;
}

int32_t
run_strlen_test(cl_device_id device_id,
                cl_context context,
                cl_command_queue command_queue)
{
    // Setting up program.
    PMS_INFO("Setting up program");
    cl_program program;
    cl_kernel kernel;
    setup_kernel(device_id, context, 
                 &program, &kernel, 
                 "strlen_test", 
                 "kernels/strlen_test.cl",
                 "-Ikernels/pms_lib/ -Werror");

    // Setting up string.
    char string[] = "Test";

    // Creating buffers.
    PMS_INFO("Creating buffers");
    cl_int error = 0;
    cl_mem d_str = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                  (sizeof(char) * strlen(string) + 1), string, &error);
    PMS_CHECK_CL_ERROR(error,"creating char* buffer");
    
    cl_mem d_result = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                     sizeof(uint32_t), NULL, &error);
    PMS_CHECK_CL_ERROR(error,"creating len buffer");

    // Setting up 
    PMS_INFO("Setting up arguments");
    error  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_str);
    error |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_result);
    PMS_CHECK_CL_ERROR(error, "setting arguments");

    // Enqueueing.
    PMS_INFO("Enqueueing");
    const size_t work_size = 1;
    error = clEnqueueNDRangeKernel(command_queue, 
                                   kernel,
                                   1, NULL, 
                                   &work_size, 
                                   NULL, 0, NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "enqueueing kernel");

    PMS_INFO("Finishing");
    error = clFinish(command_queue);
    PMS_CHECK_CL_ERROR(error, "waiting to finish");

    PMS_INFO("Reading");
    uint32_t h_result = 0;
    error = clEnqueueReadBuffer(command_queue, d_result, CL_TRUE, 0,
                                sizeof(uint32_t), &h_result, 0, NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "reading result");
    PMS_INFO("Correct length: %zu OpenCL length: %u", strlen(string), h_result);

    // cleanup.
    clReleaseMemObject(d_str);
    clReleaseMemObject(d_result);
    cleanup_kernel(program, kernel);

    return PMS_SUCCESS;
}

int
main(int argc, char** argv)
{
    cl_device_id device_id;
    cl_context context;
    cl_command_queue command_queue;
    pms_setup_opencl_cpu(&device_id, &context, &command_queue);

    run_strlen_test(device_id, context, command_queue);

    pms_cleanup_opencl(&context, &command_queue);

    PMS_INFO("Exiting");
    return 0;
}
