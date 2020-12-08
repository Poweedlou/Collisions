#include <stdio.h>
#define CL_TARGET_OPENCL_VERSION 220
#include "CL/cl.h"
#define MY_HOST
#include "Types.h"


position * gen_array(position * molecules, dimension dims, unsigned long N, const position *shape, const double radius);
struct program loads(const char* filename);

cl_int platform_info(cl_platform_id id);
cl_platform_id choose_platform();
cl_int device_info(cl_int id);
cl_device_id choose_device(cl_platform_id platform);

void make_2d_kernel(cl_device_id device, double * buffer, unsigned long mol, cl_context context, cl_device_id * devices, cl_command_queue queue, double * shape);
void make_3d_kernel(cl_device_id device, double * buffer, unsigned long mol, cl_context context, cl_device_id * devices, cl_command_queue queue, double * shape);

int main()
{
    cl_platform_id platform = choose_platform();
    char info[128];
    clGetPlatformInfo(platform, CL_PLATFORM_VERSION, sizeof(char) * 128, &info, NULL);
    printf("- got platform\n");
    printf(info);
    printf("\n");

    cl_device_id device = choose_device(platform);
    printf("- got device\n");

    
    
    printf("How many molecules: ");
    unsigned long mol;
    scanf("%lu", &mol);
    
    printf("How many dimensions? 2 or 3: ");
    dimension dim;
    scanf("%hu", &dim);
    if (dim != 3 && dim != 2)
    {
        printf("that's wrong number");
        return 2;
    }

    cl_device_id devices[1] = {device};
    cl_context context = clCreateContext(NULL, 1, devices, NULL, NULL, NULL);
    cl_int ret;
    cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, NULL, &ret);

    position * buffer = (position *) malloc(mol * dim * sizeof(position));
    const position shape[3] = {20.0, 20.0, 20.0};
    const double radius = RADIUS;
    gen_array(buffer, dim, mol, &shape, radius);
    if (dim == 2)
        make_2d_kernel(device, buffer, mol, context, devices, queue, shape);
    else
        make_3d_kernel(device, buffer, mol, context, devices, queue, shape);
    return 0;
}