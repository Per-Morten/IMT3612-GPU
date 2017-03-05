#ifndef PMS_LIB_STRLEN
#define PMS_LIB_STRLEN
#include <pms_lib_types.h>

uint32_t
pms_strlen_g(__global const char* str)
{
    uint32_t len = 0;
    while ((*str++))
    {
        len++;
    }
    return len;
}

uint32_t
pms_strlen_c(__constant const char* str)
{
    uint32_t len = 0;
    while ((*str++))
    {
        len++;
    }
    return len;
}

uint32_t
pms_strlen_l(__local const char* str)
{
    uint32_t len = 0;
    while ((*str++))
    {
        len++;
    }
    return len;
}

uint32_t
pms_strlen_p(__private const char* str)
{
    uint32_t len = 0;
    while ((*str++))
    {
        len++;
    }
    return len;
}

#endif
