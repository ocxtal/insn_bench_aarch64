
/**
 * @file main.cc
 * @brief uarch benchmark program for Armv8-A AArch64 processors
 * @author Hajime Suzuki
 */
#include <stdio.h>
#include "utils.h"
// #include "ldst.h"
// #include "alu.h"

void bench_load(bool md, double freq);
void bench_store(bool md, double freq);
void bench_alu(bool md, double freq);

int main(void) {
	bool const md = true;

	init(md);
	dump_cpuinfo(md);
	dump_uname_a(md);
	double const freq = estimate_cpu_freq(md, 3);

	bench_load(md, freq);
	bench_store(md, freq);
	bench_alu(md, freq);
	return(0);
}

/*
 * end of main.cc
 */
