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

struct settings 
{
    uint8_t mode; //page mode = 1, standard mode = 0

    /* paper sensor (2 = stop, 1 = signal, 0 = off) */
    uint8_t RP_NE_A;    //roll paper near end
    uint8_t RP_NE_B;    //roll paper near end
    uint8_t RP_E_A;     //roll paper end
    uint8_t RP_E_B;     //roll paper end

    uint8_t panel_but;  //panel buttons

    int8_t right_side_char_spacing;     //right side
    uint8_t underline_dots;             //underline thickness
    uint8_t cw_rotation;                //font clockwise rotation mode
    uint8_t char_width;                 //multiplier value
    uint8_t char_height;                //multiplier value
    int8_t justification;               //justification

    uint8_t char_font;      //font selection
    uint8_t user_char_set;  //user defined character set
    uint8_t emphasize;      //emphasized mode
    uint8_t double_height;  //double height mode
    uint8_t double_width;   //double width mode
    uint8_t underline;      //underline mode
    uint8_t double_strike;  //double strike mode
    uint8_t reverse;        //reverse printing mode
    uint8_t code_table;     //character code table
    uint8_t country_code;   //region code
    uint8_t smoothing;      //smoothing mode

    uint8_t upside_dwn;     //upside down print mode

    uint16_t left_margin;   //left margin

    uint16_t abs_h_ppos;    //absolute horizontal print position
    uint16_t abs_v_ppos;    //absolute vertical print position
    uint16_t r_ppos;        //relative print position
    uint16_t cw_pmd;        //page mode direction

    /* page mode print area */
    uint16_t x;             //position x
    uint16_t y;             //position y
    uint16_t width;         //area width
    uint16_t height;        //area height

};

static settings device;


void default_settings(void);