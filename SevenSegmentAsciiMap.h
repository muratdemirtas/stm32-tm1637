#ifndef SevenSegmentAsciiMap_H
#define SevenSegmentAsciiMap_H

#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"
// ASCII MAPPINGS
#define TM1637_CHAR_SPACE       0x00 //B00000000 // 32  (ASCII)
#define TM1637_CHAR_EXC         0x05 //B00000110
#define TM1637_CHAR_D_QUOTE     0x22 //B00100010
#define TM1637_CHAR_POUND       0x75 //B01110110
#define TM1637_CHAR_DOLLAR      0x6D //B01101101
#define TM1637_CHAR_PERC        0x24 //B00100100
#define TM1637_CHAR_AMP         0x7F //B01111111
#define TM1637_CHAR_S_QUOTE     0x20 //B00100000
#define TM1637_CHAR_L_BRACKET   0x39 //B00111001
#define TM1637_CHAR_R_BRACKET   0x0F //B00001111
#define TM1637_CHAR_STAR        0x5C //B01011100
#define TM1637_CHAR_PLUS        0x50 //B01010000
#define TM1637_CHAR_COMMA       0x10 //B00010000
#define TM1637_CHAR_MIN         0x40 //B01000000
#define TM1637_CHAR_DOT         0x08 //B00001000
#define TM1637_CHAR_F_SLASH     0x06 //B00000110
#define TM1637_CHAR_0           0x3F //B00111111   // 48
#define TM1637_CHAR_1           0x06 //B00000110
#define TM1637_CHAR_2           0x5B //B01011011
#define TM1637_CHAR_3           0x4F //B01001111
#define TM1637_CHAR_4           0x66 //B01100110
#define TM1637_CHAR_5           0x6D //B01101101
#define TM1637_CHAR_6           0x7D //B01111101
#define TM1637_CHAR_7           0x07 //B00000111
#define TM1637_CHAR_8           0x7F //B01111111
#define TM1637_CHAR_9           0x6F //B01101111
#define TM1637_CHAR_COLON       0x30 //B00110000
#define TM1637_CHAR_S_COLON     0x30 //B00110000
#define TM1637_CHAR_LESS        0x58 //B01011000
#define TM1637_CHAR_EQUAL       0x48 //B01001000
#define TM1637_CHAR_GREAT       0x4C //B01001100
#define TM1637_CHAR_QUEST       0x53 //B01010011
#define TM1637_CHAR_AT          0x5F //B01011111
#define TM1637_CHAR_A           0x77 //B01110111 // 65  (ASCII)
#define TM1637_CHAR_B           0x7F //B01111111
#define TM1637_CHAR_C           0x57 //B00111001
#define TM1637_CHAR_D           TM1637_CHAR_d
#define TM1637_CHAR_E           0x79 //B01111001
#define TM1637_CHAR_F           0x71 //B01110001
#define TM1637_CHAR_G           0x3D //B00111101
#define TM1637_CHAR_H           0x77 //B01110110
#define TM1637_CHAR_I           0x06 //B00000110
#define TM1637_CHAR_J           0x0E //B00001110
#define TM1637_CHAR_K           0x75 //B01110101
#define TM1637_CHAR_L           0x38 //B00111000
#define TM1637_CHAR_M           0x15 //B00010101
#define TM1637_CHAR_N           0x37 //B00110111
#define TM1637_CHAR_O           0x3F //B00111111
#define TM1637_CHAR_P           0x73 //B01110011
#define TM1637_CHAR_Q           0x67 //B01100111
#define TM1637_CHAR_R           0x33 //B00110011
#define TM1637_CHAR_S           0x6D //B01101101
#define TM1637_CHAR_T           TM1637_CHAR_t
#define TM1637_CHAR_U           0x3E //B00111110
#define TM1637_CHAR_V           0x1C //B00011100
#define TM1637_CHAR_W           0x2A //B00101010
#define TM1637_CHAR_X           TM1637_CHAR_H
#define TM1637_CHAR_Y           0x6E //B01101110
#define TM1637_CHAR_Z           0x5B //B01011011
#define TM1637_CHAR_L_S_BRACKET 0x39 //B00111001 // 91 (ASCII)
#define TM1637_CHAR_B_SLASH     0x30 //B00110000
#define TM1637_CHAR_R_S_BRACKET 0x0F //B00001111
#define TM1637_CHAR_A_CIRCUM    0x13 //B00010011
#define TM1637_CHAR_UNDERSCORE  0x08 //B00001000
#define TM1637_CHAR_A_GRAVE     0x10 //B00010000
#define TM1637_CHAR_a           0x5F //B01011111 // 97 (ASCII)
#define TM1637_CHAR_b           0x7C //B01111100
#define TM1637_CHAR_c           0x58 //B01011000
#define TM1637_CHAR_d           0x5E //B01011110
#define TM1637_CHAR_e           0xFB //B01111011
#define TM1637_CHAR_f           TM1637_CHAR_F
#define TM1637_CHAR_g           0x6F //B01101111
#define TM1637_CHAR_h           0x74 //B01110100
#define TM1637_CHAR_i           0x04 //B00000100
#define TM1637_CHAR_j           0x0C //B00001100
#define TM1637_CHAR_k           TM1637_CHAR_K
#define TM1637_CHAR_l           0x60 //B00110000
#define TM1637_CHAR_m           TM1637_CHAR_M
#define TM1637_CHAR_n           0x54 //B01010100
#define TM1637_CHAR_o           0x5C //B01011100
#define TM1637_CHAR_p           TM1637_CHAR_P
#define TM1637_CHAR_q           TM1637_CHAR_Q
#define TM1637_CHAR_r           0x50 //B01010000
#define TM1637_CHAR_s           TM1637_CHAR_S
#define TM1637_CHAR_t           0x78 //B01111000
#define TM1637_CHAR_u           0x1C //B00011100
#define TM1637_CHAR_v           0x1C //B00011100
#define TM1637_CHAR_w           TM1637_CHAR_W
#define TM1637_CHAR_x           TM1637_CHAR_X
#define TM1637_CHAR_y           0x66 //B01100110
#define TM1637_CHAR_z           TM1637_CHAR_Z
#define TM1637_CHAR_L_ACCON     0x79 //B01111001 // 123 (ASCII)
#define TM1637_CHAR_BAR         0x06 //B00000110
#define TM1637_CHAR_R_ACCON     0x4F //B01001111
#define TM1637_CHAR_TILDE       0x40 // 126 (ASCII)

extern const uint8_t Asciimap[96];
#endif
