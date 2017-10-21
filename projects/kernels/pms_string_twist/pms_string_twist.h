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

///////////////////////////////////////////////////////////////////////////////
/// Validate
///////////////////////////////////////////////////////////////////////////////
#define PMS_STRING_TWIST_VALIDATE_GENERATOR(memory_region_a, memory_region_b, memory_region_c, pfix_1, pfix_2, pfix_3)  \
bool                                                                                                                    \
pms_string_twist_validate_##pfix_1##pfix_2##pfix_3(memory_region_a const char* base_str,                                \
                                                   memory_region_b const char* target_str,                              \
                                                   memory_region_c const uint32_t* sequence,                            \
                                                   const uint32_t sequence_count)                                       \
{                                                                                                                       \
    const size_t max_string_length = 1024;                                                                              \
    __private char base_buffer[max_string_length];                                                                      \
    __private char buffer[max_string_length];                                                                           \
    pms_strcpy_p##pfix_1(base_buffer, base_str);                                                                        \
                                                                                                                        \
    for (uint32_t i = 0; i < sequence_count; ++i)                                                                       \
    {                                                                                                                   \
        switch(sequence[i])                                                                                             \
        {                                                                                                               \
            case 0:                                                                                                     \
                pms_string_twist_1_pp(buffer, base_buffer);                                                             \
                 break;                                                                                                 \
            case 1:                                                                                                     \
                pms_string_twist_2_pp(buffer, base_buffer);                                                             \
                break;                                                                                                  \
            case 2:                                                                                                     \
                pms_string_twist_3_pp(buffer, base_buffer);                                                             \
                break;                                                                                                  \
            case 3:                                                                                                     \
                pms_string_twist_4_pp(buffer, base_buffer);                                                             \
                break;                                                                                                  \
            default:                                                                                                    \
                break;                                                                                                  \
        }                                                                                                               \
        pms_strcpy_pp(base_buffer, buffer);                                                                             \
    }                                                                                                                   \
                                                                                                                        \
    return pms_strcmp_p##pfix_2(base_buffer, target_str) == 0;                                                          \
}                                                                                                                       \

PMS_STRING_TWIST_VALIDATE_GENERATOR(__global, __global, __global,   g, g, g);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__global, __global, __constant, g, g, c);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__global, __global, __local,    g, g, l);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__global, __global, __private,  g, g, p);

PMS_STRING_TWIST_VALIDATE_GENERATOR(__global, __local, __global,   g, l, g);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__global, __local, __constant, g, l, c);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__global, __local, __local,    g, l, l);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__global, __local, __private,  g, l, p);

PMS_STRING_TWIST_VALIDATE_GENERATOR(__global, __private, __global,   g, p, g);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__global, __private, __constant, g, p, c);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__global, __private, __local,    g, p, l);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__global, __private, __private,  g, p, p);

PMS_STRING_TWIST_VALIDATE_GENERATOR(__local, __global, __global,   l, g, g);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__local, __global, __constant, l, g, c);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__local, __global, __local,    l, g, l);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__local, __global, __private,  l, g, p);

PMS_STRING_TWIST_VALIDATE_GENERATOR(__local, __local, __global,   l, l, g);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__local, __local, __constant, l, l, c);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__local, __local, __local,    l, l, l);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__local, __local, __private,  l, l, p);

PMS_STRING_TWIST_VALIDATE_GENERATOR(__local, __private, __global,   l, p, g);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__local, __private, __constant, l, p, c);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__local, __private, __local,    l, p, l);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__local, __private, __private,  l, p, p);

PMS_STRING_TWIST_VALIDATE_GENERATOR(__private, __global, __global,   p, g, g);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__private, __global, __constant, p, g, c);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__private, __global, __local,    p, g, l);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__private, __global, __private,  p, g, p);

PMS_STRING_TWIST_VALIDATE_GENERATOR(__private, __local, __global,   p, l, g);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__private, __local, __constant, p, l, c);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__private, __local, __local,    p, l, l);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__private, __local, __private,  p, l, p);

PMS_STRING_TWIST_VALIDATE_GENERATOR(__private, __private, __global,   p, p, g);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__private, __private, __constant, p, p, c);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__private, __private, __local,    p, p, l);
PMS_STRING_TWIST_VALIDATE_GENERATOR(__private, __private, __private,  p, p, p);

#endif
