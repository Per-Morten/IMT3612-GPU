#ifndef PMS_LIB_STRCHR
#define PMS_LIB_STRCHR

#define PMS_LIB_STRCHR_GENERATOR(memory_region, postfix)     \
memory_region const char*                                    \
pms_strchr_##postfix(memory_region const char* str, char ch) \
{                                                            \
    memory_region const char* ret = NULL;                    \
    while ((!(ret = (*str == ch) ? str : NULL)) && (*str++)) \
    {                                                        \
                                                             \
    }                                                        \
    return ret;                                              \
}                                                            \

PMS_LIB_STRCHR_GENERATOR(__global,   g);
PMS_LIB_STRCHR_GENERATOR(__constant, c);
PMS_LIB_STRCHR_GENERATOR(__local,    l);
PMS_LIB_STRCHR_GENERATOR(__private,  p);

#endif
