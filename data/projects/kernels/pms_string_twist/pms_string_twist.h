#ifndef PMS_STRING_TWIST
#define PMS_STRING_TWIST
#include <pms_lib_types.h>
#include <pms_lib_string.h>

///////////////////////////////////////////////////////////////////////////////
/// String Twist 1
///////////////////////////////////////////////////////////////////////////////
#define PMS_STRING_TWIST_1_GENERATOR(memory_region_a, memory_region_b, pfix_1, pfix_2) \
void                                                                                   \
pms_string_twist_1_##pfix_1##pfix_2(memory_region_a char* buffer,                      \
                                    memory_region_b const char* base_str)              \
{                                                                                      \
    const size_t len = pms_strlen_##pfix_2(base_str) * 2 / 3;                          \
                                                                                       \
    pms_strcpy_##pfix_1##pfix_2(buffer, base_str + len);                               \
    pms_strncat_##pfix_1##pfix_2(buffer, base_str, len);                               \
}                                                                                      \

PMS_STRING_TWIST_1_GENERATOR(__global, __global,   g, g);
PMS_STRING_TWIST_1_GENERATOR(__global, __constant, g, c);
PMS_STRING_TWIST_1_GENERATOR(__global, __local,    g, l);
PMS_STRING_TWIST_1_GENERATOR(__global, __private,  g, p);

PMS_STRING_TWIST_1_GENERATOR(__local, __global,   l, g);
PMS_STRING_TWIST_1_GENERATOR(__local, __constant, l, c);
PMS_STRING_TWIST_1_GENERATOR(__local, __local,    l, l);
PMS_STRING_TWIST_1_GENERATOR(__local, __private,  l, p);

PMS_STRING_TWIST_1_GENERATOR(__private, __global,   p, g);
PMS_STRING_TWIST_1_GENERATOR(__private, __constant, p, c);
PMS_STRING_TWIST_1_GENERATOR(__private, __local,    p, l);
PMS_STRING_TWIST_1_GENERATOR(__private, __private,  p, p);

///////////////////////////////////////////////////////////////////////////////
/// String Twist 2
///////////////////////////////////////////////////////////////////////////////
#define PMS_STRING_TWIST_2_GENERATOR(memory_region_a, memory_region_b, pfix_1, pfix_2)  \
void                                                                                    \
pms_string_twist_2_##pfix_1##pfix_2(memory_region_a char* buffer,                       \
                                    memory_region_b const char* base_str)               \
{                                                                                       \
    const size_t len = pms_strlen_##pfix_2(base_str) / 2;                               \
                                                                                        \
    pms_strcpy_##pfix_1##pfix_2(buffer, base_str + len);                                \
    pms_strncat_##pfix_1##pfix_2(buffer, base_str, len);                                \
}                                                                                       \

PMS_STRING_TWIST_2_GENERATOR(__global, __global,   g, g);
PMS_STRING_TWIST_2_GENERATOR(__global, __constant, g, c);
PMS_STRING_TWIST_2_GENERATOR(__global, __local,    g, l);
PMS_STRING_TWIST_2_GENERATOR(__global, __private,  g, p);

PMS_STRING_TWIST_2_GENERATOR(__local, __global,   l, g);
PMS_STRING_TWIST_2_GENERATOR(__local, __constant, l, c);
PMS_STRING_TWIST_2_GENERATOR(__local, __local,    l, l);
PMS_STRING_TWIST_2_GENERATOR(__local, __private,  l, p);

PMS_STRING_TWIST_2_GENERATOR(__private, __global,   p, g);
PMS_STRING_TWIST_2_GENERATOR(__private, __constant, p, c);
PMS_STRING_TWIST_2_GENERATOR(__private, __local,    p, l);
PMS_STRING_TWIST_2_GENERATOR(__private, __private,  p, p);

///////////////////////////////////////////////////////////////////////////////
/// String Twist 3
///////////////////////////////////////////////////////////////////////////////
#define PMS_STRING_TWIST_3_GENERATOR(memory_region_a, memory_region_b, pfix_1, pfix_2)  \
void                                                                                    \
pms_string_twist_3_##pfix_1##pfix_2(memory_region_a char* buffer,                       \
                                    memory_region_b const char* base_str)               \
{                                                                                       \
    const size_t len = pms_strlen_##pfix_2(base_str);                                   \
    pms_strcpy_##pfix_1##pfix_2(buffer, base_str + (len * 3 / 4));                      \
    pms_strncat_##pfix_1##pfix_2(buffer, base_str + (len / 4), len / 2);                \
    pms_strncat_##pfix_1##pfix_2(buffer, base_str, len / 4);                            \
}                                                                                       \

PMS_STRING_TWIST_3_GENERATOR(__global, __global,   g, g);
PMS_STRING_TWIST_3_GENERATOR(__global, __constant, g, c);
PMS_STRING_TWIST_3_GENERATOR(__global, __local,    g, l);
PMS_STRING_TWIST_3_GENERATOR(__global, __private,  g, p);

PMS_STRING_TWIST_3_GENERATOR(__local, __global,   l, g);
PMS_STRING_TWIST_3_GENERATOR(__local, __constant, l, c);
PMS_STRING_TWIST_3_GENERATOR(__local, __local,    l, l);
PMS_STRING_TWIST_3_GENERATOR(__local, __private,  l, p);

PMS_STRING_TWIST_3_GENERATOR(__private, __global,   p, g);
PMS_STRING_TWIST_3_GENERATOR(__private, __constant, p, c);
PMS_STRING_TWIST_3_GENERATOR(__private, __local,    p, l);
PMS_STRING_TWIST_3_GENERATOR(__private, __private,  p, p);

///////////////////////////////////////////////////////////////////////////////
/// String Twist 4
///////////////////////////////////////////////////////////////////////////////
#define PMS_STRING_TWIST_4_GENERATOR(memory_region_a, memory_region_b, pfix_1, pfix_2)  \
void                                                                                    \
pms_string_twist_4_##pfix_1##pfix_2(memory_region_a char* buffer,                       \
                                    memory_region_b const char* base_str)               \
{                                                                                       \
    memory_region_b const char* space = pms_strchr_##pfix_2(base_str, ' ');             \
    const size_t dist = (space && *space) ?                                             \
                         space - base_str : (size_t)-1;                                 \
                                                                                        \
    pms_strcpy_##pfix_1##pfix_2(buffer, base_str + dist + 1);                           \
    pms_strcat_ ##pfix_1##c(buffer, " ");                                               \
    pms_strncat_##pfix_1##pfix_2(buffer, base_str, dist);                               \
}                                                                                       \

PMS_STRING_TWIST_4_GENERATOR(__global, __global,   g, g);
PMS_STRING_TWIST_4_GENERATOR(__global, __constant, g, c);
PMS_STRING_TWIST_4_GENERATOR(__global, __local,    g, l);
PMS_STRING_TWIST_4_GENERATOR(__global, __private,  g, p);

PMS_STRING_TWIST_4_GENERATOR(__local, __global,   l, g);
PMS_STRING_TWIST_4_GENERATOR(__local, __constant, l, c);
PMS_STRING_TWIST_4_GENERATOR(__local, __local,    l, l);
PMS_STRING_TWIST_4_GENERATOR(__local, __private,  l, p);

PMS_STRING_TWIST_4_GENERATOR(__private, __global,   p, g);
PMS_STRING_TWIST_4_GENERATOR(__private, __constant, p, c);
PMS_STRING_TWIST_4_GENERATOR(__private, __local,    p, l);
PMS_STRING_TWIST_4_GENERATOR(__private, __private,  p, p);


#endif
