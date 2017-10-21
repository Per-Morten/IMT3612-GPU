#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <CL/cl.h>
#include <pms_common.h>

int32_t
setup_program(cl_device_id device_id,
              cl_context context,
              cl_program* out_program,
              const char* filepath,
              const char* arguments)
{
    int32_t result = pms_create_program(context,
                                        filepath,
                                        out_program,
                                        1 << 20);
    if (result != PMS_SUCCESS)
    {
        PMS_ERROR("Could not create program");
    }

    result = pms_build_program((*out_program), device_id, arguments);
    if (result != PMS_SUCCESS)
    {
        PMS_ERROR("Could not build program");
    }

    return PMS_SUCCESS;
}

int32_t
run_strlen_test(cl_context context,
                cl_command_queue command_queue,
                cl_program program)
{

    cl_int error = 0;
    cl_kernel kernel = clCreateKernel(program,
                                      "strlen_test",
                                      &error);
    PMS_CHECK_CL_ERROR(error, "create kernel");
    
    // Setting up string.
    char string[] = "Test";

    // Creating buffers.
    PMS_DEBUG("Creating buffers");
    cl_mem d_str = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                  (sizeof(char) * strlen(string) + 1), string, &error);
    PMS_CHECK_CL_ERROR(error,"creating char* buffer");
    
    cl_mem d_result = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                     sizeof(uint32_t), NULL, &error);
    PMS_CHECK_CL_ERROR(error,"creating len buffer");

    // Setting up 
    PMS_DEBUG("Setting up arguments");
    error  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_str);
    error |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_result);
    PMS_CHECK_CL_ERROR(error, "setting arguments");

    // Enqueueing.
    PMS_DEBUG("Enqueueing");
    const size_t work_size = 1;
    error = clEnqueueNDRangeKernel(command_queue, 
                                   kernel,
                                   1, NULL, 
                                   &work_size, 
                                   NULL, 0, NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "enqueueing kernel");
    PMS_DEBUG("Finishing");
    error = clFinish(command_queue);
    PMS_CHECK_CL_ERROR(error, "waiting to finish");

    PMS_DEBUG("Reading");
    uint32_t h_result = 0;
    error = clEnqueueReadBuffer(command_queue, d_result, CL_TRUE, 0,
                                sizeof(uint32_t), &h_result, 0, NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "reading result");

    // cleanup.
    clReleaseMemObject(d_str);
    clReleaseMemObject(d_result);
    clReleaseKernel(kernel);

    if (strlen(string) != h_result)
    {
        return PMS_FAILURE;
    }

    return PMS_SUCCESS;
}

int32_t
run_strcmp_test(cl_context context,
                cl_command_queue command_queue,
                cl_program program)
{
    cl_int error = 0;
    cl_kernel kernel = clCreateKernel(program, 
                                      "strcmp_test", 
                                      &error);
    PMS_CHECK_CL_ERROR(error, "create kernel");

    char h_lhs[] = "Testing string_cmp";
    char h_rhs[] = "Testing strcmp";

    // Creating buffers
    PMS_DEBUG("Creating Buffers");
    cl_mem d_lhs_str = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                      sizeof(char) * strlen(h_lhs) + 1, h_lhs, &error);
    PMS_CHECK_CL_ERROR(error, "creating h_lhs buffer");
    
    cl_mem d_rhs_str = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                      sizeof(char) * strlen(h_rhs) + 1, h_rhs, &error);
    PMS_CHECK_CL_ERROR(error, "creating h_rhs buffer");
    
    cl_mem d_result = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                     sizeof(int32_t), NULL, &error);
    PMS_CHECK_CL_ERROR(error, "creating d_result buffer");

    // Setting up
    PMS_DEBUG("Setting up arguments");
    error  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_lhs_str);
    error |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_rhs_str);
    error |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_result);
    PMS_CHECK_CL_ERROR(error, "setting up arguments");

    // Enqueueing.
    PMS_DEBUG("Enqueueing");
    const size_t work_size = 1;
    error = clEnqueueNDRangeKernel(command_queue, kernel, 1, 
                                   NULL, &work_size, NULL, 
                                   0, NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "enqueueing kernel");
    PMS_DEBUG("Finishing");
    error = clFinish(command_queue);
    PMS_CHECK_CL_ERROR(error, "waiting to finish");

    PMS_DEBUG("Reading");
    int32_t h_result = 0;
    error = clEnqueueReadBuffer(command_queue, d_result, CL_TRUE, 0,
                                sizeof(int32_t), &h_result, 0, NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "reading result");

    // Cleanup
    clReleaseMemObject(d_lhs_str);
    clReleaseMemObject(d_rhs_str);
    clReleaseMemObject(d_result);
    clReleaseKernel(kernel);

    if (strcmp(h_lhs, h_rhs) != h_result)
    {
        return PMS_FAILURE;
    }

    return PMS_SUCCESS;
}

int32_t
run_strncmp_test(cl_context context,
                 cl_command_queue command_queue,
                 cl_program program)
{
    cl_int error = 0;
    cl_kernel kernel = clCreateKernel(program, 
                                      "strncmp_test", 
                                      &error);
    PMS_CHECK_CL_ERROR(error, "create kernel");

    char h_lhs[] = "Testing string_cmp";
    char h_rhs[] = "Testing strcmp";

    // Creating buffers
    PMS_DEBUG("Creating Buffers");
    cl_mem d_lhs_str = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                      sizeof(char) * strlen(h_lhs) + 1, h_lhs, &error);
    PMS_CHECK_CL_ERROR(error, "creating h_lhs buffer");
    
    cl_mem d_rhs_str = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                      sizeof(char) * strlen(h_rhs) + 1, h_rhs, &error);
    PMS_CHECK_CL_ERROR(error, "creating h_rhs buffer");
    
    cl_mem d_result = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                     sizeof(int32_t), NULL, &error);
    PMS_CHECK_CL_ERROR(error, "creating d_result buffer");

    // Setting up
    PMS_DEBUG("Setting up arguments");
    const uint32_t compare_count = 10;
    error  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_lhs_str);
    error |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_rhs_str);
    error |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_result);
    error |= clSetKernelArg(kernel, 3, sizeof(uint32_t), &compare_count);
    PMS_CHECK_CL_ERROR(error, "setting up arguments");

    // Enqueueing.
    PMS_DEBUG("Enqueueing");
    const size_t work_size = 1;
    error = clEnqueueNDRangeKernel(command_queue, kernel, 1, 
                                   NULL, &work_size, NULL, 
                                   0, NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "enqueueing kernel");
    PMS_DEBUG("Finishing");
    error = clFinish(command_queue);
    PMS_CHECK_CL_ERROR(error, "waiting to finish");

    PMS_DEBUG("Reading");
    int32_t h_result = 0;
    error = clEnqueueReadBuffer(command_queue, d_result, CL_TRUE, 0,
                                sizeof(int32_t), &h_result, 0, NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "reading result");

    // Cleanup
    clReleaseMemObject(d_lhs_str);
    clReleaseMemObject(d_rhs_str);
    clReleaseMemObject(d_result);
    clReleaseKernel(kernel);

    int32_t compare_result = strncmp(h_lhs, h_rhs, compare_count);
    if (compare_result != h_result)
    {
        PMS_WARN("strncmp returned: %d, pms_strncmp returned: %d", compare_result, d_result);
        return PMS_FAILURE;
    }

    return PMS_SUCCESS;
}

int32_t
run_strcpy_test(cl_context context,
                cl_command_queue command_queue,
                cl_program program)
{
    cl_int error = 0;
    cl_kernel kernel = clCreateKernel(program,
                                      "strcpy_test",
                                      &error);
    PMS_CHECK_CL_ERROR(error, "create kernel");

    #define RUN_STRCPY_TEST_STRLEN 100

    char h_src[RUN_STRCPY_TEST_STRLEN] = "Testing strcpy";

    PMS_DEBUG("Creating Buffers");
    cl_mem d_src = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 
                                  RUN_STRCPY_TEST_STRLEN, h_src, &error);
    PMS_CHECK_CL_ERROR(error, "creating h_src buffer");

    cl_mem d_dest = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                   RUN_STRCPY_TEST_STRLEN, NULL, &error);
    PMS_CHECK_CL_ERROR(error, "creating h_dest buffer");

    // Setting up
    PMS_DEBUG("Setting up arguments");
    error  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_src);
    error |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_dest);
    PMS_CHECK_CL_ERROR(error, "setting up arguments");

    // Enqueueing
    PMS_DEBUG("Enqueueing");
    const size_t work_size = 1;
    error = clEnqueueNDRangeKernel(command_queue, kernel, 1, 
                                   NULL, &work_size, NULL, 
                                   0, NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "enqueueing kernel");

    PMS_DEBUG("Finishing");
    error = clFinish(command_queue);
    PMS_CHECK_CL_ERROR(error, "waiting to finish");

    PMS_DEBUG("Reading");
    char h_dest[RUN_STRCPY_TEST_STRLEN];
    error = clEnqueueReadBuffer(command_queue, d_dest, CL_TRUE, 0, 
                                RUN_STRCPY_TEST_STRLEN, h_dest, 0, NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "reading result");

    clReleaseMemObject(d_dest);
    clReleaseMemObject(d_src);
    clReleaseKernel(kernel);

    if (strcmp(h_src, h_dest) != 0)
    {
        return PMS_FAILURE;
    }

    return PMS_SUCCESS;
}

int32_t
run_strcat_test(cl_context context,
                cl_command_queue command_queue,
                cl_program program)
{
    cl_int error = 0;
    cl_kernel kernel = clCreateKernel(program,
                                      "strcat_test",
                                      &error);
    PMS_CHECK_CL_ERROR(error, "create kernel");

    #define RUN_STRCAT_TEST_STRLEN 100

    char h_src_1[RUN_STRCAT_TEST_STRLEN / 2] = "Testing ";
    char h_src_2[RUN_STRCAT_TEST_STRLEN / 2] = "strcat"; 

    PMS_DEBUG("Creating Buffers");
    cl_mem d_src_1 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 
                                  RUN_STRCAT_TEST_STRLEN / 2, h_src_1, &error);
    PMS_CHECK_CL_ERROR(error, "creating h_src_1 buffer");

    cl_mem d_src_2 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                    RUN_STRCAT_TEST_STRLEN / 2, h_src_2, &error);
    PMS_CHECK_CL_ERROR(error, "creating h_src_2 buffer");

    cl_mem d_dest = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                   RUN_STRCAT_TEST_STRLEN, NULL, &error);
    PMS_CHECK_CL_ERROR(error, "creating h_dest buffer");

    // Setting up
    PMS_DEBUG("Setting up arguments");
    error  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_src_1);
    error  = clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_src_2);
    error |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_dest);
    PMS_CHECK_CL_ERROR(error, "setting up arguments");

    // Enqueueing
    PMS_DEBUG("Enqueueing");
    const size_t work_size = 1;
    error = clEnqueueNDRangeKernel(command_queue, kernel, 1, 
                                   NULL, &work_size, NULL, 
                                   0, NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "enqueueing kernel");

    PMS_DEBUG("Finishing");
    error = clFinish(command_queue);
    PMS_CHECK_CL_ERROR(error, "waiting to finish");

    PMS_DEBUG("Reading");
    char h_dest[RUN_STRCAT_TEST_STRLEN];
    error = clEnqueueReadBuffer(command_queue, d_dest, CL_TRUE, 0, 
                                RUN_STRCAT_TEST_STRLEN, h_dest, 0, NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "reading result");

    clReleaseMemObject(d_dest);
    clReleaseMemObject(d_src_1);
    clReleaseMemObject(d_src_2);
    clReleaseKernel(kernel);

    char target[RUN_STRCAT_TEST_STRLEN];
    strcpy(target, h_src_1);
    strcat(target, h_src_2);

    if (strcmp(target, h_dest) != 0)
    {
        PMS_WARN("Target: %s, GPU: %s", target, h_dest);
        return PMS_FAILURE;
    }

    return PMS_SUCCESS;
}

int32_t
run_strncat_test(cl_context context,
                 cl_command_queue command_queue,
                 cl_program program)
{
    cl_int error = 0;
    cl_kernel kernel = clCreateKernel(program,
                                      "strncat_test",
                                      &error);
    PMS_CHECK_CL_ERROR(error, "create kernel");

    #define RUN_STRNCAT_TEST_STRLEN 100

    char h_src_1[RUN_STRNCAT_TEST_STRLEN / 2] = "Testing ";
    char h_src_2[RUN_STRNCAT_TEST_STRLEN / 2] = "strncat"; 

    PMS_DEBUG("Creating Buffers");
    cl_mem d_src_1 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 
                                  RUN_STRNCAT_TEST_STRLEN / 2, h_src_1, &error);
    PMS_CHECK_CL_ERROR(error, "creating h_src_1 buffer");

    cl_mem d_src_2 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                    RUN_STRNCAT_TEST_STRLEN / 2, h_src_2, &error);
    PMS_CHECK_CL_ERROR(error, "creating h_src_2 buffer");

    cl_mem d_dest = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                   RUN_STRNCAT_TEST_STRLEN, NULL, &error);
    PMS_CHECK_CL_ERROR(error, "creating h_dest buffer");

    // Setting up
    PMS_DEBUG("Setting up arguments");
    const uint32_t max_cat = 5;
    error  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_src_1);
    error |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_src_2);
    error |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_dest);
    error |= clSetKernelArg(kernel, 3, sizeof(uint32_t), &max_cat);
    PMS_CHECK_CL_ERROR(error, "setting up arguments");

    // Enqueueing
    PMS_DEBUG("Enqueueing");
    const size_t work_size = 1;
    error = clEnqueueNDRangeKernel(command_queue, kernel, 1, 
                                   NULL, &work_size, NULL, 
                                   0, NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "enqueueing kernel");

    PMS_DEBUG("Finishing");
    error = clFinish(command_queue);
    PMS_CHECK_CL_ERROR(error, "waiting to finish");

    PMS_DEBUG("Reading");
    char h_dest[RUN_STRNCAT_TEST_STRLEN];
    error = clEnqueueReadBuffer(command_queue, d_dest, CL_TRUE, 0, 
                                RUN_STRNCAT_TEST_STRLEN, h_dest, 0, NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "reading result");

    clReleaseMemObject(d_dest);
    clReleaseMemObject(d_src_1);
    clReleaseMemObject(d_src_2);
    clReleaseKernel(kernel);

    char target[RUN_STRNCAT_TEST_STRLEN];
    strcpy(target, h_src_1);
    strncat(target, h_src_2, max_cat);

    if (strcmp(target, h_dest) != 0)
    {
        PMS_WARN("Target: %s, GPU: %s", target, h_dest);
        return PMS_FAILURE;
    }

    return PMS_SUCCESS;
}

int32_t
run_strchr_test(cl_context context,
                cl_command_queue command_queue,
                cl_program program)
{
    cl_int error = 0;
    cl_kernel kernel = clCreateKernel(program,
                                      "strchr_test",
                                      &error);
    PMS_CHECK_CL_ERROR(error, "create kernel");
    
    // Setting up string.
    char string[] = "Test";
    const char target = 'e';

    // Creating buffers.
    PMS_DEBUG("Creating buffers");
    cl_mem d_str = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                  (sizeof(char) * strlen(string) + 1), string, &error);
    PMS_CHECK_CL_ERROR(error,"creating char* buffer");
    
    cl_mem d_result = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                     sizeof(int32_t), NULL, &error);
    PMS_CHECK_CL_ERROR(error,"creating len buffer");

    // Setting up 
    PMS_DEBUG("Setting up arguments");
    error  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_str);
    error |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_result);
    error |= clSetKernelArg(kernel, 2, sizeof(char), &target);
    PMS_CHECK_CL_ERROR(error, "setting arguments");

    // Enqueueing.
    PMS_DEBUG("Enqueueing");
    const size_t work_size = 1;
    error = clEnqueueNDRangeKernel(command_queue, 
                                   kernel,
                                   1, NULL, 
                                   &work_size, 
                                   NULL, 0, NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "enqueueing kernel");
    PMS_DEBUG("Finishing");
    error = clFinish(command_queue);
    PMS_CHECK_CL_ERROR(error, "waiting to finish");

    PMS_DEBUG("Reading");
    uint32_t h_result = 0;
    error = clEnqueueReadBuffer(command_queue, d_result, CL_TRUE, 0,
                                sizeof(int32_t), &h_result, 0, NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "reading result");

    // cleanup.
    clReleaseMemObject(d_str);
    clReleaseMemObject(d_result);
    clReleaseKernel(kernel);

    const char* host_char = strchr(string, target);
    const ptrdiff_t distance = host_char - string;

    if (distance != h_result)
    {
        PMS_WARN("host distance: %d, GPU distance: %d", distance, h_result);
        return PMS_FAILURE;
    }

    return PMS_SUCCESS;
}

int32_t
run_toupper_test(cl_context context,
                 cl_command_queue command_queue,
                 cl_program program)
{
    cl_int error = 0;
    cl_kernel kernel = clCreateKernel(program, 
                                      "toupper_test", 
                                      &error);
    PMS_CHECK_CL_ERROR(error, "create kernel");

    #define RUN_TOUPPER_TEST_STRLEN 100

    char h_src[RUN_TOUPPER_TEST_STRLEN] = "Testing toUpper";
    
    PMS_DEBUG("Creating Buffers");
    cl_mem d_src = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 
                                  RUN_TOUPPER_TEST_STRLEN, h_src, &error);
    PMS_CHECK_CL_ERROR(error, "creating d_src buffer");

    cl_mem d_dest = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
                                   RUN_TOUPPER_TEST_STRLEN, NULL, &error);
    PMS_CHECK_CL_ERROR(error, "creating d_dest buffer");

    // Setting up
    PMS_DEBUG("Setting up arguments");
    error  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_src);
    error |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_dest);
    PMS_CHECK_CL_ERROR(error, "setting up arguments");

    // Enqueueing
    PMS_DEBUG("Enqueueing");
    const size_t work_size = 1;
    error = clEnqueueNDRangeKernel(command_queue, kernel, 1, 
                                   NULL, &work_size, NULL, 
                                   0, NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "enqueueing kernel");

    PMS_DEBUG("Finishing");
    error = clFinish(command_queue);
    PMS_CHECK_CL_ERROR(error, "waiting to finish");

    PMS_DEBUG("Reading");
    char h_dest[RUN_STRCPY_TEST_STRLEN];
    error = clEnqueueReadBuffer(command_queue, d_dest, CL_TRUE, 0, 
                                RUN_TOUPPER_TEST_STRLEN, h_dest, 0, NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "reading result");

    clReleaseMemObject(d_src);
    clReleaseMemObject(d_dest);
    clReleaseKernel(kernel);

    for (size_t i = 0; i < strlen(h_src); ++i)
    {
        if (h_dest[i] != toupper(h_src[i]))
        {
            return PMS_FAILURE;
        }
    }

    return PMS_SUCCESS;
}

int32_t
run_tolower_test(cl_context context,
                 cl_command_queue command_queue,
                 cl_program program)
{
    cl_int error = 0;
    cl_kernel kernel = clCreateKernel(program, 
                                      "tolower_test", 
                                      &error);
    PMS_CHECK_CL_ERROR(error, "create kernel");

    #define RUN_TOLOWER_TEST_STRLEN 100

    char h_src[RUN_TOLOWER_TEST_STRLEN] = "Testing toLower";

    PMS_DEBUG("Creating Buffers");
    cl_mem d_src = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                  RUN_TOLOWER_TEST_STRLEN, h_src, &error);
    PMS_CHECK_CL_ERROR(error, "creating d_src buffer");

    cl_mem d_dest = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
                                   RUN_TOLOWER_TEST_STRLEN, NULL, &error);
    PMS_CHECK_CL_ERROR(error, "creating d_dest buffer");

    // Setting up
    PMS_DEBUG("Setting up arguments");
    error  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_src);
    error |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_dest);
    PMS_CHECK_CL_ERROR(error, "setting up arguments");

    // Enqueueing
    PMS_DEBUG("Enqueueing");
    const size_t work_size = 1;
    error = clEnqueueNDRangeKernel(command_queue, kernel, 1, 
                                   NULL, &work_size, NULL, 
                                   0, NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "enqueueing kernel");

    PMS_DEBUG("Finishing");
    error = clFinish(command_queue);
    PMS_CHECK_CL_ERROR(error, "waiting to finish");

    PMS_DEBUG("Reading");
    char h_dest[RUN_TOLOWER_TEST_STRLEN];
    error = clEnqueueReadBuffer(command_queue, d_dest, CL_TRUE, 0, 
                                RUN_TOLOWER_TEST_STRLEN, h_dest, 0, NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "reading result");

    clReleaseMemObject(d_src);
    clReleaseMemObject(d_dest);
    clReleaseKernel(kernel);

    for (size_t i = 0; i < strlen(h_src); ++i)
    {
        if (h_dest[i] != tolower(h_src[i]))
        {
            return PMS_FAILURE;
        }
    }

    return PMS_SUCCESS;
}

int
main(PMS_UNUSED int argc, 
     PMS_UNUSED char** argv)
{
    cl_device_id device_id;
    cl_context context;
    cl_command_queue command_queue;
    pms_setup_opencl_cpu(&device_id, &context, &command_queue);

    // Setting up program.
    cl_program program;
    setup_program(device_id, context, 
                  &program, 
                  "kernels/str_lib_test.cl",
                  "-Ikernels/pms_lib/ -Werror");

    const size_t number_of_tests = 9;
    size_t success_count = number_of_tests;
    if (run_strlen_test(context, command_queue, program) == PMS_SUCCESS)
    {
        PMS_INFO("Passed test: strlen");
    }
    else
    {
        --success_count;
        PMS_WARN("Failed test: strlen!");
    }
    if (run_strcmp_test(context, command_queue, program) == PMS_SUCCESS)
    {
        PMS_INFO("Passed test: strcmp");
    }
    else
    {
        --success_count;
        PMS_WARN("Failed test: strcmp!");
    }
    if (run_strncmp_test(context, command_queue, program) == PMS_SUCCESS)
    {
        PMS_INFO("Passed test: strncmp");
    }
    else
    {
        --success_count;
        PMS_WARN("Failed test: strncmp!");
    }
    if (run_strcpy_test(context, command_queue, program) == PMS_SUCCESS)
    {
        PMS_INFO("Passed test: strcpy");
    }
    else
    {
        --success_count;
        PMS_WARN("Failed test: strcpy!");
    }
    if (run_strcat_test(context, command_queue, program) == PMS_SUCCESS)
    {
        PMS_INFO("Passed test: strcat");
    }
    else
    {
        --success_count;
        PMS_WARN("Failed test: strcat!");
    }
    if (run_strncat_test(context, command_queue, program) == PMS_SUCCESS)
    {
        PMS_INFO("Passed test: strncat");
    }
    else
    {
        --success_count;
        PMS_WARN("Failed test: strncat!");
    }
    if (run_strchr_test(context, command_queue, program) == PMS_SUCCESS)
    {
        PMS_INFO("Passed test: strchr");
    }
    else
    {
        --success_count;
        PMS_WARN("Failed test: strchr!");
    }
    if (run_toupper_test(context, command_queue, program) == PMS_SUCCESS)
    {
        PMS_INFO("Passed test: toupper");
    }
    else
    {
        --success_count;
        PMS_WARN("Failed test: toupper!");
    }
    if (run_tolower_test(context, command_queue, program) == PMS_SUCCESS)
    {
        PMS_INFO("Passed test: tolower");
    }
    else
    {
        --success_count;
        PMS_WARN("Failed tolower test!");
    }

    clReleaseProgram(program);
    pms_cleanup_opencl(&context, &command_queue);

    PMS_INFO("Passed %zu of %zu tests", success_count, number_of_tests)
    return 0;
}
