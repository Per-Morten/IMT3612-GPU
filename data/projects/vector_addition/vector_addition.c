#include <CL/cl.h>
#include <stdio.h>
#include <string.h>
#include <pms_common.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <omp.h>
#include <time.h>

int32_t
pms_get_cpu_platform_id(cl_device_id* out_device_id)
{
    cl_uint count = 0;
    cl_int error = clGetPlatformIDs(1, NULL, &count);

    PMS_CHECK_CL_ERROR(error, "get_platform_ids");
    if (count == 0)
    {
        return PMS_FAILURE;
    }

    cl_platform_id platform_ids[count];
    error = clGetPlatformIDs(1, platform_ids, NULL);
    PMS_CHECK_CL_ERROR(error, "get_platform_ids");
    for (size_t i = 0; i < count; ++i)
    {
        error = clGetDeviceIDs(platform_ids[i],
                               CL_DEVICE_TYPE_CPU, 
                               1, 
                               out_device_id, 
                               NULL);
        if (error == CL_SUCCESS)
        {
            break;
        }
    }

    if (out_device_id == NULL)
    {
        PMS_CHECK_CL_ERROR(error, "get_device_id");
    }

    return PMS_SUCCESS;
}

void
pms_create_matrixes(float** lhs, float** rhs, float** result, const size_t count)
{
    *lhs = calloc(count, sizeof(float));
    *rhs = calloc(count, sizeof(float));
    *result = calloc(count, sizeof(float));

    for (size_t i = 0; i < count; ++i)
    {
        (*lhs)[i] = rand() / (float)RAND_MAX;
        (*rhs)[i] = rand() / (float)RAND_MAX;
    }
}

int
main(int argc, char** argv)
{
    // Setup device
    cl_device_id device_id;
    pms_get_cpu_platform_id(&device_id);
    pms_output_device_info(device_id);

    // Create context
    cl_int error = 0;
    cl_context context = clCreateContext(NULL, 1, 
                                         &device_id, 
                                         NULL, 
                                         NULL, 
                                         &error);
    PMS_CHECK_CL_ERROR(error, "create_context");

    // Create command queue
    cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, 
                                                                        device_id,
                                                                        NULL,
                                                                        &error);

    PMS_CHECK_CL_ERROR(error, "create_command_queue");

    // Reading program from file
    cl_program program;
    char kernel_filepath[] = "kernels/vector_addition.cl";
    int32_t result = pms_create_program(context, kernel_filepath, &program, 2048);
    if (result != PMS_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    // Building program    
    result = pms_build_program(program, device_id);
    if (result != PMS_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    // Creating kernel
    cl_kernel vector_addition_kernel = clCreateKernel(program, "vector_addition", &error);
    PMS_CHECK_CL_ERROR(error, "create kernel");


    // Creating matrix
    const size_t count = (1 << 10);
    float* h_lhs;
    float* h_rhs;
    float* h_result;
    pms_create_matrixes(&h_lhs, &h_rhs, &h_result, count);

    // Creating buffers
    cl_mem d_lhs = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * count, NULL, &error);
    PMS_CHECK_CL_ERROR(error, "create_d_lhs_buffer");

    cl_mem d_rhs = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * count, NULL, &error);
    PMS_CHECK_CL_ERROR(error, "create_d_rhs_buffer");

    cl_mem d_result = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * count, NULL, &error);
    PMS_CHECK_CL_ERROR(error, "create_d_result_buffer");

    // Copying over buffers
    error = clEnqueueWriteBuffer(command_queue, d_lhs, CL_TRUE, 0, 
                                 sizeof(float) * count, h_lhs, 0, 
                                 NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "copying_h_lhs_to_d_lhs");

    error = clEnqueueWriteBuffer(command_queue, d_rhs, CL_TRUE, 0, 
                                 sizeof(float) * count, h_rhs, 0, 
                                 NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "copying_h_rhs_to_d_rhs");

    // Sending the arguments to the kernel
    error =  clSetKernelArg(vector_addition_kernel, 0, sizeof(cl_mem), &d_lhs);
    error |= clSetKernelArg(vector_addition_kernel, 1, sizeof(cl_mem), &d_rhs);
    error |= clSetKernelArg(vector_addition_kernel, 2, sizeof(cl_mem), &d_result);
    error |= clSetKernelArg(vector_addition_kernel, 3, sizeof(size_t), &count);
    PMS_CHECK_CL_ERROR(error, "sending_arguments_to_kernel");


    // Its happening! =D
    const double start = omp_get_wtime();

    // We are allowing opencl to decide the size of the work-group
    const size_t global_domain_size = count;
    error = clEnqueueNDRangeKernel(command_queue, vector_addition_kernel, 
                                   1, NULL, &global_domain_size, NULL, 0, NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "enqueueing_kernal");

    error = clFinish(command_queue);
    PMS_CHECK_CL_ERROR(error, "waiting_for_cl_to_finish");

    const double stop = omp_get_wtime();
    printf("Kernel ran for wtime: %lf seconds\n", stop - start);
    fflush(stdout);

    // Reading values back
    error = clEnqueueReadBuffer(command_queue, d_result, CL_TRUE, 0, 
                                sizeof(float) * count, h_result, 0, 
                                NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "reading_output_array");

    uint32_t number_of_correct_calculations = 0;
    for (size_t i = 0; i < count; ++i)
    {
        float tmp = h_lhs[i] + h_rhs[i];
        tmp -= h_result[i];
        if (powf(tmp, 2) < powf(0.001f, 2))
        {
            ++number_of_correct_calculations;
        }
        else
        {
            printf("tmp: %f h_lhs: %f h_rhs: %f h_result: %f\n", tmp, h_lhs[i], h_rhs[i], h_result[i]);
        }
    }

    printf("result = lhs + rhs: %u of %zu were correct\n", number_of_correct_calculations, count);

    // cleanup
    free(h_lhs);
    free(h_rhs);
    free(h_result);

    clReleaseMemObject(d_lhs);
    clReleaseMemObject(d_rhs);
    clReleaseMemObject(d_result);
    clReleaseProgram(program);
    clReleaseKernel(vector_addition_kernel);
    clReleaseCommandQueue(command_queue);
    clReleaseContext(context);

    printf("Quiting successfully\n");
    return 0;
}
