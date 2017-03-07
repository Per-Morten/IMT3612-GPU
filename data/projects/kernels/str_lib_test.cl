#include <pms_lib_types.h>
#include <pms_lib_string.h>

#define PMS_MAX_STRLEN 200

__kernel void strlen_test(__global const char* str,
                          __global uint32_t* strlen)
{
    int32_t global_id = get_global_id(0);

    if (global_id == 0)
    {
        strlen[global_id] = pms_strlen_g(str);
    }
}

__kernel void strcmp_test(__global const char* lhs,
                          __global const char* rhs,
                          __global int32_t* result)
{
    int32_t global_id = get_global_id(0);

    if (global_id == 0)
    {
        result[global_id] = pms_strcmp_gg(lhs, rhs);
    }
}

__kernel void strcpy_test(__global const char* src,
                          __global char* dest)
{
    int32_t global_id = get_global_id(0);

    if (global_id == 0)
    {
        pms_strcpy_gg(dest, src);
    }
}

__kernel void toupper_test(__global const char* src,
                           __global char* dest)
{
    int32_t global_id = get_global_id(0);

    if (global_id == 0)
    {
        int32_t len = pms_strlen_g(src);
        for (int32_t i = 0; i < len; i++)
        {
            dest[i] = pms_toupper(src[i]);
        }
    }
}

__kernel void tolower_test(__global const char* src,
                           __global char* dest)
{
    int32_t global_id = get_global_id(0);

    if (global_id == 0)
    {
        int32_t len = pms_strlen_g(src);
        for (int32_t i = 0; i < len; ++i)
        {
            dest[i] = pms_tolower(src[i]);
        }
    }
}
