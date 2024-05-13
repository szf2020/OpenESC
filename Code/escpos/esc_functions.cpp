// MJM 2024

#include "esc_functions.h"

//----------------------------------------------------------------

static const int DPI_203 = (int)(8.0 * 25.4); //real is 203.2 dots/inch

static float vertical_motion_unit = 1;
static float horizontal_motion_unit = 1;
static int line_spacing = 30;


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
        default:                                                     break;
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

    printf("-Character Width: %d\n", ((n >> 4) & 0x0F) + 1);
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
        (uint8_t)b->getNext(), //pL
        (uint8_t)b->getNext(), //pH
        (uint8_t)b->getNext(), //m
        (uint8_t)b->getNext()  //fn
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
    int index = -1;
    uint8_t args[4] = {
        (uint8_t)b->getNext(), //p1
        (uint8_t)b->getNext(), //p2
        (uint8_t)b->getNext(), //p3
        (uint8_t)b->getNext()  //p4
    };

    uint8_t m  = (uint8_t)b->getNext();   //48, also assumed to be color mode 
    uint8_t fn = (uint8_t)b->getNext();  //function mode
    uint8_t a  = (uint8_t)b->getNext();   

    printf("<GS 8 L>\n");
    printf("- p1=0x%.2X, p2=0x%.2X, p3=0x%.2X, p4=0x%.2X\n",
        args[0], args[1], args[2], args[3]);
    printf("- m=0x%.2X, fn=0x%.2X, a=0x%.2X\n", m, fn, a);

    if ((fn >= 0) && (fn <= 4)) {           //0-4
        index = fn;
    }
    else if ((fn >= 48) && (fn <= 52)) {    //0-4
        index = fn - 48;
    }
    else if ((fn >= 64) && (fn <= 69)) {    //5-10
        index = (args[3] - 64) + 5;
    }
    else if ((fn >= 80) && (fn <= 85)) {    //11-16
        index = (fn - 80) + 11;
    }
    else if ((fn >= 112) && (fn <= 113)) {  //17-18
        index = (fn - 112) + 17;
    }

    if (index != -1)
        gs_function[index].ptr(b, 4, args);

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

int SYM_NULL(RxBuffer* b, int k)
{
    return -1;
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
    uint8_t bx = (uint8_t)b->getNext();
    uint8_t by = (uint8_t)b->getNext();
    uint8_t c  = (uint8_t)b->getNext();
    uint8_t xl = (uint8_t)b->getNext();
    uint8_t xh = (uint8_t)b->getNext();
    uint8_t yl = (uint8_t)b->getNext();
    uint8_t yh = (uint8_t)b->getNext();

    int k = (((xl + xh * 256) + 7) / 8) * (yl + yh * 256);
    int sum = (argv[0] + argv[1] * 256 + argv[2] * 65536 + argv[3] * 16777216);

    printf("--[raster info]--\n");
    printf("p1:%d, p2:%d, p3:%d, p4:%d\n", argv[0], argv[1], argv[2], argv[3]);
    printf("bx:%d, by:%d\n", bx, by);
    printf("c:%d\n", c);
    printf("xl:%d, xh:%d, yl:%d, yh:%d\n", xl, xh, yl, yh);
    printf("k:%d\n", k);
    printf("X: %d, Y: %d\n", (xl + xh * 256), (yl + yh * 256));
    printf("sum:%d\n", sum);

    //yBytes = (yl + 7) / 8; //total bytes in the y direction

    printf("------ Raster Dump ----------\n");
    for (int i = 0; i < k; i++) {
        uint8_t h = (uint8_t)b->getNext();
        printf("0x%.2X ", h);
    }
    printf("\n----------------------\n");

    return 0;
}

int GS_function_113(RxBuffer* b, int argc, uint8_t* argv)
{
    return 0;
}
