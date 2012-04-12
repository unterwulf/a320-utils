#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmf.h"
#include "xintio.h"
#include "xstdio.h"

static void print_bitmap(char ch)
{
    size_t i;

    for (i = 0; i < 8; i++, ch <<= 1)
        putchar((ch & 0x80) ? '1' : '0');
}

int main(int argc, char **argv)
{
    FILE *fp;
    int row;
    int byte;
    char buf[4];
    char desc[BMF_DESC_SIZE + 1] = "";
    uint16_t width;
    uint16_t chwidth;
    uint16_t height;
    uint16_t bpr;
    uint16_t symcount;
    uint16_t param1;
    uint16_t param2;
    uint32_t sig;

    if (argc != 2) {
        printf("Dingoo A320 bmf-to-text converter by unterwulf\n"
               "usage: %s <bmf-file>\n", argv[0]);
        return EXIT_SUCCESS;
    }

    fp = xfopen(argv[1], "r");
    sig = xfreaddw(fp); /* signature */

    if (sig != BMF_SIGNATURE) {
        fprintf(stderr, "%s: not a bmf-file\n", argv[1]);
        return EXIT_FAILURE;
    }

    param1 = xfreadw(fp); /* ??? */
    symcount = xfreadw(fp);
    width = xfreadw(fp);
    height = xfreadw(fp);
    bpr = xfreadw(fp);
    param2 = xfreadw(fp); /* ??? */
    xfread(desc, BMF_DESC_SIZE, 1, fp);
    xfseek(fp, BMF_HDR_SIZE, SEEK_SET);

    printf("%s# desc\n"            /* no space before comment */
           "%u # width\n"
           "%u # height\n"
           "%u # bytes per row\n"
           "%u # symbol count\n"
           "%u # param1\n"
           "%u # param2\n\n",
           desc, width, height, bpr, symcount, param1, param2);

    for (; symcount > 0; symcount--) {
        if (width == 0) {
            chwidth = xfreadw(fp);
            printf("%u # width\n", chwidth);
        }

        for (row = 0; row < height; row++) {
            xfread(buf, bpr, 1, fp);
            for (byte = 0; byte < bpr; byte++)
                print_bitmap(buf[byte]);
            putchar('\n');
        }
        putchar('\n');
    }

    fclose(fp);
    return EXIT_SUCCESS;
}
