#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <CL/cl.h>
#include <omp.h>

#include <pms_common.h>
#include <pms_string_twist.h>

typedef struct
{
    cl_device_id device_id;
    cl_context context;
    cl_command_queue command_queue;
    cl_program program;
    cl_kernel reset_flag;
} pms_cl_api;

typedef struct
{
    cl_mem base_str;
    cl_mem target_str;
    cl_mem root;
    cl_mem sequence;
    cl_mem sequence_count;
} pms_device_puzzle_container;

typedef struct
{
    char* base_str;
    char* target_str;
    uint32_t* sequence; // pointer to where to store the result sequence.
    uint32_t sequence_count;
    uint32_t max_depth;
    size_t max_sequence_size;
    size_t max_string_size;
} pms_host_puzzle_container;


int32_t
setup_opencl_cpu(cl_device_id* out_device_id,
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

    cl_command_queue_properties properties[] = {CL_QUEUE_PROPERTIES, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, 0};
    *out_command_queue = clCreateCommandQueueWithProperties((*out_context),
                                                            (*out_device_id),
                                                            properties,
                                                            &error);

    PMS_CHECK_CL_ERROR(error, "create command queue");

    return PMS_SUCCESS;
}

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
setup_opencl_memory(pms_cl_api api,
                    pms_device_puzzle_container* d_out_container,
                    size_t string_memory_size,
                    size_t sequence_memory_size)
{
    cl_int error = CL_SUCCESS;
    d_out_container->base_str = clCreateBuffer(api.context, CL_MEM_READ_ONLY,
                                               string_memory_size * sizeof(char),
                                               NULL, &error);
    PMS_CHECK_CL_ERROR(error, "creating base_str buffer");

    d_out_container->target_str = clCreateBuffer(api.context, CL_MEM_READ_ONLY,
                                                 string_memory_size * sizeof(char),
                                                 NULL, &error);
    PMS_CHECK_CL_ERROR(error, "creating target_str buffer");

    uint32_t roots[] = {0, 1, 2, 3};
    d_out_container->root = clCreateBuffer(api.context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 
                                           sizeof(roots), 
                                           roots, &error);
    PMS_CHECK_CL_ERROR(error, "creating root buffer");

    d_out_container->sequence = clCreateBuffer(api.context, CL_MEM_WRITE_ONLY,
                                               sequence_memory_size * sizeof(uint32_t), 
                                               NULL, &error);
    PMS_CHECK_CL_ERROR(error, "creating sequence buffer");

    d_out_container->sequence_count = clCreateBuffer(api.context, CL_MEM_WRITE_ONLY,
                                                     sizeof(uint32_t), 
                                                     NULL, &error);
    PMS_CHECK_CL_ERROR(error, "creating sequence_count buffer");

    return PMS_SUCCESS;
}

int32_t
release_opencl_memory(pms_device_puzzle_container d_container)
{
    clReleaseMemObject(d_container.base_str);
    clReleaseMemObject(d_container.target_str);
    clReleaseMemObject(d_container.root);
    clReleaseMemObject(d_container.sequence);
    clReleaseMemObject(d_container.sequence_count);

    return PMS_SUCCESS;
}

int32_t
copy_arguments(cl_command_queue command_queue,
               cl_mem d_base_str,
               cl_mem d_target_str,
               cl_mem d_sequence_count,
               const char* h_base_str,
               const char* h_target_str,
               cl_event* event_queue,
               size_t* event_queue_count)
{
    const size_t base_len = sizeof(char) * strlen(h_base_str) + 1;
    cl_int error = clEnqueueWriteBuffer(command_queue,
                                        d_base_str,
                                        CL_FALSE, 0,
                                        base_len,
                                        h_base_str, 0,
                                        NULL, &event_queue[0]);
    PMS_CHECK_CL_ERROR(error, "enqueue base_str write");

    const size_t target_len = sizeof(char) * strlen(h_target_str) + 1;
    error = clEnqueueWriteBuffer(command_queue,
                                 d_target_str,
                                 CL_FALSE, 0,
                                 target_len,
                                 h_target_str, 0,
                                 NULL, &event_queue[1]);
    PMS_CHECK_CL_ERROR(error, "enqueue target_str write");

    uint32_t value = 0;
    error = clEnqueueFillBuffer(command_queue, 
                                d_sequence_count, 
                                &value, sizeof(uint32_t), 
                                0, sizeof(uint32_t), 0, 
                                NULL, &event_queue[2]);
    PMS_CHECK_CL_ERROR(error, "enqueue fill buffer");

    *event_queue_count += 3;

    return PMS_SUCCESS;
}

int32_t
set_arguments(cl_kernel kernel,
              uint32_t max_depth,
              cl_mem d_base_str,
              cl_mem d_target_str,
              cl_mem d_sequence,
              cl_mem d_sequence_count,
              cl_mem d_root)
{
    cl_int error = CL_SUCCESS;
    error |= clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_base_str);
    error |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_target_str);
    error |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_root);
    error |= clSetKernelArg(kernel, 3, sizeof(uint32_t), &max_depth);
    error |= clSetKernelArg(kernel, 4, sizeof(cl_mem), &d_sequence);
    error |= clSetKernelArg(kernel, 5, sizeof(cl_mem), &d_sequence_count);
    PMS_CHECK_CL_ERROR(error, "set kernel arguments");
    return PMS_SUCCESS;    
}

bool
validate_sequence(const char* base_str,
                  const char* target_str,
                  const uint32_t* sequence,
                  const uint32_t sequence_count)
{
    char buffer[1024];
    char scratch_pad[1024];
    strcpy(buffer, base_str);
    for (size_t i = 0; i < sequence_count; ++i)
    {
        switch(sequence[i])
        {
            case 0:
                pms_string_twist_1(scratch_pad, buffer);
                break;
            case 1:
                pms_string_twist_2(scratch_pad, buffer);
                break;
            case 2:
                pms_string_twist_3(scratch_pad, buffer);
                break;
            case 3:
                pms_string_twist_4(scratch_pad, buffer);
                break;
            default:
                PMS_WARN("Illegal Sequence, sequence: %zu, sequence_count: %zu", sequence[i], sequence_count);
                break;
        }
        strcpy(buffer, scratch_pad);
    }

    return strcmp(target_str, buffer) == 0;
}

int32_t
run_puzzle_solver(pms_cl_api api,
                  cl_kernel kernel,
                  pms_device_puzzle_container d_container,
                  pms_host_puzzle_container* h_container)
{
    size_t event_queue_count = 0;
    cl_event event_queue[5];
    copy_arguments(api.command_queue,
                   d_container.base_str,
                   d_container.target_str,
                   d_container.sequence_count,
                   h_container->base_str,
                   h_container->target_str,
                   event_queue,
                   &event_queue_count);

    set_arguments(kernel,
                  h_container->max_depth,
                  d_container.base_str,
                  d_container.target_str,
                  d_container.sequence,
                  d_container.sequence_count,
                  d_container.root);

    const size_t reset_work_size = 1;
    cl_int error = clEnqueueNDRangeKernel(api.command_queue, api.reset_flag, 1, 
                                          NULL, &reset_work_size, NULL, 
                                          0, NULL, &event_queue[event_queue_count++]);
    PMS_CHECK_CL_ERROR(error, "enqueue clear flag kernel");

    const size_t work_size = 4;
    error = clEnqueueNDRangeKernel(api.command_queue, kernel, 1, 
                                          NULL, &work_size, NULL, 
                                          event_queue_count, event_queue, &event_queue[event_queue_count]);
    event_queue_count++;
    PMS_CHECK_CL_ERROR(error, "enqueue kernel");

    error = clEnqueueReadBuffer(api.command_queue, d_container.sequence_count, CL_FALSE, 
                                0, sizeof(uint32_t), &h_container->sequence_count, 
                                event_queue_count, event_queue, NULL);
    PMS_CHECK_CL_ERROR(error, "enqueue sequence count read");

    error = clEnqueueReadBuffer(api.command_queue, d_container.sequence, CL_FALSE, 
                                0, h_container->max_sequence_size, h_container->sequence, 
                                event_queue_count, event_queue, NULL);

    PMS_CHECK_CL_ERROR(error, "enqueue sequence read");

    error = clFinish(api.command_queue);
    PMS_CHECK_CL_ERROR(error, "waiting for cl to finish");

    PMS_DEBUG("Sequence count: %zu", h_container->sequence_count);

    if (validate_sequence(h_container->base_str, h_container->target_str, 
                          h_container->sequence, h_container->sequence_count))
    {
        return PMS_SUCCESS;
    }

    return PMS_FAILURE;
}

int32_t
get_string_puzzles(pms_host_puzzle_container** out_containers,
                   size_t* out_count,
                   size_t max_string_size,
                   size_t max_sequence_size)
{
    const char* bases[] = 
    {
        "Blah","1234 5678", "Simon McCallum", "Test", "Multithreaded", "Game Programming",
        "Programming Patterns", "CppCon 2016", "FTCP Engine Is Best Engine",
        "Wish there was proper vim support in Git bash", "Potetstappe", "Kumocha", 
        "This code is best code", "En text"
    };

    const char* targets[] =
    {
        "Hah","6781 2345", "monCa imlSMclu", "Tet", "tithreaeadedMultiMuld edhr", "Prmminograg Game", 
        "nsrammiProgngPatter ", "Cp0 16nopC", " EngineFTCP ngine Es BestI", 
        " is shroper Wi Gthere wvinpit bash am support", "aptstpePot aptstpePot", "mochcKumoha Ku", 
        "dcob esstde  iscoeT hi", "E nxte"
    };
    
    const size_t max_depths[] = {10, 10, 10, 10, 10, 7, 10, 10, 5, 10, 8, 12, 11, 10};

    *out_count = sizeof(bases) / sizeof(bases[0]);
    (*out_containers) = malloc((*out_count) * sizeof(pms_host_puzzle_container));
    for (size_t i = 0; i < *out_count; ++i)
    {
        pms_host_puzzle_container* ptr = &(*out_containers)[i];

        ptr->base_str = malloc(strlen(bases[i]) + 1);
        strcpy(ptr->base_str, bases[i]);

        ptr->target_str = malloc(strlen(targets[i]) + 1);
        strcpy(ptr->target_str, targets[i]);

        ptr->sequence = malloc(max_sequence_size * sizeof(uint32_t));
        ptr->sequence_count = 0; 
        ptr->max_depth = max_depths[i];

        ptr->max_string_size = max_string_size;
        ptr->max_sequence_size = max_sequence_size;
    }

    return PMS_SUCCESS;
}

int32_t
free_string_puzzles(pms_host_puzzle_container* containers, const size_t count)
{
    for (size_t i = 0; i < count; ++i)
    {
        free(containers[i].base_str);
        free(containers[i].target_str);
        free(containers[i].sequence);
    }
    return PMS_SUCCESS;
}

int
main(PMS_UNUSED int argc, 
     PMS_UNUSED char** argv)
{
    const char filepath[] = "kernels/string_puzzle.cl";
    const char compiler_args[] = "-Ikernels/pms_lib/ -Ikernels/pms_string_twist/ -Werror -cl-std=CL2.0";
    const char* kernel_names[] = {"bottoms_up_search"};
    const size_t max_string_size = 1024;
    const size_t max_sequence_size = 100;
    cl_int error = 0;

    PMS_DEBUG("setting up api");    
    pms_cl_api api;
    setup_opencl_cpu(&api.device_id, &api.context, &api.command_queue);
    setup_program(api.device_id, api.context, &api.program, filepath, compiler_args);

    PMS_DEBUG("creating kernels");
    api.reset_flag = clCreateKernel(api.program, "reset_flag", &error);
    PMS_CHECK_CL_ERROR(error, "Creating set flag");

    const size_t kernel_count = sizeof(kernel_names) / sizeof(kernel_names[0]);
    cl_kernel* kernel = malloc(sizeof(cl_kernel) * kernel_count);
    
    for (size_t i = 0; i < kernel_count; ++i)
    {
        kernel[i] = clCreateKernel(api.program,
                                   kernel_names[i],
                                   &error);
        PMS_CHECK_CL_ERROR(error, "create kernel");
    }

    PMS_DEBUG("creating buffers");
    pms_device_puzzle_container d_container;
    setup_opencl_memory(api, &d_container, max_string_size, max_sequence_size);

    PMS_DEBUG("creating string puzzles");
    pms_host_puzzle_container* h_container = NULL;
    size_t h_container_count = 0;
    get_string_puzzles(&h_container, &h_container_count, max_string_size, max_sequence_size);

    PMS_DEBUG("running puzzle solver");
    for (size_t i = 0; i < h_container_count; ++i)
    {
        for (size_t j = 0; j < kernel_count; ++j)
        {
            const double start = omp_get_wtime();
            const int32_t result = run_puzzle_solver(api, kernel[j], d_container, &h_container[i]);
            const double stop = omp_get_wtime();
            PMS_INFO("Search took: %lf seconds", stop - start);

            // Readying the sequence for printing.
            char sequence[h_container[i].sequence_count + 1];
            char* seq_ptr = sequence;
            for (size_t k = 0; k < h_container[i].sequence_count; ++k)
            {
                sprintf(seq_ptr++, "%u", h_container[i].sequence[k]);
            }

            if (result == PMS_SUCCESS)
            {
                PMS_INFO("Success! base: %-45s target: %-45s sequence: %s", h_container[i].base_str, h_container[i].target_str, sequence);
            }
            else
            {
                PMS_WARN("Failure! base: %-45s target: %-45s", h_container[i].base_str, h_container[i].target_str);
            }

            PMS_INFO("");
        }
    }

    PMS_DEBUG("releasing string puzzles");
    free_string_puzzles(h_container, h_container_count);
    free(h_container);

    PMS_DEBUG("releasing buffers");
    release_opencl_memory(d_container);

    PMS_DEBUG("releasing kernel");
    for (size_t i = 0; i < kernel_count; ++i)
    {
        clReleaseKernel(kernel[i]);
    }
    free(kernel);

    PMS_DEBUG("releasing api");
    clReleaseProgram(api.program);
    pms_cleanup_opencl(&api.context, &api.command_queue);

    PMS_DEBUG("Clean exit");
    return 0;
}
