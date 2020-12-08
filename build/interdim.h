#define CL_TARGET_OPENCL_VERSION 220
#include "CL/cl.h"
#define MY_HOST
#include "Types.h"
#include <stdio.h>
#include <time.h>

struct program loads(const char* filename);
void GEN_SPEED(position * molecules, unsigned long N, const double max_speed);
FILE * create_of(dimension dims, unsigned long mol, position * shape, double raduis);

void MAKE_KERNEL(cl_device_id device, double * buffer, unsigned long mol, cl_context context, cl_device_id * devices, cl_command_queue queue, position * shape)
{
    FILE * of = create_of(dim, mol, shape, RADIUS);

    VECTOR *molecules = (VECTOR *) malloc(sizeof(VECTOR) * mol);
    for (unsigned long i = 0; i < mol; i++)
        for (dimension j = 0; j < dim; j++)
            molecules[i].s[j] = buffer[mol * j + i];

    fwrite(buffer, sizeof(position), dim * mol, of);
    unsigned long turns;
    printf("How many turns: ");
    scanf("%lu", &turns);

    position * speed_buffer = (position *) malloc(mol * dim * sizeof(position));
    GEN_SPEED(speed_buffer, mol, 0.1);
    VECTOR *speeds = (VECTOR *) malloc(sizeof(VECTOR) * mol);
    for (unsigned long i = 0; i < mol; i++)
        for (dimension j = 0; j < dim; j++)
            speeds[i].s[j] = speed_buffer[mol * j + i];

    printf("- generated molecules\n");

    cl_mem_flags flags = CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR;
    cl_mem pos_memory = clCreateBuffer(context, flags, sizeof(VECTOR) * mol, molecules, NULL);
    cl_mem spd_memory = clCreateBuffer(context, flags, sizeof(VECTOR) * mol, speeds, NULL);
    printf("- got buffer\n");

    struct program source = loads(SOURCE);
    printf("1");
    cl_int ret;
    cl_program program = clCreateProgramWithSource(context, source.lines, (const char **) source.source, NULL, &ret);
    for (unsigned int i = 0; i < source.lines; i++)
        free(source.source[i]);

    free(source.source);
    clBuildProgram(program, 1, devices, NULL, NULL, NULL);
    printf("2");
    printf("- compiled a program\n");
    cl_ulong bar = (cl_ulong) mol;
    VECTOR shp;
    for (dimension i = 0; i < dim; i++)
        shp.s[i] = shape[i];

    size_t size[5];
    ret = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t) * 5, &size, NULL);
    printf("CL_DEVICE_MAX_WORK_ITEM_SIZES %llu\n", size[0]);
    size_t work_units;
    printf("How many work units: ");
    scanf("%llu", &work_units);
    const size_t global_work_size[1] = { work_units };
    cl_kernel kernel = clCreateKernel(program, "test", &ret);
    cl_ulong wu = (cl_ulong)work_units;
    clSetKernelArg(kernel, 5, sizeof(cl_ulong), (void *)&wu);
    clSetKernelArg(kernel, 0, sizeof(cl_ulong), (void *)&bar);
    clSetKernelArg(kernel, 3, sizeof(VECTOR), (void *)&shp);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&spd_memory);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&pos_memory);
        
    cl_ulong *count = (cl_ulong *) malloc(sizeof(cl_ulong) * mol);
    for (unsigned long i = 0; i < mol; i++)
        count[i] = 0;
    cl_mem count_buf = clCreateBuffer(context, flags, sizeof(cl_ulong) * mol, count, NULL);
    clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)&count_buf);
    
    time_t start;
    time_t finish;
    time(&start);
    for (unsigned long turn = 0; turn < turns; turn++)
    {    
        clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_work_size, &global_work_size, 0, NULL, NULL);
        clEnqueueReadBuffer(queue, pos_memory, CL_TRUE, 0, mol * sizeof(VECTOR), molecules, 0, NULL, NULL);
        for (unsigned long i = 0; i < mol; i++)
            for (dimension j = 0; j < dim; j++)
                buffer[mol * j + i] = molecules[i].s[j];
        fwrite(buffer, sizeof(position), dim * mol, of);
        if (turn % 100000 == 0)
            printf("%lu\n", turn);
    }
    if (start != 1)
    {
        time(&finish);
        printf("%f sec\n", difftime(finish, start));
    }
    else
        printf("no time");
    clEnqueueReadBuffer(queue, count_buf, CL_TRUE, 0, mol * sizeof(cl_ulong), count, 0, NULL, NULL);
    unsigned long long total = 0;
    for (unsigned long i = 0; i < mol; i++)
        total += count[i];
    printf("%llu hits\n", total);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    clReleaseMemObject(pos_memory);
    clReleaseMemObject(spd_memory);
    clReleaseMemObject(count_buf);
    clReleaseDevice(device);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    free(molecules);
    free(buffer);
    free(speed_buffer);
    free(count);
    free(speeds);
    fclose(of);
    printf("END\n");
}
