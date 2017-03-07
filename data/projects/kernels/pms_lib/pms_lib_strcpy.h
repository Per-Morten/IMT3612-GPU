#ifndef PMS_LIB_STRCPY
#define PMS_LIB_STRCPY
#include <pms_lib_types.h>
///////////////////////////////////////////////////////////////////////////////
/// \brief Copies string from src into dest.
///        Behavior is undefined if there is not enough
///        room within dest.
///
/// \return The number of characters copied. 
///         I.e. new strlen.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Global
///////////////////////////////////////////////////////////////////////////////
int32_t
pms_strcpy_gg(__global      char* dest,
              __global      const char* src)
{
    int32_t count = 0;
    while ((*src))
    {
        ++count;
        *dest++ = *src++;
    }
    *dest = '\0';
    return count;
}

int32_t
pms_strcpy_gc(__global      char* dest,
              __constant    const char* src)
{
    int32_t count = 0;
    while ((*src))
    {
        ++count;
        *dest++ = *src++;
    }
    *dest = '\0';
    return count;
}

int32_t
pms_strcpy_gl(__global      char* dest,
              __local       const char* src)
{
    int32_t count = 0;
    while ((*src))
    {
        ++count;
        *dest++ = *src++;
    }
    *dest = '\0';
    return count;
}

int32_t
pms_strcpy_gp(__global      char* dest,
              __private     const char* src)
{
    int32_t count = 0;
    while ((*src))
    {
        ++count;
        *dest++ = *src++;
    }
    *dest = '\0';
    return count;
}

///////////////////////////////////////////////////////////////////////////////
/// Local
///////////////////////////////////////////////////////////////////////////////
int32_t
pms_strcpy_ll(__local       char* dest,
              __local       const char* src)
{
    int32_t count = 0;
    while ((*src))
    {
        ++count;
        *dest++ = *src++;
    }
    *dest = '\0';
    return count;
}

int32_t
pms_strcpy_lg(__local       char* dest,
              __global      const char* src)
{
    int32_t count = 0;
    while ((*src))
    {
        ++count;
        *dest++ = *src++;
    }
    *dest = '\0';
    return count;
}

int32_t
pms_strcpy_lc(__local       char* dest,
              __constant    const char* src)
{
    int32_t count = 0;
    while ((*src))
    {
        ++count;
        *dest++ = *src++;
    }
    *dest = '\0';
    return count;
}


int32_t
pms_strcpy_lp(__local       char* dest,
              __private     const char* src)
{
    int32_t count = 0;
    while ((*src))
    {
        ++count;
        *dest++ = *src++;
    }
    *dest = '\0';
    return count;
}

///////////////////////////////////////////////////////////////////////////////
/// Private
///////////////////////////////////////////////////////////////////////////////
int32_t
pms_strcpy_pl(__private     char* dest,
              __local       const char* src)
{
    int32_t count = 0;
    while ((*src))
    {
        ++count;
        *dest++ = *src++;
    }
    *dest = '\0';
    return count;
}

int32_t
pms_strcpy_pg(__private     char* dest,
              __global      const char* src)
{
    int32_t count = 0;
    while ((*src))
    {
        ++count;
        *dest++ = *src++;
    }
    *dest = '\0';
    return count;
}

int32_t
pms_strcpy_pc(__private     char* dest,
              __constant    const char* src)
{
    int32_t count = 0;
    while ((*src))
    {
        ++count;
        *dest++ = *src++;
    }
    *dest = '\0';
    return count;
}


int32_t
pms_strcpy_pp(__private     char* dest,
              __private     const char* src)
{
    int32_t count = 0;
    while ((*src))
    {
        ++count;
        *dest++ = *src++;
    }
    *dest = '\0';
    return count;
}

#endif
