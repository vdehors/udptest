#ifndef COMMON_H
#define COMMON_H

#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>

//#define LOCAL_TEST
#ifdef LOCAL_TEST
	#define HOST_ADDR "127.0.0.1"
	#define SUBSCRIBER_ADDR "127.0.0.1"
#else
	#define HOST_ADDR "151.157.1.27"
	#define SUBSCRIBER_ADDR "151.157.0.1"
#endif

#define HOST_LOOP_PERIOD_MS 10
#define SUBSCRIBER_PORT 15001
#define SUBSCRIBER_COMPUTE_TIME_US 0
#define PACKET_NB_TO_HOST 100
#define PACKET_SIZE_TO_HOST 1
#define PACKET_NB_TO_SUB 100
#define PACKET_SIZE_TO_SUB 1

#define clock_s struct timespec

int com_init(struct sockaddr_in *sockaddr, socklen_t *socklen, char host); /* Return fd to use */
int com_send(struct sockaddr_in *sockaddr, socklen_t *socklen, int fd, char *packet_out, int size);
int com_receive(struct sockaddr_in *sockaddr, socklen_t *socklen, int fd, char *packet_in, int size);

int timer_init(void); /* Return fd to use */
int timer_setdelay(int fd, unsigned int delay_ms);
int timer_wait(int fd);

void clock_current(clock_s *now);
int clock_diff_us(clock_s c1, clock_s c2); /* Return the difference in milliseconds */
void clock_latency_update(clock_s now, clock_s prev, int *latency_counter, const char* name);

int start_ftrace(); /* Return fd to use with stop_ftrace() */
void stop_ftrace();
void reset_ftrace();

#endif
