#ifndef PMS_LIB_STRNCAT
#define PMS_LIB_STRNCAT
#include <pms_lib_types.h>

#define PMS_LIB_STRNCAT_GENERATOR(memory_region_a, memory_region_b, postfix)    \
size_t                                                                          \
pms_strncat_##postfix(memory_region_a char* dest,                               \
                      memory_region_b const char* src,                          \
                      const size_t count)                                       \
{                                                                               \
    size_t len = 0;                                                             \
    while ((*dest))                                                             \
    {                                                                           \
        ++dest;                                                                 \
        ++len;                                                                  \
    }                                                                           \
                                                                                \
    size_t i = 0;                                                               \
    while (i++ < count && ((*dest++ = *src++) != '\0'))                         \
    {                                                                           \
                                                                                \
    }                                                                           \
    *dest = '\0';                                                               \
    return len + i;                                                             \
}                                                                               \

PMS_LIB_STRNCAT_GENERATOR(__global, __global,   gg);
PMS_LIB_STRNCAT_GENERATOR(__global, __constant, gc);
PMS_LIB_STRNCAT_GENERATOR(__global, __local,    gl);
PMS_LIB_STRNCAT_GENERATOR(__global, __private,  gp);

PMS_LIB_STRNCAT_GENERATOR(__local, __global,   lg);
PMS_LIB_STRNCAT_GENERATOR(__local, __constant, lc);
PMS_LIB_STRNCAT_GENERATOR(__local, __local,    ll);
PMS_LIB_STRNCAT_GENERATOR(__local, __private,  lp);

PMS_LIB_STRNCAT_GENERATOR(__private, __global,   pg);
PMS_LIB_STRNCAT_GENERATOR(__private, __constant, pc);
PMS_LIB_STRNCAT_GENERATOR(__private, __local,    pl);
PMS_LIB_STRNCAT_GENERATOR(__private, __private,  pp);

#endif
