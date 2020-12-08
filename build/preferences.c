#include <stdio.h>
#define CL_TARGET_OPENCL_VERSION 220
#include "CL/cl.h"



cl_int platform_info(cl_platform_id id)
{   
    char name[32];
    cl_int err = clGetPlatformInfo(id, CL_PLATFORM_NAME, sizeof(char) * 32, &name, NULL);
    if (err == CL_SUCCESS)
        printf("Name: %s\n", name);
    else
        printf("WTF is this platform? errno %d\n", (int) err);
    return err;
}

cl_platform_id choose_platform()
{
    cl_platform_id platforms[4];
    cl_uint avail;
    cl_int err = clGetPlatformIDs(4, &platforms, &avail);

    if (avail < 1) {
        printf("no OpenCL lol\n");
        exit(1);
    } else if (avail == 1) {
        return platforms[0];
    } else
    {
        printf("Choose a platform: \n");
        for (int i = 0; i < avail; i++)
        {
            printf("%d ", i);
            platform_info(platforms[i]);
        }
        int i;
        scanf("%d", &i);
        return platforms[i];
    }
}

cl_int device_info(cl_device_id id)
{   
    char name[128];
    cl_int err = clGetDeviceInfo(id, CL_DEVICE_NAME, sizeof(char) * 128, &name, NULL);
    if (err == CL_SUCCESS)
        printf("Name: %s\n", name);
    else
    {
        printf("WTF is this device?\n");
        return err;
    }
    cl_ulong size;
    clGetDeviceInfo(id, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &size, NULL);
    printf("    Local mem: %lu\n", size);
    clGetDeviceInfo(id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_ulong), &size, NULL);
    printf("    Cores: %lu\n", size);
    size_t sizes [20];
    clGetDeviceInfo(id, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t) * 20, &sizes, NULL);
    printf("    work items 1d: %llu\n", sizes[0]);
    printf("    work items 2d: %llu\n", sizes[1]);
    cl_device_fp_config conf;
    clGetDeviceInfo(id, CL_DEVICE_DOUBLE_FP_CONFIG, sizeof(cl_device_fp_config), &conf, NULL);
    cl_device_fp_config needed = CL_FP_FMA | CL_FP_ROUND_TO_NEAREST | CL_FP_INF_NAN | CL_FP_DENORM;
    if (conf & needed == needed)
        printf("    Double allowed\n");
}

cl_device_id choose_device(cl_platform_id platform)
{
    cl_device_id devices[8];
    cl_uint avail;
    cl_int err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 8, &devices, &avail);

    if (avail < 1) {
        printf("no devices lol\n");
        exit(1);
    } else if (avail == 1) {
        return devices[0];
    } else
    {
        printf("Choose a device:\n");
        for (int i = 0; i < avail; i++)
        {
            printf("%d ", i);
            device_info(devices[i]);
        }
        int i;
        scanf("%d", &i);
        return devices[i];
    }
}