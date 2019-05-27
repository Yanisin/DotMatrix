#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <endian.h>
#include "sim_proto.h"
#include "proto_defs.h"
#include "../cell_id.h"
#include <arpa/inet.h>
#include <memory.h>

#define MAX_MSG_SIZE 256

int server_socket;
char sim_recvbuf[MAX_MSG_SIZE];

struct msg_hdr {
	uint16_t len;
	uint16_t msgid;
};

bool sim_connect(const char *server, const char *port)
{
	struct addrinfo* ai;
	struct addrinfo hints;

	memset (&hints, 0, sizeof (hints));
	/* Python's soccketserver does not seem to support ipv6 yet */
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags |= AI_CANONNAME;

	int r = getaddrinfo(server, port, &hints, &ai);
	if (r != 0) {
		fprintf(stderr, "can not resolve address: %s", gai_strerror(r));
		return false;
	}

	server_socket = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	if (server_socket < 0) {
		perror("open socket");
		freeaddrinfo(ai);
		return false;
	}

	if (connect(server_socket, ai->ai_addr, ai->ai_addrlen) < 0) {
		fprintf(stderr, "Cannot connect to %s:%s\n", server, port);
		perror("connect");
		freeaddrinfo(ai);
		return false;
	}

	sim_send(MSGID_CLIENT_HELLO, CELL_ID_LEN, get_cell_id()->bytes);

	freeaddrinfo(ai);
	return true;
}

static void perror_fail(const char *msg)
{
	perror(msg);
	abort();
}

void sim_lock()
{

}

void sim_unlock()
{

}

void sim_send(uint16_t msgid, uint16_t len, const void *data)
{
	ssize_t r;
	struct msg_hdr hdr = {
		.len = htobe16(len),
		.msgid = htobe16(msgid),
	};
	sim_lock();
	r = send(server_socket, &hdr, sizeof(hdr), 0);
	if (r != sizeof(hdr))
		perror_fail("send");

	r = send(server_socket, data, len, 0);
	if (r != len)
		perror_fail("send");
	sim_unlock();
}

static void sim_recv(uint16_t *msgid, uint16_t *len)
{
	struct msg_hdr hdr;
	ssize_t r;

	r = recv(server_socket, &hdr, sizeof(hdr), MSG_WAITALL);
	if (r != sizeof(hdr))
		perror_fail("recv");
	*len = be16toh(hdr.len);
	*msgid = be16toh(hdr.msgid);
	if (*len > MAX_MSG_SIZE) {
		fprintf(stderr, "oversize message received from server\n");
		abort();
	}
	r = recv(server_socket, sim_recvbuf, *len, MSG_WAITALL);
	if (r != *len)
		perror_fail("recv");
}

void sim_recv_loop(void)
{
	for(;;) {
		uint16_t msg, len;
		sim_recv(&msg, &len);
	}
}
