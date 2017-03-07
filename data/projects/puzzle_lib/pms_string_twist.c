#include <pms_string_twist.h>
#include <pms_common.h>

#include <string.h>
#include <stdlib.h>
#include <stddef.h>

int32_t
pms_string_twist_1(char* buffer,
                   const char* base_str)
{
    size_t len = strlen(base_str) * 2 / 3;

    strcpy(buffer, base_str + len);
    strncat(buffer, base_str, len);

    return 0;
}

int32_t
pms_string_twist_2(char* buffer,
                   const char* base_str)
{
    size_t len = strlen(base_str) / 2;

    strcpy(buffer, base_str + len);
    strncat(buffer, base_str, len);

    return 0;
}

int32_t
pms_string_twist_3(char* buffer,
                   const char* base_str)
{
    size_t len = strlen(base_str);
    strcpy(buffer, base_str + (len * 3 / 4));    
    strncat(buffer, base_str + (len / 4), len / 2);
    strncat(buffer, base_str, len / 4);

    return 0;
}

int32_t
pms_string_twist_4(char* buffer,
                   const char* base_str)
{
    char* space = strchr(base_str, ' ');
    size_t distance = (space && *space) ? 
                       space - base_str : (size_t)-1;
    
    strcpy(buffer, base_str + distance + 1);
    strcat(buffer, " ");
    strncat(buffer, base_str, distance);
    
    return 0;
}
