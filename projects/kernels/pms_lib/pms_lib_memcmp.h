#ifndef PMS_LIB_MEMCMP
#define PMS_LIB_MEMCMP
#include <pms_lib_types.h>

#define PMS_LIB_MEMCMP_GENERATOR(memory_region_a, memory_region_b, postfix)     \
int32_t                                                                         \
pms_memcmp_##postfix(memory_region_a const void* restrict lhs,                  \
                     memory_region_b const void* restrict rhs,                  \
                     const size_t count)                                        \
{                                                                               \
    memory_region_a const uint8_t* lhs_ptr = lhs;                               \
    memory_region_b const uint8_t* rhs_ptr = rhs;                               \
                                                                                \
    for (size_t i = 0; i != count; ++i, ++lhs_ptr, ++rhs_ptr)                   \
    {                                                                           \
        if (*lhs_ptr != *rhs_ptr)                                               \
        {                                                                       \
            return *lhs_ptr - *rhs_ptr;                                         \
        }                                                                       \
    }                                                                           \
    return 0;                                                                   \
}                                                                               \

PMS_LIB_MEMCMP_GENERATOR(__global, __global,   gg);
PMS_LIB_MEMCMP_GENERATOR(__global, __constant, gc);
PMS_LIB_MEMCMP_GENERATOR(__global, __local,    gl);
PMS_LIB_MEMCMP_GENERATOR(__global, __private,  gp);

PMS_LIB_MEMCMP_GENERATOR(__local, __global,   lg);
PMS_LIB_MEMCMP_GENERATOR(__local, __constant, lc);
PMS_LIB_MEMCMP_GENERATOR(__local, __local,    ll);
PMS_LIB_MEMCMP_GENERATOR(__local, __private,  lp);

PMS_LIB_MEMCMP_GENERATOR(__private, __global,   pg);
PMS_LIB_MEMCMP_GENERATOR(__private, __constant, pc);
PMS_LIB_MEMCMP_GENERATOR(__private, __local,    pl);
PMS_LIB_MEMCMP_GENERATOR(__private, __private,  pp);

#endif
