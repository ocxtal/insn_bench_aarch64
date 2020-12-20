
/**
 * @file utils.h
 * @brief CPU frequency and cycle measuring utils
 * @author Hajime Suzuki
 */
#ifndef _UTILS_H_INCLUDED
#define _UTILS_H_INCLUDED

#include <stdint.h>
#include <time.h>
#include <arm_neon.h>
#include "xbyak_aarch64.h"

using namespace Xbyak_aarch64;

/*
 * measuring cycles; macOS does not allow sampling cycle counter by default,
 * so I adopted `clock_gettime` instead, and the actual cycles are calculated
 * from an estimated CPU frequency.
 */
#define read_clock_nsec() ({ \
	struct timespec ts; \
	clock_gettime(CLOCK_MONOTONIC, &ts); \
	(size_t)ts.tv_sec * 1000000000ULL + (size_t)ts.tv_nsec; \
})

#define measure_nsec(_code) ({ \
	size_t const start = read_clock_nsec(); \
	_code; \
	size_t const end = read_clock_nsec(); \
	end - start; \
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
double estimate_cpu_freq_core(void) {
	size_t const start = read_clock_nsec();
	size_t const cnt = 4 * 1024 * 1024;
	size_t acc = 0, i = cnt;

	/* do 256 adds. I assume reg-reg latency of a 64bit add operation is 1 cycle. */
	__asm__ volatile ("\n\
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
	size_t const end = read_clock_nsec();
	return((double)acc / (double)(end - start) * 1000000000.0);
}

static inline
double estimate_cpu_freq(void) {
	double r = 0.0;
	for(size_t i = 0; i < 3; i++) {
		r = estimate_cpu_freq_core();
	}
	return(r);
}


typedef void (*op_t)(
	CodeGenerator *g,
	XReg xd, XReg xs, XReg xc,
	WReg wd, WReg ws, WReg wc,
	VReg vd, VReg vs, VReg vc,
	Label l
);
#define op(_body) ( []( \
	CodeGenerator *g, \
	XReg xd, XReg xs, XReg xc, \
	WReg wd, WReg ws, WReg wc, \
	VReg vd, VReg vs, VReg vc, \
	Label l \
) { \
	(void)xd; (void)xs; (void)xc; \
	(void)wd; (void)ws; (void)wc; \
	(void)vd; (void)vs; (void)vc; \
	(void)l; \
	_body; \
} )


/* result returned in this */
struct measure_t {
	double lat;
	double rct;		/* reciprocal throughput */
};

/* printer */
static
void print(char const *name, measure_t c) {
	if(c.lat == 0.0) {
		printf("%s\t-\t%.2f\n", name, c.rct);
	} else if(c.rct == 0.0) {
		printf("%s\t%.2f\t-\n", name, c.lat);
	} else {
		printf("%s\t%.2f\t%.2f\n", name, c.lat, c.rct);
	}
}


/*
 * provides prologue and epilogue for benchmarking
 */
class bench : CodeGenerator {
private:
	/* for computing cycles per instruction */
	double const freq = 0.0;

	/* constants passed to the core loop */
	size_t const xseed  = 0;
	size_t const xconst = 0;
	uint8x16_t const vseed  = vdupq_n_u8(0);
	uint8x16_t const vconst = vdupq_n_u8(0);

	/* for storing registers */
	static size_t const stack_size = (size_t)32 * 8 + 32 * 16;

	/* #iterations */
	static size_t const n_insns = (size_t)21 * 900 * 1024;

public:
	bench(
		double const &_freq,
		size_t const &_xseed,
		size_t const &_xconst,
		uint8x16_t const &_vseed,
		uint8x16_t const &_vconst
	) : CodeGenerator(65536),
		freq(_freq),
		xseed(_xseed),
		xconst(_xconst),
		vseed(_vseed),
		vconst(_vconst) {
	}
	bench(
		double const &_freq,
		size_t const &_xseed = 1,
		size_t const &_xconst = 17,
		uint8_t const &_vseed = 1,
		uint8_t const &_vconst = 17
	) : bench(_freq, _xseed, _xconst, vdupq_n_u8(_vseed), vdupq_n_u8(_vconst)) {
	}
	bench(
		double const &_freq,
		void *const &_xseed,
		size_t const &_xconst = 17,
		uint8_t const &_vseed = 1,
		uint8_t const &_vconst = 17
	) : bench(_freq, (size_t const)_xseed, _xconst, vdupq_n_u8(_vseed), vdupq_n_u8(_vconst)) {
	}

	/* prologue and epilogue */
	void head(void) {
		sub(sp, sp, stack_size);

		/* save everything */
		#define x(_n)	str(x##_n, ptr(sp, (uint32_t)(stack_size - ((_n) + 1) * 8)))
		x(0);  x(1);  x(2);  x(3);  x(4);  x(5);  x(6);  x(7);
		x(8);  x(9);  x(10); x(11); x(12); x(13); x(14); x(15);
		x(16); x(17); x(18); x(19); x(20); x(21); x(22); x(23);
		x(24); x(25); x(26); x(27); x(28); x(29); x(30);
		#undef x

		#define q(_n)	str(q##_n, ptr(sp, (uint32_t)(stack_size - ((_n) + 1) * 16 - 32 * 8)))
		q(0);  q(1);  q(2);  q(3);  q(4);  q(5);  q(6);  q(7);
		q(8);  q(9);  q(10); q(11); q(12); q(13); q(14); q(15);
		q(16); q(17); q(18); q(19); q(20); q(21); q(22); q(23);
		q(24); q(25); q(26); q(27); q(28); q(29); q(30); q(31);
		#undef q

		return;
	}
	void tail(void) {
		/* load everything */
		#define x(_n) 	ldr(x##_n, ptr(sp, (uint32_t)(stack_size - ((_n) + 1) * 8)));
		x(0);  x(1);  x(2);  x(3);  x(4);  x(5);  x(6);  x(7);
		x(8);  x(9);  x(10); x(11); x(12); x(13); x(14); x(15);
		x(16); x(17); x(18); x(19); x(20); x(21); x(22); x(23);
		x(24); x(25); x(26); x(27); x(28); x(29); x(30);
		#undef x

		#define q(_n) 	ldr(q##_n, ptr(sp, (uint32_t)(stack_size - ((_n) + 1) * 16 - 32 * 8)));
		q(0);  q(1);  q(2);  q(3);  q(4);  q(5);  q(6);  q(7);
		q(8);  q(9);  q(10); q(11); q(12); q(13); q(14); q(15);
		q(16); q(17); q(18); q(19); q(20); q(21); q(22); q(23);
		q(24); q(25); q(26); q(27); q(28); q(29); q(30); q(31);
		#undef q

		add(sp, sp, stack_size);
		ret();
		return;
	}

	/* measure latency and throughput */
	measure_t lat(op_t fp_body, op_t fp_collect, double offset = 0.0) {
		/* save registers, core loop, and restore registers */
		head(); body_lat(fp_body, fp_collect); tail(); ready();

		/* generate the code for the loop; then run twice. first is for warming up. */
		auto fn = getCode<void (*)(size_t xs, size_t xc, uint8x16_t vs, uint8x16_t vc)>();
		fn(xseed, xconst, vseed, vconst);
		size_t const ns = measure_nsec({ fn(xseed, xconst, vseed, vconst); });

		/* flush code area for the next measure */
		reset(); setProtectModeRW();
		return((measure_t){
			.lat = (double)ns * freq / ((double)n_insns * 1000000000.0) - offset,
			.rct = 0
		});
	}
	measure_t lat(op_t fp_body, double offset = 0.0) {
		return(lat(fp_body, op( (void)g ), offset));
	}
	measure_t rct(op_t fp_body, double factor = 1.0) {
		/* save registers, core loop, and restore registers */
		head(); body_thr(fp_body); tail(); ready();

		/* generate the code for the loop; then run twice. first is for warming up. */
		auto fn = getCode<void (*)(size_t xs, size_t xc, uint8x16_t vs, uint8x16_t vc)>();
		fn(xseed, xconst, vseed, vconst);
		size_t const ns = measure_nsec({ fn(xseed, xconst, vseed, vconst); });

		/* flush code area for the next measure */
		reset(); setProtectModeRW();
		return((measure_t){
			.lat = 0,
			.rct = ((double)n_insns * 1000000000.0) / ((double)ns * freq) * factor
		});
	}
	measure_t both(op_t fp_body, double offset = 0.0, double factor = 1.0) {
		measure_t const m = {
			.lat = lat(fp_body, offset).lat,
			.rct = rct(fp_body, factor).rct
		};
		return(m);
	}
	measure_t both(op_t fp_body, op_t fp_collect, double offset = 0.0, double factor = 1.0) {
		measure_t const m = {
			.lat = lat(fp_body, fp_collect, offset).lat,
			.rct = rct(fp_body, factor).rct
		};
		return(m);
	}

	/* default template for measuring them */
	virtual void body_lat(op_t fp_body, op_t fp_collect) {
		/*
		 * place constants at x16 and v16.
		 * x0: xseed, x1: xconst, v0: vseed, v1: vconst when the function is called
		 */
		mov(x25,     x1);
		mov(v25.b16, v1.b16);

		/* cut dependency chains */
		#define x(_n)	eor(x##_n, x##_n, x##_n)
		#define v(_n)	eor(v##_n.b16, v##_n.b16, v##_n.b16)
		x(1);  x(2);  x(3);  x(4);  x(5);  x(6);  x(7);  x(8);
		x(9);  x(10); x(11); x(12); x(13); x(14); x(15); x(16);
		x(17); x(18); x(19); x(20); x(21); x(22); x(23); x(24);
		v(1);  v(2);  v(3);  v(4);  v(5);  v(6);  v(7);  v(8);
		v(9);  v(10); v(11); v(12); v(13); v(14); v(15); v(16);
		v(17); v(18); v(19); v(20); v(21); v(22); v(23); v(24);
		#undef x
		#undef v

		/* initialize loop counter */
		mov(x26, n_insns / (25 * 4));

		align(64);
		Label loop;
		L(loop);
		for(size_t i = 0; i < 4; i++) {
			fp_body(this, x1,  x0,  x25, w1,  w0,  w25, v1,  v0,  v25, loop); fp_collect(this, x1,  x0,  x25, w1,  w0,  w25, v1,  v0,  v25, loop);
			fp_body(this, x2,  x1,  x25, w2,  w1,  w25, v2,  v1,  v25, loop); fp_collect(this, x2,  x1,  x25, w2,  w1,  w25, v2,  v1,  v25, loop);
			fp_body(this, x3,  x2,  x25, w3,  w2,  w25, v3,  v2,  v25, loop); fp_collect(this, x3,  x2,  x25, w3,  w2,  w25, v3,  v2,  v25, loop);
			fp_body(this, x4,  x3,  x25, w4,  w3,  w25, v4,  v3,  v25, loop); fp_collect(this, x4,  x3,  x25, w4,  w3,  w25, v4,  v3,  v25, loop);
			fp_body(this, x5,  x4,  x25, w5,  w4,  w25, v5,  v4,  v25, loop); fp_collect(this, x5,  x4,  x25, w5,  w4,  w25, v5,  v4,  v25, loop);
			fp_body(this, x6,  x5,  x25, w6,  w5,  w25, v6,  v5,  v25, loop); fp_collect(this, x6,  x5,  x25, w6,  w5,  w25, v6,  v5,  v25, loop);
			fp_body(this, x7,  x6,  x25, w7,  w6,  w25, v7,  v6,  v25, loop); fp_collect(this, x7,  x6,  x25, w7,  w6,  w25, v7,  v6,  v25, loop);
			fp_body(this, x8,  x7,  x25, w8,  w7,  w25, v8,  v7,  v25, loop); fp_collect(this, x8,  x7,  x25, w8,  w7,  w25, v8,  v7,  v25, loop);
			fp_body(this, x9,  x8,  x25, w9,  w8,  w25, v9,  v8,  v25, loop); fp_collect(this, x9,  x8,  x25, w9,  w8,  w25, v9,  v8,  v25, loop);
			fp_body(this, x10, x9,  x25, w10, w9,  w25, v10, v9,  v25, loop); fp_collect(this, x10, x9,  x25, w10, w9,  w25, v10, v9,  v25, loop);
			fp_body(this, x11, x10, x25, w11, w10, w25, v11, v10, v25, loop); fp_collect(this, x11, x10, x25, w11, w10, w25, v11, v10, v25, loop);
			fp_body(this, x12, x11, x25, w12, w11, w25, v12, v11, v25, loop); fp_collect(this, x12, x11, x25, w12, w11, w25, v12, v11, v25, loop);
			fp_body(this, x13, x12, x25, w13, w12, w25, v13, v12, v25, loop); fp_collect(this, x13, x12, x25, w13, w12, w25, v13, v12, v25, loop);
			fp_body(this, x14, x13, x25, w14, w13, w25, v14, v13, v25, loop); fp_collect(this, x14, x13, x25, w14, w13, w25, v14, v13, v25, loop);
			fp_body(this, x15, x14, x25, w15, w14, w25, v15, v14, v25, loop); fp_collect(this, x15, x14, x25, w15, w14, w25, v15, v14, v25, loop);
			fp_body(this, x16, x15, x25, w16, w15, w25, v16, v15, v25, loop); fp_collect(this, x16, x15, x25, w16, w15, w25, v16, v15, v25, loop);
			fp_body(this, x17, x16, x25, w17, w16, w25, v17, v16, v25, loop); fp_collect(this, x17, x16, x25, w17, w16, w25, v17, v16, v25, loop);
			fp_body(this, x18, x17, x25, w18, w17, w25, v18, v17, v25, loop); fp_collect(this, x18, x17, x25, w18, w17, w25, v18, v17, v25, loop);
			fp_body(this, x19, x18, x25, w19, w18, w25, v19, v18, v25, loop); fp_collect(this, x19, x18, x25, w19, w18, w25, v19, v18, v25, loop);
			fp_body(this, x20, x19, x25, w20, w19, w25, v20, v19, v25, loop); fp_collect(this, x20, x19, x25, w20, w19, w25, v20, v19, v25, loop);
			fp_body(this, x21, x20, x25, w21, w20, w25, v21, v20, v25, loop); fp_collect(this, x21, x20, x25, w21, w20, w25, v21, v20, v25, loop);
			fp_body(this, x22, x21, x25, w22, w21, w25, v22, v21, v25, loop); fp_collect(this, x22, x21, x25, w22, w21, w25, v22, v21, v25, loop);
			fp_body(this, x23, x22, x25, w23, w22, w25, v23, v22, v25, loop); fp_collect(this, x23, x22, x25, w23, w22, w25, v23, v22, v25, loop);
			fp_body(this, x24, x23, x25, w24, w23, w25, v24, v23, v25, loop); fp_collect(this, x24, x23, x25, w24, w23, w25, v24, v23, v25, loop);
			fp_body(this, x0,  x24, x25, w0,  w24, w25, v0,  v24, v25, loop); fp_collect(this, x0,  x24, x25, w0,  w24, w25, v0,  v24, v25, loop);
		}
		subs(x26, x26, 1);
		bne(loop);
		return;
	};
	virtual void body_thr(op_t fp_body) {
		/*
		 * place constants at x16 and v16.
		 * x0: xseed, x1: xconst, v0: vseed, v1: vconst when the function is called
		 */
		mov(x25,     x1);
		mov(v25.b16, v1.b16);

		/* copy input variable to all the registers used in the loop */
		#define x(_n)	mov(x##_n, x0)
		#define v(_n)	mov(v##_n.b16, v0.b16)
		x(1);  x(2);  x(3);  x(4);  x(5);  x(6);  x(7);  x(8);
		x(9);  x(10); x(11); x(12); x(13); x(14); x(15); x(16);
		x(17); x(18); x(19); x(20); x(21); x(22); x(23); x(24);
		v(1);  v(2);  v(3);  v(4);  v(5);  v(6);  v(7);  v(8);
		v(9);  v(10); v(11); v(12); v(13); v(14); v(15); v(16);
		v(17); v(18); v(19); v(20); v(21); v(22); v(23); v(24);
		#undef x
		#undef v

		/* initialize loop counter */
		mov(x26, n_insns / (25 * 64));

		align(64);
		Label loop;
		L(loop);
		for(size_t i = 0; i < 64; i++) {
			fp_body(this, x0,  x0,  x25, w0,  w0,  w25, v0,  v0,  v25, loop);
			fp_body(this, x1,  x1,  x25, w1,  w1,  w25, v1,  v1,  v25, loop);
			fp_body(this, x2,  x2,  x25, w2,  w2,  w25, v2,  v2,  v25, loop);
			fp_body(this, x3,  x3,  x25, w3,  w3,  w25, v3,  v3,  v25, loop);
			fp_body(this, x4,  x4,  x25, w4,  w4,  w25, v4,  v4,  v25, loop);
			fp_body(this, x5,  x5,  x25, w5,  w5,  w25, v5,  v5,  v25, loop);
			fp_body(this, x6,  x6,  x25, w6,  w6,  w25, v6,  v6,  v25, loop);
			fp_body(this, x7,  x7,  x25, w7,  w7,  w25, v7,  v7,  v25, loop);
			fp_body(this, x8,  x8,  x25, w8,  w8,  w25, v8,  v8,  v25, loop);
			fp_body(this, x9,  x9,  x25, w9,  w9,  w25, v9,  v9,  v25, loop);
			fp_body(this, x10, x10, x25, w10, w10, w25, v10, v10, v25, loop);
			fp_body(this, x11, x11, x25, w11, w11, w25, v11, v11, v25, loop);
			fp_body(this, x12, x12, x25, w12, w12, w25, v12, v12, v25, loop);
			fp_body(this, x13, x13, x25, w13, w13, w25, v13, v13, v25, loop);
			fp_body(this, x14, x14, x25, w14, w14, w25, v14, v14, v25, loop);
			fp_body(this, x15, x15, x25, w15, w15, w25, v15, v15, v25, loop);
			fp_body(this, x16, x16, x25, w16, w16, w25, v16, v16, v25, loop);
			fp_body(this, x17, x17, x25, w17, w17, w25, v17, v17, v25, loop);
			fp_body(this, x18, x18, x25, w18, w18, w25, v18, v18, v25, loop);
			fp_body(this, x19, x19, x25, w19, w19, w25, v19, v19, v25, loop);
			fp_body(this, x20, x20, x25, w20, w20, w25, v20, v20, v25, loop);
			fp_body(this, x21, x21, x25, w21, w21, w25, v21, v21, v25, loop);
			fp_body(this, x22, x22, x25, w22, w22, w25, v22, v22, v25, loop);
			fp_body(this, x23, x23, x25, w23, w23, w25, v23, v23, v25, loop);
			fp_body(this, x24, x24, x25, w24, w24, w25, v24, v24, v25, loop);
		}
		subs(x26, x26, 1);
		bne(loop);
		return;
	};
};

#endif	/* _UTILS_H_INCLUDED */
/*
 * end of utils.h
 */
