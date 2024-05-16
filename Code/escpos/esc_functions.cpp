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
 *   File: esc_functions.cpp
 *   Note: none
 *
 *********************************************************************************************/

#include "esc_functions.h"
//----------------------------------------------------------------
#include "virtual_mem.h"
//----------------------------------------------------------------
extern settings device;
//----------------------------------------------------------------

// Command: Horizontal Tab 
int8_t _HT(RxBuffer* b) {
    printf("<HT>\n");
    return 0;
}

// Command: Print a line feed
int8_t _LF(RxBuffer* b) {
    printf("<LF>\n");
    return 0;
}

// Command: Print and return to standard mode (in page mode)
int8_t _FF(RxBuffer* b) {
    printf("<FF>\n");
    return 0;
}

// Command: Print a carriage return
int8_t _CR(RxBuffer* b) {
    printf("<CR>\n");
    return 0;
}

// Command: Transmit real-time status
int8_t _DLE_ENQ(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<DLE ENQ> n=0x%.2X\n", n);
    return 0;
}

// ?
int8_t _DLE_DC4(RxBuffer* b) {
    // TODO:
    printf("<DLE DC4>\n");
    return 0;
}

// Command: Transmit real-time status
int8_t _DLE_EOT(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    uint8_t a = (uint8_t)b->getNext();
    printf("<DLE EOT> n=0x%.2X, a=0x%.2X\n", n, a);
    return 0;
}

// Command: Cancel print data in page mode
int8_t _CAN(RxBuffer* b) {
    printf("<CAN>\n");
    return 0;
}

// Command: Print data in page mode
int8_t _ESC_FF(RxBuffer* b) {
    printf("<ESC FF>\n");
    return 0;
}

// Command: Set right-side character spacing
int8_t _ESC_SP(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC SP>\n");
    if (device.mode == 1) {
        if ((device.page_mode.print_direction == 0) || (device.page_mode.print_direction == 180)) {
            device.page_mode.right_side_char_spacing = (uint16_t)(n * device.horizontal_motion_unit); //rot [0 | 180]
        } else {
            device.page_mode.right_side_char_spacing = (uint16_t)(n * device.vertical_motion_unit); //rot [90 | 270]
        }
    } else {
        device.std_mode.right_side_char_spacing = (uint16_t)(n * device.horizontal_motion_unit);
    }
    return 0;
}

// Command: Select print modes(s)
int8_t _ESC_EXCLAMATION_SYM(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC !>\n");

    /*
    Character configurations
        Bit 0: Font 1 = Font A(12 × 24)
               Font 2 = Font B(9 × 17)
    Each character’s baseline is as follows :
        Font A(12 × 24) : 21 dots from the top of a character.
        Font B(9 × 17) : 16 dots from the top of a character.
    */

    device.font_sel     = (n & 0b00000001) ? 1 : 0; //0=F1, 1=F2
    device.style_emph   = (n & 0b00001000) ? 1 : 0;
    device.style_dh     = (n & 0b00010000) ? 1 : 0;
    device.style_dw     = (n & 0b00100000) ? 1 : 0;
    device.style_unline = (n & 0b10000000) ? 1 : 0;
    return 0;
}

// Command: Set absolute print position
int8_t _ESC_DOLLAR_SYM(RxBuffer* b) {
    uint8_t nL = (uint8_t)b->getNext();
    uint8_t nH = (uint8_t)b->getNext();
    printf("<ESC $>\n");
    //move print position
    return 0;
}

// Command: Select / cancel user-defined character set
int8_t _ESC_PERCENT_SYM(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC %%>\n");
    device.usermode_font = (n > 0) ? 1 : 0;
    return 0;
}

// Command: Define user-defined characters
int8_t _ESC_AND_SYM(RxBuffer* b) {
    // TODO:
    printf("<ESC &>\n");
    printf("Define user-defined chars <here>\n");
    return 0;
}

// Command: Select bit-image mode
int8_t _ESC_STAR_SYM(RxBuffer* b) {
    // TODO:
    printf("<ESC *>\n");
    return 0;
}

// Command: Turn underline mode on/off
int8_t _ESC_SUBTRACT_SYM(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC ->\n");
    switch (n) {
        case 0: case 48: device.style_unline = 0; break; //0 dots thick
        case 1: case 49: device.style_unline = 1; break; //1 dots thick
        case 2: case 50: device.style_unline = 2; break; //2 dots thick
        default:                                  break;
    }
    return 0;
}

// Command: Select default line spacing
int8_t _ESC_ASCII_TWO(RxBuffer* b) {
    printf("<ESC 2>\n");
    (device.mode > 0) ? \
        device.page_mode.line_spaceing = DEFAULT_LINE_SPACEING : \
        device.std_mode.line_spaceing = DEFAULT_LINE_SPACEING;
    return 0;
}

// Command: Set line spacing
int8_t _ESC_ASCII_THREE(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC 3>\n");
    (device.mode > 0) ? \
        device.page_mode.line_spaceing = n : \
        device.std_mode.line_spaceing = n;
    return 0;
}

// Command: Select peripheral device
int8_t _ESC_EQUAL_SYM(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC => n=0x%.2X\n", n);
    return 0;
}

// Command: Cancel user-defined characters, or delete? I'm not really sure at the moment
int8_t _ESC_QUESTION_SYM(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC ?> n=0x%.2X\n", n); //?
    return 0;
}

// Command: initialize printer
int8_t _ESC_AT_SYM(RxBuffer* b) {
    printf("<ESC @>\n");
    default_settings(); //move later
    return 0;
}

// Command: Set horizontal tab position 
int8_t _ESC_UPR_D(RxBuffer* b) {
    // TODO:
    printf("<ESC D>\n");
    return 0;
}

// Command: Turn emphasized mode on/off
int8_t _ESC_UPR_E(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC E>\n");
    device.style_emph = (n > 0) ? 1 : 0;
    return 0;
}

// Command: Turn double-strike mode on/off
int8_t _ESC_UPR_G(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC G>\n");
    //assumed the be a TTY physical hammer strike emulation?
    device.style_strike = (n > 0) ? 1 : 0; 
    return 0;
}

// Command: Print and feed paper
int8_t _ESC_UPR_J(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC J> n=0x%.2X\n", n);
    return 0;
}

// Command: Select page mode
int8_t _ESC_UPR_L(RxBuffer* b) {
    printf("<ESC L>\n");
    device.mode = 1; //switch to page mode
    return 0;
}

// Command: Select character font
int8_t _ESC_UPR_M(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC M>\n");
    switch (n) {
        case 0: case 48: device.font_sel = 0; break; //Font A (12x24)
        case 1: case 49: device.font_sel = 1; break; //Font B (9x17)
        case 2: case 50: device.font_sel = 1; break; //also Font B, but should be C (8x16)
        case 97:         device.font_sel = 2; break; //Extended (special font: 24x48)
        default:                              break;
    }
    return 0;
}

// Command: Select an international character set
int8_t _ESC_UPR_R(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC R>\n");    
    device.region = ((n > 0) && (n < 16)) ? n : 0; //region 0-15 
    return 0;
}

// Command: Select standard mode
int8_t _ESC_UPR_S(RxBuffer* b) {
    printf("<ESC S>\n");
    device.mode = 1; //switch to std mode
    return 0;
}

// Command: Select print direction in page mode
int8_t _ESC_UPR_T(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC T>\n");
    switch (n) {
        case 0: case 48: device.page_mode.print_direction = 0;   break; //Upper Left
        case 1: case 49: device.page_mode.print_direction = 90;  break; //Lower Left
        case 2: case 50: device.page_mode.print_direction = 180; break; //Lower Right
        case 3: case 51: device.page_mode.print_direction = 270; break; //Upper Right 
        default:                                                 break;
    }
    return 0;
}

// Command: Turn clockwise rotation mode on/off
int8_t _ESC_UPR_V(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC V>\n");

    if(device.mode == 0) {
        switch (n) {
            case 0: case 48: device.std_mode.clockwise_char_rotation = 0; break; //clockwise rotation mode off
            case 1: case 49: device.std_mode.clockwise_char_rotation = 1; break; //clockwise rotation mode on (1 dot)
            case 2: case 50: device.std_mode.clockwise_char_rotation = 2; break; //clockwise rotation mode on (1.5 dot)
            default:                                                      break;
        }
    }
    return 0;
}

// Command: Set print area in page mode
int8_t _ESC_UPR_W(RxBuffer* b) {
    // TODO:
    uint8_t xL = (uint8_t)b->getNext();
    uint8_t xH = (uint8_t)b->getNext();
    uint8_t yL = (uint8_t)b->getNext();
    uint8_t yH = (uint8_t)b->getNext();
    uint8_t dxL = (uint8_t)b->getNext();
    uint8_t dxH = (uint8_t)b->getNext();
    uint8_t dyL = (uint8_t)b->getNext();
    uint8_t dyH = (uint8_t)b->getNext();
    printf("<ESC W>\n");

    //device.x      = (uint16_t)((xL + xH * 256) * horizontal_motion_unit);
    //device.y      = (uint16_t)((yL + yH * 256) * vertical_motion_unit);
    //device.width  = (uint16_t)((dxL + dxH * 256) * horizontal_motion_unit);
    //device.height = (uint16_t)((dyL + dyH * 256) * vertical_motion_unit);   
    return 0;
}

// Command: Set relative print position
int8_t _ESC_BACKSLASH_SYM(RxBuffer* b) {
    uint8_t nL = (uint8_t)b->getNext();
    uint8_t nH = (uint8_t)b->getNext();
    printf("<ESC \\>\n");
    //device.r_ppos = (uint16_t)(nL + nH * 256);
    return 0;
}

// Command: Select justification
int8_t _ESC_LWR_a(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC a> n=0x%.2X\n", n);
    if(device.mode == 0) {
        switch (n) {
            case 0: case 48: device.std_mode.justification = -1; break; //left
            case 1: case 49: device.std_mode.justification = 0;  break; //center
            case 2: case 50: device.std_mode.justification = 1;  break; //right
            default:                                             break;
        }
    }
    return 0;
}

// Command: Select paper sensor to output paper-end signals
int8_t _ESC_LWR_c_THREE(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC c 3>\n");
    device.RP_NE_A = (n & 0b00000001) ? 1 : 0;
    device.RP_NE_B = (n & 0b00000010) ? 1 : 0;
    device.RP_E_A  = (n & 0b00000100) ? 1 : 0;
    device.RP_E_B  = (n & 0b00001000) ? 1 : 0;
    return 0;
}

// Command: Select paper sensor to stop printing
int8_t _ESC_LWR_c_FOUR(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    device.RP_NE_A = (n & 0b00000001) ? 2 : 0;
    device.RP_NE_B = (n & 0b00000010) ? 2 : 0;
    device.RP_E_A  = (n & 0b00000100) ? 2 : 0;
    device.RP_E_B  = (n & 0b00001000) ? 2 : 0;
    return 0;
}

// Command: Enable / disable panel buttons
int8_t _ESC_LWR_c_FIVE(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC c 5>\n");
    device.panel_btn = (n > 0) ? 1 : 0;
    return 0;
}

// Command: Print and feed n lines
int8_t _ESC_LWR_d(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC d> n=0x%.2X\n", n);
    return 0;
}

// Command: Generate pulse
int8_t _ESC_LWR_p(RxBuffer* b) {
    uint8_t m = (uint8_t)b->getNext();
    uint8_t t1 = (uint8_t)b->getNext();
    uint8_t t2 = (uint8_t)b->getNext();
    printf("<ESC p> m=0x%.2X, t1=0x%.2X, t2=0x%.2X\n", m, t1, t2);
    return 0;
}

// Command: Select character code table
int8_t _ESC_LWR_t(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC t>\n");
    device.char_code_table = ((n > 0) && (n < 20)) ? n : 0; //ignores page 254 & 255
    return 0;
}

// Command: Turn upside-down print mode on/off
int8_t _ESC_LEFT_QBRACKET_SYM(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC {>\n");
    if (device.mode == 0) {
        device.std_mode.upside_down = (n > 0) ? 1 : 0;
    }
    return 0;
}

// Command: Select print mode(s) for Kanji characters
int8_t _FS_EXCLAMATION_SYM(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<FS !> n=0x%.2X\n", n);
    return 0;
}

// Command: Select Kanji character mode
int8_t _FS_AND_SYM(RxBuffer* b) {
    printf("<FS &>\n");
    return 0;
}

// Command: Define character effects of Kanji characters
int8_t _FS_LEFT_PERNTH_UPR_A(RxBuffer* b) {
    uint8_t pL = (uint8_t)b->getNext();
    uint8_t pH = (uint8_t)b->getNext();
    int l = (pL + pH * 256);

    if (l == 2) {
        uint8_t fn = (uint8_t)b->getNext();
        uint8_t m = (uint8_t)b->getNext();
        printf("<FS ( A> pL=0x%.2X, pH=0x%.2X, fn=0x%.2X, m=0x%.2X\n", pL, pH, fn, m);

        switch (m) {
            case 0: case 48: printf("-Select Kanji character Font A\n"); break;
            case 1: case 49: printf("-Select Kanji character Font B\n"); break;
            case 2: case 50: printf("-Select Kanji character Font C\n"); break;
            default:                                                     break;
        }
        return 0;
    }
    return -1;
}

// Command: Turn underline mode on/off for Kanji characters
int8_t _FS_SUBTRACT_SYM(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<FS -> n=0x%.2X\n", n);

    switch (n) {
        case 0: case 48: printf("-Turns off Kanji-underline mode\n");               break;
        case 1: case 49: printf("-Turns on Kanji-underline mode (1-dot thick)\n");  break;
        case 2: case 50: printf("-Turns on Kanji-underline mode (2-dots thick)\n"); break;
        default:                                                                    break;
    }
    return 0;
}

// Command: Cancel Kanji character mode
int8_t _FS_PERIOD_SYM(RxBuffer* b) {
    printf("<FS .>\n");
    printf("-Cancel Kanji character mode\n");
    return 0;
}

// Command: Define user-defined Kanji characters
int8_t _FS_ASCII_TWO(RxBuffer* b) {
    // TODO:
    printf("<FS 2>\n");
    return 0;
}

// Command: Select Kanji character code system
int8_t _FS_UPR_C(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<FS C> n=0x%.2X\n", n);

    switch (n) {
        case 0: case 48: printf("-JIS Code\n");        break;
        case 1: case 49: printf("-SHIFT JIS Code\n");  break;
        default:                                       break;
    }
    return 0;
}

// Command: Set Kanji character spacing
int8_t _FS_UPR_S(RxBuffer* b) {
    uint8_t n1 = (uint8_t)b->getNext();
    uint8_t n2 = (uint8_t)b->getNext();
    printf("<FS S> n1=0x%.2X, n2=0x%.2X\n", n1, n2);
    return 0;
}

// Command: Turn quadruple-size mode on/off for Kanji characters
int8_t _FS_UPR_W(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<FS W> n=0x%.2X\n", n);
    return 0;
}

// Command: Print NV bit image
int8_t _FS_LWR_p(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    uint8_t m = (uint8_t)b->getNext();
    printf("<FS p> n=0x%.2X, m=0x%.2X\n", n, m);
    return 0;
}

// Command: Select character size
int8_t _GS_EXCLAMATION_SYM(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS !>\n");
    device.style_size[0] = ((n >> 4) & 0x0F) + 1; //This multiplied by the normal font size
    device.style_size[1] = (n & 0x0F) + 1;        //This multiplied by the normal font size
    return 0;
}

// Command: Set absolute vertical print position in page mode
int8_t _GS_DOLLAR_SYM(RxBuffer* b) {
    uint8_t nL = (uint8_t)b->getNext();
    uint8_t nH = (uint8_t)b->getNext();
    printf("<GS $>\n");
    //device.abs_v_ppos = (uint16_t)(nL + nH * 256);
    return 0;
}

// Command: Execute test print
int8_t _GS_LEFT_PERNTH_UPR_A(RxBuffer* b) {
    uint8_t pL = (uint8_t)b->getNext();
    uint8_t pH = (uint8_t)b->getNext();
    int l = (pL + pH * 256);

    if (l == 2) {
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
    return -1;
}

// Command: Edit NV user memory
int8_t _GS_LEFT_PERNTH_UPR_C(RxBuffer* b) {
    // TODO:
    printf("<GS ( C>\n");
    return 0;
}

// Command: Enable / disable real-time command
int8_t _GS_LEFT_PERNTH_UPR_D(RxBuffer* b) {
    // TODO:
    printf("<GS ( D>\n");
    return 0;
}

// Command: Set user commands
int8_t _GS_LEFT_PERNTH_UPR_E(RxBuffer* b) {
    // TODO:
    printf("<GS ( E>\n");
    return 0;
}

// Command: Request transmission of response or status
int8_t _GS_LEFT_PERNTH_UPR_H(RxBuffer* b) {
    // TODO:
    printf("<GS ( H>\n");
    return 0;
}

// Command: Select print control method(s)
int8_t _GS_LEFT_PERNTH_UPR_K(RxBuffer* b) {
    // TODO:
    printf("<GS ( K>\n");
    return 0;
}

// Command: Set graphics data
int8_t _GS_LEFT_PERNTH_UPR_L(RxBuffer* b) {
    uint8_t args[4] = {
        (uint8_t)b->getNext(), //pL
        (uint8_t)b->getNext(), //pH
        (uint8_t) 0x00,
        (uint8_t) 0x00
    };

    int l = (args[0] + args[1] * 256 + args[2] * 65536 + args[3] * 16777216);

    if (l >= 2) {
        uint8_t m = (uint8_t)b->getNext();  //m
        uint8_t fn = (uint8_t)b->getNext(); //fn

        printf("<GS ( L> pL=0x%.2X, pH=0x%.2X, m=0x%.2X, fn=0x%.2X\n",
            args[0], args[1], m, fn);

        int index = -1;

        if ((fn >= 0) && (fn <= 4)) {           //0-4
            index = fn;
        }
        else if ((fn >= 48) && (fn <= 52)) {    //0-4
            index = fn - 48;
        }
        else if ((fn >= 64) && (fn <= 69)) {    //5-10
            index = (fn - 64) + 5;
        }
        else if ((fn >= 80) && (fn <= 85)) {    //11-16
            index = (fn - 80) + 11;
        }
        else if ((fn >= 112) && (fn <= 113)) {  //17-18
            index = (fn - 112) + 17;
        }

        if (index != -1)
            return gs_function[index].ptr(b, l - 2);
    }
    return -1;
}

// Command: Customize printer control value(s)
int8_t _GS_LEFT_PERNTH_UPR_M(RxBuffer* b) {
    // TODO:
    printf("<GS ( M>\n");
    return 0;
}

// Command: Select character effects
int8_t _GS_LEFT_PERNTH_UPR_N(RxBuffer* b) {
    // TODO:
    printf("<GS ( N>\n");
    return 0;
}

// Command: Set up and print the symbol
int8_t _GS_LEFT_PERNTH_LWR_k(RxBuffer* b) {
    uint8_t pL = (uint8_t)b->getNext();
    uint8_t pH = (uint8_t)b->getNext();
    int k = (pL + pH * 256);

    if (k > 2) {
        uint8_t cn = (uint8_t)b->getNext();
        uint8_t fn = (uint8_t)b->getNext();
        
        printf("<GS ( k> pL=0x%.2X, pH=0x%.2X, cn=0x%.2X, fn=0x%.2X, k:%d\n", pL, pH, cn, fn, k);

        if ((cn < 53) && (cn > 47)) {
            symbols* ptr = sym_ptr[cn - 48];    //index symbol type
            if ((fn < 83) && (fn > 64))
                return ptr[fn - 65].ptr(b, k - 2);  //index function code
            return -1;
        }
    }
    return -1;
}

// Command: Define downloaded bit image
int8_t _GS_STAR_SYM(RxBuffer* b) {
    // TODO:
    printf("<GS *>\n");
    return 0;
}

// Command: Print downloaded bit image
int8_t _ESC_FWRDSLASH_SYM(RxBuffer* b) {
    uint8_t m = (uint8_t)b->getNext();
    printf("<GS /> m=0x%.2X\n", m);
    return 0;
}

// Command: Set graphics data
int8_t _GS_EIGHT_UPR_L(RxBuffer* b) {
    int index = -1;
    uint8_t args[4] = {
        (uint8_t)b->getNext(), //p1
        (uint8_t)b->getNext(), //p2
        (uint8_t)b->getNext(), //p3
        (uint8_t)b->getNext()  //p4
    };

    int l = (args[0] + args[1] * 256 + args[2] * 65536 + args[3] * 16777216);

    if (l >= 3) {
        uint8_t m = (uint8_t)b->getNext();   //48, also assumed to be color mode byte? 
        uint8_t fn = (uint8_t)b->getNext();  //function mode
        uint8_t a = (uint8_t)b->getNext();

        printf("<GS 8 L>\n");
        printf("- p1=0x%.2X, p2=0x%.2X, p3=0x%.2X, p4=0x%.2X\n",
            args[0], args[1], args[2], args[3]);
        printf("- m=0x%.2X, fn=0x%.2X, a=0x%.2X\n", m, fn, a);

        if ((fn >= 67) && (fn <= 68)) {         //67-68
            index = (args[3] - 64) + 5;
        }
        else if ((fn >= 83) && (fn <= 84)) {    //83-84
            index = (fn - 80) + 11;
        }
        else if ((fn >= 112) && (fn <= 113)) {  //112-113
            index = (fn - 112) + 17;
        }

        if (index != -1)
            return gs_function[index].ptr(b, l - 3);
    }
    return -1;
}

// Command: Start/end macro definition 
int8_t _GS_COLON_SYM(RxBuffer* b) {
    // TODO:
    printf("<GS :>\n");
    return 0;
}

// Command: Turn white/black reverse print mode on/off
int8_t _GS_UPR_B(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS B>\n");
    device.reverse = (n > 0) ? 1 : 0;
    return 0;
}

// Command: Select print position of HRI characters
int8_t _GS_UPR_H(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS H>\n");
    device.general_barcode.HRI = (n != 2) ? 0 : 1; //HRI on/off, uses zint default config
    return 0;
}

// Command: Transmit printer ID
int8_t _GS_UPR_I(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS I> n=0x%.2X\n", n);
    return 0;
}

// Command: Set left margin
int8_t _GS_UPR_L(RxBuffer* b) {
    uint8_t nL = (uint8_t)b->getNext();
    uint8_t nH = (uint8_t)b->getNext();    
    printf("<GS L>\n");
    device.std_mode.left_margin = (uint16_t)((nL + nH * 256) * device.horizontal_motion_unit);
    return 0;
}

// Command: Set horizontal and vertical motion units
int8_t _GS_UPR_P(RxBuffer* b) {
    uint8_t x = (uint8_t)b->getNext();
    uint8_t y = (uint8_t)b->getNext();
    printf("<GS P>\n");
    device.horizontal_motion_unit = (float)(x / DPI_203);
    device.vertical_motion_unit   = (float)(y / DPI_203);

    /* ------------------------------------------------------------
        unit maths might be wrong, spec sheet says default is zero
            but the pos * 0, would always be 0.... so it should be 1

        if x = 203, 203/203 = 1
        if x = 406, 406/203 = 2
        if x = 102, 102/203 = 0.5    

        (I'll spare you my compounded rounding error rant...)
        (thank goodness this isn't banking software)
    ------------------------------------------------------------ */
    return 0;
}

// Command: Set print position to the beginning of print line
int8_t _GS_UPR_T(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS T> n=0x%.2X\n", n);
    return 0;
}

// Command: Select cut mode and cut paper
int8_t _GS_UPR_V(RxBuffer* b) {
    uint8_t m = (uint8_t)b->getNext();
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS V> m=0x%.2X, n=0x%.2X\n", m, n);
    return 0;
}

// Command: Set print area width
int8_t _GS_UPR_W(RxBuffer* b) {
    uint8_t nL = (uint8_t)b->getNext();
    uint8_t nH = (uint8_t)b->getNext();
    printf("<GS W>\n");
    device.std_mode.print_area_width = (uint16_t)((nL + nH * 256) * device.horizontal_motion_unit);
    return 0;
}

// Command: Set relative vertical print position in page mode
int8_t _GS_BACKSLASH_SYM(RxBuffer* b) {
    uint8_t nL = (uint8_t)b->getNext();
    uint8_t nH = (uint8_t)b->getNext();
    uint16_t vertical = (nL + nH * 256);
    printf("<GS \\> Set Relative Vertical Print Position: %d\n", vertical);
    return 0;
}

// Command: Execute macro
int8_t _GS_CARROT_SYM(RxBuffer* b) {
    uint8_t r = (uint8_t)b->getNext();
    uint8_t t = (uint8_t)b->getNext();
    uint8_t m = (uint8_t)b->getNext();
    printf("<GS ^> r=0x%.2X, t=0x%.2X, m=0x%.2X\n", r, t, m);
    return 0;
}

// Command: Enable /disable Automatic Status Back (ASB)
int8_t _GS_LWR_a(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS a> n=0x%.2X\n", n);
    return 0;
}

// Command: Turn smoothing mode on / off
int8_t _GS_LWR_b(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS b>\n");
    device.smoothing = (n > 0) ? 1 : 0;
    return 0;
}

// Command: Select font for HRI characters
int8_t _GS_LWR_f(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS f> n=0x%.2X\n", n);
    return 0;
}

// Command: Initialize maintenance counter
int8_t _GS_LWR_g_ZERO(RxBuffer* b) {
    uint8_t m = (uint8_t)b->getNext();
    uint8_t nL = (uint8_t)b->getNext();
    uint8_t nH = (uint8_t)b->getNext();
    uint16_t m_reg = (uint16_t)(nL + nH * 256);
    printf("<GS g 0> m=0x%.2X, counter reg:%d\n", m, m_reg);
    return 0;
}

// Command: Transmit maintenance counter
int8_t _GS_LWR_g_TWO(RxBuffer* b) {
    uint8_t m = (uint8_t)b->getNext();
    uint8_t nL = (uint8_t)b->getNext();
    uint8_t nH = (uint8_t)b->getNext();
    uint16_t m_reg = (uint16_t)(nL + nH * 256);
    printf("<GS g 2> m=0x%.2X, counter reg:%d\n", m, m_reg);
    return 0;
}

// Command: Set bar code height
int8_t _GS_LWR_h(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS h>\n");
    device.general_barcode.h = n;
    return 0;
}

// Command: Print bar code
int8_t _GS_LWR_k(RxBuffer* b) {
    uint8_t m = (uint8_t)b->getNext();
    printf("<GS k> m=0x%.2X (m=%d)\n", m, m);

    if ((m >= 0) && (m <= 6)) {
        return barcode_function_a[m].ptr(b);
    }
    else if ((m >= 65) && (m <= 78)) {
        uint8_t len = (uint8_t)b->getNext();
        return barcode_function_b[(m - 65)].ptr(b, len);
    }
    return -1;
}

// Command: Transmit status
int8_t _GS_LWR_r(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS r> n=0x%.2X\n", n);
    return 0;
}

// Command: Print raster bit image
int8_t _GS_LWR_v_ZERO(RxBuffer* b) {
    uint8_t m  = (uint8_t)b->getNext();
    uint8_t xL = (uint8_t)b->getNext();
    uint8_t xH = (uint8_t)b->getNext();
    uint8_t yL = (uint8_t)b->getNext();
    uint8_t yH = (uint8_t)b->getNext();
    int k = (xL + xH * 256) * (yL + yH * 256);
    
    printf("<GS v 0>\n");
    printf("Raster Bit Image: ");
    for (int i = 0; i < k; i++)
        printf("0x%.2X ", (uint8_t)b->getNext());
    printf("\n");
    return 0;
}

// Command: Set bar code width
int8_t _GS_LWR_w(RxBuffer* b) {
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS w>\n");
    device.general_barcode.w = n; //ignored for now, using zint default
    return 0;
}

// PDF417: Set the number of columns in the data region
int8_t SYM_function_065(RxBuffer* b, int s) {
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 65) n=0x%.2X\n", n);
        return 0;
    }
    return -1;
}

// PDF417: Set the number of rows
int8_t SYM_function_066(RxBuffer* b, int s) {
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 66) n=0x%.2X\n", n);
        return 0;
    }
    return -1;
}

// PDF417: Set the width of the module
int8_t SYM_function_067(RxBuffer* b, int s) {
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 67) n=0x%.2X\n", n);
        return 0;
    }
    return -1;
}

// PDF417: Set the row height
int8_t SYM_function_068(RxBuffer* b, int s) {
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 68) n=0x%.2X\n", n);
        return 0;
    }
    return -1;
}

// PDF417: Set the error correction level
int8_t SYM_function_069(RxBuffer* b, int s) {
    if (s == 2) {
        uint8_t m = (uint8_t)b->getNext();
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 69) m=0x%.2X, n=0x%.2X\n", m, n);
        return 0;
    }
    return -1;
}

// PDF417: Select the options
int8_t SYM_function_070(RxBuffer* b, int s) {
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 70) n=0x%.2X\n", n);
        return 0;
    }
    return -1;
}

// PDF417: Store the data in the symbol storage area
int8_t SYM_function_080(RxBuffer* b, int s) {
    if (s >= 1) {
        printf("(Function 80)\n");
        /* trash m */
        b->getNext(); s--;
        for (int i = 0; i < s; i++)
            printf("0x%.2X ", (uint8_t)b->getNext());
        printf("\n");
        return 0;
    }
    return -1;
}

// PDF417: Print the symbol data in the symbol storage area
int8_t SYM_function_081(RxBuffer* b, int s) {
    if (s == 1) {
        uint8_t m = (uint8_t)b->getNext();
        printf("(function 81) m=0x%.2X\n", m);
        return 0;
    }
    return -1;
}

// PDF417: Transmit the size information of the symbol data in the symbol storage area
int8_t SYM_function_082(RxBuffer* b, int s) {
    if (s == 1) {
        uint8_t m = (uint8_t)b->getNext();
        printf("(function 82) m=0x%.2X\n", m);
        return -0;
    }
    return -1;
}

// QR Code: Select the model
int8_t SYM_function_165(RxBuffer* b, int s) {
    if (s == 2) {
        uint8_t n1 = (uint8_t)b->getNext();
        uint8_t n2 = (uint8_t)b->getNext();
        printf("(function 165) n1=0x%.2X, n2=0x%.2X\n", n1, n2);
        return 0;
    }
    return -1;
}

// QR Code : Set the size of module
int8_t SYM_function_167(RxBuffer* b, int s) {
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 167) n=0x%.2X\n", n);
        return 0;
    }
    return -1;
}

// QR Code: Select the error correction level
int8_t SYM_function_169(RxBuffer* b, int s) {
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 169) n=0x%.2X\n", n);
        return 0;
    }
    return -1;
}

// QR Code: Store the data in the symbol storage area
int8_t SYM_function_180(RxBuffer* b, int s) {
    if (s >= 1) {
        printf("(Function 180)\n");
        /* trash m */
        b->getNext(); s--;
        for (int i = 0; i < s; i++)
            printf("0x%.2X ", (uint8_t)b->getNext());
        printf("\n");
        return 0;
    }
    return -1;
}

// QR Code: Print the symbol data in the symbol storage area
int8_t SYM_function_181(RxBuffer* b, int s) {
    if (s == 1) {
        uint8_t m = (uint8_t)b->getNext();
        printf("(function 181) m=0x%.2X\n", m);
        return 0;
    }
    return -1;
}

// QR Code: Transmit the size information of the symbol data in the symbol storage area
int8_t SYM_function_182(RxBuffer* b, int s) {
    if (s == 1) {
        uint8_t m = (uint8_t)b->getNext();
        printf("(function 182) m=0x%.2X\n", m);
        return 0;
    }
    return -1;
}

// MaxiCode: Select the mode
int8_t SYM_function_265(RxBuffer* b, int s) {
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 265) n=0x%.2X\n", n);
        return 0;
    }
    return -1;
}

// MaxiCode: Store the data in the symbol storage area
int8_t SYM_function_280(RxBuffer* b, int s) {
    if (s >= 1) {
        printf("(Function 280)\n");
        /* trash m */
        b->getNext(); s--;
        for (int i = 0; i < s; i++)
            printf("0x%.2X ", (uint8_t)b->getNext());
        printf("\n");
        return 0;
    }
    return -1;
}

// MaxiCode: Print the symbol data in the symbol storage area
int8_t SYM_function_281(RxBuffer* b, int s) {
    if (s == 1) {
        uint8_t m = (uint8_t)b->getNext();
        printf("(function 281) m=0x%.2X\n", m);
        return 0;
    }
    return -1;
}

// MaxiCode: Transmit the size information of the symbol data in the symbol storage area
int8_t SYM_function_282(RxBuffer* b, int s) {
    if (s == 1) {
        uint8_t m = (uint8_t)b->getNext();
        printf("(function 282) m=0x%.2X\n", m);
        return 0;
    }
    return -1;
}

// RSS: Set the width of the module
int8_t SYM_function_367(RxBuffer* b, int s) {
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 367) n=0x%.2X\n", n);
        return 0;
    }
    return -1;
}

// 2-dimensional RSS: RSS Expanded Stacked maximum width setting
int8_t SYM_function_371(RxBuffer* b, int s) {
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 371) n=0x%.2X\n", n);
        return 0;
    }
    return -1;
}

// 2-dimensional RSS: Store the data in the symbol storage area
int8_t SYM_function_380(RxBuffer* b, int s) {
    if (s >= 2) {
        printf("(Function 380)\n");
        /* trash m */
        b->getNext(); s--;
        /* get n */
        uint8_t n = (uint8_t)b->getNext(); s--;
        printf("n: 0x%.2X\n", n);
        for (int i = 0; i < s; i++)
            printf("0x%.2X ", (uint8_t)b->getNext());
        printf("\n");
        return 0;
    }
    return -1;
}

// 2-dimensional RSS: Print the symbol data in the symbol storage area
int8_t SYM_function_381(RxBuffer* b, int s) {
    if (s == 1) {
        uint8_t m = (uint8_t)b->getNext();
        printf("(function 381) m=0x%.2X\n", m);
        return 0;
    }
    return -1;
}

// 2-dimensional RSS: Transmit the size information of the symbol data in the symbol storage area
int8_t SYM_function_382(RxBuffer* b, int s) {
    if (s == 1) {
        uint8_t m = (uint8_t)b->getNext();
        printf("(function 382) m=0x%.2X\n", m);
        return 0;
    }
    return -1;
}

// Composite Symbol: Set the width of the module
int8_t SYM_function_467(RxBuffer* b, int s) {
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 467) n=0x%.2X\n", n);
        return 0;
    }
    return -1;
}

// Composite Symbol: RSS Expanded Stacked maximum width setting
int8_t SYM_function_471(RxBuffer* b, int s) {
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 471) n=0x%.2X\n", n);
        return 0;
    }
    return -1;
}

// Composite Symbol: Select font HRI characters
int8_t SYM_function_472(RxBuffer* b, int s) {
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 472) n=0x%.2X\n", n);
        return 0;
    }
    return -1;
}

// Composite Symbol: Store the data in the symbol storage area
int8_t SYM_function_480(RxBuffer* b, int s) {
    if (s >= 3) {
        printf("(Function 80)\n");
        /* trash m */
        b->getNext(); s--;
        /* get a b */
        uint8_t A = (uint8_t)b->getNext(); s--;
        uint8_t B = (uint8_t)b->getNext(); s--;
        printf("A: 0x%.2X, B: 0x%.2X\n", A, B);
        for (int i = 0; i < s; i++)
            printf("0x%.2X ", (uint8_t)b->getNext());
        printf("\n");
        return 0;
    }
    return -1;
}

// Composite Symbol: Print the symbol data in the symbol storage area
int8_t SYM_function_481(RxBuffer* b, int s) {
    if (s == 1) {
        uint8_t m = (uint8_t)b->getNext();
        printf("(function 481) m=0x%.2X\n", m);
        return 0;
    }
    return -1;
}

// Composite Symbol: Transmit the size information of the symbol data in the symbol storage area
int8_t SYM_function_482(RxBuffer* b, int s) {
    if (s == 1) {
        uint8_t m = (uint8_t)b->getNext();
        printf("(function 482) m=0x%.2X\n", m);
        return 0;
    }
    return -1;
}

// NULL function to fill up pointer array
int8_t SYM_NULL(RxBuffer* b, int s) {
    printf("<NOT LOLZ, BUT NULLZ!>\n");
    return -1;
}

// Transmit the NV graphics memory capacity
int8_t GS_function_48(RxBuffer* b, int s) {
    // TODO:
    printf("(Function 480)\n");
    return 0;
}

// Set the reference dot density for graphics
int8_t GS_function_49(RxBuffer* b, int s) {
    if (s == 2) {
        uint8_t x = (uint8_t)b->getNext();
        uint8_t y = (uint8_t)b->getNext();
        printf("(Function 49) x=0x%.2X, y=0x%.2X\n", x, y);
        return 0;
    }
    return -1;
}

// Print the graphics data in the print buffer
int8_t GS_function_50(RxBuffer* b, int s) {
    // TODO:
    printf("(Function 50)\n");
    return 0;
}

// Transmit the remaining capacity of the NV graphics memory
int8_t GS_function_51(RxBuffer* b, int s) {
    // TODO:
    printf("(Function 51)\n");
    return 0;
}

// Transmit the remaining capacity of the download graphics memory
int8_t GS_function_52(RxBuffer* b, int s) {
    // TODO:
    printf("(Function 52)\n");
    return 0;
}

// Transmit the key code list for defined NV graphics
int8_t GS_function_64(RxBuffer* b, int s) {
    if (s == 2) {
        uint8_t d1 = (uint8_t)b->getNext();
        uint8_t d2 = (uint8_t)b->getNext();
        printf("(Function 64) d1=0x%.2X, d2=0x%.2X\n", d1, d2);
        return 0;
    }
    return -1;
}

// Delete all NV graphics data
int8_t GS_function_65(RxBuffer* b, int s) {
    if (s == 3) {
        uint8_t d1 = (uint8_t)b->getNext();
        uint8_t d2 = (uint8_t)b->getNext();
        uint8_t d3 = (uint8_t)b->getNext();
        printf("(Function 65) d1=0x%.2X, d2=0x%.2X, d3=0x%.2X\n", d1, d2, d3);
        return 0;
    }
    return -1;
}

// Delete the specified NV graphics data
int8_t GS_function_66(RxBuffer* b, int s) {
    if (s == 2) {
        uint8_t kc1 = (uint8_t)b->getNext();
        uint8_t kc2 = (uint8_t)b->getNext();
        printf("(Function 66) kc1=0x%.2X, kc2=0x%.2X\n", kc1, kc2);
        return 0;
    }
    return -1;
}

// Define the NV graphics data (raster format)
int8_t GS_function_67(RxBuffer* b, int s) {
    // TODO:
    printf("(Function 67)\n");
    return 0;
}

// Define the NV graphics data(column format)
int8_t GS_function_68(RxBuffer* b, int s) {
    // TODO:
    printf("(Function 69)\n");
    return 0;
}

// Print the specified NV graphics data
int8_t GS_function_69(RxBuffer* b, int s) {
    if (s == 4) {
        uint8_t kc1 = (uint8_t)b->getNext();
        uint8_t kc2 = (uint8_t)b->getNext();
        uint8_t x = (uint8_t)b->getNext();
        uint8_t y = (uint8_t)b->getNext();
        printf("(Function 69) kc1=0x%.2X, kc2=0x%.2X, x=0x%.2X, y=0x%.2X\n", kc1, kc2, x, y);
        return 0;
    }
    return -1;
}

// Transmit the key code list for defined download graphics
int8_t GS_function_80(RxBuffer* b, int s) {
    if (s == 2) {
        uint8_t d1 = (uint8_t)b->getNext();
        uint8_t d2 = (uint8_t)b->getNext();
        printf("(Function 80) d1=0x%.2X, d2=0x%.2X\n", d1, d2);
        return 0;
    }
    return -1;
}

// Delete all NV graphics data
int8_t GS_function_81(RxBuffer* b, int s) {
    if (s == 3) {
        uint8_t d1 = (uint8_t)b->getNext();
        uint8_t d2 = (uint8_t)b->getNext();
        uint8_t d3 = (uint8_t)b->getNext();
        printf("(Function 81) d1=0x%.2X, d2=0x%.2X, d3=0x%.2X\n", d1, d2, d3);
        return 0;
    }
    return -1;
}

// Delete the specified download graphics data
int8_t GS_function_82(RxBuffer* b, int s) {
    if (s == 2) {
        uint8_t kc1 = (uint8_t)b->getNext();
        uint8_t kc2 = (uint8_t)b->getNext();
        printf("(Function 82) kc1=0x%.2X, kc2=0x%.2X\n", kc1, kc2);
        return 0;
    }
    return -1;
}

// Define the downloaded graphics data (raster format)
int8_t GS_function_83(RxBuffer* b, int s) {
    // TODO:
    printf("(Function 83)\n");
    return 0;
}

// Define the downloaded graphics data
int8_t GS_function_84(RxBuffer* b, int s) {
    // TODO:
    printf("(Function 84)\n");
    return 0;
}

// Print the specified download graphics data
int8_t GS_function_85(RxBuffer* b, int s) {
    // TODO:
    printf("(Function 85)\n");
    return 0;
}

// Helper function for debugging raster format
static inline int8_t ascii_raster_dump(uint8_t* bmp, int s, int x, int y) {
    uint8_t l; int fancy_mod = (x + 7) / 8;
    for (int i = 0; i < s; i++) {
        if (!(i % fancy_mod)) printf("\n");
        for(int j = 0; j < 8; j++) {
            l = (bmp[i] & (0x80 >> j)) ? 219 : 32;
            printf("%c%c", l, l);
        }
    } printf("\n");
    return 0;
}

// Store the graphics data in the print buffer (raster format)
int8_t GS_function_112(RxBuffer* b, int s) {
    if (s >= 7) {
        uint8_t bx = (uint8_t)b->getNext();
        uint8_t by = (uint8_t)b->getNext();
        uint8_t c = (uint8_t)b->getNext();
        uint8_t xl = (uint8_t)b->getNext();
        uint8_t xh = (uint8_t)b->getNext();
        uint8_t yl = (uint8_t)b->getNext();
        uint8_t yh = (uint8_t)b->getNext();

        int X = (xl + xh * 256);
        int Y = (yl + yh * 256);
        int k = ((X + 7) / 8) * Y;

        //------------------------------------------------------------
        if ((s -= 7) == k) {
            if (v_create(k) != -1) {
                vmem* ptr = v_get_ptr();
                for (int i = 0; i < k; i++)
                    ptr->v_memory[i] = (uint8_t)b->getNext();
                v_dump(X, Y, "raster.xbm");
                v_destroy();
                return 0;
            } 
            //----------------------------------------------------------------
            //uint8_t* raster_data = (uint8_t*)malloc(k * sizeof(uint8_t));
            //if (raster_data != NULL) {
            //    int fancy_mod = (X + 7) / 8;
            //    for (int i = 0; i < k; i++) {
            //        raster_data[i] = (uint8_t)b->getNext();
            //        if (!(i % fancy_mod)) printf("\n");
            //        printf("0x%.2X ", raster_data[i]);
            //    } printf("\n");
            //    ascii_raster_dump(raster_data, k, X, Y);
            //    free(raster_data);
            //    return 0;
            //}
            //----------------------------------------------------------------
            return - 1;
        }
        return -1;
    }
    return -1;
}

// Store the graphics data in the print buffer (column format)
int8_t GS_function_113(RxBuffer* b, int s) {
    // TODO:
    printf("(Function 113)\n");
    return 0;
}

// In-line Bar-code Type A Helper function (NULL TERM)
static int8_t inline bcodeA_helper(RxBuffer* b) {
    int i = 0;
    while (b->peekNext() != NULL)
        printf("[%d] 0x%.2X ", i++, (uint8_t)b->getNext());
    printf("\n");
    return 0;
}

// In-line Bar-code Type B Helper function (SIZE = s)
static int8_t inline bcodeB_helper(RxBuffer* b, int s) {
    for (int i = 0; i < s; i++)
        printf("[%d] 0x%.2X ", i, (uint8_t)b->getNext());
    printf("\n");
    return 0;
}

// Helper function for preliminary debugging
static int8_t inline ascii_barcode_dump(zint_symbol* bmp) {
    uint8_t l;
    int row, col, i = 0;
    for (row = 0; row < bmp->bitmap_height; row++) {
        for (col = 0; col < bmp->bitmap_width; col++) {
            l = (bmp->bitmap[i] == 0xFF) ? 32 : 219;
            i += 3; // I'm Just looking at one of the RGB values
        } printf("\n");
    } printf("\n");
    return 0;
}

// Generic helper
static inline int8_t barcodeA_parse_gen_dump(RxBuffer* b, int symbology) {
    int s = 0;
    uint8_t c = 0x00;
    uint8_t* data = NULL;

    while (b->peekNext() != NULL) {
        c = (uint8_t)b->getNext();
        uint8_t* _d = (uint8_t*)realloc(data, sizeof(uint8_t) * (s + 1));
        if (_d != NULL) {
            data = _d; data[s++] = c;
        } else {
            if (data != NULL) free(data);
            return -1;
        }
    } c = (uint8_t)b->getNext(); //trash NULL char

    if (data != NULL) {
        struct zint_symbol* my_symbol;
        my_symbol = ZBarcode_Create();
        my_symbol->symbology = symbology;
        my_symbol->show_hrt = device.general_barcode.HRI;
        my_symbol->height = device.general_barcode.h;
        int err = ZBarcode_Encode_and_Buffer(my_symbol, data, s, 0);

        if (err != 0) {
            printf("%s\n", my_symbol->errtxt);
        } else {
            ascii_barcode_dump(my_symbol);
        }

        ZBarcode_Delete(my_symbol);
        free(data);
        return 0;
    }
    return -1;
}

// Generic helper (will be removed once I figure out what I'm doing)
static inline int8_t barcodeB_parse_gen_dump(RxBuffer* b, int s, int symbology) {
    uint8_t* data = (uint8_t*)malloc(s * sizeof(uint8_t));

    if (data != NULL) {
        for (int i = 0; i < s; i++)
            data[i] = (uint8_t)b->getNext();

        struct zint_symbol* my_symbol;
        my_symbol = ZBarcode_Create();
        my_symbol->symbology = symbology;
        my_symbol->show_hrt = device.general_barcode.HRI;
        my_symbol->height = device.general_barcode.h;
        
        int err = ZBarcode_Encode_and_Buffer(my_symbol, data, s, 0);

        /* --- more bread crumbs ---------------------------------------
            I can use my_symbol->text to get the HRI
            then render it myself independent of the bar-code rasterizer
            at a [position / font] defined by the escpos stream (over/under/both).
                                    IE per the technical specification.
        ----------------------------------------------------------------- */

        if (err != 0) {
            printf("%s\n", my_symbol->errtxt);
        } else {
            ascii_barcode_dump(my_symbol);
        }

        ZBarcode_Delete(my_symbol);
        free(data);
        return 0;
    }
    return -1;
}

// UPC-A
int8_t BAR_00(RxBuffer* b) {
    printf("(UPC-A){A}\n");
    return barcodeA_parse_gen_dump(b, BARCODE_UPCA);
}

// UPC-E
int8_t BAR_01(RxBuffer* b) {
    printf("(UPC-E){A}\n");
    return barcodeA_parse_gen_dump(b, BARCODE_UPCE);
}

// JAN13 (EAN13)
int8_t BAR_02(RxBuffer* b) {
    printf("(JAN13|EAN13){A}\n");
    return barcodeA_parse_gen_dump(b, BARCODE_EANX); //?
}

// JAN8 (EAN8)
int8_t BAR_03(RxBuffer* b) { 
    printf("(JAN8|EAN8){A}\n");
    return barcodeA_parse_gen_dump(b, BARCODE_EANX); //?
}

// CODE39
int8_t BAR_04(RxBuffer* b) {
    printf("(CODE39){A}\n");
    return barcodeA_parse_gen_dump(b, BARCODE_CODE39);
}

// ITF (interleaved 2 of 5)
int8_t BAR_05(RxBuffer* b) {
    printf("(ITF){A}\n");
    return barcodeA_parse_gen_dump(b, BARCODE_C25INTER);
}

// CODABAR (NW-7)
int8_t BAR_06(RxBuffer* b) {
    printf("(CODABAR){A}\n");
    return barcodeA_parse_gen_dump(b, BARCODE_CODABAR);
}

// UPC-A
int8_t BAR_65(RxBuffer* b, int s) {
    printf("(UPC-A)\n");
    return barcodeB_parse_gen_dump(b, s, BARCODE_UPCA);
}

// UPC-E
int8_t BAR_66(RxBuffer* b, int s) {
    printf("(UPC-E)\n");
    return barcodeB_parse_gen_dump(b, s, BARCODE_UPCE);
}

// JAN13 (EAN13)
int8_t BAR_67(RxBuffer* b, int s) {
    printf("(JAN13|EAN13)\n");
    return barcodeB_parse_gen_dump(b, s, BARCODE_EANX); //?
}

// JAN8 (EAN8)
int8_t BAR_68(RxBuffer* b, int s) {
    printf("(JAN8|EAN8)\n");
    return barcodeB_parse_gen_dump(b, s, BARCODE_EANX); //?
}

// CODE39
int8_t BAR_69(RxBuffer* b, int s) {
    printf("(CODE39)\n");
    return barcodeB_parse_gen_dump(b, s, BARCODE_CODE39);
}

// ITF (interleaved 2 of 5)
int8_t BAR_70(RxBuffer* b, int s) {
    printf("(ITF)\n");
    return barcodeB_parse_gen_dump(b, s, BARCODE_C25INTER);
}

// CODABAR (NW-7)
int8_t BAR_71(RxBuffer* b, int s) {
    printf("(CODABAR)\n");
    return barcodeB_parse_gen_dump(b, s, BARCODE_CODABAR);
}

// CODE93
int8_t BAR_72(RxBuffer* b, int s) {
    printf("(CODE93)\n");
    return barcodeB_parse_gen_dump(b, s, BARCODE_CODE93);
}

// CODE128
int8_t BAR_73(RxBuffer* b, int s) {
    printf("(CODE128)\n");
    return barcodeB_parse_gen_dump(b, s, BARCODE_CODE128);
}

// UCC/EAN128
int8_t BAR_74(RxBuffer* b, int s) {
    printf("(UCC|EAN128)\n");
    return barcodeB_parse_gen_dump(b, s, BARCODE_EAN128_CC);
}

// RSS-14
int8_t BAR_75(RxBuffer* b, int s) {
    printf("(RSS-14)\n");
    return barcodeB_parse_gen_dump(b, s, BARCODE_RSS14);
}

// RSS-14 Truncated
int8_t BAR_76(RxBuffer* b, int s) {
    printf("(RSS-14 Truncated)\n");
    return barcodeB_parse_gen_dump(b, s, BARCODE_DBAR_OMN); //?
}

// RSS Limited
int8_t BAR_77(RxBuffer* b, int s) {
    printf("(RSS Limited)\n");
    return barcodeB_parse_gen_dump(b, s, BARCODE_DBAR_LTD); //?
}

// RSS Expanded
int8_t BAR_78(RxBuffer* b, int s) {
    printf("(RSS Expanded)\n");
    return barcodeB_parse_gen_dump(b, s, BARCODE_DBAR_EXP); //?
}

// YaY only... nth Lines!?