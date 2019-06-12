#ifndef CELL_ID_H
#define CELL_ID_H

#include <stdint.h>

#define CELL_ID_LEN 12

typedef struct {
	uint8_t bytes[CELL_ID_LEN];
} cell_id_t;

const cell_id_t *get_cell_id(void);

#endif
