#include "common.h"

int com_init(struct sockaddr_in *sockaddr, socklen_t *socklen, char host)
{
	int s;
	struct timeval tv;

	s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s < 0)
	{
		printf("Failed to create socket\n");
		return -1;
	}

	if (host)
	{
		*socklen = sizeof(struct sockaddr_in);
		memset((char *) sockaddr, 0, sizeof(struct sockaddr_in));
		sockaddr->sin_family = AF_INET;
		sockaddr->sin_port = htons(SUBSCRIBER_PORT);

		if (inet_aton(SUBSCRIBER_ADDR, &(sockaddr->sin_addr)) == 0) 
		{
			printf("Failed to binarize IP addr\n");
			return -2;
		}

		tv.tv_sec = 0;
		tv.tv_usec = HOST_LOOP_PERIOD_MS * 500; /* Half of the period */
		setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));
	}
	else
	{
		/* This will be filled by the first recvfrom() with the peer's addr */
		*socklen = sizeof(struct sockaddr_in);
		memset((char *) sockaddr, 0, sizeof(struct sockaddr_in));

		struct sockaddr_in host;
		host.sin_family = AF_INET;
		host.sin_port = htons(SUBSCRIBER_PORT);
		host.sin_addr.s_addr = htonl(INADDR_ANY);

		if(bind(s, (struct sockaddr*)&host, sizeof(struct sockaddr_in)) == -1)
		{
			printf("Failed to bind sockaddr\n");
			return -2;
		}
	}

	return s;
}

int com_send(struct sockaddr_in *sockaddr, socklen_t *socklen, int fd, char *packet_out, int size)
{
	ssize_t sent;

	sent = sendto(fd, packet_out, size, 0 , (struct sockaddr *) sockaddr, *socklen);

	if (sent < 0)
	{
		printf("Failed to send packet\n");
		return -1;
	}
	else if (sent != size)
	{
		printf("Sendto has not sent all the bytes : %d sent and %d expected.\n", sent, size);
		return -1;
	}

	return 0;
}

int com_receive(struct sockaddr_in *sockaddr, socklen_t *socklen, int fd, char *packet_in, int size)
{
	ssize_t rcvd;

	memset(packet_in,'\0', size);
	rcvd = recvfrom(fd, packet_in, size, 0, (struct sockaddr *) sockaddr, socklen);

	if (rcvd < 0)
	{
		printf("Failed to receive packet\n");
		return -1;
	}
	else if (rcvd != size)
	{
		printf("Recvfrom has not received all the bytes : %d received and %d expected.\n", rcvd, size);
		return -1;
	}

	return 0;
}
