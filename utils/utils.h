
/**
 * @file utils.h
 * @brief CPU frequency and cycle measuring utils
 * @author Hajime Suzuki
 */
#ifndef _UTILS_H_INCLUDED
#define _UTILS_H_INCLUDED

/* make sure POSIX APIs are properly activated */
#if defined(__linux__) && !defined(_POSIX_C_SOURCE)
#  define _POSIX_C_SOURCE		200112L
#endif

#if (defined(__APPLE__) || defined(__darwin__)) && !defined(_BSD_SOURCE)
#  define _BSD_SOURCE			/* for sigsetjmp / siglongjmp on linux */
#endif
// #define _DEFAULT_SOURCE

/* C */
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>
#include <time.h>
#include <math.h>

/* C++ */
#include <functional>
#include <random>

/* arm */
#include <arm_neon.h>
#include "xbyak_aarch64.h"
using namespace Xbyak_aarch64;

/*
 * forward declarations for exported functions
 */
void init(bool md, size_t core);
void dump_uname_a(bool md);
void dump_cpuinfo(bool md);
double estimate_cpu_freq(bool md, size_t trials);

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

/* result returned in this */
struct measure_t {
	double lat;
	double thr;		/* reciprocal throughput */
};

/*
 * illegal instruction trapping
 */
extern jmp_buf jb;
#define try_setjmp(_body) ({ \
	measure_t const r = sigsetjmp(jb, 1) == 0 ? ( _body ) : (measure_t){ -1.0, -1.0 }; \
	/* measure_t const r = ( _body ); */ \
	r; \
})

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

	size_t rat_depth = 0;

	void thdr(void) {
		leader();
		printf("%sinstruction%slatency%sthroughput%s\n", seps[md][0], seps[md][1], seps[md][1], seps[md][2]);
	}
	void tbrk(void) {
		if(md) { printf("|--------|--------|--------|\n"); }
	}
	uint32_t gcd(uint32_t x, uint32_t y) {
		if(y == 0) { return(x); } else { return(gcd(y, x % y)); }
	}
	void estimate_rationel(double x) {
		struct rat_est_t {
			double z;
			uint32_t n;
			uint32_t d;
		};
		std::vector<struct rat_est_t> b;
		for(size_t i = 1; i < rat_depth; i++) {
			double const y = x * (double)i;
			double const z = std::abs(y - std::round(y));
			uint32_t const n = (uint32_t)std::round(y);
			uint32_t const d = (uint32_t)i;

			if(gcd(n, d) > 1) { continue; }
			b.push_back((struct rat_est_t){ .z = z, .n = n, .d = d });
		}
		std::sort(b.begin(), b.end(), [](struct rat_est_t const x, struct rat_est_t const y){
			return(x.z < y.z || x.d < y.d);
		});
		if(b[0].d == 1) { b.resize(1); }

		for(size_t i = 0; i < b.size(); i++) {
			if(i > 0 && i < b.size() - 1 && b[i + 1].z > b[0].z + 0.05) { break; }
			printf("%s%u/%u", &" (~ \0, "[i == 0 ? 0 : 5], b[i].n, b[i].d);
		}
		printf(")");
	}

public:
	table(
		bool _md,
		char const *title = NULL,
		size_t depth = 1,
		size_t _rat_depth = 0
	) : printer(_md, title, depth), rat_depth(_rat_depth) {
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
		if(rat_depth) { estimate_rationel(c.thr); }
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

static pattern_t const lat_flat0[] = {
	{ 144, 0, 0, 24 },
	{ 0, 0, 0, 0 }
};
static pattern_t const *lat_flat0_pattern[] = { (pattern_t const *)&lat_flat0, NULL };
static pattern_t const lat_flat1[] = {
	{ 144, 0, 1, 24 },
	{ 0, 0, 0, 0 }
};
static pattern_t const *lat_flat1_pattern[] = { (pattern_t const *)&lat_flat1, NULL };
static pattern_t const lat_flat2[] = {
	{ 144, 0, 2, 24 },
	{ 0, 0, 0, 0 }
};
static pattern_t const *lat_flat2_pattern[] = { (pattern_t const *)&lat_flat2, NULL };
static pattern_t const lat_flat3[] = {
	{ 144, 0, 3, 24 },
	{ 0, 0, 0, 0 }
};
static pattern_t const *lat_flat3_pattern[] = { (pattern_t const *)&lat_flat3, NULL };
static pattern_t const lat_half[] = {
	{ 144, 0, 0, 12 },
	{ 0, 0, 0, 0 }
};
static pattern_t const *lat_half_pattern[] = { (pattern_t const *)&lat_half, NULL };
static pattern_t const lat_inc0[] = {
	{ 5, 0, 0, 0 },
	{ 24, 1, 0, 24 },
	{ 0, 0, 0, 0 }
};
static pattern_t const *lat_inc0_pattern[] = { (pattern_t const *)&lat_inc0, NULL };
static pattern_t const lat_inc1[] = {
	{ 5, 0, 0, 0 },
	{ 24, 1, 1, 24 },
	{ 0, 0, 0, 0 }
};
static pattern_t const *lat_inc1_pattern[] = { (pattern_t const *)&lat_inc1, NULL };
static pattern_t const lat_inc2[] = {
	{ 10, 0, 0, 0 },
	{ 24, 2, 1, 24 },
	{ 0, 0, 0, 0 }
};
static pattern_t const *lat_inc2_pattern[] = { (pattern_t const *)&lat_inc2, NULL };
static pattern_t const lat_inc3[] = {
	// { 15, 0, 0, 0 },
	{ 10, 0, 0, 0 },
	{ 24, 3, 1, 24 },
	{ 0, 0, 0, 0 }
};
static pattern_t const *lat_inc3_pattern[] = { (pattern_t const *)&lat_inc3, NULL };
static pattern_t const lat_inc4[] = {
	{ 10, 0, 0, 0 },
	{ 24, 4, 1, 24 },
	{ 0, 0, 0, 0 }
};
static pattern_t const *lat_inc4_pattern[] = { (pattern_t const *)&lat_inc4, NULL };
static pattern_t const lat_inc5[] = {
	{ 10, 0, 0, 0 },
	{ 25, 5, 1, 25 },
	{ 0, 0, 0, 0 }
};
static pattern_t const *lat_inc5_pattern[] = { (pattern_t const *)&lat_inc5, NULL };
static pattern_t const lat_inc6[] = {
	{ 10, 0, 0, 0 },
	{ 24, 6, 1, 24 },
	{ 0, 0, 0, 0 }
};
static pattern_t const *lat_inc6_pattern[] = { (pattern_t const *)&lat_inc6, NULL };


/* default */
static pattern_t const *lat_patterns[] = {
	(pattern_t const *)&lat_flat0,
	(pattern_t const *)&lat_half,
	NULL
};

static pattern_t const *lat_flat_patterns[] = {
	(pattern_t const *)&lat_flat0,
	(pattern_t const *)&lat_flat1,
	(pattern_t const *)&lat_flat2,
	(pattern_t const *)&lat_flat3,
	NULL
};

#define table_thr(_w, _x, _y, _z, _p) { \
	{ (_z), 0, 0, 0 }, \
	{ (_x), (_w), 0, 0 }, \
	{ (_y), 0, 0, 0 }, \
	{ (_w), (_p), 0, (_w) * (_x) }, \
	{ 0, 0, 0, 0 } \
}
#define dec_thr(_w, _x, _y, _z, _p)		static pattern_t const thr_##_w##_##_x##_##_y##_##_z##_##_p[] = table_thr(_w, _x, _y, _z, _p)
#define ptr_thr(_w, _x, _y, _z, _p)		(pattern_t const *)&( thr_##_w##_##_x##_##_y##_##_z##_##_p )

#ifdef SCAN_PARAMS
dec_thr(3, 8, 1, 120, 1); dec_thr(4, 7, 1, 120, 1); dec_thr(5, 5, 1, 120, 1); dec_thr(6, 4, 1, 120, 1); dec_thr(7, 4, 1, 120, 1); dec_thr(8, 3, 1, 120, 1); dec_thr(9, 3, 1, 120, 1);
dec_thr(3, 8, 2, 60, 1);  dec_thr(4, 7, 2, 60, 1);  dec_thr(5, 5, 2, 60, 1);  dec_thr(6, 4, 2, 60, 1);  dec_thr(7, 4, 2, 60, 1);  dec_thr(8, 3, 2, 60, 1);  dec_thr(9, 3, 2, 60, 1);
dec_thr(3, 8, 3, 40, 1);  dec_thr(4, 7, 3, 40, 1);  dec_thr(5, 5, 3, 40, 1);  dec_thr(6, 4, 3, 40, 1);  dec_thr(7, 4, 3, 40, 1);  dec_thr(8, 3, 3, 40, 1);  dec_thr(9, 3, 3, 40, 1);
dec_thr(3, 8, 4, 30, 1);  dec_thr(4, 7, 4, 30, 1);  dec_thr(5, 5, 4, 30, 1);  dec_thr(6, 4, 4, 30, 1);  dec_thr(7, 4, 4, 30, 1);  dec_thr(8, 3, 4, 30, 1);  dec_thr(9, 3, 4, 30, 1);
dec_thr(3, 8, 5, 30, 1);  dec_thr(4, 7, 5, 30, 1);  dec_thr(5, 5, 5, 30, 1);  dec_thr(6, 4, 5, 30, 1);  dec_thr(7, 4, 5, 30, 1);  dec_thr(8, 3, 5, 30, 1);  dec_thr(9, 3, 5, 30, 1);
dec_thr(3, 8, 6, 20, 1);  dec_thr(4, 7, 6, 20, 1);  dec_thr(5, 5, 6, 20, 1);  dec_thr(6, 4, 6, 20, 1);  dec_thr(7, 4, 6, 20, 1);  dec_thr(8, 3, 6, 20, 1);  dec_thr(9, 3, 6, 20, 1);
#else
dec_thr(24, 1, 1, 120, 1);
dec_thr(25, 1, 1, 120, 1);
dec_thr(27, 1, 1, 120, 1);
// dec_thr(28, 1, 1, 120, 1);
dec_thr(6, 4, 3, 40, 1);
dec_thr(9, 3, 3, 40, 1);
dec_thr(6, 4, 4, 30, 1);
dec_thr(3, 7, 5, 30, 1);
// dec_thr(4, 7, 4, 30, 1);
dec_thr(6, 4, 5, 30, 1);
dec_thr(6, 4, 6, 20, 1);
// dec_thr(6, 4, 3, 40, 1);
// dec_thr(4, 7, 5, 30, 1);
#endif

static pattern_t const *thr_patterns[] = {
#ifdef SCAN_PARAMS
	ptr_thr(3, 8, 1, 120, 1), ptr_thr(4, 7, 1, 120, 1), ptr_thr(5, 5, 1, 120, 1), ptr_thr(6, 4, 1, 120, 1), ptr_thr(7, 4, 1, 120, 1), ptr_thr(8, 3, 1, 120, 1), ptr_thr(9, 3, 1, 120, 1),
	ptr_thr(3, 8, 2, 60, 1),  ptr_thr(4, 7, 2, 60, 1),  ptr_thr(5, 5, 2, 60, 1),  ptr_thr(6, 4, 2, 60, 1),  ptr_thr(7, 4, 2, 60, 1),  ptr_thr(8, 3, 2, 60, 1),  ptr_thr(9, 3, 2, 60, 1),
	ptr_thr(3, 8, 3, 40, 1),  ptr_thr(4, 7, 3, 40, 1),  ptr_thr(5, 5, 3, 40, 1),  ptr_thr(6, 4, 3, 40, 1),  ptr_thr(7, 4, 3, 40, 1),  ptr_thr(8, 3, 3, 40, 1),  ptr_thr(9, 3, 3, 40, 1),
	ptr_thr(3, 8, 4, 30, 1),  ptr_thr(4, 7, 4, 30, 1),  ptr_thr(5, 5, 4, 30, 1),  ptr_thr(6, 4, 4, 30, 1),  ptr_thr(7, 4, 4, 30, 1),  ptr_thr(8, 3, 4, 30, 1),  ptr_thr(9, 3, 4, 30, 1),
	ptr_thr(3, 8, 5, 30, 1),  ptr_thr(4, 7, 5, 30, 1),  ptr_thr(5, 5, 5, 30, 1),  ptr_thr(6, 4, 5, 30, 1),  ptr_thr(7, 4, 5, 30, 1),  ptr_thr(8, 3, 5, 30, 1),  ptr_thr(9, 3, 5, 30, 1),
	ptr_thr(3, 8, 6, 20, 1),  ptr_thr(4, 7, 6, 20, 1),  ptr_thr(5, 5, 6, 20, 1),  ptr_thr(6, 4, 6, 20, 1),  ptr_thr(7, 4, 6, 20, 1),  ptr_thr(8, 3, 6, 20, 1),  ptr_thr(9, 3, 6, 20, 1),
#else
	ptr_thr(24, 1, 1, 120, 1),
	ptr_thr(25, 1, 1, 120, 1),
	ptr_thr(27, 1, 1, 120, 1),
	// ptr_thr(28, 1, 1, 120, 1),
	ptr_thr(6, 4, 3, 40, 1),
	ptr_thr(9, 3, 3, 40, 1),
	ptr_thr(6, 4, 6, 20, 1),
	ptr_thr(6, 4, 5, 30, 1),
	ptr_thr(6, 4, 4, 30, 1),
	ptr_thr(3, 7, 5, 30, 1),
	// ptr_thr(4, 7, 4, 30, 1),
	// ptr_thr(6, 4, 3, 40, 1),
	// ptr_thr(4, 7, 5, 30, 1),
#endif
	NULL
};
static pattern_t const **thr_skip1_patterns  = thr_patterns;
static pattern_t const **thr_skip1x_patterns = thr_patterns;

/* for instructions with four-bit operand fields */
dec_thr(4, 3, 1, 120, 1);
dec_thr(6, 2, 3, 40, 1);
dec_thr(3, 4, 1, 120, 1);
dec_thr(4, 3, 3, 40, 1);
dec_thr(7, 2, 1, 120, 1);
dec_thr(6, 2, 6, 20, 1);
dec_thr(6, 2, 5, 30, 1);
dec_thr(6, 2, 4, 30, 1);

static pattern_t const *thr_half_patterns[] = {
	ptr_thr(4, 3, 1, 120, 1),
	ptr_thr(6, 2, 3, 40, 1),
	ptr_thr(3, 4, 1, 120, 1),
	ptr_thr(4, 3, 3, 40, 1),
	ptr_thr(7, 2, 1, 120, 1),
	ptr_thr(6, 2, 6, 20, 1),
	ptr_thr(6, 2, 5, 30, 1),
	ptr_thr(6, 2, 4, 30, 1),
	NULL
};

/* for ldp and ld{2,3,4} */
dec_thr(21, 1, 1, 120, 2);
dec_thr(22, 1, 1, 120, 2);
dec_thr(24, 1, 1, 120, 2);
dec_thr(25, 1, 1, 120, 2);
dec_thr(26, 1, 1, 120, 2);
dec_thr(27, 1, 1, 120, 2);
// dec_thr(28, 1, 1, 120, 2);

static pattern_t const *thr_skip2_patterns[] = {
	ptr_thr(21, 1, 1, 120, 2),
	ptr_thr(24, 1, 1, 120, 2),
	ptr_thr(25, 1, 1, 120, 2),
	ptr_thr(27, 1, 1, 120, 2),
	// ptr_thr(28, 1, 1, 120, 2),
	NULL
};
static pattern_t const *thr_skip2x_patterns[] = {
	ptr_thr(22, 1, 1, 120, 2),
	ptr_thr(24, 1, 1, 120, 2),
	ptr_thr(26, 1, 1, 120, 2),
	// ptr_thr(28, 1, 1, 120, 2),
	NULL
};

dec_thr(21, 1, 1, 120, 3);
dec_thr(24, 1, 1, 120, 3);
dec_thr(25, 1, 1, 120, 3);
dec_thr(27, 1, 1, 120, 3);
// dec_thr(28, 1, 1, 120, 3);

static pattern_t const *thr_skip3_patterns[] = {
	ptr_thr(21, 1, 1, 120, 3),
	ptr_thr(24, 1, 1, 120, 3),
	ptr_thr(25, 1, 1, 120, 3),
	ptr_thr(27, 1, 1, 120, 3),
	// ptr_thr(28, 1, 1, 120, 3),
	NULL
};
static pattern_t const *thr_skip3x_patterns[] = {
	ptr_thr(21, 1, 1, 120, 3),
	ptr_thr(24, 1, 1, 120, 3),
	ptr_thr(27, 1, 1, 120, 3),
	NULL
};

dec_thr(20, 1, 1, 120, 4);
dec_thr(21, 1, 1, 120, 4);
dec_thr(24, 1, 1, 120, 4);
dec_thr(25, 1, 1, 120, 4);
dec_thr(27, 1, 1, 120, 4);
// dec_thr(28, 1, 1, 120, 4);

static pattern_t const *thr_skip4_patterns[] = {
	ptr_thr(21, 1, 1, 120, 4),
	ptr_thr(24, 1, 1, 120, 4),
	ptr_thr(25, 1, 1, 120, 4),
	ptr_thr(27, 1, 1, 120, 4),
	// ptr_thr(28, 1, 1, 120, 4),
	NULL
};
static pattern_t const *thr_skip4x_patterns[] = {
	ptr_thr(20, 1, 1, 120, 4),
	ptr_thr(24, 1, 1, 120, 4),
	// ptr_thr(28, 1, 1, 120, 4),
	NULL
};

dec_thr(20, 1, 1, 120, 5);
dec_thr(25, 1, 1, 120, 5);
static pattern_t const *thr_skip5x_patterns[] = {
	ptr_thr(20, 1, 1, 120, 5),
	ptr_thr(25, 1, 1, 120, 5),
	NULL
};

dec_thr(18, 1, 1, 120, 6);
dec_thr(24, 1, 1, 120, 6);
static pattern_t const *thr_skip6x_patterns[] = {
	ptr_thr(18, 1, 1, 120, 6),
	ptr_thr(24, 1, 1, 120, 6),
	NULL
};

dec_thr(14, 1, 1, 120, 7);
dec_thr(21, 1, 1, 120, 7);
// dec_thr(28, 1, 1, 120, 7);
static pattern_t const *thr_skip7x_patterns[] = {
	ptr_thr(14, 1, 1, 120, 7),
	ptr_thr(21, 1, 1, 120, 7),
	// ptr_thr(28, 1, 1, 120, 7),
	NULL
};

dec_thr(16, 1, 1, 120, 8);
dec_thr(24, 1, 1, 120, 8);
static pattern_t const *thr_skip8x_patterns[] = {
	ptr_thr(16, 1, 1, 120, 8),
	ptr_thr(24, 1, 1, 120, 8),
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
	AReg(AReg const &_a) : w(_a.w), x(_a.x), v(_a.v), b(_a.b), h(_a.h), s(_a.s), d(_a.d), q(_a.q) {
	}
	AReg &operator=(AReg const &a) {
		w = a.w; x = a.x; v = a.v; b = a.b; h = a.h; s = a.s; d = a.d; q = a.q;
		return(*this);
	}

	WReg w;
	XReg x;
	VReg v;
	BReg b;
	HReg h;
	SReg s;
	DReg d;
	QReg q;
};

/* op generation function template and helper */
// typedef void (*op_t)(CodeGenerator *g, AReg const *r);
typedef std::function<size_t (CodeGenerator *, AReg const *, AReg const *, Label *)> op_t;
#define op(_body)			( [ ](CodeGenerator *g, AReg const *d, AReg const *s, Label *gl) -> size_t { (void)g; (void)d; (void)s; (void)gl; _body; return(1); } )
#define op_cap(_body)		( [&](CodeGenerator *g, AReg const *d, AReg const *s, Label *gl) -> size_t { (void)g; (void)d; (void)s; (void)gl; _body; return(1); } )

typedef std::function<void (CodeGenerator *, Label *)> op_init_t;
#define op_init(_body)		( [](CodeGenerator *g, Label *gl) { (void)g; (void)gl; _body; } )

/* measure only latency */
#define lat_i(_freq, _op) ({ \
	bench _b(_freq); \
	double const _l = _b.lat_(0, _op).lat; \
	(_l); \
})

/*
 * provides prologue and epilogue for benchmarking
 */
class bench : public CodeGenerator {
private:
	/* for computing cycles per instruction */
	double const freq = 0.0;

	/* constants passed to the core loop */
	size_t const xseed  = 0;
	size_t const xconst = 0;
	uint8x16_t const vseed  = vdupq_n_u8(0);
	uint8x16_t const vconst = vdupq_n_u8(0);

	/* helper SIMD-scalar registers */
	#define a(_n, _m)	AReg(w##_n, x##_n, v##_m, b##_m, h##_m, s##_m, d##_m, q##_m)
	AReg regs[36] = {
		a(0, 0),   a(1, 1),   a(2, 2),   a(3, 3),   a(4, 4),   a(5, 5),   a(6, 6),   a(7, 7),
		a(8, 8),   a(9, 9),   a(10, 10), a(11, 11), a(12, 12), a(13, 13), a(14, 14), a(15, 15),
		a(16, 16), a(17, 17), a(19, 18), a(20, 19), a(21, 20), a(22, 21), a(23, 22),
		a(24, 23), a(25, 24), a(26, 25), a(27, 26),
		a(0, 27),  a(1, 28),  a(2, 29),  a(3, 30),  a(4, 31),  a(5, 0),   a(6, 1),   a(7, 2),
		a(8, 3)
	};
	AReg const regs_rot[36] = {
		a(0, 0),   a(1, 1),   a(2, 2),   a(3, 3),   a(4, 4),   a(5, 5),   a(6, 6),   a(7, 7),
		a(8, 8),   a(9, 9),   a(10, 10), a(11, 11), a(12, 12), a(13, 13), a(14, 14), a(15, 15),
		a(16, 16), a(17, 17), a(19, 18), a(20, 19), a(21, 20), a(22, 21), a(23, 22),
		a(24, 23), a(25, 24), a(26, 25), a(27, 26),
		a(0, 27),  a(1, 28),  a(2, 29),  a(3, 30),  a(4, 31),  a(5, 0),   a(6, 1),   a(7, 2),
		a(8, 3)
	};
	AReg const regs_all[31] = {
		a(0, 0),   a(1, 1),   a(2, 2),   a(3, 3),   a(4, 4),   a(5, 5),   a(6, 6),   a(7, 7),
		a(8, 8),   a(9, 9),   a(10, 10), a(11, 11), a(12, 12), a(13, 13), a(14, 14), a(15, 15),
		a(16, 16), a(17, 17), a(18, 18), a(19, 19), a(20, 20), a(21, 21), a(22, 22), a(23, 23),
		a(24, 24), a(25, 25), a(26, 26), a(27, 27), a(28, 28), a(29, 29), a(30, 30)
	};
	#undef a

	/* prologue and epilogue */
	static size_t const stack_size = (size_t)32 * 16 + 32 * 8;

	void head() {
		/* extend stack */
		sub(sp, sp, stack_size);

		/* save everything */
		for(size_t i = 0; i < 31; i++) {
			str(regs_all[i].q, ptr(sp, (uint32_t)(stack_size - (i + 1) * 16 - 32 * 8)));
			str(regs_all[i].x, ptr(sp, (uint32_t)(stack_size - (i + 1) * 8)));
		}
		return;
	}
	void seed() {
		/* initialize constant registers */
		mov(x28, x1);
		mov(v28.b16, v1.b16);
		mov(v29.b16, v1.b16);
		mov(v30.b16, v1.b16);
		mov(v31.b16, v1.b16);

		/* initialize seed registers */
		for(size_t i = 1; i < 27; i++) {
			mov(regs[i].v.b16, v0.b16);
			mov(regs[i].x, x0);
		}
		return;
	}
	void tail(void) {
		/* load everything */
		for(size_t i = 0; i < 31; i++) {
			ldr(regs_all[i].q, ptr(sp, (uint32_t)(stack_size - (i + 1) * 16 - 32 * 8)));
			ldr(regs_all[i].x, ptr(sp, (uint32_t)(stack_size - (i + 1) * 8)));
		}

		/* unwind stack and return */
		add(sp, sp, stack_size);
		ret();
		return;
	}

	/* pattern generator */
	static size_t const n_insns_tot_base = (size_t)4 * 21 * 900 * 1001;
	size_t const n_insns_tot  = n_insns_tot_base;
	size_t const n_insns_body = 1;

	/* pattern: (count, pitch, offset, mod) */
	size_t count_insns(pattern_t const *p) {
		if(p == NULL || p->count == 0) { return(n_insns_body); }
		return(p->count * count_insns(p + 1));
	}
	size_t expand(op_t fp, Label *l, pattern_t const *p, size_t base) {
		if(p == NULL || p->count == 0) {
			// assert(base < 30);
			size_t const offset = p[-1].offset;
			size_t const mod = p[-1].mod;
			fp(this, &regs[(base + offset) % mod], &regs[base % mod], l);
			return(0);
		}
		for(size_t i = 0; i < p->count; i++) {
			expand(fp, l, p + 1, i * p->pitch + base);
		}
		return(0);
	}
	size_t expand(op_t fp, op_init_t fp_clear, Label *l, pattern_t const *p) {
		/* loop counter; roundup fractions */
		size_t const count  = count_insns(p);
		size_t const n_iter = (n_insns_tot + count - 1) / count;
		mov(x29, n_iter);

		/* core loop */
		align(128);
		Label loop = L();
		{
			fp_clear(this, l);
			expand(fp, l, p, 0);
			subs(x29, x29, 1);
			bne(loop);
		}
		return(count * n_iter);
	}
	void init_reglist(pattern_t const *p) {
		if(p == NULL) { return; }
		while(p->count != 0) { p++; }

		size_t const mod = p[-1].mod;
		for(size_t i = 0; i < 36; i++) {
			regs[i] = regs_rot[i % mod];
		}
	}
	double run(op_t fp_body, op_init_t fp_init, op_init_t fp_clear, pattern_t const *p) {
		/* initialize reglist */
		init_reglist(p);

		/* flush code area */
		reset(); setProtectModeRW();

		/* save registers, core loop, and restore registers */
		Label l;
		head(); fp_init(this, &l); seed();
		size_t const n_insns = expand(fp_body, fp_clear, &l, p);
		tail(); ready();

		/* generate the code for the loop; then run twice. first is for warming up. */
		auto fn = getCode<void (*)(size_t xs, size_t xc, uint8x16_t vs, uint8x16_t vc)>();

		/* warmup */
		fn(xseed, xconst, vseed, vconst);

		/* measure */
		size_t const ns = measure_nsec({ fn(xseed, xconst, vseed, vconst); });
		return(
			  ((double)ns * freq)
			/ ((double)n_insns * 1000000000.0)
		);
	}
	double run(op_t fp_body, op_init_t fp_init, op_init_t fp_clear, pattern_t const **q, size_t line = 0) {
		double res = 1000000000.0;
		while(*q != NULL) {
			double const r = run(fp_body, fp_init, fp_clear, *q++);
			if(line != 0) {
				printf("#%zu\t%zu_%zu_%zu_%zu\t%.3f\n", line, q[-1][3].count, q[-1][1].count, q[-1][2].count, q[-1][0].count, 1.0 / r);
			}
			res = std::min(res, r);
		}
		return(res);
	}

public:
	/* global label for creating subroutine */
	Label glabel;

	/* constructors */
	bench(
		double const &_freq,
		size_t const &_xseed,
		size_t const &_xconst,
		uint8x16_t const &_vseed,
		uint8x16_t const &_vconst,
		size_t const &_rep,
		size_t const &_div,
		size_t const &_n_insns_body
	) : CodeGenerator(4 * 1024 * 1024),
		freq(_freq),
		xseed(_xseed),
		xconst(_xconst),
		vseed(_vseed),
		vconst(_vconst),
		n_insns_tot((_rep * n_insns_tot_base) / (_div * _n_insns_body)),
		n_insns_body(_n_insns_body) {
	}
	bench(
		double const &_freq,
		size_t const &_xseed = 1,
		size_t const &_xconst = 1,
		uint8_t const &_vseed = 1,
		uint8_t const &_vconst = 1,
		size_t const &_rep = 1,
		size_t const &_div = 1,
		size_t const &_n_insns_body = 1
	) : bench(_freq, _xseed, _xconst, vdupq_n_u8(_vseed), vdupq_n_u8(_vconst), _rep, _div, _n_insns_body) {
	}
	bench(
		double const &_freq,
		void *const &_xseed,
		void *const &_xconst = NULL,
		uint8_t const &_vseed = 1,
		uint8_t const &_vconst = 1,
		size_t const &_rep = 1,
		size_t const &_div = 1,
		size_t const &_n_insns_body = 1
	) : bench(_freq, (size_t)_xseed, (size_t)_xconst, vdupq_n_u8(_vseed), vdupq_n_u8(_vconst), _rep, _div, _n_insns_body) {
	}

	void print_pattern(FILE *fp, pattern_t const *p, size_t base = 0) {
		if(p == NULL || p->count == 0) {
			size_t const offset = p[-1].offset;
			size_t const mod = p[-1].mod;
			fprintf(fp, "%zu -> %zu\n", base % mod, (base + offset) % mod);
			return;
		}
		for(size_t i = 0; i < p->count; i++) {
			print_pattern(fp, p + 1, i * p->pitch + base);
		}
	}

	measure_t lat_(size_t line, op_t fp_body,                                        double offset = 0.0, pattern_t const **q = lat_patterns) { return(lat_(line, fp_body, op_init(), op_init(), offset, q)); }
	measure_t lat_(size_t line, op_t fp_body, op_init_t fp_init,                     double offset = 0.0, pattern_t const **q = lat_patterns) { return(lat_(line, fp_body, fp_init,   op_init(), offset, q)); }
	measure_t lat_(size_t line, op_t fp_body, op_init_t fp_init, op_init_t fp_clear, double offset = 0.0, pattern_t const **q = lat_patterns) {
		(void)line;
		return((measure_t){
			.lat = run(fp_body, fp_init, fp_clear, q) - offset,
			.thr = 0.0
		});
	}

	measure_t thr_(size_t line, op_t fp_body,                                        pattern_t const **q = thr_patterns) { return(thr_(line, fp_body, op_init(), op_init(), q)); }
	measure_t thr_(size_t line, op_t fp_body, op_init_t fp_init,                     pattern_t const **q = thr_patterns) { return(thr_(line, fp_body, fp_init,   op_init(), q)); }
	measure_t thr_(size_t line, op_t fp_body, op_init_t fp_init, op_init_t fp_clear, pattern_t const **q = thr_patterns) {
		(void)line;
		return((measure_t){
			.lat = 0.0,
			.thr = 1.0 / run(fp_body, fp_init, fp_clear, q, line)
		});
	}

	measure_t both_(size_t line, op_t fp_body,                                                         double offset = 0.0, pattern_t const **lq = lat_patterns, pattern_t const **tq = thr_patterns) { return(both_(line, fp_body, op(),       op_init(), op_init(), offset, lq, tq)); }
	measure_t both_(size_t line, op_t fp_body,                  op_init_t fp_init,                     double offset = 0.0, pattern_t const **lq = lat_patterns, pattern_t const **tq = thr_patterns) { return(both_(line, fp_body, op(),       fp_init,   op_init(), offset, lq, tq)); }
	measure_t both_(size_t line, op_t fp_body,                  op_init_t fp_init, op_init_t fp_clear, double offset = 0.0, pattern_t const **lq = lat_patterns, pattern_t const **tq = thr_patterns) { return(both_(line, fp_body, op(),       fp_init,   fp_clear,  offset, lq, tq)); }
	measure_t both_(size_t line, op_t fp_body, op_t fp_collect,                                        double offset = 0.0, pattern_t const **lq = lat_patterns, pattern_t const **tq = thr_patterns) { return(both_(line, fp_body, fp_collect, op_init(), op_init(), offset, lq, tq)); }
	measure_t both_(size_t line, op_t fp_body, op_t fp_collect, op_init_t fp_init, op_init_t fp_clear, double offset = 0.0, pattern_t const **lq = lat_patterns, pattern_t const **tq = thr_patterns) {
		return((measure_t){
			.lat = lat_(
				line,
				op_cap( fp_body(g, d, s, gl); fp_collect(g, d, s, gl) ),
				fp_init, fp_clear,
				offset, lq
			).lat,
			.thr = thr_(line, fp_body, fp_init, fp_clear, tq).thr
		});
	}
};

/* memory manager for load / store bench */
class memmgr;		/* forward declaration */

typedef void *(*mem_init_t)(memmgr *g, void **p, size_t i, size_t n);
#define mem_init(_body) [](memmgr *g, void **p, size_t i, size_t n) -> void * { (void)g; (void)p; (void)i; (void)n; return( _body ); }

#ifdef __APPLE__
#  define PAGE_SIZE			( 16 * 1024 )
#  define CACHE_LINE_SIZE	( 128 )
#else
#  define PAGE_SIZE			( 4 * 1024 )
#  define CACHE_LINE_SIZE	( 64 )
#endif

class memmgr {
private:
	uint8_t *base;
	size_t offset;
	static size_t const page_size = PAGE_SIZE;

	/* random source */
	std::mt19937_64 mt;

	uint8_t *aligned_malloc(size_t size, size_t align) {
		void *p = NULL;
		if(posix_memalign(&p, align, size)) { return(NULL); }
		return((uint8_t *)p);
	}
public:
	memmgr(
		mem_init_t fp,
		size_t const &_offset = 0,
		size_t const &n_pages = 64,
		uint64_t const &_rseed = 42
	) : base(NULL), offset(_offset), mt(_rseed) {

		size_t const size = n_pages * page_size;
		base = aligned_malloc(size + page_size, page_size);		/* add margin for offset */
		memset(base, 0, size + page_size);

		/* note: p might be unaligned due to the offset */
		void **p = ptr();
		for(size_t i = 0; i < size / sizeof(void *); i++) {
			p[i] = fp(this, p, i, size);
		}
	}
	~memmgr() {
		free(base);
	}
	void **ptr() {
		if(base == NULL) { return(NULL); }
		return((void **)&base[offset]);
	}
	void *rnd() {
		return((void *)mt());
	}
};

#endif	/* _UTILS_H_INCLUDED */
/*
 * end of utils.h
 */
