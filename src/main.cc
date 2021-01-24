
/**
 * @file main.cc
 * @brief uarch benchmark program for Armv8-A AArch64 processors
 * @author Hajime Suzuki
 */
#include <stdio.h>
#include "utils.h"

void bench_load(bool md, double freq);
void bench_store(bool md, double freq);
void bench_branch(bool md, double freq);
void bench_mov(bool md, double freq);
void bench_arith(bool md, double freq);
void bench_logic(bool md, double freq);
void bench_cmp(bool md, double freq);
void bench_cond(bool md, double freq);
void bench_crypto(bool md, double freq);
void bench_atomic(bool md, double freq);

void bench_load_vec(bool md, double freq);
void bench_store_vec(bool md, double freq);
void bench_mov_vec(bool md, double freq);
void bench_arith_vec(bool md, double freq);
void bench_ext_arith_vec(bool md, double freq);
void bench_logic_vec(bool md, double freq);
void bench_cmp_vec(bool md, double freq);
void bench_perm_vec(bool md, double freq);

void bench_arith_fp_vec(bool md, double freq);
void bench_math_fp_vec(bool md, double freq);
void bench_cmp_fp_vec(bool md, double freq);
void bench_cond_fp(bool md, double freq);
void bench_conv_fp_vec(bool md, double freq);

#define DEBUG

int main(void) {
	bool const md = true;
	init(md, 8);

	#ifdef DEBUG
	double const freq = 3200000000.0;

	#else
	dump_cpuinfo(md);
	dump_uname_a(md);

	/* warm up */
	lat_i(1000000000.0, op( g->add(d->x, d->x, s->x) ));
	lat_i(1000000000.0, op( g->add(d->x, d->x, s->x) ));
	lat_i(1000000000.0, op( g->add(d->x, d->x, s->x) ));
	double const freq = estimate_cpu_freq(md, 3);

	#endif


	bench_branch(md, freq);
	bench_ext_arith_vec(md, freq);
	// bench_crypto(md, freq);
	/*
	bench_load(md, freq);
	bench_store(md, freq);
	bench_branch(md, freq);
	bench_mov(md, freq);
	bench_arith(md, freq);
	bench_logic(md, freq);
	bench_cmp(md, freq);
	bench_cond(md, freq);
	bench_crypto(md, freq);
	bench_atomic(md, freq);

	bench_load_vec(md, freq);
	bench_store_vec(md, freq);
	bench_mov_vec(md, freq);
	bench_arith_vec(md, freq);
	bench_ext_arith_vec(md, freq);
	bench_logic_vec(md, freq);
	bench_cmp_vec(md, freq);
	bench_perm_vec(md, freq);

	bench_arith_fp_vec(md, freq);
	bench_math_fp_vec(md, freq);
	bench_cmp_fp_vec(md, freq);
	bench_cond_fp(md, freq);
	bench_conv_fp_vec(md, freq);
	*/
	return(0);
}

/*
 * end of main.cc
 */
