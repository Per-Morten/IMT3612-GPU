#ifndef PMS_LIB_MEMSET
#define PMS_LIB_MEMSET
#include <pms_lib_types.h>

#define PMS_LIB_MEMSET_GENERATOR(memory_region, postfix)                                    \
memory_region void*                                                                         \
pms_memset_##postfix(memory_region void* dest,                                              \
                     const uint8_t value,                                                   \
                     const size_t count)                                                    \
{                                                                                           \
    memory_region uint8_t* ptr = dest;                                                      \
    for (size_t i = 0; i < count; ++i)                                                      \
    {                                                                                       \
        ptr[i] = value;                                                                     \
    }                                                                                       \
    return dest;                                                                            \
}                                                                                           \

PMS_LIB_MEMSET_GENERATOR(__global,  g);
PMS_LIB_MEMSET_GENERATOR(__local,   l);
PMS_LIB_MEMSET_GENERATOR(__private, p);

#endif
