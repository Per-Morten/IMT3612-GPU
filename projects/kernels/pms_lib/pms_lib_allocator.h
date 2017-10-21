#ifndef PMS_LIB_ALLOCATOR
#define PMS_LIB_ALLOCATOR

#define PMS_LIB_STACK_ALLOCATE_GENERATOR(memory_region, postfix)            \
memory_region void*                                                         \
pms_stack_allocate_##postfix(memory_region void* memory,                    \
                             size_t* heap_index,                            \
                             size_t size)                                   \
{                                                                           \
    memory_region void* mem = (memory_region void*)&memory[*heap_index];    \
    (*heap_index) += size;                                                  \
    return mem;                                                             \
}                                                                           \

PMS_LIB_STACK_ALLOCATE_GENERATOR(__global,   g);
PMS_LIB_STACK_ALLOCATE_GENERATOR(__local,    l);
PMS_LIB_STACK_ALLOCATE_GENERATOR(__private,  p);

#define PMS_LIB_STACK_DEALLOCATE_GENERATOR(memory_region, postfix)          \
void                                                                        \
pms_stack_deallocate_##postfix(memory_region void* memory,                  \
                               size_t* heap_index,                          \
                               size_t size)                                 \
{                                                                           \
    (*heap_index) -= size;                                                  \
}                                                                           \

PMS_LIB_STACK_DEALLOCATE_GENERATOR(__global,   g);
PMS_LIB_STACK_DEALLOCATE_GENERATOR(__local,    l);
PMS_LIB_STACK_DEALLOCATE_GENERATOR(__private,  p);

#endif
