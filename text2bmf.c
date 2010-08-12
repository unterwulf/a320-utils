#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmf.h"
#include "xintio.h"
#include "xstdio.h"

static uint32_t readparam(FILE *fp)
{
    uint32_t param;
    char buf[256];

    do {
        if (fgets(buf, sizeof(buf), fp)) {
            param = atoi(buf);
        } else {
            fputs("read error\n", stderr);
            exit(EXIT_FAILURE);
        }
    } while (strlen(buf) == 1); /* skip empty lines */

    return param;
}

static void xgetnonemptys(char *buf, size_t size, FILE *fp)
{
    while (fgets(buf, size, fp))
        if (buf[0] != '\n' && buf[1] != '\0')
            return;

    fputs("read error\n", stderr);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    FILE *fpin;
    FILE *fpout;
    int count;
    int byte;
    int bit;
    char desc[33] = "";
    size_t i;
    uint8_t val;
    uint16_t width;
    uint16_t chwidth;
    uint16_t height;
    uint16_t bpr;
    uint16_t symcount;
    uint16_t param1;
    uint16_t param2;
    char bitmap[256];

    if (argc != 3) {
        printf("Dingoo A320 text-to-bmf converter by unterwulf\n"
               "usage: %s <text-file> <bmf-file>\n", argv[0]);
        return EXIT_SUCCESS;
    }

    fpin = xfopen(argv[1], "r");
    fpout = xfopen(argv[2], "w");

    if (!fgets(desc, sizeof(desc), fpin)) {
        fputs("read error or description too long\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < sizeof(desc); i++)
        if (desc[i] == '#')
            for (; i < sizeof(desc); i++)
                desc[i] = '\0';

    width = readparam(fpin);
    height = readparam(fpin);
    bpr = readparam(fpin);
    symcount = readparam(fpin);
    param1 = readparam(fpin);
    param2 = readparam(fpin);

    xfwritedw(BMF_SIGNATURE, fpout); /* signature */
    xfwritew(param1, fpout); /* ??? */
    xfwritew(symcount, fpout);
    xfwritew(width, fpout);
    xfwritew(height, fpout);
    xfwritew(bpr, fpout);
    xfwritew(param2, fpout); /* ??? */
    xfwrite(desc, 0x20, 1, fpout);
    xfwritedw(0x40, fpout); /* unknown const dword */
    xfwritedw(symcount*(height*bpr + 2*(width == 0)), fpout);
    xfwritedw(0x00, fpout); /* padding */
    xfwritedw(0x00, fpout); /* padding */

    for (; symcount > 0; symcount--) {
        if (width == 0) {
            chwidth = readparam(fpin);
            xfwritew(chwidth, fpout);
        }

        for (count = 0; count < height; count++) {
            xgetnonemptys(bitmap, sizeof(bitmap), fpin);
            if (strlen(bitmap) < bpr * 8 + 1) {
                off_t offset = ftell(fpin);
                fprintf(stderr, "malformed bitmap at offset %lu\n", offset);
                exit(EXIT_FAILURE);
            }
            for (byte = 0; byte < bpr; byte++) {
                for (bit = 0, val = 0; bit < 8; bit++)
                    if (bitmap[byte*8 + bit] == '1')
                        val |= 1 << (7 - bit);
                xfwrite(&val, 1, 1, fpout);
            }
        }
    }

    fclose(fpin);
    fclose(fpout);
    return EXIT_SUCCESS;
}
