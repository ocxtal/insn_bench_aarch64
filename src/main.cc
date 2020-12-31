
/**
 * @file main.cc
 * @brief uarch benchmark program for Armv8-A AArch64 processors
 * @author Hajime Suzuki
 */
#include <stdio.h>
#include "utils.h"

void bench_load(bool md, double freq);
void bench_store(bool md, double freq);
void bench_mov(bool md, double freq);
void bench_arith(bool md, double freq);
void bench_logic(bool md, double freq);
void bench_cmp(bool md, double freq);
void bench_cond(bool md, double freq);
void bench_crypto(bool md, double freq);

void bench_mov_vec(bool md, double freq);
void bench_arith_vec(bool md, double freq);
void bench_logic_vec(bool md, double freq);
void bench_cmp_vec(bool md, double freq);
void bench_perm_vec(bool md, double freq);

void bench_arith_fp_vec(bool md, double freq);
void bench_math_fp_vec(bool md, double freq);
void bench_cmp_fp_vec(bool md, double freq);
void bench_cond_fp(bool md, double freq);
void bench_conv_fp_vec(bool md, double freq);

int main(void) {
	bool const md = true;

	init(md);
	dump_cpuinfo(md);
	dump_uname_a(md);
	double const freq = estimate_cpu_freq(md, 3);

	bench_load(md, freq);
	bench_store(md, freq);
	bench_mov(md, freq);
	bench_arith(md, freq);
	bench_logic(md, freq);
	bench_cmp(md, freq);
	bench_cond(md, freq);
	bench_crypto(md, freq);

	bench_mov_vec(md, freq);
	bench_arith_vec(md, freq);
	bench_logic_vec(md, freq);
	bench_cmp_vec(md, freq);
	bench_perm_vec(md, freq);

	bench_arith_fp_vec(md, freq);
	bench_math_fp_vec(md, freq);
	bench_cmp_fp_vec(md, freq);
	bench_cond_fp(md, freq);
	bench_conv_fp_vec(md, freq);
	return(0);
}

/*
 * end of main.cc
 */
