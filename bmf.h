#ifndef BMF_H
#define BMF_H

/***
 * BMF file header:
 * 0x00 42 46 01 01 ; BMF signature dword
 * 0x04 xx xx       ; param1 (unknown word)
 * 0x06 xx xx       ; symbol count word
 * 0x08 xx xx       ; width word (00 means variable width)
 * 0x0A xx xx       ; height word
 * 0x0C xx xx       ; bytes per row word
 * 0x0E xx xx       ; param2 (unknown word)
 * 0x10             ; 32 byte string with description
 * 0x30 40 00 00 00 ; unknown const dword
 * 0x34 xx xx xx xx ; data size dword
 * 0x38 00 00 00 00 ; padding dword
 * 0x3C 00 00 00 00 ; padding dword
 *
 * Bitmaps list:
 * If width == 0 then there is a word with the width of this particular
 * character before each bitmap. Bitmap size is bytes per row * height bytes.
 */

#define BMF_SIGNATURE 0x01014642
#define BMF_DESC_SIZE 0x20
#define BMF_HDR_SIZE  0x40

#endif /* BMF_H */
