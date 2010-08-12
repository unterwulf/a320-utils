#ifndef HXF_H
#define HXF_H

#include <stddef.h>
#include <stdio.h>

/***
 * HXF file header:
 * 0x00 57 41 46 44 ; HXF signature dword
 * 0x04 30 31 30 30 ; unknown const dword
 * 0x08 xx xx xx xx ; year 4 byte ASCII string
 * 0x0C xx xx       ; month 2 byte ASCII string
 * 0x0E xx xx       ; day 2 byte ASCII string
 * 0x10 xx xx       ; hour 2 byte ASCII string
 * 0x12 xx xx       ; minute 2 byte ASCII string
 * 0x14 xx xx xx xx ; HXF file size dword
 * 0x18 xx xx xx xx ; checksum is a sum of HXF file payload dwords
 * 0x1C 00 00 00 00 ; padding dword
 * 0x20             ; 32 byte string with description
 *
 * Files list:
 * 0x00 xx xx xx xx ; filename length dword
 * 0x04             ; filename string
 *      xx          ; attributes byte
 *      xx xx xx xx ; file size dword
 *                  ; file data
 */

#define HXF_SIGNATURE 0x46444157
#define HXF_HDR_SIZE  0x40

extern void copy_data(FILE *src, FILE *dst, size_t size);

#endif /* HXF_H */
