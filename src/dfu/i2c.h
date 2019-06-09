#pragma once

#include <stdbool.h>
#include <stdint.h>

void i2c_init(bool master);
void i2c_teardown(void);
bool i2c_poll(void);
void i2c_send_release(void);

#define MSG_RELEASE 0
#define MSG_DATA 1
#define MSG_FLASH 2

void i2c_tx_start(uint8_t len);
bool i2c_tx_char(uint8_t c, uint32_t tick_timeout);
bool i2c_tx_data(uint8_t size, const uint8_t* data,  uint32_t tick_timeout);
bool i2c_tx_wait(uint32_t tick_timeout);
