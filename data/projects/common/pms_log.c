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
    va_list args;
    va_start(args, fmt);
    fprintf(file, "[%-5s]: %-8s:%4d: ", type, func, line); 
    vfprintf(file, fmt, args);
    fprintf(file, "\n");
    va_end(args);

    fflush(file);
}
