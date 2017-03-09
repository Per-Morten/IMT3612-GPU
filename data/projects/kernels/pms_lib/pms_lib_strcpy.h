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
#define PMS_LIB_STRCPY_GENERATOR(memory_region_a, memory_region_b, postfix)     \
size_t                                                                          \
pms_strcpy_##postfix(memory_region_a char* dest,                                \
                     memory_region_b const char* src)                           \
{                                                                               \
    size_t count = 0;                                                           \
    while ((*dest++ = *src++) != '\0')                                          \
    {                                                                           \
        ++count;                                                                \
    }                                                                           \
    *dest = '\0';                                                               \
    return count;                                                               \
}                                                                               \

PMS_LIB_STRCPY_GENERATOR(__global, __global,   gg);
PMS_LIB_STRCPY_GENERATOR(__global, __constant, gc);
PMS_LIB_STRCPY_GENERATOR(__global, __local,    gl);
PMS_LIB_STRCPY_GENERATOR(__global, __private,  gp);

PMS_LIB_STRCPY_GENERATOR(__local, __global,   lg);
PMS_LIB_STRCPY_GENERATOR(__local, __constant, lc);
PMS_LIB_STRCPY_GENERATOR(__local, __local,    ll);
PMS_LIB_STRCPY_GENERATOR(__local, __private,  lp);

PMS_LIB_STRCPY_GENERATOR(__private, __global,   pg);
PMS_LIB_STRCPY_GENERATOR(__private, __constant, pc);
PMS_LIB_STRCPY_GENERATOR(__private, __local,    pl);
PMS_LIB_STRCPY_GENERATOR(__private, __private,  pp);

#endif
