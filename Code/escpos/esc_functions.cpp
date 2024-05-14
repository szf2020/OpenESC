// MJM 2024

#include "esc_functions.h"

//----------------------------------------------------------------

static const int DPI_203 = (int)(8.0 * 25.4); //real is 203.2 dots/inch

static float vertical_motion_unit = 1;
static float horizontal_motion_unit = 1;
static uint16_t line_spacing = 30;

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
    printf("<DLE DC4>\n");
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
    uint16_t right_scs = (uint16_t)(n * horizontal_motion_unit);
    printf("<ESC SP> Right Side Char Spacing: %d\n", right_scs);
    return 0;
}

// Command: Select print modes(s)
int _ESC_EXCLAMATION_SYM(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC !> n=0x%.2X\n", n);

    uint8_t char_font      = (n & 0b00000001);
    uint8_t char_emph      = (n & 0b00001000);
    uint8_t char_double_h  = (n & 0b00010000);
    uint8_t char_double_w  = (n & 0b00100000);
    uint8_t char_underline = (n & 0b10000000);

    if (char_font)      printf("-Character font 2\n");
    else                printf("-Character font 1\n");
    if(char_emph)       printf("-Emphasized ON\n");
    else                printf("-Emphasized OFF\n");
    if (char_double_h)  printf("-Double height ON\n");
    else                printf("-Double height OFF\n");
    if (char_double_w)  printf("-Double width ON\n");
    else                printf("-Double width OFF\n");
    if (char_underline) printf("-Underline ON\n");
    else                printf("-Underline OFF\n");

    return 0;
}

// Command: Set absolute print position
int _ESC_DOLLAR_SYM(RxBuffer* b)
{
    uint8_t nL = (uint8_t)b->getNext();
    uint8_t nH = (uint8_t)b->getNext();
    uint16_t app = (uint16_t)((nL + nH * 256) * horizontal_motion_unit);
    printf("<ESC $> Absolute print position: %d\n", app);
    return 0;
}

// Command: Select / cancel user-defined character set
int _ESC_PERCENT_SYM(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC %%> n=0x%.2X\n", n);
    if (n) printf("-User-defined character set is selected\n");
    else   printf("-User-defined character set is canceled\n");
    return 0;
}

// Command: Define user-defined characters
int _ESC_AND_SYM(RxBuffer* b)
{
    // TODO:
    printf("<ESC &>\n");
    return 0;
}

// Command: Select bit-image mode
int _ESC_STAR_SYM(RxBuffer* b)
{
    // TODO:
    printf("<ESC *>\n");
    return 0;
}

// Command: Turn underline mode on/off
int _ESC_SUBTRACT_SYM(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC -> n=0x%.2X\n", n);

    switch (n)
    {
    case 0: case 48: printf("-Underline mode off\n");     break;
    case 1: case 49: printf("-Underline 1 dot thick\n");  break;
    case 2: case 50: printf("-Underline 2 dots thick\n"); break;
    default:                                              break;
    }

    return 0;
}

// Command: Select default line spacing
int _ESC_ASCII_TWO(RxBuffer* b)
{
    printf("<ESC 2>\n");
    printf("-Default line spacing set\n");
    line_spacing = 30;
    return 0;
}

// Command: Set line spacing
int _ESC_ASCII_THREE(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    line_spacing = (uint16_t)(n * vertical_motion_unit);
    printf("<ESC 3> line spacing: %d\n", line_spacing);
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
    printf("<ESC D>\n");
    return 0;
}

// Command: Turn emphasized mode on/off
int _ESC_UPR_E(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC E> n=0x%.2X\n", n);
    if (n) printf("-Emphasized mode ON\n");
    else   printf("-Emphasized mode OFF\n");
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

    switch (n)
    {
    case 0: case 48: printf("-Font A\n");        break;
    case 1: case 49: printf("-Font B\n");        break;
    case 2: case 50: printf("-Font C\n");        break;
    case 97:         printf("-Extended Font\n"); break;
    default:                                     break;
    }

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
        default:                                    break;
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
        (int)((xL + xH * 256) * horizontal_motion_unit),
        (int)((yL + yH * 256) * vertical_motion_unit),
        (int)((dxL + dxH * 256) * horizontal_motion_unit),
        (int)((dyL + dyH * 256) * vertical_motion_unit));

    // Horizontal logical origin = (xL + xH × 256)   × (horizontal motion unit) from absolute origin.
    // Vertical logical origin   = (yL + yH × 256)   × (vertical motion unit) from absolute origin.
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
    uint16_t rpp = (uint16_t)((nL + nH * 256) * horizontal_motion_unit);
    printf("<ESC \\> Relative print position: %d\n", rpp);
    // Moves the print position to 
    // (nL + nH × 256) × (horizontal or vertical motion unit) 
    // from the current position
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
        default:                                             break;
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
        default:                                                    break;
    }

    return 0;
}

// Command: Turn upside-down print mode on/off
int _ESC_LEFT_QBRACKET_SYM(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<ESC {> n=0x%.2X\n", n);
    if (n) printf("-upside-down mode ON\n");
    else   printf("-upside-down mode OFF\n");
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
        default:                                                                    break;
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
    printf("<FS 2>\n");
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
        default:                                       break;
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
    printf("-Character Width: x%d\n", ((n >> 4) & 0x0F) + 1);
    printf("-Character Height: x%d\n", (n & 0x0F) + 1);
    return 0;
}

// Command: Set absolute vertical print position in page mode
int _GS_DOLLAR_SYM(RxBuffer* b)
{
    uint8_t nL = (uint8_t)b->getNext();
    uint8_t nH = (uint8_t)b->getNext();
    uint16_t avpp = (uint16_t)((nL + nH * 256) * vertical_motion_unit);
    printf("<GS $> Absolute Vertical Print Position: %d\n", avpp);
    return 0;
}

// Command: Execute test print
int _GS_LEFT_PERNTH_UPR_A(RxBuffer* b)
{
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
    }

    return 0;
}

// Command: Edit NV user memory
int _GS_LEFT_PERNTH_UPR_C(RxBuffer* b)
{
    // TODO:
    printf("<GS ( C>\n");
    return 0;
}

// Command: Enable / disable real-time command
int _GS_LEFT_PERNTH_UPR_D(RxBuffer* b)
{
    // TODO:
    printf("<GS ( D>\n");
    return 0;
}

// Command: Set user commands
int _GS_LEFT_PERNTH_UPR_E(RxBuffer* b)
{
    // TODO:
    printf("<GS ( E>\n");
    return 0;
}

// Command: Request transmission of response or status
int _GS_LEFT_PERNTH_UPR_H(RxBuffer* b)
{
    // TODO:
    printf("<GS ( H>\n");
    return 0;
}

// Command: Select print control method(s)
int _GS_LEFT_PERNTH_UPR_K(RxBuffer* b)
{
    // TODO:
    printf("<GS ( K>\n");
    return 0;
}

// Command: Set graphics data
int _GS_LEFT_PERNTH_UPR_L(RxBuffer* b)
{
    int index = -1;
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
            gs_function[index].ptr(b, l - 2);
    }

    return 0;
}

// Command: Customize printer control value(s)
int _GS_LEFT_PERNTH_UPR_M(RxBuffer* b)
{
    // TODO:
    printf("<GS ( M>\n");
    return 0;
}

// Command: Select character effects
int _GS_LEFT_PERNTH_UPR_N(RxBuffer* b)
{
    // TODO:
    printf("<GS ( N>\n");
    return 0;
}

// Command: Set up and print the symbol
int _GS_LEFT_PERNTH_LWR_k(RxBuffer* b)
{
    uint8_t pL = (uint8_t)b->getNext();
    uint8_t pH = (uint8_t)b->getNext();
    int k = (pL + pH * 256);

    if (k > 2)
    {
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
int _GS_STAR_SYM(RxBuffer* b)
{
    // TODO:
    printf("<GS *>\n");
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
            gs_function[index].ptr(b, l - 3);
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
    if (n) printf("-Reverse print mode ON\n");
    else   printf("-Reverse print mode OFF\n");
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
    uint16_t left_m = (uint16_t)(nL + nH * 256);
    printf("<GS L> Left Margin: %d\n", left_m);
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
    uint16_t width = (nL + nH * 256);
    printf("<GS W> Print area Width: %d\n", width);
    return 0;
}

// Command: Set relative vertical print position in page mode
int _GS_BACKSLASH_SYM(RxBuffer* b)
{
    uint8_t nL = (uint8_t)b->getNext();
    uint8_t nH = (uint8_t)b->getNext();
    uint16_t vertical = (nL + nH * 256);
    printf("<GS \\> Set Relative Vertical Print Position: %d\n", vertical);
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
    uint16_t m_reg = (uint16_t)(nL + nH * 256);
    printf("<GS g 0> m=0x%.2X, counter reg:%d\n", m, m_reg);
    return 0;
}

// Command: Transmit maintenance counter
int _GS_LWR_g_TWO(RxBuffer* b)
{
    uint8_t m = (uint8_t)b->getNext();
    uint8_t nL = (uint8_t)b->getNext();
    uint8_t nH = (uint8_t)b->getNext();
    uint16_t m_reg = (uint16_t)(nL + nH * 256);
    printf("<GS g 2> m=0x%.2X, counter reg:%d\n", m, m_reg);
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

    if ((m >= 0) && (m <= 6)) {
        barcode_function_a[m].ptr(b);
    }
    else if ((m >= 65) && (m <= 78)) {
        uint8_t len = (uint8_t)b->getNext();
        barcode_function_b[(m - 65)].ptr(b, len);
    }

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
    printf("<GS v 0>\n");
    return 0;
}

// Command: Set bar code width
int _GS_LWR_w(RxBuffer* b)
{
    uint8_t n = (uint8_t)b->getNext();
    printf("<GS w> n=0x%.2X\n", n);
    return 0;
}

// PDF417: Set the number of columns in the data region
int SYM_function_065(RxBuffer* b, int s)
{
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 65) n=0x%.2X\n", n);
    }
    return 0;
}

// PDF417: Set the number of rows
int SYM_function_066(RxBuffer* b, int s)
{
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 66) n=0x%.2X\n", n);
    }
    return 0;
}

// PDF417: Set the width of the module
int SYM_function_067(RxBuffer* b, int s)
{
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 67) n=0x%.2X\n", n);
    }
    return 0;
}

// PDF417: Set the row height
int SYM_function_068(RxBuffer* b, int s)
{
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 68) n=0x%.2X\n", n);
    }
    return 0;
}

// PDF417: Set the error correction level
int SYM_function_069(RxBuffer* b, int s)
{
    if (s == 2) {
        uint8_t m = (uint8_t)b->getNext();
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 69) m=0x%.2X, n=0x%.2X\n", m, n);
    }
    return 0;
}

// PDF417: Select the options
int SYM_function_070(RxBuffer* b, int s)
{
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 70) n=0x%.2X\n", n);
    }
    return 0;
}

// PDF417: Store the data in the symbol storage area
int SYM_function_080(RxBuffer* b, int s)
{
    printf("(Function 80)\n");
    return 0;
}

// PDF417: Print the symbol data in the symbol storage area
int SYM_function_081(RxBuffer* b, int s)
{
    if (s == 1) {
        uint8_t m = (uint8_t)b->getNext();
        printf("(function 81) m=0x%.2X\n", m);
    }
    return 0;
}

// PDF417: Transmit the size information of the symbol data in the symbol storage area
int SYM_function_082(RxBuffer* b, int s)
{
    if (s == 1) {
        uint8_t m = (uint8_t)b->getNext();
        printf("(function 82) m=0x%.2X\n", m);
    }
    return 0;
}

// QR Code: Select the model
int SYM_function_165(RxBuffer* b, int s)
{
    if (s == 2) {
        uint8_t n1 = (uint8_t)b->getNext();
        uint8_t n2 = (uint8_t)b->getNext();
        printf("(function 165) n1=0x%.2X, n2=0x%.2X\n", n1, n2);
    }
    return 0;
}

// QR Code : Set the size of module
int SYM_function_167(RxBuffer* b, int s)
{
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 167) n=0x%.2X\n", n);
    }
    return 0;
}

// QR Code: Select the error correction level
int SYM_function_169(RxBuffer* b, int s)
{
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 169) n=0x%.2X\n", n);
    }
    return 0;
}

// QR Code: Store the data in the symbol storage area
int SYM_function_180(RxBuffer* b, int s)
{
    printf("(Function 180)\n");
    return 0;
}

// QR Code: Print the symbol data in the symbol storage area
int SYM_function_181(RxBuffer* b, int s)
{
    if (s == 1) {
        uint8_t m = (uint8_t)b->getNext();
        printf("(function 181) m=0x%.2X\n", m);
    }
    return 0;
}

// QR Code: Transmit the size information of the symbol data in the symbol storage area
int SYM_function_182(RxBuffer* b, int s)
{
    if (s == 1) {
        uint8_t m = (uint8_t)b->getNext();
        printf("(function 182) m=0x%.2X\n", m);
    }
    return 0;
}

// MaxiCode: Select the mode
int SYM_function_265(RxBuffer* b, int s)
{
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 265) n=0x%.2X\n", n);
    }
    return 0;
}

// MaxiCode: Store the data in the symbol storage area
int SYM_function_280(RxBuffer* b, int s)
{
    printf("(Function 280)\n");
    return 0;
}

// MaxiCode: Print the symbol data in the symbol storage area
int SYM_function_281(RxBuffer* b, int s)
{
    if (s == 1) {
        uint8_t m = (uint8_t)b->getNext();
        printf("(function 281) m=0x%.2X\n", m);
    }
    return 0;
}

// MaxiCode: Transmit the size information of the symbol data in the symbol storage area
int SYM_function_282(RxBuffer* b, int s)
{
    if (s == 1) {
        uint8_t m = (uint8_t)b->getNext();
        printf("(function 282) m=0x%.2X\n", m);
    }
    return 0;
}

// RSS: Set the width of the module
int SYM_function_367(RxBuffer* b, int s)
{
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 367) n=0x%.2X\n", n);
    }
    return 0;
}

// 2-dimensional RSS: RSS Expanded Stacked maximum width setting
int SYM_function_371(RxBuffer* b, int s)
{
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 371) n=0x%.2X\n", n);
    }
    return 0;
}

// 2-dimensional RSS: Store the data in the symbol storage area
int SYM_function_380(RxBuffer* b, int s)
{
    printf("(Function 380)\n");
    return 0;
}

// 2-dimensional RSS: Print the symbol data in the symbol storage area
int SYM_function_381(RxBuffer* b, int s)
{
    if (s == 1) {
        uint8_t m = (uint8_t)b->getNext();
        printf("(function 381) m=0x%.2X\n", m);
    }
    return 0;
}

// 2-dimensional RSS: Transmit the size information of the symbol data in the symbol storage area
int SYM_function_382(RxBuffer* b, int s)
{
    if (s == 1) {
        uint8_t m = (uint8_t)b->getNext();
        printf("(function 382) m=0x%.2X\n", m);
    }
    return 0;
}

// Composite Symbol: Set the width of the module
int SYM_function_467(RxBuffer* b, int s)
{
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 467) n=0x%.2X\n", n);
    }
    return 0;
}

// Composite Symbol: RSS Expanded Stacked maximum width setting
int SYM_function_471(RxBuffer* b, int s)
{
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 471) n=0x%.2X\n", n);
    }
    return 0;
}

// Composite Symbol: Select font HRI characters
int SYM_function_472(RxBuffer* b, int s)
{
    if (s == 1) {
        uint8_t n = (uint8_t)b->getNext();
        printf("(function 472) n=0x%.2X\n", n);
    }
    return 0;
}

// Composite Symbol: Store the data in the symbol storage area
int SYM_function_480(RxBuffer* b, int s)
{
    printf("(Function 480)\n");
    return 0;
}

// Composite Symbol: Print the symbol data in the symbol storage area
int SYM_function_481(RxBuffer* b, int s)
{
    if (s == 1) {
        uint8_t m = (uint8_t)b->getNext();
        printf("(function 481) m=0x%.2X\n", m);
    }
    return 0;
}

// Composite Symbol: Transmit the size information of the symbol data in the symbol storage area
int SYM_function_482(RxBuffer* b, int s)
{
    if (s == 1) {
        uint8_t m = (uint8_t)b->getNext();
        printf("(function 482) m=0x%.2X\n", m);
    }
    return 0;
}

// NULL function to fill up pointer array
int SYM_NULL(RxBuffer* b, int s)
{
    printf("<NOT LOLZ, BUT NULLZ!>\n");
    return -1;
}

// Transmit the NV graphics memory capacity
int GS_function_48(RxBuffer* b, int s)
{
    printf("(Function 480)\n");
    return 0;
}

// Set the reference dot density for graphics
int GS_function_49(RxBuffer* b, int s)
{
    if (s == 2) {
        uint8_t x = (uint8_t)b->getNext();
        uint8_t y = (uint8_t)b->getNext();
        printf("(Function 49) x=0x%.2X, y=0x%.2X\n", x, y);
    }
    return 0;
}

// Print the graphics data in the print buffer
int GS_function_50(RxBuffer* b, int s)
{
    printf("(Function 50) s=%d\n", s);
    return 0;
}

// Transmit the remaining capacity of the NV graphics memory
int GS_function_51(RxBuffer* b, int s)
{
    printf("(Function 51)\n");
    return 0;
}

// Transmit the remaining capacity of the download graphics memory
int GS_function_52(RxBuffer* b, int s)
{
    printf("(Function 52)\n");
    return 0;
}

// Transmit the key code list for defined NV graphics
int GS_function_64(RxBuffer* b, int s)
{
    if (s == 2) {
        uint8_t d1 = (uint8_t)b->getNext();
        uint8_t d2 = (uint8_t)b->getNext();
        printf("(Function 64) d1=0x%.2X, d2=0x%.2X\n", d1, d2);
    }
    return 0;
}

// Delete all NV graphics data
int GS_function_65(RxBuffer* b, int s)
{
    if (s == 3) {
        uint8_t d1 = (uint8_t)b->getNext();
        uint8_t d2 = (uint8_t)b->getNext();
        uint8_t d3 = (uint8_t)b->getNext();
        printf("(Function 65) d1=0x%.2X, d2=0x%.2X, d3=0x%.2X\n", d1, d2, d3);
    }
    return 0;
}

// Delete the specified NV graphics data
int GS_function_66(RxBuffer* b, int s)
{
    if (s == 2) {
        uint8_t kc1 = (uint8_t)b->getNext();
        uint8_t kc2 = (uint8_t)b->getNext();
        printf("(Function 66) kc1=0x%.2X, kc2=0x%.2X\n", kc1, kc2);
    }
    return 0;
}

// Define the NV graphics data (raster format)
int GS_function_67(RxBuffer* b, int s)
{
    printf("(Function 67)\n");
    return 0;
}

// Define the NV graphics data(column format)
int GS_function_68(RxBuffer* b, int s)
{
    printf("(Function 69)\n");
    return 0;
}

// Print the specified NV graphics data
int GS_function_69(RxBuffer* b, int s)
{
    if (s == 4) {
        uint8_t kc1 = (uint8_t)b->getNext();
        uint8_t kc2 = (uint8_t)b->getNext();
        uint8_t x = (uint8_t)b->getNext();
        uint8_t y = (uint8_t)b->getNext();
        printf("(Function 69) kc1=0x%.2X, kc2=0x%.2X, x=0x%.2X, y=0x%.2X\n", kc1, kc2, x, y);
    }
    return 0;
}

// Transmit the key code list for defined download graphics
int GS_function_80(RxBuffer* b, int s)
{
    if (s == 2) {
        uint8_t d1 = (uint8_t)b->getNext();
        uint8_t d2 = (uint8_t)b->getNext();
        printf("(Function 80) d1=0x%.2X, d2=0x%.2X\n", d1, d2);
    }
    return 0;
}

// Delete all NV graphics data
int GS_function_81(RxBuffer* b, int s)
{
    if (s == 3) {
        uint8_t d1 = (uint8_t)b->getNext();
        uint8_t d2 = (uint8_t)b->getNext();
        uint8_t d3 = (uint8_t)b->getNext();
        printf("(Function 81) d1=0x%.2X, d2=0x%.2X, d3=0x%.2X\n", d1, d2, d3);
    }
    return 0;
}

// Delete the specified download graphics data
int GS_function_82(RxBuffer* b, int s)
{
    if (s == 2) {
        uint8_t kc1 = (uint8_t)b->getNext();
        uint8_t kc2 = (uint8_t)b->getNext();
        printf("(Function 82) kc1=0x%.2X, kc2=0x%.2X\n", kc1, kc2);
    }
    return 0;
}

// Define the downloaded graphics data (raster format)
int GS_function_83(RxBuffer* b, int s)
{
    printf("(Function 83)\n");
    return 0;
}

// Define the downloaded graphics data
int GS_function_84(RxBuffer* b, int s)
{
    printf("(Function 84)\n");
    return 0;
}

// Print the specified download graphics data
int GS_function_85(RxBuffer* b, int s)
{
    printf("(Function 85)\n");
    return 0;
}

// Store the graphics data in the print buffer (raster format)
int GS_function_112(RxBuffer* b, int s)
{
    if (s >= 7)
    {
        uint8_t bx = (uint8_t)b->getNext();
        uint8_t by = (uint8_t)b->getNext();
        uint8_t c = (uint8_t)b->getNext();
        uint8_t xl = (uint8_t)b->getNext();
        uint8_t xh = (uint8_t)b->getNext();
        uint8_t yl = (uint8_t)b->getNext();
        uint8_t yh = (uint8_t)b->getNext();

        int k = (((xl + xh * 256) + 7) / 8) * (yl + yh * 256);

        printf("--[raster info]--\n");
        printf("-bx:%d, by:%d\n", bx, by);
        printf("-c:%d\n", c);
        printf("-xl:%d, xh:%d, yl:%d, yh:%d\n", xl, xh, yl, yh);
        printf("-k:%d\n", k);
        printf("-X: %d, Y: %d\n", (xl + xh * 256), (yl + yh * 256));
        printf("-sum:%d, new:%d\n", s, s - 7);

        //yBytes = (yl + 7) / 8; //total bytes in the y direction

        if ((s -= 7) == k) {
            printf("------ Raster Debug Dump ----------\n");
            for (int i = 0; i < k; i++) {
                uint8_t h = (uint8_t)b->getNext();
                printf("0x%.2X ", h);
            }
            printf("\n---------------------------------\n");
        }
    }

    return 0;
}

// Store the graphics data in the print buffer (column format)
int GS_function_113(RxBuffer* b, int s)
{
    printf("(Function 113)\n");
    return 0;
}

// UPC-A
int BAR_00(RxBuffer* b)
{
    int i = 0;
    printf("(UPC-A) data: ");
    while (b->peekNext() != NULL)
        printf("[%d] 0x%.2X ", i++, (uint8_t)b->getNext());
    printf("\n");
    return 0;
}

// UPC-E
int BAR_01(RxBuffer* b)
{
    int i = 0;
    printf("(UPC-E) data: ");
    while (b->peekNext() != NULL)
        printf("[%d] 0x%.2X ", i++, (uint8_t)b->getNext());
    printf("\n");
    return 0;
}

// JAN13 (EAN13)
int BAR_02(RxBuffer* b)
{
    int i = 0;
    printf("(JAN13|EAN13) data: ");
    while (b->peekNext() != NULL)
        printf("[%d] 0x%.2X ", i++, (uint8_t)b->getNext());
    printf("\n");
    return 0;
}

// JAN8 (EAN8)
int BAR_03(RxBuffer* b)
{
    int i = 0;
    printf("(JAN8|EAN8) data: ");
    while (b->peekNext() != NULL)
        printf("[%d] 0x%.2X ", i++, (uint8_t)b->getNext());
    printf("\n");
    return 0;
}

// CODE39
int BAR_04(RxBuffer* b)
{
    int i = 0;
    printf("(CODE39) data: ");
    while (b->peekNext() != NULL)
        printf("[%d] 0x%.2X ", i++, (uint8_t)b->getNext());
    printf("\n");
    return 0;
}

// ITF (interleaved 2 of 5)
int BAR_05(RxBuffer* b)
{
    int i = 0;
    printf("(ITF) data: ");
    while (b->peekNext() != NULL)
        printf("[%d] 0x%.2X ", i++, (uint8_t)b->getNext());
    printf("\n");
    return 0;
}

// CODABAR (NW-7)
int BAR_06(RxBuffer* b)
{
    int i = 0;
    printf("(CODABAR) data: ");
    while (b->peekNext() != NULL)
        printf("[%d] 0x%.2X ", i++, (uint8_t)b->getNext());
    printf("\n");
    return 0;
}

// UPC-A
int BAR_65(RxBuffer* b, int s)
{
    printf("(UPC-A) data: ");
    for(int i = 0; i < s; i++)
        printf("[%d] 0x%.2X ", i, (uint8_t)b->getNext());
    printf("\n");
    return 0;
}

// UPC-E
int BAR_66(RxBuffer* b, int s)
{
    printf("(UPC-E) data: ");
    for (int i = 0; i < s; i++)
        printf("[%d] 0x%.2X ", i, (uint8_t)b->getNext());
    printf("\n");
    return 0;
}

// JAN13 (EAN13)
int BAR_67(RxBuffer* b, int s)
{
    printf("(JAN13|EAN13) data: ");
    for (int i = 0; i < s; i++)
        printf("[%d] 0x%.2X ", i, (uint8_t)b->getNext());
    printf("\n");
    return 0;
}

// JAN8 (EAN8)
int BAR_68(RxBuffer* b, int s)
{
    printf("(JAN8|EAN8) data: ");
    for (int i = 0; i < s; i++)
        printf("[%d] 0x%.2X ", i, (uint8_t)b->getNext());
    printf("\n");
    return 0;
}

// CODE39
int BAR_69(RxBuffer* b, int s)
{
    printf("(CODE39) data: ");
    for (int i = 0; i < s; i++)
        printf("[%d] 0x%.2X ", i, (uint8_t)b->getNext());
    printf("\n");
    return 0;
}

// ITF (interleaved 2 of 5)
int BAR_70(RxBuffer* b, int s)
{
    printf("(ITF) data: ");
    for (int i = 0; i < s; i++)
        printf("[%d] 0x%.2X ", i, (uint8_t)b->getNext());
    printf("\n");
    return 0;
}

// CODABAR (NW-7)
int BAR_71(RxBuffer* b, int s)
{
    printf("(CODABAR) data: ");
    for (int i = 0; i < s; i++)
        printf("[%d] 0x%.2X ", i, (uint8_t)b->getNext());
    printf("\n");
    return 0;
}

// CODE93
int BAR_72(RxBuffer* b, int s)
{
    printf("(CODE93) data: ");
    for (int i = 0; i < s; i++)
        printf("[%d] 0x%.2X ", i, (uint8_t)b->getNext());
    printf("\n");
    return 0;
}

// CODE128
int BAR_73(RxBuffer* b, int s)
{
    printf("(CODE128) data: ");
    for (int i = 0; i < s; i++)
        printf("[%d] 0x%.2X ", i, (uint8_t)b->getNext());
    printf("\n");
    return 0;
}

// UCC/EAN128
int BAR_74(RxBuffer* b, int s)
{
    printf("(UCC|EAN128) data: ");
    for (int i = 0; i < s; i++)
        printf("[%d] 0x%.2X ", i, (uint8_t)b->getNext());
    printf("\n");
    return 0;
}

// RSS-14
int BAR_75(RxBuffer* b, int s)
{
    printf("(RSS-14) data: ");
    for (int i = 0; i < s; i++)
        printf("[%d] 0x%.2X ", i, (uint8_t)b->getNext());
    printf("\n");
    return 0;
}

// RSS-14 Truncated
int BAR_76(RxBuffer* b, int s)
{
    printf("(RSS-14 Truncated) data: ");
    for (int i = 0; i < s; i++)
        printf("[%d] 0x%.2X ", i, (uint8_t)b->getNext());
    printf("\n");
    return 0;
}

// RSS Limited
int BAR_77(RxBuffer* b, int s)
{
    printf("(RSS Limited) data: ");
    for (int i = 0; i < s; i++)
        printf("[%d] 0x%.2X ", i, (uint8_t)b->getNext());
    printf("\n");
    return 0;
}

// RSS Expanded
int BAR_78(RxBuffer* b, int s)
{
    printf("(RSS Expanded) data: ");
    for (int i = 0; i < s; i++)
        printf("[%d] 0x%.2X ", i, (uint8_t)b->getNext());
    printf("\n");
    return 0;
}
