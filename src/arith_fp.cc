
/**
 * @file arith_fp.cc
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>

static
void bench_basic_arith_fp_vec(bool md, double freq) {
	table t(md, "Floating point basic arithmetic");
	bench b(freq);

	t.put("fadd.h (scl)",               both(b, op( g->fadd(d->h, d->h, s->h) )));
	t.put("fadd.s (scl)",               both(b, op( g->fadd(d->s, d->s, s->s) )));
	t.put("fadd.d (scl)",               both(b, op( g->fadd(d->d, d->d, s->d) )));
	t.put("fadd.h (vec)",               both(b, op( g->fadd(d->v.h, d->v.h, s->v.h) )));
	t.put("fadd.s (vec)",               both(b, op( g->fadd(d->v.s, d->v.s, s->v.s) )));
	t.put("fadd.d (vec)",               both(b, op( g->fadd(d->v.d, d->v.d, s->v.d) )));

	t.put("faddp.h (scl)",              both(b, op( g->faddp(d->h, s->v.h2) )));
	t.put("faddp.s (scl)",              both(b, op( g->faddp(d->s, s->v.s2) )));
	t.put("faddp.d (scl)",              both(b, op( g->faddp(d->d, s->v.d2) )));
	t.put("faddp.h (vec)",              both(b, op( g->faddp(d->v.h, d->v.h, s->v.h) )));
	t.put("faddp.s (vec)",              both(b, op( g->faddp(d->v.s, d->v.s, s->v.s) )));
	t.put("faddp.d (vec)",              both(b, op( g->faddp(d->v.d, d->v.d, s->v.d) )));

	t.put("fsub.h (scl)",               both(b, op( g->fsub(d->h, d->h, s->h) )));
	t.put("fsub.s (scl)",               both(b, op( g->fsub(d->s, d->s, s->s) )));
	t.put("fsub.d (scl)",               both(b, op( g->fsub(d->d, d->d, s->d) )));
	t.put("fsub.h (vec)",               both(b, op( g->fsub(d->v.h, d->v.h, s->v.h) )));
	t.put("fsub.s (vec)",               both(b, op( g->fsub(d->v.s, d->v.s, s->v.s) )));
	t.put("fsub.d (vec)",               both(b, op( g->fsub(d->v.d, d->v.d, s->v.d) )));

	t.put("fcadd.h (deg = 90)",         both(b, op( g->fcadd(d->v.h, d->v.h, s->v.h, 90) )));
	t.put("fcadd.s (deg = 90)",         both(b, op( g->fcadd(d->v.s, d->v.s, s->v.s, 90) )));
	t.put("fcadd.d (deg = 90)",         both(b, op( g->fcadd(d->v.d, d->v.d, s->v.d, 90) )));

	t.put("fabs.h (scl)",               both(b, op( g->fabs(d->h, s->h) )));
	t.put("fabs.s (scl)",               both(b, op( g->fabs(d->s, s->s) )));
	t.put("fabs.d (scl)",               both(b, op( g->fabs(d->d, s->d) )));
	t.put("fabs.h (vec)",               both(b, op( g->fabs(d->v.h, s->v.h) )));
	t.put("fabs.s (vec)",               both(b, op( g->fabs(d->v.s, s->v.s) )));
	t.put("fabs.d (vec)",               both(b, op( g->fabs(d->v.d, s->v.d) )));

	t.put("fabd.h (scl)",               both(b, op( g->fabd(d->h, d->h, s->h) )));
	t.put("fabd.s (scl)",               both(b, op( g->fabd(d->s, d->s, s->s) )));
	t.put("fabd.d (scl)",               both(b, op( g->fabd(d->d, d->d, s->d) )));
	t.put("fabd.h (vec)",               both(b, op( g->fabd(d->v.h, d->v.h, s->v.h) )));
	t.put("fabd.s (vec)",               both(b, op( g->fabd(d->v.s, d->v.s, s->v.s) )));
	t.put("fabd.d (vec)",               both(b, op( g->fabd(d->v.d, d->v.d, s->v.d) )));

	t.put("fneg.h (scl)",               both(b, op( g->fneg(d->h, s->h) )));
	t.put("fneg.s (scl)",               both(b, op( g->fneg(d->s, s->s) )));
	t.put("fneg.d (scl)",               both(b, op( g->fneg(d->d, s->d) )));
	t.put("fneg.h (vec)",               both(b, op( g->fneg(d->v.h, s->v.h) )));
	t.put("fneg.s (vec)",               both(b, op( g->fneg(d->v.s, s->v.s) )));
	t.put("fneg.d (vec)",               both(b, op( g->fneg(d->v.d, s->v.d) )));
	return;
}

static
void bench_max_min_fp_vec(bool md, double freq) {
	table t(md, "Floating point max / min");
	bench b(freq);

	t.put("fmax.h (scl)",               both(b, op( g->fmax(d->h, d->h, s->h) )));
	t.put("fmax.s (scl)",               both(b, op( g->fmax(d->s, d->s, s->s) )));
	t.put("fmax.d (scl)",               both(b, op( g->fmax(d->d, d->d, s->d) )));
	t.put("fmax.h (vec)",               both(b, op( g->fmax(d->v.h, d->v.h, s->v.h) )));
	t.put("fmax.s (vec)",               both(b, op( g->fmax(d->v.s, d->v.s, s->v.s) )));
	t.put("fmax.d (vec)",               both(b, op( g->fmax(d->v.d, d->v.d, s->v.d) )));

	t.put("fmaxp.h (scl)",              both(b, op( g->fmaxp(d->h, s->v.h2) )));
	t.put("fmaxp.s (scl)",              both(b, op( g->fmaxp(d->s, s->v.s2) )));
	t.put("fmaxp.d (scl)",              both(b, op( g->fmaxp(d->d, s->v.d2) )));
	t.put("fmaxp.h (vec)",              both(b, op( g->fmaxp(d->v.h, d->v.h, s->v.h) )));
	t.put("fmaxp.s (vec)",              both(b, op( g->fmaxp(d->v.s, d->v.s, s->v.s) )));
	t.put("fmaxp.d (vec)",              both(b, op( g->fmaxp(d->v.d, d->v.d, s->v.d) )));

	t.put("fmaxv.h",                    both(b, op( g->fmaxv(d->h, s->v.h) )));
	t.put("fmaxv.s",                    both(b, op( g->fmaxv(d->s, s->v.s) )));

	t.put("fmaxnm.h (scl)",             both(b, op( g->fmaxnm(d->h, d->h, s->h) )));
	t.put("fmaxnm.s (scl)",             both(b, op( g->fmaxnm(d->s, d->s, s->s) )));
	t.put("fmaxnm.d (scl)",             both(b, op( g->fmaxnm(d->d, d->d, s->d) )));
	t.put("fmaxnm.h (vec)",             both(b, op( g->fmaxnm(d->v.h, d->v.h, s->v.h) )));
	t.put("fmaxnm.s (vec)",             both(b, op( g->fmaxnm(d->v.s, d->v.s, s->v.s) )));
	t.put("fmaxnm.d (vec)",             both(b, op( g->fmaxnm(d->v.d, d->v.d, s->v.d) )));

	t.put("fmaxnmp.h (scl)",            both(b, op( g->fmaxnmp(d->h, s->v.h2) )));
	t.put("fmaxnmp.s (scl)",            both(b, op( g->fmaxnmp(d->s, s->v.s2) )));
	t.put("fmaxnmp.d (scl)",            both(b, op( g->fmaxnmp(d->d, s->v.d2) )));
	t.put("fmaxnmp.h (vec)",            both(b, op( g->fmaxnmp(d->v.h, d->v.h, s->v.h) )));
	t.put("fmaxnmp.s (vec)",            both(b, op( g->fmaxnmp(d->v.s, d->v.s, s->v.s) )));
	t.put("fmaxnmp.d (vec)",            both(b, op( g->fmaxnmp(d->v.d, d->v.d, s->v.d) )));

	t.put("fmaxnmv.h",                  both(b, op( g->fmaxnmv(d->h, s->v.h) )));
	t.put("fmaxnmv.s",                  both(b, op( g->fmaxnmv(d->s, s->v.s) )));

	t.put("fmin.h (scl)",               both(b, op( g->fmin(d->h, d->h, s->h) )));
	t.put("fmin.s (scl)",               both(b, op( g->fmin(d->s, d->s, s->s) )));
	t.put("fmin.d (scl)",               both(b, op( g->fmin(d->d, d->d, s->d) )));
	t.put("fmin.h (vec)",               both(b, op( g->fmin(d->v.h, d->v.h, s->v.h) )));
	t.put("fmin.s (vec)",               both(b, op( g->fmin(d->v.s, d->v.s, s->v.s) )));
	t.put("fmin.d (vec)",               both(b, op( g->fmin(d->v.d, d->v.d, s->v.d) )));

	t.put("fminp.h (scl)",              both(b, op( g->fminp(d->h, s->v.h2) )));
	t.put("fminp.s (scl)",              both(b, op( g->fminp(d->s, s->v.s2) )));
	t.put("fminp.d (scl)",              both(b, op( g->fminp(d->d, s->v.d2) )));
	t.put("fminp.h (vec)",              both(b, op( g->fminp(d->v.h, d->v.h, s->v.h) )));
	t.put("fminp.s (vec)",              both(b, op( g->fminp(d->v.s, d->v.s, s->v.s) )));
	t.put("fminp.d (vec)",              both(b, op( g->fminp(d->v.d, d->v.d, s->v.d) )));

	t.put("fminv.h",                    both(b, op( g->fminv(d->h, s->v.h) )));
	t.put("fminv.s",                    both(b, op( g->fminv(d->s, s->v.s) )));

	t.put("fminnm.h (scl)",             both(b, op( g->fminnm(d->h, d->h, s->h) )));
	t.put("fminnm.s (scl)",             both(b, op( g->fminnm(d->s, d->s, s->s) )));
	t.put("fminnm.d (scl)",             both(b, op( g->fminnm(d->d, d->d, s->d) )));
	t.put("fminnm.h (vec)",             both(b, op( g->fminnm(d->v.h, d->v.h, s->v.h) )));
	t.put("fminnm.s (vec)",             both(b, op( g->fminnm(d->v.s, d->v.s, s->v.s) )));
	t.put("fminnm.d (vec)",             both(b, op( g->fminnm(d->v.d, d->v.d, s->v.d) )));

	t.put("fminnmp.h (scl)",            both(b, op( g->fminnmp(d->h, s->v.h2) )));
	t.put("fminnmp.s (scl)",            both(b, op( g->fminnmp(d->s, s->v.s2) )));
	t.put("fminnmp.d (scl)",            both(b, op( g->fminnmp(d->d, s->v.d2) )));
	t.put("fminnmp.h (vec)",            both(b, op( g->fminnmp(d->v.h, d->v.h, s->v.h) )));
	t.put("fminnmp.s (vec)",            both(b, op( g->fminnmp(d->v.s, d->v.s, s->v.s) )));
	t.put("fminnmp.d (vec)",            both(b, op( g->fminnmp(d->v.d, d->v.d, s->v.d) )));

	t.put("fminnmv.h",                  both(b, op( g->fminnmv(d->h, s->v.h) )));
	t.put("fminnmv.s",                  both(b, op( g->fminnmv(d->s, s->v.s) )));
	return;
}

static
void bench_mul_fp_vec(bool md, double freq) {
	table t(md, "Floating point multiplication and multiply-accumulate");
	bench b(freq);

	t.put("fmul.h (scl)",               both(b, op( g->fmul(d->h, d->h, s->h) )));
	t.put("fmul.s (scl)",               both(b, op( g->fmul(d->s, d->s, s->s) )));
	t.put("fmul.d (scl)",               both(b, op( g->fmul(d->d, d->d, s->d) )));
	t.put("fmul.h (vec)",               both(b, op( g->fmul(d->v.h, d->v.h, s->v.h) )));
	t.put("fmul.s (vec)",               both(b, op( g->fmul(d->v.s, d->v.s, s->v.s) )));
	t.put("fmul.d (vec)",               both(b, op( g->fmul(d->v.d, d->v.d, s->v.d) )));

	t.put("fmulx.h (scl)",              both(b, op( g->fmulx(d->h, d->h, s->h) )));
	t.put("fmulx.s (scl)",              both(b, op( g->fmulx(d->s, d->s, s->s) )));
	t.put("fmulx.d (scl)",              both(b, op( g->fmulx(d->d, d->d, s->d) )));
	t.put("fmulx.h (vec)",              both(b, op( g->fmulx(d->v.h, d->v.h, s->v.h) )));
	t.put("fmulx.s (vec)",              both(b, op( g->fmulx(d->v.s, d->v.s, s->v.s) )));
	t.put("fmulx.d (vec)",              both(b, op( g->fmulx(d->v.d, d->v.d, s->v.d) )));

	t.put("fnmul.h (scl)",              both(b, op( g->fnmul(d->h, d->h, s->h) )));
	t.put("fnmul.s (scl)",              both(b, op( g->fnmul(d->s, d->s, s->s) )));
	t.put("fnmul.d (scl)",              both(b, op( g->fnmul(d->d, d->d, s->d) )));

	/* source register range limited */
	// t.put("fmla.h (elem)",              both(b, op( g->fmla(d->v.h, d->v.h, s->v.h[0]) )));
	// t.put("fmla.s (elem)",              both(b, op( g->fmla(d->v.s, d->v.s, s->v.s[0]) )));
	// t.put("fmla.d (elem)",              both(b, op( g->fmla(d->v.d, d->v.d, s->v.d[0]) )));
	t.put("fmla.h (vec)",               both(b, op( g->fmla(d->v.h, d->v.h, s->v.h) )));
	t.put("fmla.s (vec)",               both(b, op( g->fmla(d->v.s, d->v.s, s->v.s) )));
	t.put("fmla.d (vec)",               both(b, op( g->fmla(d->v.d, d->v.d, s->v.d) )));
	t.put("fmlal.h (vec)",              both(b, op( g->fmlal(d->v.s, d->v.h4, s->v.h4) )));
	t.put("fmlal2.h (vec)",             both(b, op( g->fmlal2(d->v.s, d->v.h4, s->v.h4) )));

	t.put("fmls.h (vec)",               both(b, op( g->fmls(d->v.h, d->v.h, s->v.h) )));
	t.put("fmls.s (vec)",               both(b, op( g->fmls(d->v.s, d->v.s, s->v.s) )));
	t.put("fmls.d (vec)",               both(b, op( g->fmls(d->v.d, d->v.d, s->v.d) )));
	t.put("fmlsl.h (vec)",              both(b, op( g->fmlsl(d->v.s, d->v.h4, s->v.h4) )));
	t.put("fmlsl2.h (vec)",             both(b, op( g->fmlsl2(d->v.s, d->v.h4, s->v.h4) )));

	t.put("fmadd.h",                    both(b, op( g->fmadd(d->h, d->h, d->h, s->h) )));
	t.put("fmadd.s",                    both(b, op( g->fmadd(d->s, d->s, d->s, s->s) )));
	t.put("fmadd.d",                    both(b, op( g->fmadd(d->d, d->d, d->d, s->d) )));

	t.put("fmsub.h",                    both(b, op( g->fmsub(d->h, d->h, d->h, s->h) )));
	t.put("fmsub.s",                    both(b, op( g->fmsub(d->s, d->s, d->s, s->s) )));
	t.put("fmsub.d",                    both(b, op( g->fmsub(d->d, d->d, d->d, s->d) )));

	t.put("fnmadd.h",                   both(b, op( g->fnmadd(d->h, d->h, d->h, s->h) )));
	t.put("fnmadd.s",                   both(b, op( g->fnmadd(d->s, d->s, d->s, s->s) )));
	t.put("fnmadd.d",                   both(b, op( g->fnmadd(d->d, d->d, d->d, s->d) )));

	t.put("fnmsub.h",                   both(b, op( g->fnmsub(d->h, d->h, d->h, s->h) )));
	t.put("fnmsub.s",                   both(b, op( g->fnmsub(d->s, d->s, d->s, s->s) )));
	t.put("fnmsub.d",                   both(b, op( g->fnmsub(d->d, d->d, d->d, s->d) )));

	t.put("fcmla.h (elem; deg = 0; v.h[0])",    both(b, op( g->fcmla(d->v.h, d->v.h, s->v.h[0], 0) )));
	t.put("fcmla.h (elem; deg = 0; v.h[7])",    both(b, op( g->fcmla(d->v.h, d->v.h, s->v.h[7], 0) )));
	t.put("fcmla.s (elem; deg = 0; v.s[0])",    both(b, op( g->fcmla(d->v.s, d->v.s, s->v.s[0], 0) )));
	t.put("fcmla.s (elem; deg = 0; v.s[3])",    both(b, op( g->fcmla(d->v.s, d->v.s, s->v.s[3], 0) )));
	// t.put("fcmla.d (elem; deg = 0; v.h[0])",    both(b, op( g->fcmla(d->v.d, d->v.d, s->v.d[0], 0) )));
	t.put("fcmla.h (elem; deg = 90; v.h[0])",   both(b, op( g->fcmla(d->v.h, d->v.h, s->v.h[0], 90) )));
	t.put("fcmla.h (elem; deg = 90; v.h[7])",   both(b, op( g->fcmla(d->v.h, d->v.h, s->v.h[7], 90) )));
	t.put("fcmla.s (elem; deg = 90; v.s[0])",   both(b, op( g->fcmla(d->v.s, d->v.s, s->v.s[0], 90) )));
	t.put("fcmla.s (elem; deg = 90; v.s[3])",   both(b, op( g->fcmla(d->v.s, d->v.s, s->v.s[3], 90) )));
	// t.put("fcmla.d (elem; deg = 90)",   both(b, op( g->fcmla(d->v.d, d->v.d, s->v.d[0], 90) )));
	t.put("fcmla.h (vec; deg = 0)",     both(b, op( g->fcmla(d->v.h, d->v.h, s->v.h, 0) )));
	t.put("fcmla.s (vec; deg = 0)",     both(b, op( g->fcmla(d->v.s, d->v.s, s->v.s, 0) )));
	t.put("fcmla.d (vec; deg = 0)",     both(b, op( g->fcmla(d->v.d, d->v.d, s->v.d, 0) )));
	t.put("fcmla.h (vec; deg = 90)",    both(b, op( g->fcmla(d->v.h, d->v.h, s->v.h, 90) )));
	t.put("fcmla.s (vec; deg = 90)",    both(b, op( g->fcmla(d->v.s, d->v.s, s->v.s, 90) )));
	t.put("fcmla.d (vec; deg = 90)",    both(b, op( g->fcmla(d->v.d, d->v.d, s->v.d, 90) )));
	return;
}

static
void bench_div_fp_vec(bool md, double freq) {
	table t(md, "Floating point division and reciprocal");
	bench b(freq);

	/* FIXME: fdiv */

	t.put("frecpe.h (scl)",             both(b, op( g->frecpe(d->h, s->h) )));
	t.put("frecpe.s (scl)",             both(b, op( g->frecpe(d->s, s->s) )));
	t.put("frecpe.d (scl)",             both(b, op( g->frecpe(d->d, s->d) )));
	t.put("frecpe.h (vec)",             both(b, op( g->frecpe(d->v.h, s->v.h) )));
	t.put("frecpe.s (vec)",             both(b, op( g->frecpe(d->v.s, s->v.s) )));
	t.put("frecpe.d (vec)",             both(b, op( g->frecpe(d->v.d, s->v.d) )));

	t.put("frecps.h (scl)",             both(b, op( g->frecps(d->h, d->h, s->h) )));
	t.put("frecps.s (scl)",             both(b, op( g->frecps(d->s, d->s, s->s) )));
	t.put("frecps.d (scl)",             both(b, op( g->frecps(d->d, d->d, s->d) )));
	t.put("frecps.h (vec)",             both(b, op( g->frecps(d->v.h, d->v.h, s->v.h) )));
	t.put("frecps.s (vec)",             both(b, op( g->frecps(d->v.s, d->v.s, s->v.s) )));
	t.put("frecps.d (vec)",             both(b, op( g->frecps(d->v.d, d->v.d, s->v.d) )));

	t.put("frecpx.h (scl)",             both(b, op( g->frecpx(d->h, s->h) )));
	t.put("frecpx.s (scl)",             both(b, op( g->frecpx(d->s, s->s) )));
	t.put("frecpx.d (scl)",             both(b, op( g->frecpx(d->d, s->d) )));
	return;
}

void bench_arith_fp_vec(bool md, double freq) {
	bench_basic_arith_fp_vec(md, freq);
	bench_max_min_fp_vec(md, freq);
	bench_mul_fp_vec(md, freq);
	bench_div_fp_vec(md, freq);
	return;
}

/*
 * end of arith_fp.cc
 */
