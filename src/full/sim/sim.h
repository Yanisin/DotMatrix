#ifndef SIM_PROTO_H
#define SIM_PROTO_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

extern int server_socket;
extern uint8_t sim_recvbuf[];
extern bool sim_common_gpio;
extern bool sim_button_state[];

/**
 * Connect to the simulator server.
 * @returns true on success
 * @returns false on failure, error in errno
 */
bool sim_connect(const char* server, const char *port);

/**
 * Register this thread as the main CPU thread that will be running all the non-interrupt code.
 */
void sim_init_cpu_thread(void);

/**
 * Send a message to the simulator GUI.
 *
 * This funciton aborts the simulator on failure.
 */
void sim_send(uint16_t msgid, uint16_t len, const void *data);

/**
 * Wait for simulated interrupt to happen.
 *
 * The function can either be triggered by a simulated "ticker" (next tick)
 * or by an event from the simgui.
 */
void sim_irq_wait(bool wait);

#endif
