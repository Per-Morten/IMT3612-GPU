#include <pms_lib_memory.h>
#include <pms_lib_string.h>
#include <pms_string_twist.h>

#ifndef PMS_TWIST_MAX_LEN
#define PMS_TWIST_MAX_LEN 1024
#endif

__global atomic_uint g_found_flag = ATOMIC_VAR_INIT(0);

__kernel void reset_flag()
{
    atomic_store_explicit(&g_found_flag, 0, memory_order_release);
}

__kernel void bottoms_up_search(__global const char* restrict base_str,
                                __global const char* restrict target_str,
                                __global const uint32_t* restrict root_value,
                                const uint32_t max_depth,
                                __global uint32_t* restrict out_sequence, 
                                __global uint32_t* restrict out_sequence_count)
{
    __private uint32_t sequence[PMS_TWIST_MAX_LEN] = {0};
    pms_memset_p(sequence, 0, max_depth * sizeof(uint32_t));

    sequence[0] = root_value[get_global_id(0)];

    uint32_t sequence_count = max_depth;
    uint32_t already_found = atomic_load_explicit(&g_found_flag, 
                                                  memory_order_acquire);
                              
    const size_t last_legal_value = 3;

    while (sequence_count != 1 && !already_found)
    {
        const bool correct_sequence = 
            pms_string_twist_validate_ggp(base_str,
                                          target_str,
                                          sequence,
                                          sequence_count);

        if (correct_sequence)
        {
            uint32_t expected = 0;

            // Check if someone has already found a solution.
            while (!atomic_compare_exchange_weak_explicit(&g_found_flag, 
                                                          &expected,
                                                          1,
                                                          memory_order_acq_rel,
                                                          memory_order_acquire))
            {
                // If a solution is found I will leave that memory area alone.
                if (expected)
                {
                    return;
                }
            }

            *out_sequence_count = sequence_count;
            pms_memcpy_gp(out_sequence, sequence, sizeof(uint32_t) * sequence_count);
        }

        size_t head = sequence_count - 1;
        while (sequence[head] == last_legal_value)
        {
            sequence[head] = 0;

            if (head == 1)
            {
                --sequence_count;
                break;
            }
            --head;
        }
        sequence[head]++;

        already_found = atomic_load_explicit(&g_found_flag,
                                             memory_order_acquire);
    }
}

