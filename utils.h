
/**
 * @file utils.h
 * @brief CPU frequency and cycle measuring utils
 */
#include <stdint.h>
#include <time.h>

/*
 * measuring cycles; macOS does not allow sampling cycle counter by default,
 * so I adopted `clock_gettime` instead, and the actual cycles are calculated
 * from an estimated CPU frequency.
 */
#define read_clock_usec() ({ \
	struct timespec ts; \
	clock_gettime(CLOCK_MONOTONIC, &ts); \
	(size_t)ts.tv_sec * 1000000000ULL + (size_t)ts.tv_nsec; \
})

/*
 * I assume the processor has 1 cycle latency for 64bit add.
 */
#define ADD_LATENCY_CYCLES		( (size_t)1 )

/*
 * estimate CPU frequency from a sequence of adds. it assumes the processor
 * does not scale the frequency. otherwise the result becomes unreliable.
 */
static inline
double estimate_cpu_freq(void) {
	size_t const start = read_clock_usec();
	size_t const cnt = 250000000;
	size_t acc = 0, i = cnt;

	/* do 256 adds. I assume reg-reg latency of a 64bit add operation is 1 cycle. */
	__asm__ volatile (" \n\
	1: \n\
		# 16 adds \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		# 16 adds \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		# 16 adds \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		# 16 adds \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		# 16 adds \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		# 16 adds \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		# 16 adds \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		# 16 adds \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		# 16 adds \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		# 16 adds \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		# 16 adds \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		# 16 adds \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		# 16 adds \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		# 16 adds \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		# 16 adds \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		# 16 adds \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		add %0, %0, #1 \n\
		subs %1, %1, #1 \n\
		b.ne 1b"
		: "+r"(acc), "+r"(i)
		:
		:
	);
	size_t const end = read_clock_usec();
	return((double)acc / (double)(end - start) * 1000000000.0);
}

/*
 * end of utils.h
 */
