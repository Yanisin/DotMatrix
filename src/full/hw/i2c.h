#pragma once
#include <stdint.h>
#include <stdbool.h>

void i2c_init(void);
bool i2c_send(uint8_t cell, uint8_t msgid, uint8_t len, const void *data);
bool i2c_broadcast(uint8_t msgid, uint8_t len, const void *data);

#ifdef SIM
void i2c_sim_recv(uint16_t len, const uint8_t *data);
#endif
