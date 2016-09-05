#include "common.h"

void clock_current(clock_s *now)
{
	clock_gettime(CLOCK_MONOTONIC_RAW, now);
}

int clock_diff_us(clock_s c1, clock_s c2)
{
	int diff_us;

	diff_us = (c2.tv_sec - c1.tv_sec) * 1000000;
	diff_us += (c2.tv_nsec - c1.tv_nsec) / 1000;

	return diff_us;
}

void clock_latency_update(clock_s now, clock_s prev, int *latency_counter, const char* name)
{
	int diff = clock_diff_us(prev, now);
	if (diff > *latency_counter)
	{
		*latency_counter = diff;
		printf("New latency for %s : %d us.\n", name, *latency_counter);
	}
}
