#ifndef PMS_LIB_STRNCMP
#define PMS_LIB_STRNCMP
#include <pms_lib_types.h>
// Based on: https://github.com/lattera/glibc/blob/master/string/test-strcmp.c

#define PMS_LIB_STRNCMP_GENERATOR(memory_region_a, memory_region_b, postfix)    \
int32_t                                                                         \
pms_strncmp_##postfix(memory_region_a const char* lhs,                          \
                      memory_region_b const char* rhs,                          \
                      const size_t count)                                       \
{                                                                               \
    int32_t ret = 0;                                                            \
    size_t i = 0;                                                               \
    while ((ret = (*(memory_region_a uchar*)lhs) -                              \
                  (*(memory_region_b uchar*)rhs++)) == 0 &&                     \
           (*lhs++) &&                                                          \
           i++ < count)                                                         \
    {                                                                           \
                                                                                \
    }                                                                           \
    return ret;                                                                 \
}                                                                               \

PMS_LIB_STRNCMP_GENERATOR(__global, __global,   gg);
PMS_LIB_STRNCMP_GENERATOR(__global, __constant, gc);
PMS_LIB_STRNCMP_GENERATOR(__global, __local,    gl);
PMS_LIB_STRNCMP_GENERATOR(__global, __private,  gp);

PMS_LIB_STRNCMP_GENERATOR(__constant, __global,   cg);
PMS_LIB_STRNCMP_GENERATOR(__constant, __constant, cc);
PMS_LIB_STRNCMP_GENERATOR(__constant, __local,    cl);
PMS_LIB_STRNCMP_GENERATOR(__constant, __private,  cp);

PMS_LIB_STRNCMP_GENERATOR(__local, __global,   lg);
PMS_LIB_STRNCMP_GENERATOR(__local, __constant, lc);
PMS_LIB_STRNCMP_GENERATOR(__local, __local,    ll);
PMS_LIB_STRNCMP_GENERATOR(__local, __private,  lp);

PMS_LIB_STRNCMP_GENERATOR(__private, __global,   pg);
PMS_LIB_STRNCMP_GENERATOR(__private, __constant, pc);
PMS_LIB_STRNCMP_GENERATOR(__private, __local,    pl);
PMS_LIB_STRNCMP_GENERATOR(__private, __private,  pp);

#endif
