#include <math.h>
#include <omp.h>
#include <CL/cl.h>

#include <pms_common.h>

int32_t
setup_opencl(cl_device_id* out_device_id,
             size_t* out_device_count,
             cl_context* out_context,
             cl_command_queue* out_command_queue)
{
    pms_get_device_id(CL_DEVICE_TYPE_ALL,
                      out_device_id,
                      out_device_count);

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
    char kernel_filepath[] = "kernels/vec_add_3.cl";
    int32_t result = pms_create_program(context,
                                        kernel_filepath,
                                        out_program,
                                        2048);
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
    *out_kernel = clCreateKernel(*out_program, "vec_add_3", &error);
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
create_vectors(const size_t count,
               const size_t vec_length)
{
    float** vecs = malloc(count * sizeof(float*));
    for (size_t i = 0; i < count; ++i)
    {
        vecs[i] = malloc(vec_length * sizeof(float));
        for (size_t j = 0; j < vec_length; ++j)
        {
            vecs[i][j] = rand() / (float)RAND_MAX;
        }
    }

    return vecs;
}

void
cleanup_vectors(float** vecs,
                const size_t count)
{
    for (size_t i = 0; i < count; ++i)
    {
        free(vecs[i]);
    }
    free(vecs);
}

int32_t
create_buffers(cl_context context,
               cl_mem_flags flags,
               float** h_vecs,
               const size_t vec_length,
               cl_mem* out_d_vecs,
               const size_t count)
{
    const size_t size_in_bytes = sizeof(float) * vec_length;
    for (size_t i = 0; i < count; ++i)
    {
        cl_int error = 0;
        float* args = (h_vecs == NULL) ? NULL : h_vecs[i];

        out_d_vecs[i] = clCreateBuffer(context, flags,
                                       size_in_bytes, args,
                                       &error);

        PMS_CHECK_CL_ERROR(error, "creating buffer");
    }


    return PMS_SUCCESS;
}

void
ready_memory_batch(cl_kernel kernel,
                   cl_mem* d_vecs,
                   size_t d_vec_count,
                   size_t vec_length)
{
    // ERROR HERE, argument 0 is not valid for some reason.
    cl_int error = CL_SUCCESS;
    for (size_t i = 0; i < d_vec_count; ++i)
    {
        error |= clSetKernelArg(kernel, i, sizeof(cl_mem), &d_vecs[i]);
        PMS_CHECK_CL_ERROR(error, "Sett arguments");
    }

    error |= clSetKernelArg(kernel, d_vec_count, sizeof(size_t), &vec_length);

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

    uint32_t number_of_correct_calculations = 0;
    for (size_t i = 0; i < vec_length; ++i)
    {
        const float tmp = vecs[0][i] + vecs[1][i] +
                          vecs[2][i] - vecs[3][i];

        if (powf(tmp, 2) < powf(0.001f, 2))
        {
            ++number_of_correct_calculations;
        }
    }

    const double stop = omp_get_wtime();
    PMS_INFO("Calculating results on single core took: %lf seconds", stop - start);
    PMS_INFO("result = a + b + c: %u of %zu were correct", 
             number_of_correct_calculations,
             vec_length);
}

int
main(int argc, char** argv)
{
    PMS_INFO("Setting up OpenCL");
    cl_device_id device_id;
    size_t device_count;
    cl_context context;
    cl_command_queue command_queue;
    setup_opencl(&device_id, &device_count, &context, &command_queue);
    PMS_INFO("Device count: %zu", device_count);

    PMS_INFO("Setting up kernel");
    cl_program kernel_program;
    cl_kernel kernel;
    setup_kernel(context, device_id, &kernel_program, &kernel);

    PMS_INFO("Creating vectors");
    const size_t vec_count = 4;
    const size_t vec_length = (1 << 20);

    PMS_INFO("Vector length: %zu", vec_length);
    float** h_vecs = create_vectors(vec_count, vec_length);
    cl_mem* d_vecs = malloc(sizeof(cl_mem) * vec_count);

    PMS_INFO("Creating in buffers");
    create_buffers(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                   &h_vecs[0], vec_length, &d_vecs[0], 3);

    PMS_INFO("Creating out buffer");
    create_buffers(context, CL_MEM_WRITE_ONLY,
                   NULL, vec_length, &d_vecs[3], 1);

    const double start = omp_get_wtime();
    PMS_INFO("Readying memory");
    ready_memory_batch(kernel, d_vecs, vec_count, vec_length);

    PMS_INFO("Running batch");
    run_batch(command_queue, kernel, d_vecs[3], h_vecs[3], vec_length);

    const double stop = omp_get_wtime();
    PMS_INFO("Kernel ran for wtime: %lf seconds", stop - start);

    PMS_INFO("Checking results");
    check_results(h_vecs, vec_length);

    PMS_INFO("Destroying buffers");
    for(size_t i = 0; i < vec_count; ++i)
    {
        clReleaseMemObject(d_vecs[i]);
    }
    free(d_vecs);

    PMS_INFO("Cleaning up vectors");
    cleanup_vectors(h_vecs, vec_count);

    PMS_INFO("Cleaning up kernel");
    cleanup_kernel(kernel_program, kernel);

    PMS_INFO("Cleaning up OpenCL");
    cleanup_opencl(context, command_queue);

    PMS_INFO("Clean exit");
    return 0;
}
