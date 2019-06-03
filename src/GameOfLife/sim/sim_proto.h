#ifndef SIM_PROTO_H
#define SIM_PROTO_H

#include <stdint.h>
#include <stdbool.h>

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
 * Send a message to the simulator GUI.
 *
 * This funciton aborts the simulator on failure.
 */
void sim_send(uint16_t msgid, uint16_t len, const void *data);
/**
 * Start receiving messages from the simulator server.
 *
 * This funciton aborts the simulator on failure.
 */
void sim_recv_loop(void);

/**
 * Called from management threads (such as socket receive thread),
 * when they are processing a simulated interrupt. This function blocks
 * if interrupts are not enabled.
 *
 * This function also takes a lock and so prevents concurrent access by management
 * threads.
 */
void sim_irq_start(void);
/**
 * Called from mamangement threads when it has finished issuing an interrupt.
 * This will unlock the simlock and also signal he worker if it was in  #sim_irq_wait.
 *
 * @see sim_irq_start
 */
void sim_irq_end(void);

/**
 * Called from worker thread to disable interrupts (blocks sim_irq_start).
 */
void sim_int_disable(void);
/**
 * @see sim_int_disable
 */
void sim_int_restore(void);

/**
 * Wait for simulated interrupt to happen.
 */
void sim_irq_wait(void);

#endif
