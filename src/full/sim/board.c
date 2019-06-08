#include "../board.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

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

void board_halt(const char*msg)
{
	char buf[80];
	/* Try to avalid complex libc as much as possible, we are in a signal handler */
	sprintf(buf, "syshalt pid %d: %s\n", getpid(), msg);
	write(2, buf, strlen(buf));
	while(1)
		sleep(1);
}
