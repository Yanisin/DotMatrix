#pragma once

#include <stdbool.h>
#include <stdint.h>

void i2c_init(bool crossover, bool master);
void i2c_teardown(void);
bool i2c_poll(void);
void i2c_send_release(void);

#define MSG_RELEASE 0x3E
#define MSG_DATA 1
#define MSG_FLASH 2
#define MSG_COUNT 3

typedef struct {
	uint8_t msgid;
	uint16_t page;
	uint32_t crc;
} msg_flash;

void i2c_tx_start(uint8_t len);
bool i2c_tx_running(void);
extern uint8_t tx_buf[130];
