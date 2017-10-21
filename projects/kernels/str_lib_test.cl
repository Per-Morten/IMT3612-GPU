#include <pms_lib_types.h>
#include <pms_lib_string.h>

#define PMS_MAX_STRLEN 200

__kernel void strlen_test(__global const char* str,
                          __global uint32_t* strlen)
{
    size_t global_id = get_global_id(0);

    if (global_id == 0)
    {
        strlen[global_id] = pms_strlen_g(str);
    }
}

__kernel void strcmp_test(__global const char* lhs,
                          __global const char* rhs,
                          __global int32_t* result)
{
    size_t global_id = get_global_id(0);

    if (global_id == 0)
    {
        result[global_id] = pms_strcmp_gg(lhs, rhs);
    }
}

__kernel void strncmp_test(__global const char* lhs,
                           __global const char* rhs,
                           __global int32_t* result,
                           uint32_t compare_count)
{
    size_t global_id = get_global_id(0);

    __private char str[PMS_MAX_STRLEN];
    pms_strcpy_pg(str, lhs);

    if (global_id == 0)
    {
        result[global_id] = pms_strncmp_pg(str, rhs, (size_t)compare_count);
    }
}

__kernel void strcpy_test(__global const char* src,
                          __global char* dest)
{
    size_t global_id = get_global_id(0);

    if (global_id == 0)
    {
        pms_strcpy_gg(dest, src);
    }
}

__kernel void strcat_test(__global const char* src_1,
                          __global const char* src_2,
                          __global char* dest)
{
    size_t global_id = get_global_id(0);

    if (global_id == 0)
    {
        __private char str[PMS_MAX_STRLEN];
        pms_strcpy_pg(str, src_1);
        pms_strcat_pg(str, src_2);

        pms_strcpy_gp(dest, str);
    }
}

__kernel void strncat_test(__global const char* src_1,
                           __global const char* src_2,
                           __global char* dest,
                           uint32_t max_cat)
{
    size_t global_id = get_global_id(0);

    if (global_id == 0)
    {
        __private char str[PMS_MAX_STRLEN];
        pms_strcpy_pg(str, src_1);
        pms_strncat_pg(str, src_2, max_cat);

        pms_strcpy_gp(dest, str);
    }
}

__kernel void strchr_test(__global const char* src,
                          __global int32_t* dist,
                          char target)
{
    size_t global_id = get_global_id(0);

    if (global_id == 0)
    {
        __global const char* ptr = pms_strchr_g(src, target);
        if (ptr != NULL)
        {
            dist[global_id] = ptr - src;
        }
    }
}

__kernel void toupper_test(__global const char* src,
                           __global char* dest)
{
    size_t global_id = get_global_id(0);

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
    size_t global_id = get_global_id(0);

    if (global_id == 0)
    {
        int32_t len = pms_strlen_g(src);
        for (int32_t i = 0; i < len; ++i)
        {
            dest[i] = pms_tolower(src[i]);
        }
    }
}
