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
			return 0;
		}
		return -1;
	}
	return -1;
}

/* args, x_max in bits, y_max in bits */
int8_t v_create(int x_max, int y_max) {
	return v_create(((x_max + 7) / 8) * y_max);
}

void v_destroy() {
	if (virtural_memory.v_memory != NULL) {
		free(virtural_memory.v_memory);
	}	
}

/* args, x_max in bits, y_max in bits, x in bits, y in bits */
int8_t v_get(int x_max, int y_max, int x, int y) {
	if (virtural_memory.v_memory != NULL) {
		if ((y < y_max) && (x < x_max)) {
			int bind = (x_max + 7) / 8;
			if ((bind * y_max) <= virtural_memory.v_size) {
				int byte = (x / 8) + (y * bind);
				int bit = (x % 8);
				return ((virtural_memory.v_memory[byte] & 0x80 >> bit) > 0) ? 1 : 0;
			}
			return -1;
		}
		return -1;
	}
	return -1;
}

/* args, x_max in bits, y_max in bits, x in bits, y in bits */
int8_t v_set(int x_max, int y_max, int x, int y) {
	if (virtural_memory.v_memory != NULL) {
		if ((y < y_max) && (x < x_max)) {
			int bind = (x_max + 7) / 8;
			if ((bind * y_max) <= virtural_memory.v_size) {
				int byte = (x / 8) + (y * bind);
				int bit = (x % 8);
				virtural_memory.v_memory[byte] |= 0x80 >> bit;
				return 0;
			}
			return -1;
		}
		return -1;
	}
	return -1;
}

int8_t v_clear(uint8_t init_val) {
	if (virtural_memory.v_memory != NULL) {
		memset(virtural_memory.v_memory, 0x00, virtural_memory.v_size * sizeof(uint8_t));
		return 0;
	}
	return -1;
}

// https://stackoverflow.com/a/2602885
static inline uint8_t reverse(uint8_t b) {
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
	return b;
}

int8_t v_dump(int x_max, int y_max, const char* file) {
	FILE* fptr;
	fopen_s(&fptr, file, "w+");
	if (fptr != NULL) {
		fprintf(fptr, "#define debug_width %d\n", x_max);
		fprintf(fptr, "#define debug_height %d\n", y_max);
		fprintf(fptr, "static unsigned char debug_bits[] = {");
		int fancy_mod = (x_max + 7) / 8;
		for (int i = 0; i < virtural_memory.v_size; i++) {
			if (!(i % fancy_mod)) fprintf(fptr, "\n");
			if ((i + 1) != virtural_memory.v_size) {
				fprintf(fptr, "0x%.2X, ", reverse(virtural_memory.v_memory[i]));
			} else {
				fprintf(fptr, "0x%.2X\n", reverse(virtural_memory.v_memory[i]));
			}
		} fprintf(fptr, "};\n");
		fclose(fptr);
		return 0;
	} 
	printf("debug err\n");
	return -1;
}

vmem* v_get_ptr(void) {
	return &virtural_memory;
}