
/**
 * @file utils.h
 * @brief CPU frequency and cycle measuring utils
 * @author Hajime Suzuki
 */
#ifndef _UTILS_H_INCLUDED
#define _UTILS_H_INCLUDED

#ifdef _DEFAULT_SOURCE
#  undef _DEFAULT_SOURCE
#endif
#define _DEFAULT_SOURCE			/* for sigsetjmp / siglongjmp on linux */

#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>
#include <time.h>
#include <functional>
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
 * run command and dump output
 */
static
char *run_command(char const *cmd) {
	FILE *fp = popen(cmd, "r");
	if(fp == NULL) {
		return(NULL);
	}

	size_t const buf_size = 1024 * 1024;
	uint8_t *buf = (uint8_t *)malloc(buf_size);

	size_t cap = 1024, used = 0;
	uint8_t *dst = (uint8_t *)malloc(cap);

    while(fgets((char *)buf, buf_size - 1, fp) != NULL) {
        size_t const bytes = strlen((char *)buf);
		if(used + bytes >= cap) {
			dst = (uint8_t *)realloc((void *)dst, (cap *= 2));
		}
		memcpy(&dst[used], buf, bytes);
		dst[used += bytes] = 0;
    }
	free(buf);
	pclose(fp);
	return((char *)dst);
}

/* result returned in this */
struct measure_t {
	double lat;
	double thr;		/* reciprocal throughput */
};

/*
 * illegal instruction trapping
 */
extern jmp_buf jb;

static
void sigill_trap(int s) {
	(void)s;
	siglongjmp(jb, 1);
}

static
void init_sigill_trap(void) {
	struct sigaction a;
	a.sa_handler = sigill_trap;
	sigemptyset(&a.sa_mask);
	sigaddset(&a.sa_mask, SIGILL);
	sigaction(SIGILL, &a, NULL);
	return;
}

#define try_setjmp(_body) ({ \
	measure_t const r = sigsetjmp(jb, 1) == 0 ? ( _body ) : (measure_t){ -1.0, -1.0 }; \
	r; \
})

/* init */
static
void init_bench() {
	init_sigill_trap();
	/* anything else? */
}

/* printer */
class printer {
public:
	bool md = false;

	void leader() {
		if(!md) { printf("# "); }
	}
	void newline() {
		if(md) { putchar('\n'); }
	}

	printer(
		bool _md,
		char const *title = NULL,
		size_t depth = 1
	) : md(_md) {
		size_t const max_depth = 3;
		depth = std::min(depth, max_depth);
		printf("%s%s\n", &"#### "[md ? max_depth - depth : max_depth], title);
		newline();
	}
	~printer() {
		newline();
	}
};

class table : public printer {
	char const *seps[2][4] = {
		{ "",   "\t",  "",   NULL },
		{ "| ", " | ", " |", NULL }
	};

	void thdr(void) {
		leader();
		printf("%sinstruction%slatency%sthroughput%s\n", seps[md][0], seps[md][1], seps[md][1], seps[md][2]);
	}
	void tbrk(void) {
		if(md) { printf("|--------|--------|--------|\n"); }
	}
public:
	table(
		bool _md,
		char const *title = NULL,
		size_t depth = 1
	) : printer(_md, title, depth) {
		thdr();
		tbrk();
	}
	void put(char const *name, measure_t c) {
		#ifdef SCAN_PARAMS
		(void)name; (void)c;

		#else
		/* trapped instruction when both are negative */
		size_t const na = (c.lat < 0.0 && c.thr < 0.0) ? 2 : 0;

		char const *fmt[] = { "%.2f", "-", "n/a", "n/a" };
		printf("%s%s%s", seps[md][0], name, seps[md][1]);
		printf(fmt[na + (c.lat == 0.0)], c.lat);
		printf("%s", seps[md][1]);
		printf(fmt[na + (c.thr == 0.0)], c.thr);
		printf("%s\n", seps[md][2]);
		#endif

		return;
	}
};

class notes : public printer {
public:
	notes(
		bool _md,
		char const *title = NULL,
		size_t depth = 1
	) : printer(_md, title, depth){
	}
	void put(char const *fmt, ...) {
		leader();

		va_list a;
		va_start(a, fmt);
		vfprintf(stdout, fmt, a);
		va_end(a);

		printf("\n");
	}
	void item(char const *fmt, ...) {
		leader(); printf("- ");

		va_list a;
		va_start(a, fmt);
		vfprintf(stdout, fmt, a);
		va_end(a);

		printf("\n");
	}
	void quote(char const *str) {
		size_t len = strlen(str);
		while(len > 0 && str[len - 1] == '\n') { len--; }
		if(md) { printf("```\n"); }
		printf("%.*s\n", (int)len, str);
		if(md) { printf("```\n"); }
	}
};

/*
 * parameter scanning utils
 */
#ifdef SCAN_PARAMS
#  define both(_b, ...)		try_setjmp((_b).both_(__LINE__, __VA_ARGS__))
#  define lat(_b, ...)		try_setjmp((_b).lat_(__LINE__, __VA_ARGS__))
#  define thr(_b, ...)		try_setjmp((_b).thr_(__LINE__, __VA_ARGS__))
#else
#  define both(_b, ...)		try_setjmp((_b).both_(0, __VA_ARGS__))
#  define lat(_b, ...)		try_setjmp((_b).lat_(0, __VA_ARGS__))
#  define thr(_b, ...)		try_setjmp((_b).thr_(0, __VA_ARGS__))
#endif

/*
 * issue pattern configurator
 */
typedef struct {
	size_t count;
	size_t pitch;
	size_t offset;
	size_t mod;
} pattern_t;

static pattern_t const lat_inc[] = {
	{ 5, 0, 0, 0 },
	{ 24, 1, 1, 24 },
	{ 0, 0, 0, 0 }
};
static pattern_t const lat_flat[] = {
	{ 144, 1, 1, 24 },
	{ 0, 0, 0, 0 }
};
static pattern_t const *lat_patterns[] = {
	(pattern_t const *)&lat_flat,
	(pattern_t const *)&lat_inc,
	NULL
};

#define table_thr(_w, _x, _y, _z) { \
	{ (_z), 0, 0, 0 }, \
	{ (_x), (_w), 0, 0 }, \
	{ (_y), 0, 0, 0 }, \
	{ (_w), 1, 0, (_w) * (_x) }, \
	{ 0, 0, 0, 0 } \
}
#define dec_thr(_w, _x, _y, _z)		static pattern_t const thr_##_w##_##_x##_##_y##_##_z[] = table_thr(_w, _x, _y, _z)
#define ptr_thr(_w, _x, _y, _z) 	(pattern_t const *)&( thr_##_w##_##_x##_##_y##_##_z )

#ifdef SCAN_PARAMS
dec_thr(3, 8, 1, 120);
dec_thr(4, 7, 1, 120);
dec_thr(5, 5, 1, 120);
dec_thr(6, 4, 1, 120);
dec_thr(7, 4, 1, 120);
dec_thr(8, 3, 1, 120);
dec_thr(9, 3, 1, 120);
dec_thr(3, 8, 2, 60);
dec_thr(4, 7, 2, 60);
dec_thr(5, 5, 2, 60);
dec_thr(6, 4, 2, 60);
dec_thr(7, 4, 2, 60);
dec_thr(8, 3, 2, 60);
dec_thr(9, 3, 2, 60);
dec_thr(3, 8, 3, 40);
dec_thr(4, 7, 3, 40);
dec_thr(5, 5, 3, 40);
dec_thr(6, 4, 3, 40);
dec_thr(7, 4, 3, 40);
dec_thr(8, 3, 3, 40);
dec_thr(9, 3, 3, 40);
dec_thr(3, 8, 4, 30);
dec_thr(4, 7, 4, 30);
dec_thr(5, 5, 4, 30);
dec_thr(6, 4, 4, 30);
dec_thr(7, 4, 4, 30);
dec_thr(8, 3, 4, 30);
dec_thr(9, 3, 4, 30);
dec_thr(3, 8, 5, 30);
dec_thr(4, 7, 5, 30);
dec_thr(5, 5, 5, 30);
dec_thr(6, 4, 5, 30);
dec_thr(7, 4, 5, 30);
dec_thr(8, 3, 5, 30);
dec_thr(9, 3, 5, 30);
dec_thr(3, 8, 6, 20);
dec_thr(4, 7, 6, 20);
dec_thr(5, 5, 6, 20);
dec_thr(6, 4, 6, 20);
dec_thr(7, 4, 6, 20);
dec_thr(8, 3, 6, 20);
dec_thr(9, 3, 6, 20);
#else
dec_thr(9, 3, 1, 120);
dec_thr(6, 4, 3, 40);
dec_thr(3, 8, 1, 120);
dec_thr(5, 5, 1, 120);
dec_thr(9, 3, 3, 40);
dec_thr(7, 4, 1, 120);
dec_thr(6, 4, 6, 20);
dec_thr(6, 4, 5, 30);
dec_thr(6, 4, 4, 30);
dec_thr(4, 7, 4, 30);
// dec_thr(6, 4, 3, 40);
// dec_thr(4, 7, 5, 30);
#endif

static pattern_t const *thr_patterns[] = {
#ifdef SCAN_PARAMS
	ptr_thr(3, 8, 1, 120),
	ptr_thr(4, 7, 1, 120),
	ptr_thr(5, 5, 1, 120),
	ptr_thr(6, 4, 1, 120),
	ptr_thr(7, 4, 1, 120),
	ptr_thr(8, 3, 1, 120),
	ptr_thr(9, 3, 1, 120),
	ptr_thr(3, 8, 2, 60),
	ptr_thr(4, 7, 2, 60),
	ptr_thr(5, 5, 2, 60),
	ptr_thr(6, 4, 2, 60),
	ptr_thr(7, 4, 2, 60),
	ptr_thr(8, 3, 2, 60),
	ptr_thr(9, 3, 2, 60),
	ptr_thr(3, 8, 3, 40),
	ptr_thr(4, 7, 3, 40),
	ptr_thr(5, 5, 3, 40),
	ptr_thr(6, 4, 3, 40),
	ptr_thr(7, 4, 3, 40),
	ptr_thr(8, 3, 3, 40),
	ptr_thr(9, 3, 3, 40),
	ptr_thr(3, 8, 4, 30),
	ptr_thr(4, 7, 4, 30),
	ptr_thr(5, 5, 4, 30),
	ptr_thr(6, 4, 4, 30),
	ptr_thr(7, 4, 4, 30),
	ptr_thr(8, 3, 4, 30),
	ptr_thr(9, 3, 4, 30),
	ptr_thr(3, 8, 5, 30),
	ptr_thr(4, 7, 5, 30),
	ptr_thr(5, 5, 5, 30),
	ptr_thr(6, 4, 5, 30),
	ptr_thr(7, 4, 5, 30),
	ptr_thr(8, 3, 5, 30),
	ptr_thr(9, 3, 5, 30),
	ptr_thr(3, 8, 6, 20),
	ptr_thr(4, 7, 6, 20),
	ptr_thr(5, 5, 6, 20),
	ptr_thr(6, 4, 6, 20),
	ptr_thr(7, 4, 6, 20),
	ptr_thr(8, 3, 6, 20),
	ptr_thr(9, 3, 6, 20),
#else
	ptr_thr(9, 3, 1, 120),
	ptr_thr(6, 4, 3, 40),
	ptr_thr(3, 8, 1, 120),
	ptr_thr(5, 5, 1, 120),
	ptr_thr(9, 3, 3, 40),
	ptr_thr(7, 4, 1, 120),
	ptr_thr(6, 4, 6, 20),
	ptr_thr(6, 4, 5, 30),
	ptr_thr(6, 4, 4, 30),
	ptr_thr(4, 7, 4, 30),
	// ptr_thr(6, 4, 3, 40),
	// ptr_thr(4, 7, 5, 30),
#endif
	NULL
};

/*
 * register bundle for use in `bench` class below
 */
class AReg {
public:
	AReg(
		WReg const &_w,
		XReg const &_x,
		VReg const &_v,
		BReg const &_b,
		HReg const &_h,
		SReg const &_s,
		DReg const &_d,
		QReg const &_q
	) : w(_w), x(_x), v(_v), b(_b), h(_h), s(_s), d(_d), q(_q) {
	}
	WReg const w;
	XReg const x;
	VReg const v;
	BReg const b;
	HReg const h;
	SReg const s;
	DReg const d;
	QReg const q;
};

/* op generation function template and helper */
// typedef void (*op_t)(CodeGenerator *g, AReg const *r);
typedef std::function<void (CodeGenerator *, AReg const *, AReg const *)> op_t;
#define op(_body)			( [](CodeGenerator *g, AReg const *d, AReg const *s) { (void)d; (void)s; _body; } )
#define op_cap(_body)		( [=](CodeGenerator *g, AReg const *d, AReg const *s) { (void)d; (void)s; _body; } )

/*
 * provides prologue and epilogue for benchmarking
 */
class bench : CodeGenerator {
private:
	/* for computing cycles per instruction */
	double const freq = 0.0;

	/* constants passed to the core loop */
	size_t const xseed = 0;
	uint8x16_t const vseed = vdupq_n_u8(0);

	/* helper SIMD-scalar registers */
	#define a(_n)	AReg(w##_n, x##_n, v##_n, b##_n, h##_n, s##_n, d##_n, q##_n)
	AReg const regs[36] = {
		a(0),  a(1),  a(2),  a(3),  a(4),  a(5),  a(6),  a(7),
		a(8),  a(9),  a(10), a(11), a(12), a(13), a(14), a(15),
		a(16), a(17), a(18), a(19), a(20), a(21), a(22), a(23),
		a(24), a(25), a(26), a(27),
		a(0),  a(1),  a(2),  a(3),  a(4),  a(5),  a(6),  a(7)
	};
	#undef a

	/* prologue and epilogue */
	static size_t const stack_size = (size_t)32 * 16 + 32 * 8;

	void head(void) {
		/* extend stack */
		sub(sp, sp, stack_size);

		/* save everything */
		for(size_t i = 0; i < 32; i++) {
			str(regs[i].x, ptr(sp, (uint32_t)(stack_size - (i + 1) * 8)));
			str(regs[i].q, ptr(sp, (uint32_t)(stack_size - (i + 1) * 16 - 32 * 8)));
		}

		/* initialize registers */
		for(size_t i = 1; i < 32; i++) {
			mov(regs[i].x, x0);
			mov(regs[i].v.b16, v0.b16);
		}
		return;
	}
	void tail(void) {
		/* load everything */
		for(size_t i = 0; i < 32; i++) {
			ldr(regs[i].x, ptr(sp, (uint32_t)(stack_size - (i + 1) * 8)));
			ldr(regs[i].q, ptr(sp, (uint32_t)(stack_size - (i + 1) * 16 - 32 * 8)));
		}

		/* unwind stack and return */
		add(sp, sp, stack_size);
		ret();
		return;
	}

	/* pattern generator */
	static size_t const n_insns_base = (size_t)4 * 21 * 900 * 1001;
	size_t const n_insns = n_insns_base;

	/* pattern: (count, pitch, offset, mod) */
	size_t count_insns(pattern_t const *p) {
		if(p == NULL || p->count == 0) { return(1); }
		return(p->count * count_insns(p + 1));
	}
	void expand(op_t fp, pattern_t const *p, size_t base) {
		if(p == NULL || p->count == 0) {
			// assert(base < 30);
			size_t const offset = p[-1].offset;
			size_t const mod = p[-1].mod;
			fp(this, &regs[(base + offset) % mod], &regs[base % mod]);
			return;
		}
		for(size_t i = 0; i < p->count; i++) {
			expand(fp, p + 1, i * p->pitch + base);
		}
		return;
	}
	void expand(op_t fp, pattern_t const *p) {
		/* loop counter; roundup fractions */
		size_t const count = count_insns(p);
		mov(x29, (n_insns + count - 1) / count);

		/* core loop */
		align(64);
		Label loop = L();
		{
			expand(fp, p, 0);
			subs(x29, x29, 1);
			bne(loop);
		}
		return;
	}
	double run(op_t fp, pattern_t const *p) {
		/* flush code area for the next measure */
		reset(); setProtectModeRW();

		/* save registers, core loop, and restore registers */
		head(); expand(fp, p); tail(); ready();

		/* generate the code for the loop; then run twice. first is for warming up. */
		auto fn = getCode<void (*)(size_t xs, uint8x16_t vs)>();
		fn(xseed, vseed);
		size_t const ns = measure_nsec({ fn(xseed, vseed); });
		return((double)ns * freq / ((double)n_insns * 1000000000.0));
	}
	double run(op_t fp, pattern_t const **q, size_t line = 0) {
		double res = 1000000000.0;
		while(*q != NULL) {
			double const r = run(fp, *q++);
			if(line != 0) {
				printf("#%zu\t%zu_%zu_%zu_%zu\t%.3f\n", line, q[-1][3].count, q[-1][1].count, q[-1][2].count, q[-1][0].count, 1.0 / r);
			}
			res = std::min(res, r);
		}
		return(res);
	}

public:
	bench(
		double const &_freq,
		size_t const &_xseed,
		uint8x16_t const &_vseed,
		size_t const &_rep
	) : CodeGenerator(65536),
		freq(_freq),
		xseed(_xseed),
		vseed(_vseed),
		n_insns(_rep * n_insns_base) {
	}
	bench(
		double const &_freq,
		size_t const &_xseed = 1,
		uint8_t const &_vseed = 1,
		size_t const &_rep = 1
	) : bench(_freq, _xseed, vdupq_n_u8(_vseed), _rep) {
	}
	bench(
		double const &_freq,
		void *const &_xseed,
		uint8_t const &_vseed = 1,
		size_t const &_rep = 1
	) : bench(_freq, (size_t)_xseed, vdupq_n_u8(_vseed), _rep) {
	}

	void print_pattern(FILE *fp, pattern_t const *p, size_t base = 0) {
		if(p == NULL || p->count == 0) {
			fprintf(fp, "%zu\n", base);
			return;
		}
		for(size_t i = 0; i < p->count; i++) {
			print_pattern(fp, p + 1, i * p->pitch + base);
		}
	}

	measure_t lat_(size_t line, op_t fp, double offset = 0.0, pattern_t const **q = lat_patterns) {
		(void)line;
		return((measure_t){
			.lat = run(fp, *q) - offset,
			.thr = 0.0
		});
	}
	measure_t thr_(size_t line, op_t fp, pattern_t const **q = thr_patterns) {
		return((measure_t){
			.lat = 0.0,
			.thr = 1.0 / run(fp, q, line)
		});
	}
	measure_t both_(
		size_t line,
		op_t fp_body,
		op_t fp_collect,
		double offset = 0.0,
		pattern_t const **lq = lat_patterns,
		pattern_t const **tq = thr_patterns
	) {
		return((measure_t){
			.lat = lat_(
				line,
				op_cap( fp_body(g, d, s); fp_collect(g, d, s) ),
				offset, lq
			).lat,
			.thr = thr_(line, fp_body, tq).thr
		});
	}
	measure_t both_(
		size_t line,
		op_t fp,
		double offset = 0.0,
		pattern_t const **lq = lat_patterns,
		pattern_t const **tq = thr_patterns
	) {
		return((measure_t){
			.lat = lat_(line, fp, offset, lq).lat,
			.thr = thr_(line, fp, tq).thr
		});
	}
};

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
	double const coef = 100000000.0;
	bench b(coef, (size_t)0, 0, 25);

	double r = 0.0;
	for(size_t i = 0; i < 3; i++) {
		r = b.lat_(0, op( g->add(d->x, s->x, 1) )).lat;
	}
	return(coef / ((double)ADD_LATENCY_CYCLES * r));
}


#endif	/* _UTILS_H_INCLUDED */
/*
 * end of utils.h
 */
