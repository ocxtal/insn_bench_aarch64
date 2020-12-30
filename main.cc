
/**
 * @file main.cc
 * @brief uarch benchmark program for Armv8-A AArch64 processors
 * @author Hajime Suzuki
 */
#include <stdio.h>
#include "utils.h"
// #include "ldst.h"
#include "alu.h"

#ifndef GIT_COMMIT
#  define GIT_COMMIT		"unknown"
#endif

#include <setjmp.h>
jmp_buf jb;

static
void init(bool md) {
	init_bench();
	notes n(md, "AArch64 latency / throughput benchmark", 0);
	n.put("https://github.com/ocxtal/uarch_bench_aarch64 (commit: %s)", GIT_COMMIT);
	return;
}

static
void dump_uname_a(bool md) {
	notes n(md, "uname -a");

	char *s = run_command("uname -a");
	if(s) { n.quote(s); }
	free(s);
	return;
}

static
void dump_cpuinfo(bool md) {
	notes n(md, "processor info");

	#if defined(__APPLE__)
	char *s = run_command("system_profiler SPHardwareDataType");
	#elif defined(__linux__)
	char *s = run_command("lscpu");
	#endif
	if(s) { n.quote(s); }
	free(s);
	return;
}

static
double estimate_freq(bool md, size_t trials) {
	notes n(md, "CPU frequency estimation");
	n.put("measuring CPU frequency, assuming latency of 64bit addition is %zu cycle(s):", ADD_LATENCY_CYCLES);
	n.newline();

	double sum = 0.0;
	for(size_t i = 0; i < trials; i++) {
		double const f = estimate_cpu_freq();
		n.item("%.2f MHz", f / 1000000.0);
		sum += f;
	}
	return(sum / (double)trials);
}


int main(void) {
	bool const md = true;

	init(md);
	dump_cpuinfo(md);
	dump_uname_a(md);
	double const freq = estimate_freq(md, 3);

	// bench_load(md, freq);
	// bench_store(md, freq);
	// bench_alu(md, freq);
	return(0);
}

/*
 * end of main.cc
 */
