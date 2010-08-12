#ifndef DLX_H
#define DLX_H

/***
 * DLX file header:
 * 0x00 44 4C 58 2D ; DLX signature dword
 * 0x04 01 00 00 00 ; unknown const dword
 * 0x08 xx xx xx xx ; item count dword
 * 0x0C 08 11 81 19 ; DLX item signature dword
 * 0x10 xx xx xx xx ; data section offset
 * 0x14             ; 12 byte string with name
 *
 * Items list:
 * 0x00 08 11 81 19 ; DLX item signature dword
 * 0x04 xx xx xx xx ; item data length
 * 0x08 xx xx xx xx ; item data offset in data section
 *
 * Data section.
 */

#define DLX_SIGNATURE 0x2D584C44
#define DLX_ITEM_SIG  0x19811108
#define DLX_HDR_SIZE  0x20
#define DLX_NAME_SIZE 0x0C

#endif /* DLX_H */
