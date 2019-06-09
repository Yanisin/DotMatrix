#pragma once

#include <stdint.h>

#define MAX(a,b) ( (a) > (b) ? (a) : (b))
#define MIN(a,b) ( (a) < (b) ? (a) : (b))
#define ABS(a) ((a) < 0 ? (-a) : (a))

/* Should be smbus crc8 from U-boot */
static inline void crc8(uint32_t *crc, uint8_t value)
{
	*crc ^= (value << 8);
	for (int i = 8; i; i--) {
		if (*crc & 0x8000)
			*crc ^= (0x1070 << 3);
		*crc <<= 1;
	}
}

static inline uint8_t crc8_get(const uint32_t* crc)
{
	return (*crc >> 8) & 0xff;
}
