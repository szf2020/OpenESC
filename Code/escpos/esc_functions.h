// MJM 2024

#pragma once

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdbool>

#include "RxBuffer.h"

//-------------------------------------------------------
//https://stackoverflow.com/a/3208376
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 
//-------------------------------------------------------

#define CMD_SIZE 87
#define BMAX 3

//-------------------------------------------------------
typedef int (*operation_fp)(RxBuffer* b);
typedef int (*operation_sym_fp)(RxBuffer* b, int k);
typedef int (*operation_opta)(RxBuffer* b, int argc, uint8_t* argv);
//-------------------------------------------------------
struct CMD {
    uint8_t depth;
    uint8_t c[BMAX];
    operation_fp ptr;
};
//-------------------------------------------------------
struct graphics {
    operation_opta ptr;
};
//-------------------------------------------------------
struct symbols {
    operation_sym_fp ptr;
};
//-------------------------------------------------------
int _HT(RxBuffer* b);                       // horizontal tab
//-------------------------------------------------------
int _LF(RxBuffer* b);                       // line feed
//-------------------------------------------------------
int _FF(RxBuffer* b);                       // form feed
//-------------------------------------------------------
int _CR(RxBuffer* b);                       // carriage return
//-------------------------------------------------------
int _DLE_ENQ(RxBuffer* b);                  // inquiry
int _DLE_DC4(RxBuffer* b);                  // DC4
int _DLE_EOT(RxBuffer* b);                  // end of transmit
//-------------------------------------------------------
int _CAN(RxBuffer* b);                      // cancel 
//-------------------------------------------------------
int _ESC_FF(RxBuffer* b);                   // form feed
int _ESC_SP(RxBuffer* b);                   // space
int _ESC_EXCLAMATION_SYM(RxBuffer* b);      // !
int _ESC_DOLLAR_SYM(RxBuffer* b);           // $
int _ESC_PERCENT_SYM(RxBuffer* b);          // %
int _ESC_AND_SYM(RxBuffer* b);              // &
int _ESC_STAR_SYM(RxBuffer* b);             // *
int _ESC_SUBTRACT_SYM(RxBuffer* b);         // -
int _ESC_ASCII_TWO(RxBuffer* b);            // 2
int _ESC_ASCII_THREE(RxBuffer* b);          // 3
int _ESC_EQUAL_SYM(RxBuffer* b);            // =
int _ESC_QUESTION_SYM(RxBuffer* b);         // ?
int _ESC_AT_SYM(RxBuffer* b);               // @
int _ESC_UPR_D(RxBuffer* b);                // D
int _ESC_UPR_E(RxBuffer* b);                // E
int _ESC_UPR_G(RxBuffer* b);                // G
int _ESC_UPR_J(RxBuffer* b);                // J
int _ESC_UPR_L(RxBuffer* b);                // L
int _ESC_UPR_M(RxBuffer* b);                // M
int _ESC_UPR_R(RxBuffer* b);                // R
int _ESC_UPR_S(RxBuffer* b);                // S
int _ESC_UPR_T(RxBuffer* b);                // T
int _ESC_UPR_V(RxBuffer* b);                // V
int _ESC_UPR_W(RxBuffer* b);                // W
int _ESC_BACKSLASH_SYM(RxBuffer* b);        // '\'
int _ESC_LWR_a(RxBuffer* b);                // a
int _ESC_LWR_c_THREE(RxBuffer* b);          // c 3
int _ESC_LWR_c_FOUR(RxBuffer* b);           // c 4
int _ESC_LWR_c_FIVE(RxBuffer* b);           // c 5
int _ESC_LWR_d(RxBuffer* b);                // d
int _ESC_LWR_p(RxBuffer* b);                // p
int _ESC_LWR_t(RxBuffer* b);                // t
int _ESC_LEFT_QBRACKET_SYM(RxBuffer* b);    // {
//-------------------------------------------------------
int _FS_EXCLAMATION_SYM(RxBuffer* b);       // !
int _FS_AND_SYM(RxBuffer* b);               // &
int _FS_LEFT_PERNTH_UPR_A(RxBuffer* b);     // ( A
int _FS_SUBTRACT_SYM(RxBuffer* b);          // -
int _FS_PERIOD_SYM(RxBuffer* b);            // .
int _FS_ASCII_TWO(RxBuffer* b);             // 2
int _FS_UPR_C(RxBuffer* b);                 // C
int _FS_UPR_S(RxBuffer* b);                 // S
int _FS_UPR_W(RxBuffer* b);                 // W
int _FS_LWR_p(RxBuffer* b);                 // p
//-------------------------------------------------------
int _GS_EXCLAMATION_SYM(RxBuffer* b);       // !
int _GS_DOLLAR_SYM(RxBuffer* b);            // $
int _GS_LEFT_PERNTH_UPR_A(RxBuffer* b);     // ( A
int _GS_LEFT_PERNTH_UPR_C(RxBuffer* b);     // ( C
int _GS_LEFT_PERNTH_UPR_D(RxBuffer* b);     // ( D
int _GS_LEFT_PERNTH_UPR_E(RxBuffer* b);     // ( E
int _GS_LEFT_PERNTH_UPR_H(RxBuffer* b);     // ( H
int _GS_LEFT_PERNTH_UPR_K(RxBuffer* b);     // ( K
int _GS_LEFT_PERNTH_UPR_L(RxBuffer* b);     // ( L
int _GS_LEFT_PERNTH_UPR_M(RxBuffer* b);     // ( M
int _GS_LEFT_PERNTH_UPR_N(RxBuffer* b);     // ( N
int _GS_LEFT_PERNTH_LWR_k(RxBuffer* b);     // ( k
int _GS_STAR_SYM(RxBuffer* b);              // *
int _ESC_FWRDSLASH_SYM(RxBuffer* b);        // '/'
int _GS_EIGHT_UPR_L(RxBuffer* b);           // 8 L
int _GS_COLON_SYM(RxBuffer* b);             // :
int _GS_UPR_B(RxBuffer* b);                 // B
int _GS_UPR_H(RxBuffer* b);                 // H
int _GS_UPR_I(RxBuffer* b);                 // I
int _GS_UPR_L(RxBuffer* b);                 // L
int _GS_UPR_P(RxBuffer* b);                 // P
int _GS_UPR_T(RxBuffer* b);                 // T
int _GS_UPR_V(RxBuffer* b);                 // V
int _GS_UPR_W(RxBuffer* b);                 // W
int _GS_BACKSLASH_SYM(RxBuffer* b);         // '\'
int _GS_CARROT_SYM(RxBuffer* b);            // '^'
int _GS_LWR_a(RxBuffer* b);                 // a
int _GS_LWR_b(RxBuffer* b);                 // b
int _GS_LWR_f(RxBuffer* b);                 // f
int _GS_LWR_g_ZERO(RxBuffer* b);            // g 0
int _GS_LWR_g_TWO(RxBuffer* b);             // g 0
int _GS_LWR_h(RxBuffer* b);                 // h
int _GS_LWR_k(RxBuffer* b);                 // k
int _GS_LWR_r(RxBuffer* b);                 // r
int _GS_LWR_v_ZERO(RxBuffer* b);            // v 0
int _GS_LWR_w(RxBuffer* b);                 // w
//-------------------------------------------------------
int SYM_function_065(RxBuffer* b, int k); //48
int SYM_function_066(RxBuffer* b, int k);
int SYM_function_067(RxBuffer* b, int k);
int SYM_function_068(RxBuffer* b, int k);
int SYM_function_069(RxBuffer* b, int k);
int SYM_function_070(RxBuffer* b, int k);
int SYM_function_080(RxBuffer* b, int k);
int SYM_function_081(RxBuffer* b, int k);
int SYM_function_082(RxBuffer* b, int k);
//-------------------------------------------------------
int SYM_function_165(RxBuffer* b, int k); //49
int SYM_function_167(RxBuffer* b, int k);
int SYM_function_169(RxBuffer* b, int k);
int SYM_function_180(RxBuffer* b, int k);
int SYM_function_181(RxBuffer* b, int k);
int SYM_function_182(RxBuffer* b, int k);
//-------------------------------------------------------
int SYM_function_265(RxBuffer* b, int k); //50
int SYM_function_280(RxBuffer* b, int k);
int SYM_function_281(RxBuffer* b, int k);
int SYM_function_282(RxBuffer* b, int k);
//-------------------------------------------------------
int SYM_function_367(RxBuffer* b, int k); //51
int SYM_function_371(RxBuffer* b, int k);
int SYM_function_380(RxBuffer* b, int k);
int SYM_function_381(RxBuffer* b, int k);
int SYM_function_382(RxBuffer* b, int k);
//-------------------------------------------------------
int SYM_function_467(RxBuffer* b, int k); //52
int SYM_function_471(RxBuffer* b, int k);
int SYM_function_472(RxBuffer* b, int k);
int SYM_function_480(RxBuffer* b, int k);
int SYM_function_481(RxBuffer* b, int k);
int SYM_function_482(RxBuffer* b, int k);
//-------------------------------------------------------
int SYM_NULL(RxBuffer* b, int k);
//-------------------------------------------------------
int GS_function_48(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_49(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_50(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_51(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_52(RxBuffer* b, int argc, uint8_t* argv);
//-------------------------------------------------------
int GS_function_64(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_65(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_66(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_67(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_68(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_69(RxBuffer* b, int argc, uint8_t* argv);
//-------------------------------------------------------
int GS_function_80(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_81(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_82(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_83(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_84(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_85(RxBuffer* b, int argc, uint8_t* argv);
//-------------------------------------------------------
int GS_function_112(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_113(RxBuffer* b, int argc, uint8_t* argv);
//----------------------------------------------------------------
static CMD escpos[CMD_SIZE] =
{
    { 1, { 0x09, 0x00, 0x00 }, _HT },                       // 0
    { 1, { 0x0A, 0x00, 0x00 }, _LF },                       // 1
    { 1, { 0x0C, 0x00, 0x00 }, _FF },                       // 2
    { 1, { 0x0D, 0x00, 0x00 }, _CR },                       // 3
    { 2, { 0x10, 0x04, 0x00 }, _DLE_EOT },                  // 4
    { 2, { 0x10, 0x05, 0x00 }, _DLE_ENQ },                  // 5
    { 2, { 0x10, 0x14, 0x00 }, _DLE_DC4 },                  // 6
    { 1, { 0x18, 0x00, 0x00 }, _CAN },                      // 7
    { 2, { 0x1B, 0x0C, 0x00 }, _ESC_FF },                   // 8
    { 2, { 0x1B, 0x20, 0x00 }, _ESC_SP },                   // 9
    { 2, { 0x1B, 0x21, 0x00 }, _ESC_EXCLAMATION_SYM },      // 10
    { 2, { 0x1B, 0x24, 0x00 }, _ESC_DOLLAR_SYM },           // 11
    { 2, { 0x1B, 0x25, 0x00 }, _ESC_PERCENT_SYM },          // 12
    { 2, { 0x1B, 0x26, 0x00 }, _ESC_AND_SYM },              // 13
    { 2, { 0x1B, 0x2A, 0x00 }, _ESC_STAR_SYM },             // 14
    { 2, { 0x1B, 0x2D, 0x00 }, _ESC_SUBTRACT_SYM },         // 15
    { 2, { 0x1B, 0x32, 0x00 }, _ESC_ASCII_TWO },            // 16
    { 2, { 0x1B, 0x33, 0x00 }, _ESC_ASCII_THREE },          // 17
    { 2, { 0x1B, 0x3D, 0x00 }, _ESC_EQUAL_SYM },            // 18
    { 2, { 0x1B, 0x3F, 0x00 }, _ESC_QUESTION_SYM },         // 19
    { 2, { 0x1B, 0x40, 0x00 }, _ESC_AT_SYM },               // 20
    { 2, { 0x1B, 0x44, 0x00 }, _ESC_UPR_D },                // 21
    { 2, { 0x1B, 0x45, 0x00 }, _ESC_UPR_E },                // 22
    { 2, { 0x1B, 0x47, 0x00 }, _ESC_UPR_G },                // 23
    { 2, { 0x1B, 0x4A, 0x00 }, _ESC_UPR_J },                // 24
    { 2, { 0x1B, 0x4C, 0x00 }, _ESC_UPR_L },                // 25
    { 2, { 0x1B, 0x4D, 0x00 }, _ESC_UPR_M },                // 26
    { 2, { 0x1B, 0x52, 0x00 }, _ESC_UPR_R },                // 27
    { 2, { 0x1B, 0x53, 0x00 }, _ESC_UPR_S },                // 28
    { 2, { 0x1B, 0x54, 0x00 }, _ESC_UPR_T },                // 29
    { 2, { 0x1B, 0x56, 0x00 }, _ESC_UPR_V },                // 30
    { 2, { 0x1B, 0x57, 0x00 }, _ESC_UPR_W },                // 31
    { 2, { 0x1B, 0x5C, 0x00 }, _ESC_BACKSLASH_SYM },        // 32
    { 2, { 0x1B, 0x61, 0x00 }, _ESC_LWR_a },                // 33
    { 3, { 0x1B, 0x63, 0x33 }, _ESC_LWR_c_THREE },          // 34
    { 3, { 0x1B, 0x63, 0x34 }, _ESC_LWR_c_FOUR },           // 35
    { 3, { 0x1B, 0x63, 0x35 }, _ESC_LWR_c_FIVE },           // 36
    { 2, { 0x1B, 0x64, 0x00 }, _ESC_LWR_d },                // 37
    { 2, { 0x1B, 0x70, 0x00 }, _ESC_LWR_p },                // 38
    { 2, { 0x1B, 0x74, 0x00 }, _ESC_LWR_t },                // 39
    { 2, { 0x1B, 0x7B, 0x00 }, _ESC_LEFT_QBRACKET_SYM },    // 40
    { 2, { 0x1C, 0x21, 0x00 }, _FS_EXCLAMATION_SYM },       // 41
    { 2, { 0x1C, 0x26, 0x00 }, _FS_AND_SYM },               // 42
    { 3, { 0x1C, 0x28, 0x41 }, _FS_LEFT_PERNTH_UPR_A },     // 43
    { 2, { 0x1C, 0x2D, 0x00 }, _FS_SUBTRACT_SYM },          // 44
    { 2, { 0x1C, 0x2E, 0x00 }, _FS_PERIOD_SYM },            // 45
    { 2, { 0x1C, 0x32, 0x00 }, _FS_ASCII_TWO },             // 46
    { 2, { 0x1C, 0x43, 0x00 }, _FS_UPR_C },                 // 47
    { 2, { 0x1C, 0x53, 0x00 }, _FS_UPR_S },                 // 48
    { 2, { 0x1C, 0x57, 0x00 }, _FS_UPR_W },                 // 49
    { 2, { 0x1C, 0x70, 0x00 }, _FS_LWR_p },                 // 50    
    { 2, { 0x1D, 0x21, 0x00 }, _GS_EXCLAMATION_SYM },       // 51
    { 2, { 0x1D, 0x24, 0x00 }, _GS_DOLLAR_SYM },            // 52
    { 3, { 0x1D, 0x28, 0x41 }, _GS_LEFT_PERNTH_UPR_A },     // 53
    { 3, { 0x1D, 0x28, 0x43 }, _GS_LEFT_PERNTH_UPR_C },     // 54
    { 3, { 0x1D, 0x28, 0x44 }, _GS_LEFT_PERNTH_UPR_D },     // 55
    { 3, { 0x1D, 0x28, 0x45 }, _GS_LEFT_PERNTH_UPR_E },     // 56
    { 3, { 0x1D, 0x28, 0x48 }, _GS_LEFT_PERNTH_UPR_H },     // 57
    { 3, { 0x1D, 0x28, 0x4B }, _GS_LEFT_PERNTH_UPR_K },     // 58
    { 3, { 0x1D, 0x28, 0x4C }, _GS_LEFT_PERNTH_UPR_L },     // 59
    { 3, { 0x1D, 0x28, 0x4D }, _GS_LEFT_PERNTH_UPR_M },     // 60    
    { 3, { 0x1D, 0x28, 0x4E }, _GS_LEFT_PERNTH_UPR_N },     // 61
    { 3, { 0x1D, 0x28, 0x6B }, _GS_LEFT_PERNTH_LWR_k },     // 62
    { 2, { 0x1D, 0x2A, 0x00 }, _GS_STAR_SYM },              // 63
    { 2, { 0x1D, 0x2F, 0x00 }, _ESC_FWRDSLASH_SYM },        // 64
    { 3, { 0x1D, 0x38, 0x4C }, _GS_EIGHT_UPR_L },           // 65
    { 2, { 0x1D, 0x3A, 0x00 }, _GS_COLON_SYM },             // 66
    { 2, { 0x1D, 0x42, 0x00 }, _GS_UPR_B },                 // 67
    { 2, { 0x1D, 0x48, 0x00 }, _GS_UPR_H },                 // 68
    { 2, { 0x1D, 0x49, 0x00 }, _GS_UPR_I },                 // 69
    { 2, { 0x1D, 0x4C, 0x00 }, _GS_UPR_L },                 // 70
    { 2, { 0x1D, 0x50, 0x00 }, _GS_UPR_P },                 // 71
    { 2, { 0x1D, 0x54, 0x00 }, _GS_UPR_T },                 // 72
    { 2, { 0x1D, 0x56, 0x00 }, _GS_UPR_V },                 // 73
    { 2, { 0x1D, 0x57, 0x00 }, _GS_UPR_W },                 // 74
    { 2, { 0x1D, 0x5C, 0x00 }, _GS_BACKSLASH_SYM },         // 75
    { 2, { 0x1D, 0x5E, 0x00 }, _GS_CARROT_SYM },            // 76
    { 2, { 0x1D, 0x61, 0x00 }, _GS_LWR_a },                 // 77
    { 2, { 0x1D, 0x62, 0x00 }, _GS_LWR_b },                 // 78
    { 2, { 0x1D, 0x66, 0x00 }, _GS_LWR_f },                 // 79
    { 3, { 0x1D, 0x67, 0x30 }, _GS_LWR_g_ZERO },            // 80
    { 3, { 0x1D, 0x67, 0x32 }, _GS_LWR_g_TWO },             // 81
    { 2, { 0x1D, 0x68, 0x00 }, _GS_LWR_h },                 // 82
    { 2, { 0x1D, 0x6B, 0x00 }, _GS_LWR_k },                 // 83
    { 2, { 0x1D, 0x72, 0x00 }, _GS_LWR_r },                 // 84
    { 3, { 0x1D, 0x76, 0x30 }, _GS_LWR_v_ZERO },            // 85
    { 2, { 0x1D, 0x77, 0x00 }, _GS_LWR_w },                 // 86
};
//-------------------------------------------------------
static symbols sym_000[18] = {     //48
    SYM_function_065, //0
    SYM_function_065, //1
    SYM_function_067, //2
    SYM_function_068, //3
    SYM_function_069, //4
    SYM_function_070, //5
    SYM_NULL, //6
    SYM_NULL, //7
    SYM_NULL, //8
    SYM_NULL, //9
    SYM_NULL, //10
    SYM_NULL, //11
    SYM_NULL, //12
    SYM_NULL, //13
    SYM_NULL, //14
    SYM_function_080, //15
    SYM_function_081, //16
    SYM_function_082, //17
};
//-------------------------------------------------------
static symbols sym_001[18] = {     //49
    SYM_function_165, //0
    SYM_NULL, //1
    SYM_function_167, //2
    SYM_NULL, //3
    SYM_function_169, //4
    SYM_NULL, //5
    SYM_NULL, //6
    SYM_NULL, //7
    SYM_NULL, //8
    SYM_NULL, //9
    SYM_NULL, //10
    SYM_NULL, //11
    SYM_NULL, //12
    SYM_NULL, //13
    SYM_NULL, //14
    SYM_function_180, //15
    SYM_function_181, //16
    SYM_function_182, //17
};
//-------------------------------------------------------
static symbols sym_002[18] = {     //50
    SYM_function_265, //0
    SYM_NULL, //1
    SYM_NULL, //2
    SYM_NULL, //3
    SYM_NULL, //4
    SYM_NULL, //5
    SYM_NULL, //6
    SYM_NULL, //7
    SYM_NULL, //8
    SYM_NULL, //9
    SYM_NULL, //10
    SYM_NULL, //11
    SYM_NULL, //12
    SYM_NULL, //13
    SYM_NULL, //14
    SYM_function_280, //15
    SYM_function_281, //16
    SYM_function_282, //17
};
//-------------------------------------------------------
static symbols sym_003[18] = {     //51
    SYM_NULL, //0
    SYM_NULL, //1
    SYM_function_367, //2
    SYM_NULL, //3
    SYM_NULL, //4
    SYM_NULL, //5
    SYM_NULL, //6
    SYM_NULL, //7
    SYM_NULL, //8
    SYM_NULL, //9
    SYM_NULL, //10
    SYM_NULL, //11
    SYM_NULL, //12
    SYM_NULL, //13
    SYM_NULL, //14
    SYM_function_380, //15
    SYM_function_381, //16
    SYM_function_382, //17
};
//-------------------------------------------------------
static symbols sym_004[18] = {       //52
    SYM_NULL, //0
    SYM_NULL, //1
    SYM_function_467, //2
    SYM_NULL, //3
    SYM_NULL, //4
    SYM_NULL, //5
    SYM_function_471, //6
    SYM_function_472, //7
    SYM_NULL, //8
    SYM_NULL, //9
    SYM_NULL, //10
    SYM_NULL, //11
    SYM_NULL, //12
    SYM_NULL, //13
    SYM_NULL, //14
    SYM_function_480, //15
    SYM_function_481, //16
    SYM_function_482, //17
};
//-------------------------------------------------------
static symbols* sym_ptr[5] = {
    sym_000, //0
    sym_001, //1
    sym_002, //2
    sym_003, //3
    sym_004, //4
};
//-------------------------------------------------------
static graphics gs_function[19] = {
    GS_function_48,
    GS_function_49,
    GS_function_50,
    GS_function_51,
    GS_function_52,
    GS_function_64,
    GS_function_65,
    GS_function_66,
    GS_function_67,
    GS_function_68,
    GS_function_69,
    GS_function_80,
    GS_function_81,
    GS_function_82,
    GS_function_83,
    GS_function_84,
    GS_function_85,
    GS_function_112,
    GS_function_113,
};
//-------------------------------------------------------