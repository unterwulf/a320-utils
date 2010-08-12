#ifndef XINTIO_H
#define XINTIO_H

#include <inttypes.h>
#include <stdio.h>

extern uint16_t xfreadw(FILE *fp);
extern uint32_t xfreaddw(FILE *fp);
extern void xfwritew(uint16_t word, FILE *fp);
extern void xfwritedw(uint32_t dw, FILE *fp);

#endif /* XINTIO_H */
