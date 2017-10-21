#ifndef PMS_LIB_STRCAT
#define PMS_LIB_STRCAT
#include <pms_lib_types.h>
#include <pms_lib_strcpy.h>

#define PMS_LIB_STRCAT_GENERATOR(memory_region_a, memory_region_b, postfix)     \
size_t                                                                          \
pms_strcat_##postfix(memory_region_a char* dest,                                \
                     memory_region_b const char* src)                           \
{                                                                               \
    size_t count = 0;                                                           \
    while ((*dest))                                                             \
    {                                                                           \
        ++dest;                                                                 \
        ++count;                                                                \
    }                                                                           \
                                                                                \
    return count + pms_strcpy_##postfix(dest, src);                             \
}                                                                               \

PMS_LIB_STRCAT_GENERATOR(__global, __global,   gg);
PMS_LIB_STRCAT_GENERATOR(__global, __constant, gc);
PMS_LIB_STRCAT_GENERATOR(__global, __local,    gl);
PMS_LIB_STRCAT_GENERATOR(__global, __private,  gp);

PMS_LIB_STRCAT_GENERATOR(__local, __global,   lg);
PMS_LIB_STRCAT_GENERATOR(__local, __constant, lc);
PMS_LIB_STRCAT_GENERATOR(__local, __local,    ll);
PMS_LIB_STRCAT_GENERATOR(__local, __private,  lp);

PMS_LIB_STRCAT_GENERATOR(__private, __global,   pg);
PMS_LIB_STRCAT_GENERATOR(__private, __constant, pc);
PMS_LIB_STRCAT_GENERATOR(__private, __local,    pl);
PMS_LIB_STRCAT_GENERATOR(__private, __private,  pp);

#endif
