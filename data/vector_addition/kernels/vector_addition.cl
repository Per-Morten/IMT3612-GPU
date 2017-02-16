__kernel void 
vector_add(__global const float* lhs,
           __global const float* rhs,
           __global float* result)
{
    int global_id = get_global_id(0);
    result[global_id] = lhs[global_id] + rhs[global_id];
}
