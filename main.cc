
/**
 * @file main.cc
 * @brief uarch benchmark program for Armv8-A AArch64 processors
 * @author Hajime Suzuki
 */
#include <stdio.h>
#include "utils.h"
#include "ldst.h"
#include "alu.h"

int main(void) {

	printf("# measuring CPU frequency, assuming latency of 64bit addition is %zu cycle(s)...\n", ADD_LATENCY_CYCLES);
	double const freq = estimate_cpu_freq();
	printf("# estimated CPU frequency: %.2f MHz\n", freq / 1000000.0);

	printf("# measuring load / store latencies...\n");
	bench_load(freq);
	bench_store(freq);

	printf("# measuring arithmetic and logic instruction latencies...\n");
	bench_alu(freq);
	return(0);
}

/*
 * end of main.cc
 */
