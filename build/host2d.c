#define dim 2
#define VECTOR cl_double2
#define MAKE_KERNEL make_2d_kernel
#define SOURCE "./Kernel2d.cl"
#define READ_BUFFER readBuffer_2d
#define GEN_SPEED gen_speed_2d

#include "interdim.h"