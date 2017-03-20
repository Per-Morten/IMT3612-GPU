#include <pms_string_twist.h>

#ifndef PMS_TWIST_MAX_LEN
#define PMS_TWIST_MAX_LEN 1024
#endif

// Need an offset in the sequence, so we can have a 2d type array without **
// Not needed after all, can just abort the search ones the target has been found,
// Just need an atomic variable to tell when to stop.
__kernel void bottoms_up_search(__global const char* base_str,
                                __global const char* target_str,
                                __global const uint32_t* root,
                                __global uint32_t* sequence, 
                                __global uint32_t* sequence_count)
{
    size_t global_id = get_global_id(0);
    //sequence[global_id] = root[global_id];
    sequence[0] = 0;
    sequence[1] = 3;
    sequence[2] = 0;
    sequence[3] = 3;

    *sequence_count = 4;
}

