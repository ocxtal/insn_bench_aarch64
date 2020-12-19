
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
	size_t const cnt = 32 * 1024 * 1024;
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
	Xbyak_aarch64::CodeGenerator *g,
	Xbyak_aarch64::XReg xd,
	Xbyak_aarch64::XReg xs,
	Xbyak_aarch64::XReg xc,
	Xbyak_aarch64::VReg vd,
	Xbyak_aarch64::VReg vs,
	Xbyak_aarch64::VReg vc
);
#define op(_body) ( []( \
	Xbyak_aarch64::CodeGenerator *g, \
	Xbyak_aarch64::XReg xd, \
	Xbyak_aarch64::XReg xs, \
	Xbyak_aarch64::XReg xc, \
	Xbyak_aarch64::VReg vd, \
	Xbyak_aarch64::VReg vs, \
	Xbyak_aarch64::VReg vc \
) { \
	(void)xd, (void)xs, (void)xc, (void)vd, (void)vs, (void)vc; \
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
	printf("%s\t%.2f\t%.2f\n", name, c.lat, c.rct);
}


/*
 * provides prologue and epilogue for benchmarking
 */
class bench : Xbyak_aarch64::CodeGenerator {
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
	static size_t const n_insns = (size_t)16 * 1024 * 1024;

public:
	bench(
		double const &_freq,
		size_t const &_xseed,
		size_t const &_xconst,
		uint8x16_t const &_vseed,
		uint8x16_t const &_vconst
	) : Xbyak_aarch64::CodeGenerator(65536),
		freq(_freq),
		xseed(_xseed),
		xconst(_xconst),
		vseed(_vseed),
		vconst(_vconst) {
	}
	bench(
		double const &_freq,
		size_t const &_xseed = 0,
		size_t const &_xconst = 0,
		uint8_t const &_vseed = 0,
		uint8_t const &_vconst = 0
	) : bench(_freq, _xseed, _xconst, vdupq_n_u8(_vseed), vdupq_n_u8(_vconst)) {
	}
	bench(
		double const &_freq,
		void *const &_xseed,
		size_t const &_xconst = 0,
		uint8_t const &_vseed = 0,
		uint8_t const &_vconst = 0
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
	measure_t lat(op_t fp) {
		/* save registers, core loop, and restore registers */
		head(); body_lat(fp); tail(); ready();

		/* generate the code for the loop; then run twice. first is for warming up. */
		auto fn = getCode<void (*)(size_t xs, size_t xc, uint8x16_t vs, uint8x16_t vc)>();
		fn(xseed, xconst, vseed, vconst);
		size_t const ns = measure_nsec({ fn(xseed, xconst, vseed, vconst); });

		/* flush code area for the next measure */
		reset(); setProtectModeRW();
		return((measure_t){
			.lat = (double)ns * freq / ((double)n_insns * 1000000000.0),
			.rct = 0
		});
	}
	measure_t rct(op_t fp) {
		/* save registers, core loop, and restore registers */
		head(); body_thr(fp); tail(); ready();

		/* generate the code for the loop; then run twice. first is for warming up. */
		auto fn = getCode<void (*)(size_t xs, size_t xc, uint8x16_t vs, uint8x16_t vc)>();
		fn(xseed, xconst, vseed, vconst);
		size_t const ns = measure_nsec({ fn(xseed, xconst, vseed, vconst); });

		/* flush code area for the next measure */
		reset(); setProtectModeRW();
		return((measure_t){
			.lat = 0,
			.rct = ((double)n_insns * 1000000000.0) / ((double)ns * freq)
		});
	}
	measure_t both(op_t fp) {
		measure_t const m = {
			.lat = lat(fp).lat,
			.rct = rct(fp).rct
		};
		return(m);
	}

	/* default template for measuring them */
	virtual void body_lat(op_t fp) {
		/*
		 * place constants at x16 and v16.
		 * x0: xseed, x1: xconst, v0: vseed, v1: vconst when the function is called
		 */
		mov(x16,     x1);
		mov(v16.b16, v1.b16);

		/* cut dependency chains */
		#define x(_n)	eor(x##_n, x##_n, x##_n)
		#define v(_n)	eor(v##_n.b16, v##_n.b16, v##_n.b16)
		x(1); x(2); x(3); x(4); x(5); x(6); x(7); x(8); x(9); x(10); x(11); x(12); x(13); x(14); x(15);
		v(1); v(2); v(3); v(4); v(5); v(6); v(7); v(8); v(9); v(10); v(11); v(12); v(13); v(14); v(15);
		#undef x
		#undef v

		/* initialize loop counter */
		mov(x17, n_insns / (16 * 4));

		align(64);
		Xbyak_aarch64::Label loop;
		L(loop);
		for(size_t i = 0; i < 4; i++) {
			fp(this, x1,  x0,  x16, v1,  v0,  v16);
			fp(this, x2,  x1,  x16, v2,  v1,  v16);
			fp(this, x3,  x2,  x16, v3,  v2,  v16);
			fp(this, x4,  x3,  x16, v4,  v3,  v16);
			fp(this, x5,  x4,  x16, v5,  v4,  v16);
			fp(this, x6,  x5,  x16, v6,  v5,  v16);
			fp(this, x7,  x6,  x16, v7,  v6,  v16);
			fp(this, x8,  x7,  x16, v8,  v7,  v16);
			fp(this, x9,  x8,  x16, v9,  v8,  v16);
			fp(this, x10, x9,  x16, v10, v9,  v16);
			fp(this, x11, x10, x16, v11, v10, v16);
			fp(this, x12, x11, x16, v12, v11, v16);
			fp(this, x13, x12, x16, v13, v12, v16);
			fp(this, x14, x13, x16, v14, v13, v16);
			fp(this, x15, x14, x16, v15, v14, v16);
			fp(this, x0,  x15, x16, v0,  v15, v16);
		}
		subs(x17, x17, 1);
		bne(loop);
		return;
	};
	virtual void body_thr(op_t fp) {
		/*
		 * place constants at x16 and v16.
		 * x0: xseed, x1: xconst, v0: vseed, v1: vconst when the function is called
		 */
		mov(x16,     x1);
		mov(v16.b16, v1.b16);

		/* copy input variable to all the registers used in the loop */
		#define x(_n)	mov(x##_n, x0)
		#define v(_n)	mov(v##_n.b16, v0.b16)
		x(1); x(2); x(3); x(4); x(5); x(6); x(7); x(8); x(9); x(10); x(11); x(12); x(13); x(14); x(15);
		v(1); v(2); v(3); v(4); v(5); v(6); v(7); v(8); v(9); v(10); v(11); v(12); v(13); v(14); v(15);
		#undef x
		#undef v

		/* initialize loop counter */
		mov(x17, n_insns / (16 * 16));

		align(64);
		Xbyak_aarch64::Label loop;
		L(loop);
		for(size_t i = 0; i < 16; i++) {
			fp(this, x0,  x0,  x16, v0,  v0,  v16);
			fp(this, x1,  x1,  x16, v1,  v1,  v16);
			fp(this, x2,  x2,  x16, v2,  v2,  v16);
			fp(this, x3,  x3,  x16, v3,  v3,  v16);
			fp(this, x4,  x4,  x16, v4,  v4,  v16);
			fp(this, x5,  x5,  x16, v5,  v5,  v16);
			fp(this, x6,  x6,  x16, v6,  v6,  v16);
			fp(this, x7,  x7,  x16, v7,  v7,  v16);
			fp(this, x8,  x8,  x16, v8,  v8,  v16);
			fp(this, x9,  x9,  x16, v9,  v9,  v16);
			fp(this, x10, x10, x16, v10, v10, v16);
			fp(this, x11, x11, x16, v11, v11, v16);
			fp(this, x12, x12, x16, v12, v12, v16);
			fp(this, x13, x13, x16, v13, v13, v16);
			fp(this, x14, x14, x16, v14, v14, v16);
			fp(this, x15, x15, x16, v15, v15, v16);
		}
		subs(x17, x17, 1);
		bne(loop);
		return;
	};
};

#endif	/* _UTILS_H_INCLUDED */
/*
 * end of utils.h
 */
