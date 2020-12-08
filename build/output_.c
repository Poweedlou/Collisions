#include <stdio.h>
#include "Types.h"

FILE * create_of(dimension dims, unsigned long mol, position * shape, double raduis)
{
    FILE * of = fopen("moleculas.bin", "wb");
    fwrite(&dims, sizeof(dimension), 1, of);
    fwrite(&mol, sizeof(mol), 1, of);
    fwrite(shape, sizeof(position), dims, of);
    fwrite(&raduis, sizeof(double), 1, of);
    return of;
}
