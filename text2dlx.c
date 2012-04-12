#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dlx.h"
#include "xintio.h"
#include "xstdio.h"

static size_t getitem(char *buf, size_t size, FILE *fp)
{
    size_t bufsize = size;
    int ch;

    for (; size > 0; buf++, size--) {
        ch = fgetc(fp);
        if (ch == '\n') {
            fgetc(fp);
            break;
        } else if (feof(fp))
            break;
        else
            *buf = ch;
    }

    return bufsize - size;
}

static size_t unescapebuf(char *buf, size_t size)
{
    char *origbuf = buf;
    char *wrptr = buf;

    for (; size > 0; buf++, wrptr++, size--) {
        if (*buf == '\\') {
            if (size > 2 && *(buf + 2) == 'n') {
                *wrptr = '\n';
                buf += 2;
                size -= 2;
            } else if (size > 4 && *(buf + 2) == '\\' && *(buf + 4) == 'n') {
                *wrptr++ = '\\';
                *wrptr = 'n';
                buf += 4;
                size -= 4;
            }
        } else
            *wrptr = *buf;
    }

    return wrptr - origbuf;
}

static uint16_t getlinecount(FILE *fp)
{
    uint16_t count = 0;
    int ch;

    while ((ch = fgetc(fp)) != EOF)
        if (ch == '\n')
            count++;

    return count;
}

static void getdlxname(char *dlxname, const char *filename)
{
    size_t i;
    const char *basename = strrchr(filename, '/');
    
    if (basename)
        basename++; /* skip the slash */
    else
        basename = filename;

    for (i = 0; i < DLX_NAME_SIZE && basename[i] && basename[i] != '.'; i++)
        dlxname[i] = basename[i];
}

int main(int argc, char **argv)
{
    FILE *fpin;
    FILE *fpout;
    size_t len;
    uint32_t itemcount;
    off_t metapos;
    off_t datapos;
    uint32_t dataoff = 0;
    char buf[8096];
    char name[DLX_NAME_SIZE] = "\0";

    if (argc != 3) {
        printf("Dingoo A320 text-to-dlx converter by unterwulf\n"
               "usage: %s <text-file> <dlx-file>\n", argv[0]);
        return EXIT_SUCCESS;
    }

    fpin = xfopen(argv[1], "r");
    fpout = xfopen(argv[2], "w");

    itemcount = getlinecount(fpin);
    datapos = DLX_HDR_SIZE + itemcount*12;

    xfwritedw(DLX_SIGNATURE, fpout); /* signature: 'DLX-' */
    xfwritedw(1, fpout); /* ??? */
    xfwritedw(itemcount, fpout);
    xfwritedw(DLX_ITEM_SIG, fpout);
    xfwritedw(datapos, fpout);
    getdlxname(name, argv[2]);
    xfwrite(name, DLX_NAME_SIZE, 1, fpout);
    metapos = ftell(fpout);
    xfseek(fpin, 0, SEEK_SET);

    for (; itemcount > 0; itemcount--) {
        len = getitem(buf, sizeof(buf), fpin);
        len = unescapebuf(buf, len);

        xfseek(fpout, metapos, SEEK_SET);
        xfwritedw(DLX_ITEM_SIG, fpout);
        xfwritedw(len, fpout);
        xfwritedw(dataoff, fpout);
        metapos = ftell(fpout);

        xfseek(fpout, datapos, SEEK_SET);
        if (len > 0) {
            xfwrite(buf, len, 1, fpout);
            dataoff += len;
        }
        datapos = ftell(fpout);
    }

    fclose(fpout);
    fclose(fpin);
    return EXIT_SUCCESS;
}
