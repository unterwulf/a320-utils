#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "dlx.h"
#include "xintio.h"
#include "xstdio.h"

static void escwrite(char *buf, size_t len, FILE *fp)
{
    for (; len > 0; buf++, len--) {
        if (*buf == '\n')
            xfwrite("\\\0n", 3, 1, fp);
        else if (*buf == '\\' && len > 1 && *(buf+1) == 'n')
            xfwrite("\\\\", 2, 1, fp);
        else
            fputc(*buf, fp);
    }
}

int main(int argc, char **argv)
{
    FILE *fp;
    uint32_t sig;
    uint32_t len;
    uint32_t itemcount;
    off_t metapos;
    off_t datapos;
    char buf[8096];

    if (argc != 2) {
        printf("Dingoo A320 dlx-to-text converter by unterwulf\n"
               "usage: %s <dlx-file>\n", argv[0]);
        return EXIT_SUCCESS;
    }

    fp = xfopen(argv[1], "r");
    sig = xfreaddw(fp); /* signature */

    if (sig != DLX_SIGNATURE) {
        fprintf(stderr, "%s: not a dlx-file\n", argv[1]);
        return EXIT_FAILURE;
    }

    xfreaddw(fp); /* skip unknown dword */
    itemcount = xfreaddw(fp);
    xfreaddw(fp); /* skip item signature */
    datapos = xfreaddw(fp);
    /* we are not interested in the dlx name, so just skip it */
    xfseek(fp, DLX_HDR_SIZE, SEEK_SET);
    metapos = ftell(fp);

    for (; itemcount > 0; itemcount--) {
        xfseek(fp, metapos + 4, SEEK_SET);
        len = xfreaddw(fp);
        xfreaddw(fp); /* skip offset */
        metapos = ftell(fp);
        if (len) {
            xfseek(fp, datapos, SEEK_SET);
            xfread(buf, len, 1, fp);
            escwrite(buf, len, stdout);
            datapos = ftell(fp);
        }
        xfwrite("\n\0", 2, 1, stdout); /* ucs-2 new line char */
    }

    fclose(fp);
    return EXIT_SUCCESS;
}
