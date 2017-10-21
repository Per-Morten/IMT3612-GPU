#include <string.h>
#include <CL/cl.h>
#include <pms_common.h>

typedef struct
{
    cl_device_id device_id;
    cl_context context;
    cl_command_queue command_queue;
    cl_program program;
    cl_kernel kernel;
} pms_cl_api;

typedef struct
{
    cl_mem base_str;
    cl_mem sequence;
    cl_mem result;
} pms_device_container;

typedef struct
{
    char* base_str;
    char* target_str;
    char* result; // pointer to where to store the result;
    uint32_t* sequence;
    size_t    sequence_count;
    size_t    max_string_size;  // The maximum length of a string, used to know how much memory to allocate and read.
    size_t    max_sequence_size;
} pms_host_container;

// Duplicating code here to avoid breaking changes in the pms_setup_opencl_cpu.
// This will have to be refactored at a later point, the pms_setup_opencl_cpu should be tollerant of properties.
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

    cl_command_queue_properties prop[] = {CL_QUEUE_PROPERTIES, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, 0};
    *out_command_queue = clCreateCommandQueueWithProperties((*out_context), 
                                                            (*out_device_id), 
                                                            prop, 
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
                    pms_device_container* d_out_container,
                    size_t string_memory_size,
                    size_t sequence_memory_size)
{
    cl_int error = CL_SUCCESS;
    d_out_container->base_str = clCreateBuffer(api.context, CL_MEM_READ_ONLY, 
                                               string_memory_size * sizeof(char), 
                                               NULL, &error);
    PMS_CHECK_CL_ERROR(error, "creating base_str buffer");

    d_out_container->sequence = clCreateBuffer(api.context, CL_MEM_READ_ONLY,
                                               sequence_memory_size * sizeof(uint32_t),
                                               NULL, &error);
    PMS_CHECK_CL_ERROR(error, "creating sequence buffer");

    d_out_container->result = clCreateBuffer(api.context, CL_MEM_WRITE_ONLY, 
                                             string_memory_size * sizeof(char), 
                                             NULL, &error);
    PMS_CHECK_CL_ERROR(error, "creating result buffer");

    return PMS_SUCCESS;
}

int32_t
release_opencl_memory(pms_device_container d_container)
{
    clReleaseMemObject(d_container.base_str);
    clReleaseMemObject(d_container.sequence);
    clReleaseMemObject(d_container.result);

    return PMS_SUCCESS;
}

int32_t
copy_arguments(cl_command_queue command_queue,
               cl_mem d_base_str,
               cl_mem d_sequence,
               const char* h_base_str,
               const uint32_t* h_sequence,
               const size_t sequence_count,
               cl_event* event_queue)
{
    const size_t base_len = sizeof(char) * strlen(h_base_str) + 1;
    cl_int error = clEnqueueWriteBuffer(command_queue, 
                                        d_base_str, 
                                        CL_FALSE, 0, 
                                        base_len, 
                                        h_base_str, 0, 
                                        NULL, &event_queue[0]);
    PMS_CHECK_CL_ERROR(error, "enqueue base_str write");

    error = clEnqueueWriteBuffer(command_queue, 
                                 d_sequence, 
                                 CL_FALSE, 0, 
                                 sizeof(uint32_t) * sequence_count, 
                                 h_sequence, 0, 
                                 NULL, &event_queue[1]);
    PMS_CHECK_CL_ERROR(error, "enqueue sequence write");

    return PMS_SUCCESS;
}

int32_t
set_arguments(cl_kernel kernel,
              cl_mem d_base_str,
              cl_mem d_sequence,
              cl_mem d_result,
              uint32_t sequence_count)
{
    cl_int error = CL_SUCCESS;
    error |= clSetKernelArg(kernel, 0, sizeof(cl_mem),   &d_base_str);
    error |= clSetKernelArg(kernel, 1, sizeof(cl_mem),   &d_sequence);
    error |= clSetKernelArg(kernel, 2, sizeof(uint32_t), &sequence_count);
    error |= clSetKernelArg(kernel, 3, sizeof(cl_mem),   &d_result);
    PMS_CHECK_CL_ERROR(error, "set kernel arguments");
    return PMS_SUCCESS;
}

int32_t
run_twists(pms_cl_api api,
           pms_device_container d_container,
           const pms_host_container h_container)
{
    cl_event event_queue[5];
    copy_arguments(api.command_queue, 
                   d_container.base_str, 
                   d_container.sequence, 
                   h_container.base_str, 
                   h_container.sequence, 
                   h_container.sequence_count,
                   event_queue);

    set_arguments(api.kernel,
                  d_container.base_str,
                  d_container.sequence,
                  d_container.result,
                  h_container.sequence_count);

    const size_t work_size = 1;
    cl_int error = clEnqueueNDRangeKernel(api.command_queue, api.kernel, 1, 
                                          NULL, &work_size, NULL, 
                                          2, event_queue, &event_queue[2]); // 2 cause we have 2 events at this stage.
    PMS_CHECK_CL_ERROR(error, "enqueue kernel");


    error = clEnqueueReadBuffer(api.command_queue, d_container.result, CL_FALSE, 0, 
                                h_container.max_string_size * sizeof(char), h_container.result, 
                                3, event_queue, NULL);
    PMS_CHECK_CL_ERROR(error, "enqueue read buffer");

    error = clFinish(api.command_queue);
    PMS_CHECK_CL_ERROR(error, "waiting for cl to finish");

    if (strcmp(h_container.target_str, h_container.result) != 0)
    {
        PMS_WARN("Target: %s, result: %s", h_container.target_str, h_container.result);
        return PMS_FAILURE;
    }
    return PMS_SUCCESS;
}

int32_t
get_string_puzzles(pms_host_container** out_containers, 
                   size_t* out_count,
                   size_t max_string_size,
                   size_t max_sequence_size)
{
    const char* bases[] = 
    {
        "1234 5678", "Simon McCallum", "Multithreaded", "Game Programming",
        "Programming Patterns", "CppCon 2016", "FTCP Engine Is Best Engine",
        "Wish there was proper vim support in Git bash", "Potetstappe", "Kumocha", 
        "This code is best code", "En text"
    };

    const char* targets[] =
    {
        "6781 2345", "monCa imlSMclu", "tithreaeadedMultiMuld edhr", "Prmminograg Game", 
        "nsrammiProgngPatter ", "Cp0 16nopC", " EngineFTCP ngine Es BestI", 
        " is shroper Wi Gthere wvinpit bash am support", "aptstpePot aptstpePot", "mochcKumoha Ku", 
        "dcob esstde  iscoeT hi", "E nxte"
    };

    uint32_t sequences[][10] =
    {
        {0, 3, 0, 3 },
        {2, 0, 3, 2, 2, 2, 0, 2, 2, 0 },
        {3, 2, 0, 1, 3, 0, 3, 2 },
        {3, 0, 0, 0, 1, 2, 0, },
        {0, 1, 3, 0, 2, 0, 3, 1 },
        {2, 0, 2, 0, 3, 2, 3, 2 },
        {1, 2, 2, 0, 0 },
        {3, 2, 1, 1, 2, 1, 2, 1, 2, 1 },
        {0, 2, 0, 0, 0, 3 },
        {0, 3, 2, 3, 2, 0 },
        {0, 3, 0, 3, 2, 1, 3, 2, 2 },
        {0, 3, 2, 3, 2, 2, 3, 2 },
    };

    size_t sequence_count[] =
    {
        4, 10, 8, 7, 8, 8, 5, 10, 6, 6, 9, 8
    };

    *out_count = sizeof(sequence_count) / sizeof(size_t);
    (*out_containers) = malloc((*out_count) * sizeof(pms_host_container));
    for (size_t i = 0; i < *out_count; ++i)
    {
        pms_host_container* ptr = &(*out_containers)[i];

        ptr->base_str = malloc(strlen(bases[i]) + 1);
        strcpy(ptr->base_str, bases[i]);
        
        ptr->target_str = malloc(strlen(targets[i]) + 1);
        strcpy(ptr->target_str, targets[i]);

        ptr->sequence = malloc(sequence_count[i] * sizeof(uint32_t));
        memcpy(ptr->sequence, sequences[i], sequence_count[i] * sizeof(uint32_t));

        ptr->sequence_count = sequence_count[i];
        ptr->max_string_size = max_string_size;
        ptr->max_sequence_size = max_sequence_size;
    }

    return PMS_SUCCESS;
}

int32_t
free_string_puzzles(pms_host_container* containers, const size_t count)
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
    const size_t max_string_size = 1024;
    const size_t max_sequence_size = 100;
    const char filepath[] = "kernels/string_twist_test.cl";
    const char compiler_args[] = "-Ikernels/pms_lib/ -Ikernels/pms_string_twist/ -Werror";
    cl_int error = 0;

    pms_cl_api api;
    setup_opencl_cpu(&api.device_id, &api.context, &api.command_queue);
    
    setup_program(api.device_id, api.context, &api.program, 
                  filepath, compiler_args);
    
    api.kernel = clCreateKernel(api.program,
                                "string_twist_test", 
                                &error);

    pms_device_container d_container;
    setup_opencl_memory(api, &d_container, max_string_size, max_sequence_size);

    char buffer[max_string_size];

    pms_host_container* h_containers = NULL;
    size_t h_container_count = 0;

    get_string_puzzles(&h_containers, &h_container_count, max_string_size, max_sequence_size);

    for (size_t i = 0; i < h_container_count; ++i)
    {
        h_containers[i].result = buffer;
        if (run_twists(api, d_container, h_containers[i]) != PMS_FAILURE)
        {
            PMS_INFO("Test passed! Target: %s Result: %s", h_containers[i].target_str, h_containers[i].result);
        }
        else
        {
            PMS_WARN("Test failed! Target: %s Result: %s", h_containers[i].target_str, h_containers[i].result);
        }
    }

    free_string_puzzles(h_containers, h_container_count);
    release_opencl_memory(d_container);
    clReleaseKernel(api.kernel);
    clReleaseProgram(api.program);
    pms_cleanup_opencl(&api.context, &api.command_queue);

    PMS_INFO("Exited cleanly");
    return 0;
}
