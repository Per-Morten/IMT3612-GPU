#include <pms_common.h>
#include <stdio.h>

int32_t
pms_read_kernel(const char* filename, 
                char* buffer, 
                size_t size_of_buffer)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        return PMS_FAILURE;
    }

    const size_t characters_read = fread(buffer, 
                                         sizeof(char), 
                                         size_of_buffer, 
                                         file);

    int32_t result = PMS_SUCCESS;

    if (characters_read < size_of_buffer &&
        feof(file) == 0 &&
        ferror(file) != 0)
    {
        result = PMS_FAILURE;
    }

    fclose(file);

    return result;
}
