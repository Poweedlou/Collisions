#define position double
#define dimension unsigned short
#define RADIUS 1.0

struct program {
    char ** source;
    int lines;
};
#ifdef MY_HOST
struct kernel_buffer {
    cl_mem * buffer;
    cl_kernel * kernel;
};
#endif