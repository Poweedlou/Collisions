#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#define RADIUS (double)1.0
#define RAD22 (double)4.0
__kernel void test(ulong mol, __global double2 *position, __global double2 *speed, double2 shape, __global ulong *count, ulong work_items) //, ulong iter)
{
    //__private ulong time = 0;
    __private size_t gid_ = get_global_id(0);
    __private size_t gid;
    __private double2 per;
    __private double2 pra;
    __private double2 prb;
    __private double2 foo;
    __private double2 rel;
    __private double2 spd2;
    __private double2 pos;
    __private double2 pos2;
    __private double2 spd;
    //while (time < iter)
    for (ulong k = 0; k < (mol - (ulong)gid_ - 1) / (ulong)work_items + 1; k++)
    {
        gid = (ulong)gid_ + k * (ulong)work_items;
        pos = position[gid];
        spd = speed[gid];
        if (pos.x < RADIUS && spd.x < 0)
            spd.x *= -1.0;
        else if (pos.x > shape.x - RADIUS && spd.x > 0)
            spd.x *= -1.0;
        if (pos.y < RADIUS && spd.y < 0)
            spd.y *= -1.0;
        else if (pos.y > shape.y - RADIUS && spd.y > 0)
            spd.y *= -1.0;
        speed[gid] = spd;
        pos += spd;
		position[gid] = pos;
    }
    barrier(CLK_GLOBAL_MEM_FENCE | CLK_LOCAL_MEM_FENCE);
    for (ulong k = 0; k < ((mol - (ulong)gid_ - 1) / work_items + 1); k++)
    {
        gid = (ulong)gid_ + k * work_items;
        pos = position[gid];
        spd = speed[gid];
        for (ulong i = 1; i <= mol / 2; i++)
        {
            rel = position[(i + gid) % mol] - pos;
            spd2 = speed[(i + gid) % mol];
            if (dot(rel, rel) < RAD22 && dot(spd2 - spd, rel) < 0)
            {
                count[gid] += 1;
                per = (spd + spd2) / 2;
                pra = spd - per;
                prb = spd2 - per;
                foo = rel * 2 / dot(rel, rel);
                pra -= dot(pra, rel) * foo;
                prb -= dot(prb, rel) * foo;
                spd = per + pra;
                speed[gid] = spd;
                speed[(i + gid) % mol] = per + prb;
            }
        }

        //barrier(CLK_GLOBAL_MEM_FENCE | CLK_LOCAL_MEM_FENCE);
		//iter++;
    }
}