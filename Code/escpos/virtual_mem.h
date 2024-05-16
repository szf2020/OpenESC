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
 *   File: virtual_memory.h
 *   Note: none
 *
 *********************************************************************************************/

#pragma once

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdbool>
#include <cstdlib>

struct vmem {
	uint8_t* v_memory = NULL;
	size_t v_size = 0;
};

int8_t v_create(size_t size);
int8_t v_create(int x_max, int y_max);

int8_t v_get(int x_max, int y_max, int x, int y);
int8_t v_set(int x_max, int y_max, int x, int y);

int8_t v_clear(uint8_t init_val);
int8_t v_dump(int x_max, int y_max, const char* file);

vmem* v_get_ptr(void);

void v_destroy();

