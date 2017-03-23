#include <pms_lib_memory.h>
#include <pms_lib_string.h>
#include <pms_lib_allocator.h>
#include <pms_string_twist.h>

#ifndef PMS_TWIST_MAX_LEN
#define PMS_TWIST_MAX_LEN 64
#endif

#ifndef PMS_TWIST_MAX_FUNCTION_COUNT
#define PMS_TWIST_MAX_FUNCTION_COUNT 4
#endif

atomic_uint g_found_flag = ATOMIC_VAR_INIT(0);

__global uint8_t global_memory[4][4096];

bool
claim_first()
{
    uint32_t expected = 0;

    // Check if someone has already found a solution.
    while (!atomic_compare_exchange_weak_explicit(&g_found_flag, 
                                                  &expected,
                                                  1,
                                                  memory_order_acq_rel,
                                                  memory_order_acquire))
    {
        // If a solution has already been found, I can't claim first.
        if (expected)
        {
            return false;
        }
    }
    return true;
}

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

    sequence[0] = root_value[get_global_id(0)];

    uint32_t sequence_count = max_depth;
    uint32_t already_found = atomic_load_explicit(&g_found_flag, 
                                                  memory_order_acquire);
                              
    const size_t last_legal_value = PMS_TWIST_MAX_FUNCTION_COUNT - 1;

    while (sequence_count != 1 && !already_found)
    {
        const bool correct_sequence = 
            pms_string_twist_validate_ggp(base_str,
                                          target_str,
                                          sequence,
                                          sequence_count);

        if (correct_sequence)
        {
            if (!claim_first()) 
            {
                return;
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

__kernel void depth_first_search(__global const char* restrict base_str,
                                 __global const char* restrict target_str,
                                 __global const uint32_t* restrict root_value,
                                 const uint32_t max_depth,
                                 __global uint32_t* restrict out_sequence,
                                 __global uint32_t* restrict out_sequence_count)
{
    const size_t global_id = get_global_id(0);

    __private uint32_t len[128] = {0};
    __global uint32_t* stack[128];
    size_t head = 0;
    size_t heap_index = 0;

    len[head] = 1;
    stack[head] = pms_stack_allocate_g(&global_memory[global_id], &heap_index, sizeof(uint32_t));
    stack[head++][0] = root_value[global_id];

    uint32_t already_found = atomic_load_explicit(&g_found_flag,
                                                  memory_order_acquire);

    while (head != 0 && !already_found)
    {
        size_t sequence_count = len[--head];
        private uint32_t sequence[PMS_TWIST_MAX_LEN];
        pms_memcpy_pg(sequence, stack[head], sequence_count * sizeof(uint32_t));
        pms_stack_deallocate_g(&global_memory[global_id], &heap_index, sequence_count * sizeof(uint32_t));

        if (sequence_count < max_depth)
        {
            for (ptrdiff_t i = PMS_TWIST_MAX_FUNCTION_COUNT - 1; i >= 0; --i)
            {
                stack[head] = pms_stack_allocate_g(&global_memory[global_id], &heap_index, (sequence_count + 1) * sizeof(uint32_t));
                pms_memcpy_gp(stack[head], sequence, sequence_count * sizeof(uint32_t));
                stack[head][sequence_count] = i;
                len[head++] = sequence_count + 1;
            }
        }

        const bool is_correct_sequence =
            pms_string_twist_validate_ggp(base_str,
                                          target_str,
                                          sequence,
                                          sequence_count);

        if (is_correct_sequence)
        {
            if (!claim_first())
            {
                return;
            }

            *out_sequence_count = sequence_count;
            pms_memcpy_gp(out_sequence, sequence, sizeof(uint32_t) * sequence_count);
        }

        already_found = atomic_load_explicit(&g_found_flag,
                                             memory_order_acquire);
    }
}
