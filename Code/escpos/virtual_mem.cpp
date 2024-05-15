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
 *   File: virtual_memory.cpp
 *   Note: NEEDS OPTOMIZED! BAD!
 *
 *********************************************************************************************/

#include "virtual_mem.h"

static vmem virtural_memory; //local to this .cpp

int8_t v_create(size_t size) {
	if (virtural_memory.v_memory == NULL) {
		virtural_memory.v_memory = (uint8_t*)malloc(size);
		if (virtural_memory.v_memory != NULL) {
			virtural_memory.v_size = size;
			memset(virtural_memory.v_memory, 0x00, virtural_memory.v_size * sizeof(uint8_t));
			printf("trace v_create\n");
			return 0;
		}
	}
	return -1;
}

/* args, x_max in bits, y_max in bits */
int8_t v_create(int x_max, int y_max) {
	int size = (x_max + 7) / 8;
	size *= y_max;
	return v_create(size);
}

void v_destroy() {
	if (virtural_memory.v_memory != NULL) {
		free(virtural_memory.v_memory);
		printf("trace v_destroy\n");
	}	
}

/* args, x_max in bits, y_max in bits, x in bits, y in bits */
int8_t v_get(int x_max, int y_max, int x, int y) {
	if (virtural_memory.v_memory != NULL) {
		if ((y < y_max) && (x < x_max)) {
			int bind = (x_max + 7) / 8;
			if ((bind * y_max) <= virtural_memory.v_size) {
				int byte = (x / 8) + (y * bind);
				int bit = 7 - (x % 8);
				virtural_memory.v_memory[byte] |= 1 << bit;
				return ((virtural_memory.v_memory[byte] & 1 << bit) > 0) ? 1 : 0;
			}
			return -1;
		}
		return -1;
	}
	return -1;
}

/* args, x_max in bits, y_max in bits, x in bits, y in bits */
int8_t v_set(int x_max, int y_max, int x, int y)
{
	if (virtural_memory.v_memory != NULL) {
		if ((y < y_max) && (x < x_max)) {
			int bind = (x_max + 7) / 8;
			if ((bind * y_max) <= virtural_memory.v_size) {
				int byte = (x / 8) + (y * bind);
				int bit = 7 - (x % 8);
				virtural_memory.v_memory[byte] |= 1 << bit;
				return 0;
			}
			return -1;
		}
		return -1;
	}
	return -1;
}

int8_t v_clear(uint8_t init_val)
{
	if (virtural_memory.v_memory != NULL) {
		memset(virtural_memory.v_memory, 0x00, virtural_memory.v_size * sizeof(uint8_t));
		printf("trace v_create\n");
		return 0;
	}
	return -1;
}

//vmem* v_get_ptr(void) {
//	return &virtural_memmory;
//}