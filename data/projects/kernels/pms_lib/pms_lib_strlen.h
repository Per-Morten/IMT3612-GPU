#ifndef PMS_LIB_STRLEN
#define PMS_LIB_STRLEN
#include <pms_lib_types.h>

#define PMS_LIB_STRLEN_GENERATOR(memory_region_a, postfix)                      \
size_t                                                                          \
pms_strlen_##postfix(memory_region_a const char* str)                           \
{                                                                               \
    size_t len = 0;                                                             \
    while ((*str++))                                                            \
    {                                                                           \
        len++;                                                                  \
    }                                                                           \
    return len;                                                                 \
}                                                                               \

PMS_LIB_STRLEN_GENERATOR(__global,   g);
PMS_LIB_STRLEN_GENERATOR(__constant, c);
PMS_LIB_STRLEN_GENERATOR(__local,    l);
PMS_LIB_STRLEN_GENERATOR(__private,  p);

#endif
