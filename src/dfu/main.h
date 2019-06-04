#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

void continue_boot(void);
void enter_dfu(void);
void handle_page(void);
/* For debugging */
void panic(void);

#define PAGE_SIZE 2048
#define MAX_PAGE_COUNT 60
#define FLASH_RESERVED_PAGES 4

/* Just 0xc0 in practice */
#define VECTOR_TABLE_SIZE 0x100

extern int page_number;
extern uint8_t page_data[PAGE_SIZE];

#endif // MAIN_H
