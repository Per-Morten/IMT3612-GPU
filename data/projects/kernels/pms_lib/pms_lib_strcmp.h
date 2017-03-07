#ifndef PMS_LIB_STRCMP
#define PMS_LIB_STRCMP
#include <pms_lib_types.h>
// Taken from: https://github.com/lattera/glibc/blob/master/string/test-strcmp.c

///////////////////////////////////////////////////////////////////////////////
/// Global
///////////////////////////////////////////////////////////////////////////////
int32_t
pms_strcmp_gg(__global      const char* lhs,
              __global      const char* rhs)
{
    int32_t ret = 0;
    while ((ret = (*(__global uchar*)lhs) - 
                  (*(__global uchar*)rhs++)) == 0 && (*lhs++))
    {

    }
    return ret;
}

int32_t
pms_strcmp_gc(__global      const char* lhs,
              __constant    const char* rhs)
{
    int32_t ret = 0;
    while ((ret = (*(__global uchar*)lhs) - 
                  (*(__constant uchar*)rhs++)) == 0 && (*lhs++))
    {

    }
    return ret;
}

int32_t
pms_strcmp_gl(__global      const char* lhs,
              __local       const char* rhs)
{
    int32_t ret = 0;
    while ((ret = (*(__global uchar*)lhs) - 
                  (*(__local uchar*)rhs++)) == 0 && (*lhs++))
    {

    }
    return ret;
}

int32_t
pms_strcmp_gp(__global      const char* lhs,
              __private     const char* rhs)
{
    int32_t ret = 0;
    while ((ret = (*(__global uchar*)lhs) - 
                  (*(__private uchar*)rhs++)) == 0 && (*lhs++))
    {

    }
    return ret;
}

///////////////////////////////////////////////////////////////////////////////
/// Constant
///////////////////////////////////////////////////////////////////////////////
int32_t
pms_strcmp_cc(__constant    const char* lhs,
              __constant    const char* rhs)
{
    int32_t ret = 0;
    while ((ret = (*(__constant uchar*)lhs) - 
                  (*(__constant uchar*)rhs++)) == 0 && (*lhs++))
    {

    }
    return ret;
}

int32_t
pms_strcmp_cg(__constant    const char* lhs,
              __global      const char* rhs)
{
    int32_t ret = 0;
    while ((ret = (*(__constant uchar*)lhs) - 
                  (*(__global uchar*)rhs++)) == 0 && (*lhs++))
    {

    }
    return ret;
}

int32_t
pms_strcmp_cl(__constant    const char* lhs,
              __local       const char* rhs)
{
    int32_t ret = 0;
    while ((ret = (*(__constant uchar*)lhs) - 
                  (*(__local uchar*)rhs++)) == 0 && (*lhs++))
    {

    }
    return ret;
}

int32_t
pms_strcmp_cp(__constant    const char* lhs,
              __private     const char* rhs)
{
    int32_t ret = 0;
    while ((ret = (*(__constant uchar*)lhs) - 
                  (*(__private uchar*)rhs++)) == 0 && (*lhs++))
    {

    }
    return ret;
}

///////////////////////////////////////////////////////////////////////////////
/// Local
///////////////////////////////////////////////////////////////////////////////
int32_t
pms_strcmp_ll(__local       const char* lhs,
              __local       const char* rhs)
{
    int32_t ret = 0;
    while ((ret = (*(__local uchar*)lhs) - 
                  (*(__local uchar*)rhs++)) == 0 && (*lhs++))
    {

    }
    return ret;
}

int32_t
pms_strcmp_lg(__local       const char* lhs,
              __global      const char* rhs)
{
    int32_t ret = 0;
    while ((ret = (*(__local uchar*)lhs) - 
                  (*(__global uchar*)rhs++)) == 0 && (*lhs++))
    {

    }
    return ret;
}

int32_t
pms_strcmp_lc(__local       const char* lhs,
              __constant    const char* rhs)
{
    int32_t ret = 0;
    while ((ret = (*(__local uchar*)lhs) - 
                  (*(__constant uchar*)rhs++)) == 0 && (*lhs++))
    {

    }
    return ret;
}

int32_t
pms_strcmp_lp(__local       const char* lhs,
              __private     const char* rhs)
{
    int32_t ret = 0;
    while ((ret = (*(__local uchar*)lhs) - 
                  (*(__private uchar*)rhs++)) == 0 && (*lhs++))
    {

    }
    return ret;
}

///////////////////////////////////////////////////////////////////////////////
/// Private
///////////////////////////////////////////////////////////////////////////////

int32_t
pms_strcmp_pp(__private     const char* lhs,
              __private     const char* rhs)
{
    int32_t ret = 0;
    while ((ret = (*(__private uchar*)lhs) - 
                  (*(__private uchar*)rhs++)) == 0 && (*lhs++))
    {

    }
    return ret;
}

int32_t
pms_strcmp_pg(__private     const char* lhs,
              __global      const char* rhs)
{
    int32_t ret = 0;
    while ((ret = (*(__private uchar*)lhs) - 
                  (*(__global uchar*)rhs++)) == 0 && (*lhs++))
    {

    }
    return ret;
}

int32_t
pms_strcmp_pc(__private     const char* lhs,
              __constant    const char* rhs)
{
    int32_t ret = 0;
    while ((ret = (*(__private uchar*)lhs) - 
                  (*(__constant uchar*)rhs++)) == 0 && (*lhs++))
    {

    }
    return ret;
}

int32_t
pms_strcmp_pl(__private     const char* lhs,
              __local       const char* rhs)
{
    int32_t ret = 0;
    while ((ret = (*(__private uchar*)lhs) - 
                  (*(__local uchar*)rhs++)) == 0 && (*lhs++))
    {

    }
    return ret;
}

#endif
