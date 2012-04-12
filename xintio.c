#include <stdint.h>
#include <stdio.h>

#include "xstdio.h"

uint16_t xfreadw(FILE *fp)
{
    uint8_t buf[2];

    xfread(buf, sizeof(buf), 1, fp);

    return (uint16_t)buf[0] +
           ((uint16_t)buf[1] << 8);
}

uint32_t xfreaddw(FILE *fp)
{
    uint8_t buf[4];

    xfread(buf, sizeof(buf), 1, fp);

    return (uint32_t)buf[0] +
           ((uint32_t)buf[1] << 8) +
           ((uint32_t)buf[2] << 16) +
           ((uint32_t)buf[3] << 24);
}

void xfwritew(uint16_t word, FILE *fp)
{
    uint8_t buf[2];

    buf[0] = word & 0xFF;
    buf[1] = (word >> 8) & 0xFF;

    xfwrite(buf, sizeof(buf), 1, fp);
}

void xfwritedw(uint32_t dw, FILE *fp)
{
    uint8_t buf[4];

    buf[0] = dw & 0xFF;
    buf[1] = (dw >> 8) & 0xFF;
    buf[2] = (dw >> 16) & 0xFF;
    buf[3] = (dw >> 24) & 0xFF;

    xfwrite(buf, sizeof(buf), 1, fp);
}
