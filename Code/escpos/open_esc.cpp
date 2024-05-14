/**********************************************************************************************
 *
 *     .oooooo.                                    oooooooooooo  .oooooo..o   .oooooo.   
 *    d8P'  `Y8b                                   `888'     `8 d8P'    `Y8  d8P'  `Y8b  
 *   888      888 oo.ooooo.   .ooooo.  ooo. .oo.    888         Y88bo.      888          
 *   888      888  888' `88b d88' `88b `888P"Y88b   888oooo8     `"Y8888o.  888          
 *   888      888  888   888 888ooo888  888   888   888    "         `"Y88b 888          
 *   `88b    d88'  888   888 888    .o  888   888   888       o oo     .d8P `88b    ooo  
 *    `Y8bood8P'   888bod8P' `Y8bod8P' o888o o888o o888ooooood8 8""88888P'   `Y8bood8P'  
 *                 888                                                                   
 *                o888o                                                 (MJM 5-13-2024)
 * 
 *   Wrote from the EPSON TM-T90 Technical Specification
 *
 *   File: open_esc.cpp (main)
 *   Note: This is a prelude to the firmware implementation.
 *
 *********************************************************************************************/

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdbool>

#include "esc_functions.h"

//const char* file_under_test = "testFiles/001.prn";
//const char* file_under_test = "testFiles/002.prn";
//const char* file_under_test = "testFiles/003.prn";
//const char* file_under_test = "testFiles/004.prn";
//const char* file_under_test = "testFiles/005.prn";
//const char* file_under_test = "testFiles/006.prn";

const char* file_under_test = "testFiles/007.prn";

//-------------------------------------------------------

extern CMD escpos[CMD_SIZE]; //located in esc_functions.cpp

//-------------------------------------------------------

int8_t ESCPOS_parse(RxBuffer* b);
int find_esc_pos(int rec, CMD* ptr, int depth, uint8_t* c);

//-------------------------------------------------------

int main() {
    printf("File:%s\n", file_under_test);   //dump file name for debugging
    RxBuffer rx(file_under_test);           //open up file
    while (ESCPOS_parse(&rx) != -1);        //start parsing
    return 0;
}

// I don't see how ESCPOS can be used with a steaming buffer
// I think one would need to buffer the chars up first, then parse.

/*---------------------------------------------------------------------------------
    Target Operating Specification (from T88II)

        4k      Receive Buffer
        256K    Non-Volatile Bit Image Buffer
        1K      User NVM
        12k     User-defined characters & User-defined bit-image
        2k      Macro buffer (but not really, I may drop macro support)

-----------------------------------------------------------------------------------

    Scratch pad / Notes to Self:

    80mm @ 8dpmm = 640dots | 80 bytes
    (256 x 1024) / 80 = 3276.6 [row of 80 bytes]
    3276.2 / 8 = 409.525mm in length | 16.12"
    ...so, thats pretty reasonable.... 

    Test File: 007.prn is 1.57k, 
        converted to BMP_MONO, 50.4k (aprox. render size),
        At 203DPI, the image width is 2.84" x 3.53" | 72.136mm x 89.662mm (wxh)

    ...It might be wise to double the Rx buffer to 8K

-----------------------------------------------------------------------------------*/

int8_t ESCPOS_parse(RxBuffer* b) {
    if (b->peekNext() != -1) {
        uint8_t c = (uint8_t) b->getNext();
        /* ------------ operation codes ------------- */
        if (c <= 0x1F) {       
            int u = 0, depth = 1, i = 1;
            uint8_t g[BMAX] = { c, 0x00, 0x00 };

            while ((u = find_esc_pos(u, escpos, depth++, g)) >= 0) {        //check at nth depth
                if ((depth <= escpos[u].depth) && (b->peekNext() != -1)) {  //if we can go deeper
                    g[i++] = (uint8_t)b->getNext();                         //pop a char
                } 
                else {
                    break; //else break
                }
            }

            if (u >= 0) { //check result & run 'fun!'-ction
                if (escpos[u].ptr != nullptr)
                    if (escpos[u].ptr(b) != 0) {
                        printf("something broke!\n");
                        return -1;
                    }
            } 
            else {
                printf("NA [0x%.2X]\n", g[0]);
            }
        }
        /* -------- human readable text ------------ */
        else {
            printf("[");
            while (b->peekNext() > 0x1F)
                printf("%c", (uint8_t)b->getNext());
            printf("]\n");
        }
        return 0;
    }
    else {
        printf("buffer empty\n");
    }
    return -1; //EOF
}

int find_esc_pos(int rec, CMD* ptr, int depth, uint8_t* c) {
    int f = 1, i = rec;
    while (i < CMD_SIZE) {
        for (int j = 0; j < depth; j++)
            f &= (c[j] == ptr[i].c[j]);
        if (f) break;
        i++; f = 1;
    }
    if (i >= CMD_SIZE) return -1; //over run / not found
    return i;
}
