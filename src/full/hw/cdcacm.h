#ifndef _CDCACM_H
#define _CDCACM_H

#include <stdint.h>
#include "byte_queue.h"

extern output_queue_t cdcacm_tx_queue;
extern input_queue_t cdcacm_rx_queue;
extern int cdcacm_is_on(void);

#endif
