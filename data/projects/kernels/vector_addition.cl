__kernel void vector_addition(__global const float* lhs,
                              __global const float* rhs,
                              __global float* result, 
                              const size_t count)
{
    int global_id = get_global_id(0);

    if (global_id < count)
    {
        result[global_id] = lhs[global_id] + rhs[global_id];
    }
}
