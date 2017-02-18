#include <stdio.h>
#include <stdarg.h>

void
pms_log(FILE* file,
        const char* type,
        //const char* filename,
        const char* func,
        const int   line,
        const char* fmt,
        ...)
{
    va_list args1;
    va_start(args1, fmt);
    va_list args2;
    va_copy(args2, args1);
    char buffer[1+vsnprintf(NULL, 0, fmt, args1)];
    va_end(args1);
    vsnprintf(buffer, sizeof(buffer), fmt, args2);
    va_end(args2);
    fprintf(file, "[%-5s]: %-20s:%4d: %s\n", type, func, line, buffer);

    fflush(file);
}
