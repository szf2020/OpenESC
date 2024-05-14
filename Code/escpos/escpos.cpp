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

#include "esc_functions.h"

//const char* file_under_test = "testFiles/001.prn";
//const char* file_under_test = "testFiles/002.prn";
//const char* file_under_test = "testFiles/003.prn";
//const char* file_under_test = "testFiles/004.prn";
//const char* file_under_test = "testFiles/005.prn";
//const char* file_under_test = "testFiles/006.prn";
const char* file_under_test = "testFiles/007.prn";

//-------------------------------------------------------

int8_t ESCPOS_parse(RxBuffer* b);
int find_esc_pos(int rec, CMD* ptr, int depth, uint8_t* c);

extern CMD escpos[CMD_SIZE]; //located in esc_functionc.cpp

int main()
{
    printf("File:%s\n", file_under_test);   //dump file name for debugging
    RxBuffer rx(file_under_test);           //open up file
    while (ESCPOS_parse(&rx) != -1);        //start parsing

    return 0;
}


// I don't see how ESCPOS can be used with a steaming buffer
// I think one would need to buffer the chars up first, then parse.

int8_t ESCPOS_parse(RxBuffer* b)
{
    if (b->peekNext() != -1) {
        uint8_t c = (uint8_t) b->getNext();
        /* ------------ operation codes ------------- */
        if (c <= 0x1F) {       
            int u = 0, depth = 1, i = 1;
            uint8_t g[BMAX] = { c, 0x00, 0x00 };

            while ((u = find_esc_pos(u, escpos, depth++, g)) >= 0)          //check at nth depth
            {
                if ((depth <= escpos[u].depth) && (b->peekNext() != -1)) {  //if we can go deeper
                    g[i++] = (uint8_t)b->getNext();                         //pop a char
                } 
                else {
                    break; //else break
                }
            }

            if (u >= 0) { //check result & run 'fun!'-ction
                if(escpos[u].ptr != nullptr)
                    escpos[u].ptr(b);
            } 
            else {
                printf("NA [0x%.2X]\n", g[0]);
            }
        }
        /* -------- human readable text ------------ */
        else {
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
