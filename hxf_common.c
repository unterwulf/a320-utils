#include <stdio.h>
#include <stdlib.h>

#include "xstdio.h"

void copy_data(FILE *src, FILE *dst, size_t size)
{
    char buf[BUFSIZ];
    size_t chunksize = sizeof(buf);

    for (; size > 0; size -= chunksize)
    {
        if (size < chunksize)
            chunksize = size;

        xfread(buf, chunksize, 1, src);
        xfwrite(buf, chunksize, 1, dst);
    }
}
