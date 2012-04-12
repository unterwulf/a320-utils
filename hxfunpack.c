#include <errno.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "hxf.h"
#include "xintio.h"
#include "xstdio.h"

static FILE *create_file(char *filepath)
{
    char *sep = filepath;
    struct stat sb;

    for (; (sep = strchr(sep, '\\')); sep++) {
        *sep = '\0';
        if (stat(filepath, &sb) == -1)
            if (errno == ENOENT)
                if (mkdir(filepath, 0755) == -1) {
                    perror("mkdir");
                    exit(EXIT_FAILURE);
                }

        *sep = '/';
    }

    return xfopen(filepath, "w");
}

int main(int argc, char **argv)
{
    FILE *fp;
    FILE *dstfp;
    char filename[PATH_MAX];
    uint32_t sig;
    uint32_t size;
    char attr;

    if (argc != 2) {
        printf("Dingoo A320 hxf-file unpacker tool by unterwulf\n"
               "usage: %s <hxf-file>\n", argv[0]);
        return EXIT_SUCCESS;
    }

    fp = xfopen(argv[1], "rb");
    sig = xfreaddw(fp);

    if (sig != HXF_SIGNATURE) {
        fprintf(stderr, "%s: not an hxf-file\n", argv[1]);
        return EXIT_FAILURE;
    }

    xfseek(fp, HXF_HDR_SIZE, SEEK_SET);  /* skip the header */

    for (;;) {
        size = xfreaddw(fp);              /* file name size */

        if (size == 0)
            break;
        else if (size >= sizeof(filename))
        {
            fputs("too long filename\n", stderr);
            return EXIT_FAILURE;
        }

        xfread(filename, size, 1, fp);
        filename[size] = '\0';
        xfread(&attr, 1, 1, fp);
        size = xfreaddw(fp);              /* file size */
        puts(filename);
        dstfp = create_file(filename);
        copy_data(fp, dstfp, size);
        fclose(dstfp);
    }

    fclose(fp);
    return EXIT_SUCCESS;
}
