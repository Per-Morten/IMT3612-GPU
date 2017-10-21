#ifndef PMS_LIB_MEMCPY
#define PMS_LIB_MEMCPY
#include <pms_lib_types.h>

#define PMS_LIB_MEMCPY_GENERATOR(memory_region_a, memory_region_b, postfix)     \
memory_region_a void*                                                           \
pms_memcpy_##postfix(memory_region_a void* restrict dest,                       \
                     memory_region_b const void* restrict src,                  \
                     const size_t count)                                        \
{                                                                               \
    memory_region_a uint8_t* dest_ptr = dest;                                   \
    memory_region_b const uint8_t* src_ptr = src;                               \
    for (size_t i = 0; i != count; ++i)                                         \
    {                                                                           \
        *dest_ptr++ = *src_ptr++;                                               \
    }                                                                           \
    return dest;                                                                \
}                                                                               \

PMS_LIB_MEMCPY_GENERATOR(__global, __global,   gg);
PMS_LIB_MEMCPY_GENERATOR(__global, __constant, gc);
PMS_LIB_MEMCPY_GENERATOR(__global, __local,    gl);
PMS_LIB_MEMCPY_GENERATOR(__global, __private,  gp);

PMS_LIB_MEMCPY_GENERATOR(__local, __global,   lg);
PMS_LIB_MEMCPY_GENERATOR(__local, __constant, lc);
PMS_LIB_MEMCPY_GENERATOR(__local, __local,    ll);
PMS_LIB_MEMCPY_GENERATOR(__local, __private,  lp);

PMS_LIB_MEMCPY_GENERATOR(__private, __global,   pg);
PMS_LIB_MEMCPY_GENERATOR(__private, __constant, pc);
PMS_LIB_MEMCPY_GENERATOR(__private, __local,    pl);
PMS_LIB_MEMCPY_GENERATOR(__private, __private,  pp);

#endif
