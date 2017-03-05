#include <pms_lib_types.h>
#include <pms_lib_string.h>

__kernel void strlen_test(__global const char* str,
                          __global uint32_t* strlen)
{
    int32_t global_id = get_group_id(0);

    if (global_id == 0)
    {
        strlen[global_id] = pms_strlen_g(str);
    }
}
