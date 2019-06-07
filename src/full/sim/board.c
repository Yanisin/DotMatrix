#include "../board.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

char static_heap[0x10000];

static void sigsegv(int sig)
{
	(void) sig;
	board_halt("sigsegv");
}

void board_init(void)
{
	if (signal(SIGSEGV, sigsegv) == SIG_ERR) {
		perror("sigaction");
		abort();
	}
}

void board_halt(const char*msg)
{
	char buf[50];
	/* Try to avalid complex libc as much as possible, we are in a signal handler */
	sprintf(buf, "syshalt pid %d: %s\n", getpid(), msg);
	write(3, buf, strlen(buf) + 1);
	while(1)
		sleep(1);
}
