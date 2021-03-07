
/**
 * @file main.cc
 * @brief uarch benchmark program for Armv8-A AArch64 processors
 * @author Hajime Suzuki
 */
#include <stdio.h>
#include <string.h>
#include "utils.h"

void bench_port_cap(bool md, double freq);

#define DEBUG

int main(int argc, char *argv[]) {
	bool const md = argc == 2 && strcmp(argv[1], "-m") == 0;
	init(md, 8);

	#ifdef DEBUG
	#  ifdef MHZ
	double const freq = (double)(MHZ) * 1000000.0;
	#  else
	double const freq = estimate_cpu_freq(md, 3);
	#  endif

	#else
	dump_cpuinfo(md);
	dump_uname_a(md);

	/* warm up */
	lat_i(1000000000.0, op( g->add(d->x, d->x, s->x) ));
	lat_i(1000000000.0, op( g->add(d->x, d->x, s->x) ));
	lat_i(1000000000.0, op( g->add(d->x, d->x, s->x) ));
	double const freq = estimate_cpu_freq(md, 3);

	#endif

	bench_port_cap(md, freq);
	return(0);
}

/*
 * end of main.cc
 */
