#include <pms_common.h>

int32_t
pms_cleanup_opencl(cl_context* context,
                   cl_command_queue* command_queue)
{
    if (command_queue)
    {
        clReleaseCommandQueue((*command_queue));
    }

    if (context)
    {
        clReleaseContext((*context));
    }
}
