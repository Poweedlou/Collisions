#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#define RADUIS 0.01
__kernel void test(uint mol, __global double3 *position, __global double3 *speed)
{
	__private int gid = get_global_id(0);
	barrier(CLK_GLOBAL_MEM_FENCE | CLK_LOCAL_MEM_FENCE);
	position[gid].xyz = (double3)((double) RADUIS, (double) RADUIS, (double) RADUIS);
}