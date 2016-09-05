#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "common.h"

int start_ftrace()
{
	int fd;
	char one = '1';

	fd = open("/sys/kernel/debug/tracing/tracing_on", O_WRONLY);
	write(fd, &one, 1);
	close(fd);

	return fd;
}

void stop_ftrace()
{
	int fd;
	char zero = '0';

	fd = open("/sys/kernel/debug/tracing/tracing_on", O_WRONLY);
	write(fd, &zero, 1);
	close(fd);
}

void reset_ftrace()
{
	int fd;
	char zero = '0';

	fd = open("/sys/kernel/debug/tracing/trace", O_WRONLY);
	write(fd, &zero, 1);
	close(fd);
}

