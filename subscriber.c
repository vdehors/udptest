#include <unistd.h>
#include "common.h"

int main(int argc, char** argv)
{
	char packet_out[PACKET_SIZE_TO_HOST];
	char packet_in[PACKET_SIZE_TO_SUB];
	struct sockaddr_in sockaddr;
	socklen_t socklen;
	int fd_udp, i;
	int loop_iter = 0;

	printf("Subscriber side.\n");
	printf("Receive %d packets of %d bytes and send %d packets of %d bytes after %d us.\n",
	       PACKET_NB_TO_SUB, PACKET_SIZE_TO_SUB, PACKET_NB_TO_HOST, PACKET_SIZE_TO_HOST,
	       SUBSCRIBER_COMPUTE_TIME_US);

	fd_udp = com_init(&sockaddr, &socklen, 0);
	printf("Fd for UDP communication is %d.\n", fd_udp);

	memset(packet_out, 0xaa, PACKET_SIZE_TO_HOST);

	while (1)
	{
		loop_iter++;

		for (i=0; i<PACKET_NB_TO_SUB; i++)
		{
			com_receive(&sockaddr, &socklen, fd_udp, packet_in, PACKET_SIZE_TO_SUB);
		}

		//printf("All packet received. Wait %d ms. Then start sending.\n", SUBSCRIBER_COMPUTE_TIME_MS);
		usleep(SUBSCRIBER_COMPUTE_TIME_US);

		for (i=0; i<PACKET_NB_TO_HOST; i++)
		{
			com_send(&sockaddr, &socklen, fd_udp, packet_out, PACKET_SIZE_TO_HOST);
		}

		//printf("End of loop %d.\n", loop_iter);
	}

	close(fd_udp);

	return 0;
}
