#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <hw/board.h>
#include <hw/cell_id.h>

char static_heap[0x10000];

static void sigsegv(int sig, siginfo_t *info ,void *p)
{
	(void) sig;
	(void) info;
	(void) p;
	board_halt("signal");
}

void board_init(void)
{
	struct sigaction action;
	memset(&action, 0, sizeof(action));
	action.sa_flags = SA_SIGINFO;
	action.sa_sigaction = sigsegv;

	if (sigaction(SIGSEGV, &action, NULL) == -1) {
		perror("sigaction");
		abort();
	}
	if (sigaction(SIGBUS, &action, NULL) == -1) {
		perror("sigaction");
		abort();
	}
}

void board_halt(const char *msg)
{
	char buf[80];
	const cell_id_t *cid = get_cell_id();
	/* Try to avalid complex libc as much as possible, we are in a signal handler */
	sprintf(buf, "%02x%02x: syshalt pid %d: %s\n",
		cid->bytes[CELL_ID_LEN - 2], cid->bytes[CELL_ID_LEN - 1], getpid(), msg);
	write(2, buf, strlen(buf));
	while(1)
		sleep(1);
}
