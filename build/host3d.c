#define dim 3
#define VECTOR cl_double3
#define MAKE_KERNEL make_3d_kernel
#define SOURCE "./Kernel3d.cl"
#define READ_BUFFER readBuffer_3d
#define GEN_SPEED gen_speed_3d

#include "interdim.h"