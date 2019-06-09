#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <stdbool.h>

void continue_boot(void);
void enter_dfu(void);
void handle_page(void);
/* For debugging */
void panic(void);

enum mode {
	MODE_STANDALONE,
	MODE_MASTER,
	MODE_SLAVE,
};


extern bool dfu_activated;
extern enum mode main_mode;
extern volatile uint32_t tick_count;

extern bool ready_to_flash;

/* During the quiet phase, we request others to reboot, but we do not broadcast on the i2c lines.
 *
 * USB enumeration must be start by end of the phase in order to be able to tell who is the master.
 */
#define QUIET_PHASE_END 1000

/* During this phase, everyone has pullup on the SDA and SCL lines.
 * Master drives the SCL line low, which allows the others to sense the SDA/SCL lines.
 * Master is the cell that was enumerated over USB.
 */
#define LINE_DETECT_SAMPLE 1400
#define LINE_DETECT_PHASE_END 1900
/* Allow some time for i2c to catch up*/
#define FLASHING_ALLOWED (LINE_DETECT_PHASE_END + 10)

/* During this phase, I2C is activated on master. On slaves it is activated only if they detected
 * the SDA/SCL crossover.
 *
 * If this phase passes without the user starting a USB download, the master sends a release command
 * to everyone on the bus.
 */
#define USB_WAIT_PHASE_END 2000

/* Time left for the programming after the i2c "flash" command is sent */
#define PAGE_DELAY_TIME 10
#define PACKET_TX_TIMEOUT 50

#define PAGE_SIZE 2048
#define PAGE_PACKET_SIZE 128
#define MAX_PAGE_COUNT 60
#define FLASH_RESERVED_PAGES 8

/* Just 0xc0 in practice */
#define VECTOR_TABLE_SIZE 0x100

extern int page_number;
extern uint8_t page_data[PAGE_SIZE];


#endif // MAIN_H
