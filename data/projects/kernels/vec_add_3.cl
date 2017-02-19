__kernel void vec_add_3(__global const float* a,
                        __global const float* b,
                        __global const float* c,
                        __global float* result,
                        const size_t count)
{
    int gid = get_global_id(0);

    if (gid < count)
    {
        result[gid] = a[gid] + b[gid] + c[gid];
    }
}
