#include <pms_lib_types.h>
#include <pms_lib_string.h>
#include <pms_string_twist.h>

#ifndef PMS_TWIST_MAX_LEN
#define PMS_TWIST_MAX_LEN 1024
#endif

__kernel void string_twist_test(__global const char* base_str,
                                __global const uint32_t* sequence,
                                const uint32_t sequence_count,
                                __global char* result)
{
    size_t global_id = get_global_id(0);
    if (global_id == 0)
    {
        __private char base_buffer[PMS_TWIST_MAX_LEN];
        __private char buffer[PMS_TWIST_MAX_LEN];
        pms_strcpy_pg(base_buffer, base_str);

        for (uint32_t i = 0; i < sequence_count; ++i)
        {
            switch(sequence[i])
            {
                case 0: 
                    pms_string_twist_1_pp(buffer, base_buffer);
                    break;
                case 1: 
                    pms_string_twist_2_pp(buffer, base_buffer);
                    break;
                case 2: 
                    pms_string_twist_3_pp(buffer, base_buffer);
                    break;
                case 3: 
                    pms_string_twist_4_pp(buffer, base_buffer);
                    break;
                default:
                    break;
            }
            pms_strcpy_pp(base_buffer, buffer);
        }

        pms_strcpy_gp(result, base_buffer);
    }
}
