#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>
#include "common.h"

/* Use timerfd using the libc */
int timer_init(void)
{
	int timerfd = -1;

	/* Create FD */
	timerfd = timerfd_create(CLOCK_REALTIME, 0);

	return timerfd;
}

int timer_setdelay(int fd, unsigned int delay_ms)
{
	struct itimerspec ts;

	memset(&ts, sizeof(ts), 0);
	ts.it_interval.tv_sec = 0;
	ts.it_interval.tv_nsec = 0;
	ts.it_value.tv_sec = 0;
	ts.it_value.tv_nsec = delay_ms * 1000000;

	int res = timerfd_settime(fd, 0, &ts, 0);
	if(res < 0){
		printf("Failed to configure timer\n");
		return -1;
	}

	return 0;
}

int timer_wait(int fd)
{
	fd_set set;
	int ret;

	FD_ZERO(&set);
	FD_SET(fd, &set);

	ret = select(FD_SETSIZE, &set, NULL, NULL,NULL);

	return ret;
}


