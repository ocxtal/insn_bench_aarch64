
/*
 * @file cmp.cc
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>

/* for std::max */
#include <algorithm>

void bench_cmp(bool md, double freq) {
	table t(md, "Scalar integer compare and flag manipulation");
	bench b(freq);

	double const adcs_latency = lat_i(freq, op( g->adcs(d->x, d->x, s->x) ));
	#define thr_body(_i, _body) ({ \
		thr(b, op( g->adds(s->x, s->x, g->x28); for(size_t j = 0; j < (_i); j++) { _body; } )).thr * (double)(_i); \
	})
	#define sweep(_body) ({ \
		measure_t const _l = lat(b, op( g->adcs(s->x, s->x, g->x28); _body; ), adcs_latency); \
		double const _t = std::max({ \
			thr_body(1, _body), thr_body(2, _body), thr_body(3, _body), thr_body(4, _body), \
			thr_body(5, _body), thr_body(6, _body), thr_body(7, _body), thr_body(8, _body) \
		}); \
		(measure_t){ .lat = _l.lat, .thr = _t }; \
	})

	t.put("ccmn (reg; eq)",             sweep( g->ccmn(s->x, g->x28, 0xf, Cond::EQ) ));
	t.put("ccmn (reg; lt)",             sweep( g->ccmn(s->x, g->x28, 0xf, Cond::LT) ));
	t.put("ccmn (imm; eq)",             sweep( g->ccmn(s->x, 17, 0xf, Cond::EQ) ));
	t.put("ccmn (imm; lt)",             sweep( g->ccmn(s->x, 17, 0xf, Cond::LT) ));
	t.put("ccmp (reg; eq)",             sweep( g->ccmp(s->x, g->x28, 0xf, Cond::EQ) ));
	t.put("ccmp (reg; lt)",             sweep( g->ccmp(s->x, g->x28, 0xf, Cond::LT) ));
	t.put("ccmp (imm; eq)",             sweep( g->ccmp(s->x, 17, 0xf, Cond::EQ) ));
	t.put("ccmp (imm; lt)",             sweep( g->ccmp(s->x, 17, 0xf, Cond::LT) ));

	t.put("tst (reg)",                  sweep( g->tst(s->x, s->x) ));
	t.put("tst (reg<<2)",               sweep( g->tst(s->x, s->x, ShMod::LSL, 2) ));
	t.put("tst (imm)",                  sweep( g->tst(s->x, 0x1ffc) ));

	t.put("rmif",                       sweep( g->rmif(d->x, 17, 0xf) ));
	t.put("setf8",                      sweep( g->setf8(d->w) ));
	t.put("setf16",                     sweep( g->setf16(d->w) ));
	t.put("cfinv",                      sweep( g->cfinv() ));
	// t.put("axflag",                     sweep( g->axflag() ));
	// t.put("xaflag",                     sweep( g->xaflag() ));

	#undef thr_body
	#undef sweep
	return;
}

void bench_cmp_vec(bool md, double freq) {
	table t(md, "Vector integer compare");
	bench b(freq);

	t.put("cmeq.b (reg)",               both(b, op( g->cmeq(d->v.b, d->v.b, s->v.b) )));
	t.put("cmeq.h (reg)",               both(b, op( g->cmeq(d->v.h, d->v.h, s->v.h) )));
	t.put("cmeq.s (reg)",               both(b, op( g->cmeq(d->v.s, d->v.s, s->v.s) )));
	t.put("cmeq.d (reg)",               both(b, op( g->cmeq(d->v.d, d->v.d, s->v.d) )));

	t.put("cmeq.b (zero)",              both(b, op( g->cmeq(d->v.b, s->v.b, 0) )));
	t.put("cmeq.h (zero)",              both(b, op( g->cmeq(d->v.h, s->v.h, 0) )));
	t.put("cmeq.s (zero)",              both(b, op( g->cmeq(d->v.s, s->v.s, 0) )));
	t.put("cmeq.d (zero)",              both(b, op( g->cmeq(d->v.d, s->v.d, 0) )));

	t.put("cmge.b (reg)",               both(b, op( g->cmge(d->v.b, d->v.b, s->v.b) )));
	t.put("cmge.h (reg)",               both(b, op( g->cmge(d->v.h, d->v.h, s->v.h) )));
	t.put("cmge.s (reg)",               both(b, op( g->cmge(d->v.s, d->v.s, s->v.s) )));
	t.put("cmge.d (reg)",               both(b, op( g->cmge(d->v.d, d->v.d, s->v.d) )));
	t.put("cmge.b (zero)",              both(b, op( g->cmge(d->v.b, s->v.b, 0) )));
	t.put("cmge.h (zero)",              both(b, op( g->cmge(d->v.h, s->v.h, 0) )));
	t.put("cmge.s (zero)",              both(b, op( g->cmge(d->v.s, s->v.s, 0) )));
	t.put("cmge.d (zero)",              both(b, op( g->cmge(d->v.d, s->v.d, 0) )));

	t.put("cmgt.b (reg)",               both(b, op( g->cmgt(d->v.b, d->v.b, s->v.b) )));
	t.put("cmgt.h (reg)",               both(b, op( g->cmgt(d->v.h, d->v.h, s->v.h) )));
	t.put("cmgt.s (reg)",               both(b, op( g->cmgt(d->v.s, d->v.s, s->v.s) )));
	t.put("cmgt.d (reg)",               both(b, op( g->cmgt(d->v.d, d->v.d, s->v.d) )));
	t.put("cmgt.b (zero)",              both(b, op( g->cmgt(d->v.b, s->v.b, 0) )));
	t.put("cmgt.h (zero)",              both(b, op( g->cmgt(d->v.h, s->v.h, 0) )));
	t.put("cmgt.s (zero)",              both(b, op( g->cmgt(d->v.s, s->v.s, 0) )));
	t.put("cmgt.d (zero)",              both(b, op( g->cmgt(d->v.d, s->v.d, 0) )));

	t.put("cmle.b (zero)",              both(b, op( g->cmle(d->v.b, s->v.b, 0) )));
	t.put("cmle.h (zero)",              both(b, op( g->cmle(d->v.h, s->v.h, 0) )));
	t.put("cmle.s (zero)",              both(b, op( g->cmle(d->v.s, s->v.s, 0) )));
	t.put("cmle.d (zero)",              both(b, op( g->cmle(d->v.d, s->v.d, 0) )));

	t.put("cmlt.b (zero)",              both(b, op( g->cmlt(d->v.b, s->v.b, 0) )));
	t.put("cmlt.h (zero)",              both(b, op( g->cmlt(d->v.h, s->v.h, 0) )));
	t.put("cmlt.s (zero)",              both(b, op( g->cmlt(d->v.s, s->v.s, 0) )));
	t.put("cmlt.d (zero)",              both(b, op( g->cmlt(d->v.d, s->v.d, 0) )));

	t.put("cmhi.b (reg)",               both(b, op( g->cmhi(d->v.b, d->v.b, s->v.b) )));
	t.put("cmhi.h (reg)",               both(b, op( g->cmhi(d->v.h, d->v.h, s->v.h) )));
	t.put("cmhi.s (reg)",               both(b, op( g->cmhi(d->v.s, d->v.s, s->v.s) )));
	t.put("cmhi.d (reg)",               both(b, op( g->cmhi(d->v.d, d->v.d, s->v.d) )));

	t.put("cmhs.b (reg)",               both(b, op( g->cmhs(d->v.b, d->v.b, s->v.b) )));
	t.put("cmhs.h (reg)",               both(b, op( g->cmhs(d->v.h, d->v.h, s->v.h) )));
	t.put("cmhs.s (reg)",               both(b, op( g->cmhs(d->v.s, d->v.s, s->v.s) )));
	t.put("cmhs.d (reg)",               both(b, op( g->cmhs(d->v.d, d->v.d, s->v.d) )));

	t.put("cmtst.b (reg)",              both(b, op( g->cmtst(d->v.b, d->v.b, s->v.b) )));
	t.put("cmtst.h (reg)",              both(b, op( g->cmtst(d->v.h, d->v.h, s->v.h) )));
	t.put("cmtst.s (reg)",              both(b, op( g->cmtst(d->v.s, d->v.s, s->v.s) )));
	t.put("cmtst.d (reg)",              both(b, op( g->cmtst(d->v.d, d->v.d, s->v.d) )));
	return;
}

void bench_cmp_fp_vec(bool md, double freq) {
	table t(md, "Floating point compare");
	bench b(freq);

	t.put("facge.h (scl)",              both(b, op( g->facge(d->h, d->h, s->h) )));
	t.put("facge.s (scl)",              both(b, op( g->facge(d->s, d->s, s->s) )));
	t.put("facge.d (scl)",              both(b, op( g->facge(d->d, d->d, s->d) )));
	t.put("facge.h (vec)",              both(b, op( g->facge(d->v.h, d->v.h, s->v.h) )));
	t.put("facge.s (vec)",              both(b, op( g->facge(d->v.s, d->v.s, s->v.s) )));
	t.put("facge.d (vec)",              both(b, op( g->facge(d->v.d, d->v.d, s->v.d) )));

	t.put("facgt.h (scl)",              both(b, op( g->facgt(d->h, d->h, s->h) )));
	t.put("facgt.s (scl)",              both(b, op( g->facgt(d->s, d->s, s->s) )));
	t.put("facgt.d (scl)",              both(b, op( g->facgt(d->d, d->d, s->d) )));
	t.put("facgt.h (vec)",              both(b, op( g->facgt(d->v.h, d->v.h, s->v.h) )));
	t.put("facgt.s (vec)",              both(b, op( g->facgt(d->v.s, d->v.s, s->v.s) )));
	t.put("facgt.d (vec)",              both(b, op( g->facgt(d->v.d, d->v.d, s->v.d) )));

	double const fcsel_latency = lat_i(freq, op( g->fcsel(d->d, d->d, s->d, Cond::EQ) ));
	t.put("fcmp.h (reg)",               both(b, op( g->fcmp(s->h, s->h) ),  op( g->fcsel(d->h, d->h, s->h, Cond::EQ) ), fcsel_latency));
	t.put("fcmp.h (zero)",              both(b, op( g->fcmp(s->h, 0.0) ),   op( g->fcsel(d->h, d->h, s->h, Cond::EQ) ), fcsel_latency));
	t.put("fcmp.s (reg)",               both(b, op( g->fcmp(s->s, s->s) ),  op( g->fcsel(d->s, d->s, s->s, Cond::EQ) ), fcsel_latency));
	t.put("fcmp.s (zero)",              both(b, op( g->fcmp(s->s, 0.0) ),   op( g->fcsel(d->s, d->s, s->s, Cond::EQ) ), fcsel_latency));
	t.put("fcmp.d (reg)",               both(b, op( g->fcmp(s->d, s->d) ),  op( g->fcsel(d->d, d->d, s->d, Cond::EQ) ), fcsel_latency));
	t.put("fcmp.d (zero)",              both(b, op( g->fcmp(s->d, 0.0) ),   op( g->fcsel(d->d, d->d, s->d, Cond::EQ) ), fcsel_latency));
	t.put("fcmpe.h (reg)",              both(b, op( g->fcmpe(s->h, s->h) ), op( g->fcsel(d->h, d->h, s->h, Cond::EQ) ), fcsel_latency));
	t.put("fcmpe.h (zero)",             both(b, op( g->fcmpe(s->h, 0.0) ),  op( g->fcsel(d->h, d->h, s->h, Cond::EQ) ), fcsel_latency));
	t.put("fcmpe.s (reg)",              both(b, op( g->fcmpe(s->s, s->s) ), op( g->fcsel(d->s, d->s, s->s, Cond::EQ) ), fcsel_latency));
	t.put("fcmpe.s (zero)",             both(b, op( g->fcmpe(s->s, 0.0) ),  op( g->fcsel(d->s, d->s, s->s, Cond::EQ) ), fcsel_latency));
	t.put("fcmpe.d (reg)",              both(b, op( g->fcmpe(s->d, s->d) ), op( g->fcsel(d->d, d->d, s->d, Cond::EQ) ), fcsel_latency));
	t.put("fcmpe.d (zero)",             both(b, op( g->fcmpe(s->d, 0.0) ),  op( g->fcsel(d->d, d->d, s->d, Cond::EQ) ), fcsel_latency));

	/* chained comp */
	#define thr_body(_i, _body) ({ \
		thr(b, op( g->adds(s->x, s->x, g->x28); for(size_t j = 0; j < (_i); j++) { _body; } )).thr * (double)(_i); \
	})
	#define sweep(_body) ({ \
		double const _l = lat(b, op( _body; g->fcsel(d->d, d->d, s->d, Cond::EQ) ), fcsel_latency).lat; \
		double const _t = std::max({ \
			thr_body(1, _body), thr_body(2, _body), thr_body(3, _body), thr_body(4, _body), \
			thr_body(5, _body), thr_body(6, _body), thr_body(7, _body), thr_body(8, _body) \
		}); \
		(measure_t){ .lat = _l, .thr = _t }; \
	})

	t.put("fccmp.h (eq)",               sweep( g->fccmp(s->h, s->h, 0xf, Cond::EQ) ));
	t.put("fccmp.h (le)",               sweep( g->fccmp(s->h, s->h, 0xf, Cond::LE) ));
	t.put("fccmp.s (eq)",               sweep( g->fccmp(s->s, s->s, 0xf, Cond::EQ) ));
	t.put("fccmp.s (le)",               sweep( g->fccmp(s->s, s->s, 0xf, Cond::LE) ));
	t.put("fccmp.d (eq)",               sweep( g->fccmp(s->d, s->d, 0xf, Cond::EQ) ));
	t.put("fccmp.d (le)",               sweep( g->fccmp(s->d, s->d, 0xf, Cond::LE) ));
	t.put("fccmpe.h (eq)",              sweep( g->fccmpe(s->h, s->h, 0xf, Cond::EQ) ));
	t.put("fccmpe.h (le)",              sweep( g->fccmpe(s->h, s->h, 0xf, Cond::LE) ));
	t.put("fccmpe.s (eq)",              sweep( g->fccmpe(s->s, s->s, 0xf, Cond::EQ) ));
	t.put("fccmpe.s (le)",              sweep( g->fccmpe(s->s, s->s, 0xf, Cond::LE) ));
	t.put("fccmpe.d (eq)",              sweep( g->fccmpe(s->d, s->d, 0xf, Cond::EQ) ));
	t.put("fccmpe.d (le)",              sweep( g->fccmpe(s->d, s->d, 0xf, Cond::LE) ));

	#undef thr_body
	#undef sweep

	t.put("fcmeq.h (scl)",              both(b, op( g->fcmeq(d->h, d->h, s->h) )));
	t.put("fcmeq.s (scl)",              both(b, op( g->fcmeq(d->s, d->s, s->s) )));
	t.put("fcmeq.d (scl)",              both(b, op( g->fcmeq(d->d, d->d, s->d) )));
	t.put("fcmeq.h (vec)",              both(b, op( g->fcmeq(d->v.h, d->v.h, s->v.h) )));
	t.put("fcmeq.s (vec)",              both(b, op( g->fcmeq(d->v.s, d->v.s, s->v.s) )));
	t.put("fcmeq.d (vec)",              both(b, op( g->fcmeq(d->v.d, d->v.d, s->v.d) )));

	t.put("fcmeq.h (scl; zero)",        both(b, op( g->fcmeq(d->h, s->h, 0.0) )));
	t.put("fcmeq.s (scl; zero)",        both(b, op( g->fcmeq(d->s, s->s, 0.0) )));
	t.put("fcmeq.d (scl; zero)",        both(b, op( g->fcmeq(d->d, s->d, 0.0) )));
	t.put("fcmeq.h (vec; zero)",        both(b, op( g->fcmeq(d->v.h, s->v.h, 0.0) )));
	t.put("fcmeq.s (vec; zero)",        both(b, op( g->fcmeq(d->v.s, s->v.s, 0.0) )));
	t.put("fcmeq.d (vec; zero)",        both(b, op( g->fcmeq(d->v.d, s->v.d, 0.0) )));

	t.put("fcmge.h (scl)",              both(b, op( g->fcmge(d->h, d->h, s->h) )));
	t.put("fcmge.s (scl)",              both(b, op( g->fcmge(d->s, d->s, s->s) )));
	t.put("fcmge.d (scl)",              both(b, op( g->fcmge(d->d, d->d, s->d) )));
	t.put("fcmge.h (vec)",              both(b, op( g->fcmge(d->v.h, d->v.h, s->v.h) )));
	t.put("fcmge.s (vec)",              both(b, op( g->fcmge(d->v.s, d->v.s, s->v.s) )));
	t.put("fcmge.d (vec)",              both(b, op( g->fcmge(d->v.d, d->v.d, s->v.d) )));

	t.put("fcmge.h (scl; zero)",        both(b, op( g->fcmge(d->h, s->h, 0.0) )));
	t.put("fcmge.s (scl; zero)",        both(b, op( g->fcmge(d->s, s->s, 0.0) )));
	t.put("fcmge.d (scl; zero)",        both(b, op( g->fcmge(d->d, s->d, 0.0) )));
	t.put("fcmge.h (vec; zero)",        both(b, op( g->fcmge(d->v.h, s->v.h, 0.0) )));
	t.put("fcmge.s (vec; zero)",        both(b, op( g->fcmge(d->v.s, s->v.s, 0.0) )));
	t.put("fcmge.d (vec; zero)",        both(b, op( g->fcmge(d->v.d, s->v.d, 0.0) )));

	t.put("fcmgt.h (scl)",              both(b, op( g->fcmgt(d->h, d->h, s->h) )));
	t.put("fcmgt.s (scl)",              both(b, op( g->fcmgt(d->s, d->s, s->s) )));
	t.put("fcmgt.d (scl)",              both(b, op( g->fcmgt(d->d, d->d, s->d) )));
	t.put("fcmgt.h (vec)",              both(b, op( g->fcmgt(d->v.h, d->v.h, s->v.h) )));
	t.put("fcmgt.s (vec)",              both(b, op( g->fcmgt(d->v.s, d->v.s, s->v.s) )));
	t.put("fcmgt.d (vec)",              both(b, op( g->fcmgt(d->v.d, d->v.d, s->v.d) )));

	t.put("fcmgt.h (scl; zero)",        both(b, op( g->fcmgt(d->h, s->h, 0.0) )));
	t.put("fcmgt.s (scl; zero)",        both(b, op( g->fcmgt(d->s, s->s, 0.0) )));
	t.put("fcmgt.d (scl; zero)",        both(b, op( g->fcmgt(d->d, s->d, 0.0) )));
	t.put("fcmgt.h (vec; zero)",        both(b, op( g->fcmgt(d->v.h, s->v.h, 0.0) )));
	t.put("fcmgt.s (vec; zero)",        both(b, op( g->fcmgt(d->v.s, s->v.s, 0.0) )));
	t.put("fcmgt.d (vec; zero)",        both(b, op( g->fcmgt(d->v.d, s->v.d, 0.0) )));

	t.put("fcmle.h (scl; zero)",        both(b, op( g->fcmle(d->h, s->h, 0.0) )));
	t.put("fcmle.s (scl; zero)",        both(b, op( g->fcmle(d->s, s->s, 0.0) )));
	t.put("fcmle.d (scl; zero)",        both(b, op( g->fcmle(d->d, s->d, 0.0) )));
	t.put("fcmle.h (vec; zero)",        both(b, op( g->fcmle(d->v.h, s->v.h, 0.0) )));
	t.put("fcmle.s (vec; zero)",        both(b, op( g->fcmle(d->v.s, s->v.s, 0.0) )));
	t.put("fcmle.d (vec; zero)",        both(b, op( g->fcmle(d->v.d, s->v.d, 0.0) )));

	t.put("fcmlt.h (scl; zero)",        both(b, op( g->fcmlt(d->h, s->h, 0.0) )));
	t.put("fcmlt.s (scl; zero)",        both(b, op( g->fcmlt(d->s, s->s, 0.0) )));
	t.put("fcmlt.d (scl; zero)",        both(b, op( g->fcmlt(d->d, s->d, 0.0) )));
	t.put("fcmlt.h (vec; zero)",        both(b, op( g->fcmlt(d->v.h, s->v.h, 0.0) )));
	t.put("fcmlt.s (vec; zero)",        both(b, op( g->fcmlt(d->v.s, s->v.s, 0.0) )));
	t.put("fcmlt.d (vec; zero)",        both(b, op( g->fcmlt(d->v.d, s->v.d, 0.0) )));
	return;
}

/*
 * end of cmp.cc
 */
