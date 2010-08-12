#include <stdio.h>
#include <stdlib.h>

FILE *xfopen(const char *path, const char *mode)
{
    FILE *fp = fopen(path, mode);

    if (!fp) {
        fprintf(stderr, "unable to open file '%s'\n", path);
        exit(EXIT_FAILURE);
    }

    return fp;
}

void xfread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t count;
    
    count = fread(ptr, size, nmemb, stream);

    if (count != nmemb) {
        fputs("read error\n", stderr);
        exit(EXIT_FAILURE);
    }
}

void xfseek(FILE *stream, long offset, int whence)
{
    int ret = fseek(stream, offset, whence);

    if (ret != 0) {
        fputs("fseek error\n", stderr);
        exit(EXIT_FAILURE);
    }
}

void xfwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t count;
    
    count = fwrite(ptr, size, nmemb, stream);

    if (count != nmemb) {
        fputs("write error\n", stderr);
        exit(EXIT_FAILURE);
    }
}
