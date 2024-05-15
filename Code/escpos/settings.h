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
 *                o888o                                                 (MJM 5-15-2024)
 *
 *   Wrote from the EPSON TM-T90 Technical Specification
 *
 *   File: settings.h
 *   Note: none
 *
 *********************************************************************************************/

#pragma once

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdbool>


struct _page_mode
{
    uint8_t line_spaceing;
    uint16_t print_direction;
    
    uint16_t right_side_char_spacing;
};

struct _std_mode
{
    uint8_t line_spaceing;
    uint8_t clockwise_char_rotation;
    uint8_t upside_down;
    int8_t justification;
    int16_t left_margin;

    uint16_t print_area_width;

    uint16_t right_side_char_spacing;
};

struct _general_barcode
{
    uint8_t HRI;    //human readable text
    uint8_t h;      //height in dots
    uint8_t w;      //width, idk the unit
};

struct settings 
{
    uint8_t mode; //page mode = 1, standard mode = 0
    //------------------------------------------------

    /* paper sensor (2 = stop, 1 = signal, 0 = off) */
    uint8_t RP_NE_A;    //roll paper near end
    uint8_t RP_NE_B;    //roll paper near end
    uint8_t RP_E_A;     //roll paper end
    uint8_t RP_E_B;     //roll paper end

    uint8_t panel_btn;  //panel buttons
    //------------------------------------------------

    /* digital style! */        //The comments are green on my machine....
    uint8_t style_emph;         //emphasized mode
    uint8_t style_dh;           //double height 
    uint8_t style_dw;           //double width
    uint8_t style_unline;       //underline mode
    uint8_t style_strike;       //double strike mode
    uint8_t style_size[2];      //select char size (WxH)
        
    uint8_t reverse;            //reverse print mode
    uint8_t smoothing;          //smoothing mode

    uint8_t font_sel;           //Font selection
    uint8_t region;             //country / region code

    uint8_t usermode_font;      //select user-mode font
    uint8_t char_code_table;    //select which code page to use

    //------------------------------------------------
    _page_mode page_mode;               //page mode settings
    _std_mode std_mode;                 //page mode settings
    _general_barcode general_barcode;   //general bar-code settings

};

static settings device;


void default_settings(void);