#include "common.h"

int main(int argc, char** argv)
{
	char packet_out[PACKET_SIZE_TO_SUB];
	char packet_in[PACKET_SIZE_TO_HOST];
	struct sockaddr_in sockaddr;
	clock_s debug_point1, debug_point1_prev, debug_point2, debug_point3;
	socklen_t socklen;
	int fd_udp, fd_ftrace, fd_timer, i;
	int debug_latency_12 = 0, debug_latency_23 = 0, debug_latency_31 = 0, debug_latency_loop = 0;
	int current_duration;
	int loop_iter = 0;

	printf("Host side.\n");
	printf("Send %d packets of %d bytes and receive %d packets of %d bytes every %d ms.\n",
	       PACKET_NB_TO_SUB, PACKET_SIZE_TO_SUB, PACKET_NB_TO_HOST, PACKET_SIZE_TO_HOST,
	       HOST_LOOP_PERIOD_MS);

	fd_udp = com_init(&sockaddr, &socklen, 1);
	printf("Fd for UDP communication is %d.\n", fd_udp);

	memset(packet_out, 0xaa, PACKET_SIZE_TO_SUB);

	fd_timer = timer_init();
	start_ftrace();

	clock_current(&debug_point1);
	debug_point1_prev = debug_point1;
	debug_point2 = debug_point1;
	debug_point3 = debug_point1;

	while (1)
	{
		loop_iter++;
		reset_ftrace();
		timer_setdelay(fd_timer, HOST_LOOP_PERIOD_MS);

		/* Debug point 1 */
		clock_current(&debug_point1);
		clock_latency_update(debug_point1, debug_point3, &debug_latency_31, "debug_point_3_1");
		clock_latency_update(debug_point1, debug_point1_prev, &debug_latency_loop, "loop");
		debug_point1_prev = debug_point1;

		for (i=0; i<PACKET_NB_TO_SUB; i++)
		{
			if (com_send(&sockaddr, &socklen, fd_udp, packet_out, PACKET_SIZE_TO_SUB) != 0)
			{
				stop_ftrace();
				printf("Failed to send packet %d\n", i);
				exit(1);
			}
		}

		//printf("All packet sent. Receive them...\n");

		/* Debug point 2 */
		clock_current(&debug_point2);
		clock_latency_update(debug_point2, debug_point1, &debug_latency_12, "debug_point_1_2");

		for (i=0; i<PACKET_NB_TO_HOST; i++)
		{
			if (com_receive(&sockaddr, &socklen, fd_udp, packet_in, PACKET_SIZE_TO_HOST) != 0)
			{
				usleep(5000);
				stop_ftrace();
				printf("Failed to receive packet %d\n", i);
				exit(1);
			}
		}

		/* Debug point 3 */
		clock_current(&debug_point3);
		clock_latency_update(debug_point3, debug_point2, &debug_latency_23, "debug_point_2_3");

		current_duration = clock_diff_us(debug_point1, debug_point3);
		if (current_duration > HOST_LOOP_PERIOD_MS * 1000 * 0.5)
		{
			stop_ftrace();
			printf("Loop period exceeded. Exit.\n");
			exit(1);
		}

		//printf("End of loop %d.\n", loop_iter);
		timer_wait(fd_timer);
	}

	close(fd_udp);

	return 0;
}
