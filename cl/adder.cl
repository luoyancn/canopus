__kernel void adder(__global float * restrict a,
	__global float * restrict b, __global float * restrict result)
{
	int tid = get_global_id(0);
	result[tid] = a[tid] + b[tid];
}
