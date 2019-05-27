#ifndef SIM_PROTO_H
#define SIM_PROTO_H

#include <stdint.h>
#include <stdbool.h>

extern int server_socket;
extern char sim_recvbuf[];

bool sim_connect(const char* server, const char *port);
void sim_send(uint16_t msgid, uint16_t len, const void *data);
void sim_recv_loop(void);

void sim_lock();
void sim_unlock();

#endif
