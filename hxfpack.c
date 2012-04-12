#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "hxf.h"
#include "xintio.h"
#include "xstdio.h"

static void write_header(FILE *fp, uint32_t size, uint32_t chksum,
                         const char *desc)
{
    char buf[17];
    time_t cur_time = time(NULL);
    struct tm *cur_tm = localtime(&cur_time);
    size_t desc_len = strlen(desc);

    xfwritedw(HXF_SIGNATURE, fp);
    strftime(buf, sizeof(buf), "0100%Y%m%d%H%M", cur_tm);
    xfwrite(buf, 16, 1, fp);
    xfwritedw(size, fp);
    xfwritedw(chksum, fp);
    xfwritedw(0, fp);
    xfwrite(desc, desc_len > 32 ? 32 : desc_len, 1, fp);
}

static uint32_t calc_chksum(FILE *fp)
{
    uint32_t chksum = 0;
    uint32_t dw;
    size_t nread;

    while (!feof(fp)) {
        dw = 0;
        nread = fread(&dw, 1, 4, fp);
        if (nread == 0 && ferror(fp)) {
            fputs("read error\n", stderr);
            exit(EXIT_FAILURE);
        }
        chksum += dw;
    }

    return chksum;
}

static void pack_file(FILE *fp, char *filename, const struct stat *sb)
{
    size_t fnlen = strlen(filename);
    FILE *srcfp = xfopen(filename, "r");
    char *sep;

    for (sep = filename; (sep = strchr(sep, '/'));)
        *sep = '\\';

    xfwritedw(fnlen, fp);
    xfwrite(filename, fnlen, 1, fp);   
    xfwrite(" ", 1, 1, fp);
    xfwritedw(sb->st_size, fp);
    copy_data(srcfp, fp, sb->st_size);
    fclose(srcfp);
}

static void process_filelist(FILE *fp)
{
    char filename[PATH_MAX];
    char *sep;
    struct stat sb;

    while (fgets(filename, sizeof(filename), stdin)) {
        filename[strlen(filename) - 1] = '\0'; /* strip new line */

        for (sep = filename; (sep = strchr(sep, '\\'));)
            *sep = '/';

        if (stat(filename, &sb) == 0) {
            if (sb.st_mode & S_IFREG)
                pack_file(fp, filename, &sb);
            else
                fprintf(stderr, "%s: unsupported file type\n", filename);
        } else {
            fprintf(stderr, "%s: ", filename);
            perror("");
        }
    }
}

int main(int argc, char **argv)
{
    FILE *fp;
    long size;
    uint32_t chksum;
    const char *desc = "Chinachip PMP firmware V1.0";

    if (argc != 2 && argc != 3) {
        printf("Dingoo A320 hxf-file packer tool by unterwulf\n"
               "usage: %s <hxf-file>\n", argv[0]);
        return EXIT_SUCCESS;
    }

    fp = xfopen(argv[1], "w+");
    xfseek(fp, HXF_HDR_SIZE, SEEK_SET); /* skip header */
    process_filelist(fp);
    xfwritedw(0, fp); /* zero dword at the end of file */
    size = ftell(fp);
    xfseek(fp, HXF_HDR_SIZE, SEEK_SET); /* skip header */
    chksum = calc_chksum(fp);
    xfseek(fp, 0, SEEK_SET);
    write_header(fp, size, chksum, desc);
    fclose(fp);

    return EXIT_SUCCESS;
}
