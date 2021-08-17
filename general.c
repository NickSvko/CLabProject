
#include "general.h"

void *mallocWithCheck(unsigned long size)
{
    void *temp = malloc(size);

    if (temp == NULL)
    {
        fprintf(stderr,"Error: Memory allocation failed.");
        exit(1);
    }
    return temp;
}
