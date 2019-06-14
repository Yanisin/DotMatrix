#pragma once
#include <stdint.h>

/**
 * Calculate running crc8 according to the SMBus specficication.
 *
 * @example
 *   uint32_t crc = 0;
 *   crc8(&crc, byte1);
 *   crc8(&crc, byte2);
 *   return crc8_get(crc);
 *
 * @param crc algorithm state
 * @param value byte to add to the crc8 state
 */
static inline void crc8(uint32_t *crc, uint8_t value)
{
	*crc ^= (value << 8);
	for (int i = 8; i; i--) {
		if (*crc & 0x8000)
			*crc ^= (0x1070 << 3);
		*crc <<= 1;
	}
}

/**
 * Return crc8 from an algorithm state
 */
static inline uint8_t crc8_get(const uint32_t* crc)
{
	return (*crc >> 8) & 0xff;
}
