// escpos.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//
//
// (used as starting point, may revise)
// https://stackoverflow.com/a/7671028  
// 
// Wrote from the EPSON TM-T90 spec sheet (MJM 2024)
//
// ESCPOS parser test
//

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdbool>

#include "RxBuffer.h"

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


#define CMD_SIZE 87
#define BMAX 3

//const char* file_under_test = "testFiles/001.prn";
//const char* file_under_test = "testFiles/002.prn";
//const char* file_under_test = "testFiles/003.prn";
//const char* file_under_test = "testFiles/004.prn";
//const char* file_under_test = "testFiles/005.prn";
//const char* file_under_test = "testFiles/006.prn";
const char* file_under_test = "testFiles/007.prn";

const int DPI_203 = (int)(8.0 * 25.4); //real is 203.2 dots/inch

typedef int (*operation_fp)(RxBuffer* b);
typedef int (*operation_sym_fp)(RxBuffer* b, int k);

typedef int (*operation_opta)(RxBuffer* b, int argc, uint8_t* argv);

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

struct CMD {
    uint8_t depth;
    uint8_t c[BMAX];
    operation_fp ptr;
};

CMD escpos[CMD_SIZE] =
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

int raster_function_112(RxBuffer* b, int argc, uint8_t* argv);

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

int SYM_function_165(RxBuffer* b, int k); //49
int SYM_function_167(RxBuffer* b, int k);
int SYM_function_169(RxBuffer* b, int k);
int SYM_function_180(RxBuffer* b, int k);
int SYM_function_181(RxBuffer* b, int k);
int SYM_function_182(RxBuffer* b, int k);

int SYM_function_265(RxBuffer* b, int k); //50
int SYM_function_280(RxBuffer* b, int k);
int SYM_function_281(RxBuffer* b, int k);
int SYM_function_282(RxBuffer* b, int k);

int SYM_function_367(RxBuffer* b, int k); //51
int SYM_function_371(RxBuffer* b, int k);
int SYM_function_380(RxBuffer* b, int k);
int SYM_function_381(RxBuffer* b, int k);
int SYM_function_382(RxBuffer* b, int k);

int SYM_function_467(RxBuffer* b, int k); //52
int SYM_function_471(RxBuffer* b, int k);
int SYM_function_472(RxBuffer* b, int k);
int SYM_function_480(RxBuffer* b, int k);
int SYM_function_481(RxBuffer* b, int k);
int SYM_function_482(RxBuffer* b, int k);

int SYM_NULL(RxBuffer* b, int k) { return -1; };

struct symbols {
    operation_sym_fp ptr;
};

symbols sym_000[18] = {     //48
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

symbols sym_001[18] = {     //49
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

symbols sym_002[18] = {     //50
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

symbols sym_003[18] = {     //51
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

symbols sym_004[18] = {       //52
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

symbols* sym_ptr[5] = {
    sym_000, //0
    sym_001, //1
    sym_002, //2
    sym_003, //3
    sym_004, //4
};

//-------------------------------------------------------

int GS_function_48(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_49(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_50(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_51(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_52(RxBuffer* b, int argc, uint8_t* argv);

int GS_function_64(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_65(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_66(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_67(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_68(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_69(RxBuffer* b, int argc, uint8_t* argv);

int GS_function_80(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_81(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_82(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_83(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_84(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_85(RxBuffer* b, int argc, uint8_t* argv);

int GS_function_112(RxBuffer* b, int argc, uint8_t* argv);
int GS_function_113(RxBuffer* b, int argc, uint8_t* argv);

struct graphics {
    operation_opta ptr;
};

graphics gs_function[19] = {
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

static float vertical_motion_unit   = 1;
static float horizontal_motion_unit = 1;
static int line_spacing = 30;

int8_t ESCPOS_parse(RxBuffer* b);
int find_esc_pos(int rec, CMD* ptr, int depth, uint8_t* c);

int main()
{
    printf("Hello World\n");

    RxBuffer rx(file_under_test);       //open up file
    while (ESCPOS_parse(&rx) != -1);    //start parsing

    //spew sample data
    //for (int i = 0; i < CMD_SIZE; i++)
    //    for (int j = 0; j < escpos[i].depth; j++)
    //        printf("%.2X ", escpos[i].c[j]);
    //fflush(stdout);

    return 0;
}

int8_t ESCPOS_parse(RxBuffer* b)
{
    if (b->peekNext() != -1)
    {
        uint8_t c = (uint8_t) b->getNext();
        /* ------------ operation codes ------------- */
        if (c <= 0x1F)
        {       
            int u = 0, depth = 1, i = 1;
            uint8_t g[BMAX] = { c, 0x00, 0x00 };

            while ((u = find_esc_pos(u, escpos, depth++, g)) >= 0) //check at nth depth
            {
                if ((depth <= escpos[u].depth) && (b->peekNext() != -1)) //if we can go deeper
                {
                    g[i++] = (uint8_t)b->getNext(); //pop a char
                }
                else 
                {
                    break; //else break
                }
            }

            if (u >= 0) //check result & run function
            {
                if(escpos[u].ptr != nullptr)
                    escpos[u].ptr(b);
            }
            else 
            {
                printf("NA [0x%.2X]\n", g[0]);
            }

        }
        /* -------- human readable text ------------ */
        else
        {
            std::string str; str += (char)c;
            while (b->peekNext() > 0x1F)
                str += (char) b->getNext();

            printf("[%s]\n", str.c_str()); 
        }

        return 0;
    }
    else {
        printf("buffer empty\n");
    }

    return -1; //EOF
}

int find_esc_pos(int rec, CMD* ptr, int depth, uint8_t* c)
{
    int f = 1, i = rec;

    while (i < CMD_SIZE)
    {
        for (int j = 0; j < depth; j++)
            f &= (c[j] == ptr[i].c[j]);

        if (f) break;
        i++; f = 1;
    }

    if (i >= CMD_SIZE) return -1; //over run / not found
    return i;
}

// Command: Horizontal Tab 
int _HT(RxBuffer* b)
{
    printf("<HT>\n");
    return 0;
}

// Command: Print a line feed
int _LF(RxBuffer* b)
{
    printf("<LF>\n");
    return 0;
}

// Command: Print and return to standard mode (in page mode)
int _FF(RxBuffer* b)
{
    printf("<FF>\n");
    return 0;
}

// Command: Print a carriage return
int _CR(RxBuffer* b)
{
    printf("<CR>\n");
    return 0;
}

// Command: Transmit real-time status
int _DLE_ENQ(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<DLE ENQ> n=0x%.2X\n", n);
    return 0;
}

// ?
int _DLE_DC4(RxBuffer* b)
{
    // TODO:
    return 0;
}

// Command: Transmit real-time status
int _DLE_EOT(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    uint8_t a = (uint8_t)b->getNext();
    printf("<DLE EOT> n=0x%.2X, a=0x%.2X\n", n, a);
    return 0;
}

// Command: Cancel print data in page mode
int _CAN(RxBuffer* b)
{
    printf("<CAN>\n");
    return 0;
}

// Command: Print data in page mode
int _ESC_FF(RxBuffer* b)
{
    printf("<ESC FF>\n");
    return 0;
}

// Command: Set right-side character spacing
int _ESC_SP(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC SP> n=0x%.2X\n", n);
    return 0;
}

// Command: Select print modes(s)
int _ESC_EXCLAMATION_SYM(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC !> n=0x%.2X\n", n);
    return 0;
}

// Command: Set absolute print position
int _ESC_DOLLAR_SYM(RxBuffer* b)
{
    uint8_t nL = (uint8_t)b->getNext();
    uint8_t nH = (uint8_t)b->getNext();
    printf("<ESC $> nL=0x%.2X, nH=0x%.2X\n", nL, nH);
    printf("-maths: %d\n", (int)((nL + nH * 256) * horizontal_motion_unit));
    return 0;
}

// Command: Select / cancel user-defined character set
int _ESC_PERCENT_SYM(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC %%> n=0x%.2X\n", n);
    return 0;
}

// Command: Define user-defined characters
int _ESC_AND_SYM(RxBuffer* b)
{
    // TODO:
    return 0;
}

// Command: Select bit-image mode
int _ESC_STAR_SYM(RxBuffer* b)
{
    // TODO:
    return 0;
}

// Command: Turn underline mode on/off
int _ESC_SUBTRACT_SYM(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC -> n=0x%.2X\n", n);
    return 0;
}

// Command: Select default line spacing
int _ESC_ASCII_TWO(RxBuffer* b)
{
    printf("<ESC 2>\n");
    line_spacing = 30;
    return 0;
}

// Command: Set line spacing
int _ESC_ASCII_THREE(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    line_spacing = (int)(n * vertical_motion_unit);
    printf("<ESC 3> n=0x%.2X\n", n);
    printf("-maths: %d\n", line_spacing);
    
    return 0;
}

// Command: Select peripheral device
int _ESC_EQUAL_SYM(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC => n=0x%.2X\n", n);
    return 0;
}

// Command: Cancel user-defined characters
int _ESC_QUESTION_SYM(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC ?> n=0x%.2X\n", n);
    return 0;
}

// Command: initialize printer
int _ESC_AT_SYM(RxBuffer* b)
{
    printf("<ESC @>\n");
    return 0;
}

// Command: Set horizontal tab position 
int _ESC_UPR_D(RxBuffer* b)
{
    // TODO:
    return 0;
}

// Command: Turn emphasized mode on/off
int _ESC_UPR_E(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC E> n=0x%.2X\n", n);
    return 0;
}

// Command: Turn double-strike mode on/off
int _ESC_UPR_G(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC G> n=0x%.2X\n", n);
    return 0;
}

// Command: Print and feed paper
int _ESC_UPR_J(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC J> n=0x%.2X\n", n);
    return 0;
}

// Command: Select page mode
int _ESC_UPR_L(RxBuffer* b)
{
    printf("<ESC L>\n");
    return 0;
}

// Command: Select character font
int _ESC_UPR_M(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC M> n=0x%.2X\n", n);
    return 0;
}

// Command: Select an international character set
int _ESC_UPR_R(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC R> n=0x%.2X\n", n);
    return 0;
}

// Command: Select standard mode
int _ESC_UPR_S(RxBuffer* b)
{
    printf("<ESC S>\n");
    return 0;
}

// Command: Select print direction in page mode
int _ESC_UPR_T(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC T> n=0x%.2X\n", n);
    switch (n) {
        case 0: case 48: printf("-Upper Left\n");   break;
        case 1: case 49: printf("-Lower Left\n");   break;
        case 2: case 50: printf("-Lower Right\n");  break;
        case 3: case 51: printf("-Upper Right\n");  break;
        default:                                   break;
    }
    return 0;
}

// Command: Turn clockwise rotation mode on/off
int _ESC_UPR_V(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC V> n=0x%.2X\n", n);
    return 0;
}

// Command: Set print area in page mode
int _ESC_UPR_W(RxBuffer* b)
{
    // TODO:
    uint8_t xL = (uint8_t)b->getNext();
    uint8_t xH = (uint8_t)b->getNext();
    uint8_t yL = (uint8_t)b->getNext();
    uint8_t yH = (uint8_t)b->getNext();
    uint8_t dxL = (uint8_t)b->getNext();
    uint8_t dxH = (uint8_t)b->getNext();
    uint8_t dyL = (uint8_t)b->getNext();
    uint8_t dyH = (uint8_t)b->getNext();
    
    printf("<ESC W> xL=0x%.2X, xH=0x%.2X, \
        yL=0x%.2X, yH=0x%.2X, dxL=0x%.2X, \
        dxH=0x%.2X, dyL=0x%.2X, dyH=0x%.2X\n",
        xL, xH, yL, yH, dxL, dxH, dyL, dyH);

    printf("-maths: %d, %d, %d, %d\n",
        (int)((xL + xH * 256)   * horizontal_motion_unit),
        (int)((yL + yH * 256)   * vertical_motion_unit),
        (int)((dxL + dxH * 256) * horizontal_motion_unit),
        (int)((dyL + dyH * 256) * vertical_motion_unit));

    // Horizontal logical origin = (xL + xH × 256) × (horizontal motion unit) from absolute origin.
    // Vertical logical origin   = (yL + yH × 256) × (vertical motion unit) from absolute origin.
    // Print area width          = (dxL + dxH × 256) × (horizontal motion unit)
    // Print area height         = (dyL + dyH × 256) × (vertical motion unit)
    // motion units              = DPI or PDMM

    return 0;
}

// Command: Set relative print position
int _ESC_BACKSLASH_SYM(RxBuffer* b)
{
    uint8_t nL = (uint8_t)b->getNext();
    uint8_t nH = (uint8_t)b->getNext();
    printf("<ESC \\> nL=0x%.2X, nH=0x%.2X\n", (char)nL & 0xFF, (char)nH & 0xFF);
    // Moves the print position to 
    // (nL + nH × 256) × (horizontal or vertical motion unit) 
    // from the current position
    printf("-maths: %d\n", (int)((nL + nH * 256) * horizontal_motion_unit));

    return 0;
}

// Command: Select justification
int _ESC_LWR_a(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC a> n=0x%.2X\n", n);
    switch (n) {
        case 0: case 48: printf("-Left Justification\n");    break;
        case 1: case 49: printf("-Centered\n");              break;
        case 2: case 50: printf("-Right Justification\n");   break;
        default:                                            break;
    }
    return 0;
}

// Command: Select paper sensor to output paper-end signals
int _ESC_LWR_c_THREE(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC c 3> n=0x%.2X\n", n);
    return 0;
}

// Command: Select paper sensor to stop printing
int _ESC_LWR_c_FOUR(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC c 4> n=0x%.2X\n", n);
    return 0;
}

// Command: Enable / disable panel buttons
int _ESC_LWR_c_FIVE(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC c 5> n=0x%.2X\n", n);
    return 0;
}

// Command: Print and feed n lines
int _ESC_LWR_d(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC d> n=0x%.2X\n", n);
    return 0;
}

// Command: Generate pulse
int _ESC_LWR_p(RxBuffer* b)
{
    uint8_t m = (uint8_t)b->getNext();
    uint8_t t1 = (uint8_t)b->getNext();
    uint8_t t2 = (uint8_t)b->getNext();
    printf("<ESC p> m=0x%.2X, t1=0x%.2X, t2=0x%.2X\n", m, t1, t2);
    return 0;
}

// Command: Select character code table
int _ESC_LWR_t(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC t> n=0x%.2X\n", n);
    switch (n) {
        case 0:   printf("-Page 0 [PC437 (U.S.A)]\n");              break;
        case 1:   printf("-Page 1 [Katakana]\n");                   break;
        case 2:   printf("-Page 2 [PC850 (Multilingual)]\n");       break;
        case 3:   printf("-Page 3 [PC860 (Portuguese)]\n");         break;
        case 4:   printf("-Page 4 [PC863 (Canadian-French)]\n");    break;
        case 5:   printf("-Page 5 [PC865 (Nordic)]\n");             break;
        case 6:   printf("-Page 6 [Simplified Kanji, Hirakana]\n"); break;
        case 7:   printf("-Page 7 [Simplified Kanji]\n");           break;
        case 8:   printf("-Page 8 [Simplified Kanji]\n");           break;
        case 16:  printf("-Page 16 [WPC1252]\n");                   break;
        case 17:  printf("-Page 17 [PC866 (Cyrillic #2)]\n");       break;
        case 18:  printf("-Page 18 [PC852 (Latin 2)]\n");           break;
        case 19:  printf("-Page 19 [PC858 (Euro)]\n");              break;
        case 254: printf("-Page 254\n");                            break;
        case 255: printf("-Page 255\n");                            break;
        default:                                                   break;
    }

    return 0;
}

// Command: Turn upside-down print mode on/off
int _ESC_LEFT_QBRACKET_SYM(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC {> n=0x%.2X\n", n);
    return 0;
}

// Command: Select print mode(s) for Kanji characters
int _FS_EXCLAMATION_SYM(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<FS !> n=0x%.2X\n", n);
    return 0;
}

// Command: Select Kanji character mode
int _FS_AND_SYM(RxBuffer* b)
{
    printf("<FS &>\n");
    return 0;
}

// Command: Define character effects of Kanji characters
int _FS_LEFT_PERNTH_UPR_A(RxBuffer* b)
{
    uint8_t pL = (uint8_t)b->getNext();
    uint8_t pH = (uint8_t)b->getNext();
    uint8_t fn = (uint8_t)b->getNext();
    uint8_t m = (uint8_t)b->getNext();
    printf("<FS ( A> pL=0x%.2X, pH=0x%.2X, fn=0x%.2X, m=0x%.2X\n", pL, pH, fn, m);
    
    switch (m) {
        case 0: case 48: printf("-Select Kanji character Font A\n"); break;
        case 1: case 49: printf("-Select Kanji character Font B\n"); break;
        case 2: case 50: printf("-Select Kanji character Font C\n"); break;
        default:                                                    break;
    }
    
    return 0;
}

// Command: Turn underline mode on/off for Kanji characters
int _FS_SUBTRACT_SYM(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<FS -> n=0x%.2X\n", n);

    switch (n) {
        case 0: case 48: printf("-Turns off Kanji-underline mode\n");               break;
        case 1: case 49: printf("-Turns on Kanji-underline mode (1-dot thick)\n");  break;
        case 2: case 50: printf("-Turns on Kanji-underline mode (2-dots thick)\n"); break;
        default:                                                                   break;
    }

    return 0;
}

// Command: Cancel Kanji character mode
int _FS_PERIOD_SYM(RxBuffer* b)
{
    printf("<FS .>\n");
    printf("-Cancel Kanji character mode\n");
    return 0;
}

// Command: Define user-defined Kanji characters
int _FS_ASCII_TWO(RxBuffer* b)
{
    // TODO:
    return 0;
}

// Command: Select Kanji character code system
int _FS_UPR_C(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<FS C> n=0x%.2X\n", n);

    switch (n) {
        case 0: case 48: printf("-JIS Code\n");        break;
        case 1: case 49: printf("-SHIFT JIS Code\n");  break;
        default:                                      break;
    }

    return 0;
}

// Command: Set Kanji character spacing
int _FS_UPR_S(RxBuffer* b)
{
    uint8_t n1 = (uint8_t)b->getNext();
    uint8_t n2 = (uint8_t)b->getNext();
    printf("<FS S> n1=0x%.2X, n2=0x%.2X\n", n1, n2);
    return 0;
}

// Command: Turn quadruple-size mode on/off for Kanji characters
int _FS_UPR_W(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<FS W> n=0x%.2X\n", n);
    return 0;
}

// Command: Print NV bit image
int _FS_LWR_p(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    uint8_t m = (uint8_t)b->getNext();
    printf("<FS p> n=0x%.2X, m=0x%.2X\n", n, m);
    return 0;
}

// Command: Select character size
int _GS_EXCLAMATION_SYM(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS !> n=0x%.2X\n", n);

    printf("-Character Width: %d\n",  ((n >> 4) & 0x0F) + 1);
    printf("-Character Height: %d\n", (n & 0x0F) + 1);
    return 0;
}

// Command: Set absolute vertical print position in page mode
int _GS_DOLLAR_SYM(RxBuffer* b)
{
    uint8_t nL = (uint8_t)b->getNext();
    uint8_t nH = (uint8_t)b->getNext();
    printf("<GS $> nL=0x%.2X, nH=0x%.2X\n", nL, nH);
    printf("-maths: %d\n", (int)((nL + nH * 256) * vertical_motion_unit));
    return 0;
}

// Command: Execute test print
int _GS_LEFT_PERNTH_UPR_A(RxBuffer* b)
{
    uint8_t pL = (uint8_t)b->getNext();
    uint8_t pH = (uint8_t)b->getNext();
    uint8_t n = (uint8_t)b->getNext();
    uint8_t m = (uint8_t)b->getNext();
    printf("<GS ( A> pL=0x%.2X, pH=0x%.2X, n=0x%.2X, m=0x%.2X\n", pL, pH, n, m);

    switch (n) {
        case 1: case 42: case 2: case 50: printf("-Roll Paper\n");   break;
        default:                          printf("-Basic Sheet\n");  break;
    }

    switch (m) {
        case 1: case 49: printf("-Hex Dump\n");                      break;
        case 2: case 50: printf("-Printer Status Settings\n");       break;
        case 3: case 51: printf("-Rolling Pattern\n");               break;
        case 64:         printf("-Auto Setting of Paper Layout\n");  break;
        default:                                                     break;
    }

    return 0;
}

// Command: Edit NV user memory
int _GS_LEFT_PERNTH_UPR_C(RxBuffer* b)
{
    // TODO:
    return 0;
}

// Command: Enable / disable real-time command
int _GS_LEFT_PERNTH_UPR_D(RxBuffer* b)
{
    // TODO:
    return 0;
}

// Command: Set user commands
int _GS_LEFT_PERNTH_UPR_E(RxBuffer* b)
{
    // TODO:
    return 0;
}

// Command: Request transmission of response or status
int _GS_LEFT_PERNTH_UPR_H(RxBuffer* b)
{
    // TODO:
    return 0;
}

// Command: Select print control method(s)
int _GS_LEFT_PERNTH_UPR_K(RxBuffer* b)
{
    // TODO:
    return 0;
}

// Command: Set graphics data
int _GS_LEFT_PERNTH_UPR_L(RxBuffer* b)
{
    int index = -1;
    uint8_t args[4] = {
        (uint8_t)b->getNext(),
        (uint8_t)b->getNext(),
        (uint8_t)b->getNext(),
        (uint8_t)b->getNext()
    };

    printf("<GS ( L> pL=0x%.2X, pH=0x%.2X, m=0x%.2X, fn=0x%.2X\n",
        args[0], args[1], args[2], args[3]);
    
    if ((args[3] >= 0) && (args[3] <= 4)) {           //0-4
        index = args[3];
    }
    else if ((args[3] >= 48) && (args[3] <= 52)) {    //0-4
        index = args[3] - 48;
    }
    else if ((args[3] >= 64) && (args[3] <= 69)) {    //5-10
        index = (args[3] - 64) + 5;
    }
    else if ((args[3] >= 80) && (args[3] <= 85)) {    //11-16
        index = (args[3] - 80) + 11;
    }
    else if ((args[3] >= 112) && (args[3] <= 113)) {  //17-18
        index = (args[3] - 112) + 17;
    }

    if (index != -1)
        gs_function[index].ptr(b, 4, args);

    return 0;
}

// Command: Customize printer control value(s)
int _GS_LEFT_PERNTH_UPR_M(RxBuffer* b)
{
    // TODO:
    return 0;
}

// Command: Select character effects
int _GS_LEFT_PERNTH_UPR_N(RxBuffer* b)
{
    // TODO:
    return 0;
}

// Command: Set up and print the symbol
int _GS_LEFT_PERNTH_LWR_k(RxBuffer* b)
{
    uint8_t pL = (uint8_t)b->getNext();
    uint8_t pH = (uint8_t)b->getNext();
    uint8_t cn = (uint8_t)b->getNext();
    uint8_t fn = (uint8_t)b->getNext();
    int k = (pL + pH * 256);

    printf("<GS ( k> pL=0x%.2X, pH=0x%.2X, cn=0x%.2X, fn=0x%.2X, k:%d\n", pL, pH, cn, fn, k);

    if ((cn < 53) && (cn > 47)) {
        symbols* ptr = sym_ptr[cn - 48];    //index symbol type
        if ((fn < 83) && (fn > 64))
            return ptr[fn - 65].ptr(b, k);  //index function code
        return -1;
    }

    return -1;
}

// Command: Define downloaded bit image
int _GS_STAR_SYM(RxBuffer* b)
{
    // TODO:
    return 0;
}

// Command: Print downloaded bit image
int _ESC_FWRDSLASH_SYM(RxBuffer* b)
{
    uint8_t m = (uint8_t)b->getNext();
    printf("<GS /> m=0x%.2X\n", m);
    return 0;
}

// Command: Set graphics data
int _GS_EIGHT_UPR_L(RxBuffer* b)
{
    // TODO:
    uint8_t args[4] = {
        (uint8_t)b->getNext(),
        (uint8_t)b->getNext(),
        (uint8_t)b->getNext(),
        (uint8_t)b->getNext()
    };

    uint8_t m = (uint8_t)b->getNext();   //48, also assumed to be color mode 
    uint8_t fn = (uint8_t)b->getNext();  //function mode

    if (fn == 0x70) { //112
        return raster_function_112(b, 4, args);
    }

    return -1;
}

// Command: Start/end macro definition 
int _GS_COLON_SYM(RxBuffer* b)
{
    printf("<GS :>\n");
    return 0;
}

// Command: Turn white/black reverse print mode on/off
int _GS_UPR_B(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS B> n=0x%.2X\n", n);
    return 0;
}

// Command: Select print position of HRI characters
int _GS_UPR_H(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS H> n=0x%.2X\n", n);
    return 0;
}

// Command: Transmit printer ID
int _GS_UPR_I(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS I> n=0x%.2X\n", n);
    return 0;
}

// Command: Set left margin
int _GS_UPR_L(RxBuffer* b)
{
    uint8_t nL = (uint8_t)b->getNext();
    uint8_t nH = (uint8_t)b->getNext();
    printf("<GS L> nL=0x%.2X, nH=0x%.2X\n", nL, nH);
    return 0;
}

// Command: Set horizontal and vertical motion units
int _GS_UPR_P(RxBuffer* b)
{
    uint8_t x = (uint8_t)b->getNext();
    uint8_t y = (uint8_t)b->getNext();
    printf("<GS P> x=0x%.2X, y=0x%.2X\n", (int)(x / DPI_203), (int)(y / DPI_203));
    return 0;
}

// Command: Set print position to the beginning of print line
int _GS_UPR_T(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS T> n=0x%.2X\n", n);
    return 0;
}

// Command: Select cut mode and cut paper
int _GS_UPR_V(RxBuffer* b)
{
    uint8_t m = (uint8_t)b->getNext();
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS V> m=0x%.2X, n=0x%.2X\n", m, n);
    return 0;
}

// Command: Set print area width
int _GS_UPR_W(RxBuffer* b)
{
    uint8_t nL = (uint8_t)b->getNext();
    uint8_t nH = (uint8_t)b->getNext();
    printf("<GS W> nL=0x%.2X, nH=0x%.2X\n", nL, nH);
    return 0;
}

// Command: Set relative vertical print position in page mode
int _GS_BACKSLASH_SYM(RxBuffer* b)
{
    uint8_t nL = (uint8_t)b->getNext();
    uint8_t nH = (uint8_t)b->getNext();
    printf("<GS \\> nL=0x%.2X, nH=0x%.2X\n", nL, nH);
    return 0;
}

// Command: Execute macro
int _GS_CARROT_SYM(RxBuffer* b)
{
    uint8_t r = (uint8_t)b->getNext();
    uint8_t t = (uint8_t)b->getNext();
    uint8_t m = (uint8_t)b->getNext();
    printf("<GS ^> r=0x%.2X, t=0x%.2X, m=0x%.2X\n", r, t, m);
    return 0;
}

// Command: Enable /disable Automatic Status Back (ASB)
int _GS_LWR_a(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS a> n=0x%.2X\n", n);
    return 0;
}

// Command: Turn smoothing mode on / off
int _GS_LWR_b(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS b> n=0x%.2X\n", n);
    return 0;
}

// Command: Select font for HRI characters
int _GS_LWR_f(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS f> n=0x%.2X\n", n);
    return 0;
}

// Command: Initialize maintenance counter
int _GS_LWR_g_ZERO(RxBuffer* b)
{
    uint8_t m = (uint8_t)b->getNext();
    uint8_t nL = (uint8_t)b->getNext();
    uint8_t nH = (uint8_t)b->getNext();
    printf("<GS g 0> m=0x%.2X, nL=0x%.2X, nH=0x%.2X\n", m, nL, nH);
    return 0;
}

// Command: Transmit maintenance counter
int _GS_LWR_g_TWO(RxBuffer* b)
{
    uint8_t m = (uint8_t)b->getNext();
    uint8_t nL = (uint8_t)b->getNext();
    uint8_t nH = (uint8_t)b->getNext();
    printf("<GS g 2> m=0x%.2X, nL=0x%.2X, nH=0x%.2X\n", m, nL, nH);
    return 0;
}

// Command: Set bar code height
int _GS_LWR_h(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS h> n=0x%.2X\n", n);
    return 0;
}

// Command: Print bar code
int _GS_LWR_k(RxBuffer* b)
{
    uint8_t m = (uint8_t)b->getNext();
    printf("<GS k> m=0x%.2X (m=%d)\n", m, m);
    return 0;
}

// Command: Transmit status
int _GS_LWR_r(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS r> n=0x%.2X\n", n);
    return 0;
}

// Command: Print raster bit image
int _GS_LWR_v_ZERO(RxBuffer* b)
{
    // TODO:
    return 0;
}

// Command: Set bar code width
int _GS_LWR_w(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS w> n=0x%.2X\n", n);
    return 0;
}

int raster_function_112(RxBuffer* b, int argc, uint8_t* argv)
{
    uint8_t a = (uint8_t)b->getNext();   //48 = monochrome, 52 = multi-tone
    uint8_t bx = (uint8_t)b->getNext();
    uint8_t by = (uint8_t)b->getNext();
    uint8_t c = (uint8_t)b->getNext();
    uint8_t xl = (uint8_t)b->getNext();
    uint8_t xh = (uint8_t)b->getNext();
    uint8_t yl = (uint8_t)b->getNext();
    uint8_t yh = (uint8_t)b->getNext();

    int k = (((xl + xh * 256) + 7) / 8) * (yl + yh * 256);
    int dhorz = (xl + xh * 256);
    int dvertz = (yl + yh * 256);
    int sum = (argv[0] + argv[1] * 256 + argv[2] * 65536 + argv[3] * 16777216);

    printf("[raster info]\n");
    printf("p1:%d\n", argv[0]);
    printf("p2:%d\n", argv[1]);
    printf("p3:%d\n", argv[2]);
    printf("p4:%d\n", argv[3]);
    //printf("m:%d\n", m);
    //printf("fn:%d\n", fn);
    printf("a:%d\n", a);
    printf("bx:%d\n", bx);
    printf("by:%d\n", by);
    printf("c:%d\n", c);
    printf("xl:%d\n", xl);
    printf("xh:%d\n", xh);
    printf("yl:%d\n", yl);
    printf("yh:%d\n", yh);
    printf("k:%d\n", k);
    printf("dhorz:%d\n", dhorz);
    printf("dvirtz:%d\n", dvertz);
    printf("sum:%d\n", sum);

    //yBytes = (yl + 7) / 8; //total bytes in the y direction


    printf("------ Raster Dump ----------\n");

    for (int i = 0; i < k; i++) {
        uint8_t h = (uint8_t)b->getNext();
        //printf(" i:%d {0x%.2X} " BYTE_TO_BINARY_PATTERN " ", i, h, BYTE_TO_BINARY(h));
        //printf(BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(h));
        printf("0x%.2X ", h);
        //fflush(stdout);
    }

    printf("\n----------------------\n");
    return 0;
}

int SYM_function_065(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_066(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_067(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_068(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_069(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_070(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_080(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_081(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_082(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_165(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_167(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_169(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_180(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_181(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_182(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_265(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_280(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_281(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_282(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_367(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_371(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_380(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_381(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_382(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_467(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_471(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_472(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_480(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_481(RxBuffer* b, int k)
{
    return 0;
}

int SYM_function_482(RxBuffer* b, int k)
{
    return 0;
}

int GS_function_48(RxBuffer* b, int argc, uint8_t* argv)
{
    return 0;
}

int GS_function_49(RxBuffer* b, int argc, uint8_t* argv)
{
    return 0;
}

int GS_function_50(RxBuffer* b, int argc, uint8_t* argv)
{
    return 0;
}

int GS_function_51(RxBuffer* b, int argc, uint8_t* argv)
{
    return 0;
}

int GS_function_52(RxBuffer* b, int argc, uint8_t* argv)
{
    return 0;
}

int GS_function_64(RxBuffer* b, int argc, uint8_t* argv)
{
    return 0;
}

int GS_function_65(RxBuffer* b, int argc, uint8_t* argv)
{
    return 0;
}

int GS_function_66(RxBuffer* b, int argc, uint8_t* argv)
{
    return 0;
}

int GS_function_67(RxBuffer* b, int argc, uint8_t* argv)
{
    return 0;
}

int GS_function_68(RxBuffer* b, int argc, uint8_t* argv)
{
    return 0;
}

int GS_function_69(RxBuffer* b, int argc, uint8_t* argv)
{
    return 0;
}

int GS_function_80(RxBuffer* b, int argc, uint8_t* argv)
{
    return 0;
}

int GS_function_81(RxBuffer* b, int argc, uint8_t* argv)
{
    return 0;
}

int GS_function_82(RxBuffer* b, int argc, uint8_t* argv)
{
    return 0;
}

int GS_function_83(RxBuffer* b, int argc, uint8_t* argv)
{
    return 0;
}

int GS_function_84(RxBuffer* b, int argc, uint8_t* argv)
{
    return 0;
}

int GS_function_85(RxBuffer* b, int argc, uint8_t* argv)
{
    return 0;
}

int GS_function_112(RxBuffer* b, int argc, uint8_t* argv)
{
    return 0;
}

int GS_function_113(RxBuffer* b, int argc, uint8_t* argv)
{
    return 0;
}
