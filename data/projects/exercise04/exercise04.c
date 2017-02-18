#include <math.h>
#include <stdio.h>
#include <omp.h>
#include <CL/cl.h>

#include <pms_common.h>

int32_t
setup_opencl(cl_device_id* out_device_id,
             cl_context* out_context,
             cl_command_queue* out_command_queue)
{
    pms_get_device_id(CL_DEVICE_TYPE_CPU,
                      out_device_id);
    pms_output_device_info(*out_device_id);

    cl_int error = 0;
    *out_context = clCreateContext(NULL, 1, out_device_id, 
                                   NULL, NULL, &error);

    PMS_CHECK_CL_ERROR(error, "create context");

    *out_command_queue = clCreateCommandQueueWithProperties(*out_context,
                                                            *out_device_id,
                                                            NULL,
                                                            &error);
    PMS_CHECK_CL_ERROR(error, "create command queue");

    return PMS_SUCCESS;
}

int32_t
setup_kernel(cl_context context,
             cl_device_id device_id,
             cl_program* out_program,
             cl_kernel* out_kernel)
{
    char kernel_filepath[] = "kernels/vec_add_2.cl";
    int32_t result = pms_create_program(context, kernel_filepath, out_program, 2048);
    if (result != PMS_SUCCESS)
    {
        PMS_ERROR("Could not create program");
    }

    result = pms_build_program(*out_program, device_id);
    if (result != PMS_SUCCESS)
    {
        PMS_ERROR("Could not build program");
    }

    cl_int error = 0;
    *out_kernel = clCreateKernel(*out_program, "vec_add_2", &error);
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
cleanup_opencl(cl_context context,
               cl_command_queue command_queue)
{
    clReleaseCommandQueue(command_queue);
    clReleaseContext(context);

    return PMS_SUCCESS;
}

float**
create_vectors(const size_t number_of_vecs,
               const size_t length_of_vec)
{
    float** vecs = malloc(number_of_vecs * sizeof(float*));
    for (size_t i = 0; i < number_of_vecs; ++i)
    {
        vecs[i] = calloc(length_of_vec, sizeof(float));
        for (size_t j = 0; j < length_of_vec; ++j)
        {
            vecs[i][j] = rand() / (float)RAND_MAX;
        }
    }

    return vecs;
}

void
destroy_vectors(float** vecs,
                const size_t count)
{
    for (size_t i = 0; i < count; ++i)
    {
        free(vecs[i]);
    }
    free(vecs);
}

void
create_buffers(cl_context context, 
               cl_mem_flags flags,
               float** h_vecs, 
               const size_t vec_length, 
               cl_mem* out_d_vecs,
               size_t count)
{
    const size_t size_in_bytes = sizeof(float) * vec_length;
    for (size_t i = 0; i < count; ++i)
    {
        cl_int error = 0;
        // Copying in h_vecs[i] so we don't have to do the enqueueWriteBuffer copy thing.
        float* args = (h_vecs == NULL) ? NULL : h_vecs[i];
        out_d_vecs[i] = clCreateBuffer(context, flags, 
                                       size_in_bytes, args,
                                       &error);

        PMS_CHECK_CL_ERROR(error, "creating buffer");
    }
}

void
ready_memory_batch(cl_kernel kernel,
                   cl_mem d_vec_lhs,
                   cl_mem d_vec_rhs,
                   cl_mem d_vec_result,
                   size_t count)
{
    cl_int error  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_vec_lhs);
           error |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_vec_rhs);
           error |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_vec_result);
           error |= clSetKernelArg(kernel, 3, sizeof(size_t), &count);

    PMS_CHECK_CL_ERROR(error, "Sett arguments");

}

void
run_batch(cl_command_queue command_queue,
          cl_kernel kernel,
          cl_mem read_buffer,
          float* output,
          size_t length)
{
    PMS_INFO("Enqueueing task");
    cl_int error = clEnqueueNDRangeKernel(command_queue, kernel, 1, 
                                          NULL, &length, NULL, 
                                          0, NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "enqueueing task");

    PMS_INFO("Running task");
    error = clFinish(command_queue);
    PMS_CHECK_CL_ERROR(error, "waiting for cl to finish");

    PMS_INFO("Reading output");
    error = clEnqueueReadBuffer(command_queue, read_buffer, CL_TRUE, 0, 
                                sizeof(float) * length, output, 0, 
                                NULL, NULL);
    PMS_CHECK_CL_ERROR(error, "Reading output array");
}

void
check_results(float** vecs, const size_t vec_length)
{
    const double start = omp_get_wtime();
    // Check results
    uint32_t number_of_correct_calculations = 0;
    for (size_t i = 0; i < vec_length; ++i)
    {
        const float tmp = vecs[0][i] + vecs[1][i] + 
                          vecs[2][i] + vecs[3][i] -
                          vecs[6][i] ;
        if (powf(tmp, 2) < powf(0.001f, 2))
        {
            ++number_of_correct_calculations;
        }
        else
        {
            PMS_INFO("Fault");
        }
    }

    const double stop = omp_get_wtime();
    PMS_INFO("Calculating results on single core took: %lf seconds", stop - start);

    PMS_INFO("result = a + b + e + g: %u of %zu were correct", number_of_correct_calculations, vec_length);
}

int
main(int argc, char** argv)
{
    PMS_INFO("Setting up OpenCL");
    cl_device_id device_id;
    cl_context context;
    cl_command_queue command_queue;
    setup_opencl(&device_id, &context, &command_queue);

    PMS_INFO("Setting up kernel");
    cl_program kernel_program;
    cl_kernel kernel;
    setup_kernel(context, device_id, &kernel_program, &kernel);

    const size_t number_of_vecs = 7;
    const size_t vec_length = (1 << 24);
    PMS_INFO("Vector Length: %d", vec_length);
    PMS_INFO("Number of vectors to calculate", number_of_vecs - 1);

    float** h_vecs = create_vectors(number_of_vecs, vec_length);
    cl_mem* d_vecs = malloc(sizeof(cl_mem) * number_of_vecs);

    PMS_INFO("Creating in buffers");
    create_buffers(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                   &h_vecs[0], vec_length, &d_vecs[0], 4);
    
    PMS_INFO("Creating out buffers");
    create_buffers(context, CL_MEM_READ_WRITE, 
                   NULL, vec_length, &d_vecs[4], 2);

    create_buffers(context, CL_MEM_WRITE_ONLY, 
                   NULL, vec_length, &d_vecs[6], 1);

    const double start = omp_get_wtime();

    PMS_INFO("Readying memory batch 1");
    ready_memory_batch(kernel, d_vecs[0], d_vecs[1], d_vecs[4], vec_length);
    
    PMS_INFO("Running batch 1");
    run_batch(command_queue, kernel, d_vecs[4], h_vecs[4], vec_length);

    PMS_INFO("Readying memory batch 2");
    ready_memory_batch(kernel, d_vecs[4], d_vecs[2], d_vecs[5], vec_length);

    PMS_INFO("Running batch 2");
    run_batch(command_queue, kernel, d_vecs[5], h_vecs[5], vec_length);

    PMS_INFO("Readying memory batch 3");
    ready_memory_batch(kernel, d_vecs[5], d_vecs[3], d_vecs[6], vec_length);

    PMS_INFO("Running batch 3");
    run_batch(command_queue, kernel, d_vecs[6], h_vecs[6], vec_length);

    const double stop = omp_get_wtime();
    PMS_INFO("Kernel ran for wtime: %lf seconds", stop - start);

    PMS_INFO("Checking results");
    check_results(h_vecs, vec_length);


    PMS_INFO("Destroying buffers");
    destroy_vectors(h_vecs, number_of_vecs);
    for (size_t i = 0; i < number_of_vecs; ++i)
    {
        clReleaseMemObject(d_vecs[i]);
    }
    free(d_vecs);

    PMS_INFO("Cleaning up kernel");
    cleanup_kernel(kernel_program, kernel);

    PMS_INFO("Cleaning up OpenCL");
    cleanup_opencl(context, command_queue);

    PMS_INFO("Clean exit");
    return 0;
}
