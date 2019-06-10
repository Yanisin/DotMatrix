#include <stdio.h>
#include <getopt.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include "main.h"

#include "sim.h"
#include "../cell_id.h"

cell_id_t cell_id;
pthread_t worker_thread;
extern void common_main(void);

static struct option long_options[] =
{
	{"help", no_argument, 0, 'h'},
	{"server", required_argument, 0, 's'},
	{"port", required_argument, 0, 'p'},
	{"id", required_argument, 0, 'i'},
	{0, 0, 0, 0}
};

static void help(void) {
	printf("-s simulator server address\n");
	printf("-p simulator port\n");
	printf("-i cell id\n");
}

static bool parse_id(const char *text, cell_id_t *id)
{
	if (strlen(text) != CELL_ID_LEN*2)
		return false;

	for(int i = 0; i < CELL_ID_LEN; i++) {
		char digit[] = { text[i*2 + 0], text[i*2 + 1], 0};
		char *digit_last;
		unsigned long digit_value = strtoul(digit, &digit_last, 16);
		if (*digit_last != 0)
			return false;
		id->bytes[i] = digit_value;
	}
	return true;
}

const cell_id_t *get_cell_id(void) {
	return &cell_id;
}


int sim_main(int argc, char *argv[]) {
	int c;
	const char *opt_server = "localhost";
	bool opt_has_cell_id = false;
	const char *opt_port = "6788";

	setlinebuf(stdout);
	setlinebuf(stderr);

	while (1)
	{
		int option_index = 0;
		c = getopt_long (argc, argv, "h:s:p:i:", long_options, &option_index);

		if (c == -1)
			break;

		switch (c)
		{
		case 'h':
			help();
			return 1;
		case 's':
			opt_server = optarg;
			break;
		case 'p':
			opt_port = optarg;
			break;
		case 'i':
			if (!parse_id(optarg, &cell_id)) {
				fprintf(stderr, "Invalid cell id specified\n");
				return 1;
			}
			opt_has_cell_id = true;
			break;
		case '?':
			break;

		default:
			abort ();
		}
	}

	if (optind < argc)
	{
		fprintf(stderr, "Unexpected arguments\n");
		return 1;
	}

	if (!opt_has_cell_id) {
		fprintf(stderr, "Cell id must be specified\n");
		return 1;
	}

	if (!sim_connect(opt_server, opt_port))
		return 2;

	return 0;
}
