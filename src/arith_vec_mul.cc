
/**
 * @file arith_vec_muldiv.cc
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>

static
void bench_mul_vec(bool md, double freq) {
	table t(md, "Vector integer multiply");
	bench b(freq);

	/* mul */
	t.put("mul.b (vec)",                    both(b, op( g->mul(d->v.b, d->v.b, s->v.b) )));
	t.put("mul.h (vec)",                    both(b, op( g->mul(d->v.h, d->v.h, s->v.h) )));
	t.put("mul.s (vec)",                    both(b, op( g->mul(d->v.s, d->v.s, s->v.s) )));

	t.put("mul.h (elem; [0])",              both(b, op( g->mul(d->v.h, d->v.h, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("mul.h (elem; [7])",              both(b, op( g->mul(d->v.h, d->v.h, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("mul.s (elem; [0])",              both(b, op( g->mul(d->v.s, d->v.s, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("mul.s (elem; [3])",              both(b, op( g->mul(d->v.s, d->v.s, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));

	t.put("smull.b (vec)",                  both(b, op( g->smull(d->v.h, d->v.b8, s->v.b8) )));
	t.put("smull.h (vec)",                  both(b, op( g->smull(d->v.s, d->v.h4, s->v.h4) )));
	t.put("smull.s (vec)",                  both(b, op( g->smull(d->v.d, d->v.s2, s->v.s2) )));
	t.put("smull2.b (vec)",                 both(b, op( g->smull2(d->v.h, d->v.b, s->v.b) )));
	t.put("smull2.h (vec)",                 both(b, op( g->smull2(d->v.s, d->v.h, s->v.h) )));
	t.put("smull2.s (vec)",                 both(b, op( g->smull2(d->v.d, d->v.s, s->v.s) )));

	t.put("smull.h (elem; [0])",            both(b, op( g->smull(d->v.s, d->v.h4, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("smull.h (elem; [7])",            both(b, op( g->smull(d->v.s, d->v.h4, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("smull.s (elem; [0])",            both(b, op( g->smull(d->v.d, d->v.s2, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("smull.s (elem; [3])",            both(b, op( g->smull(d->v.d, d->v.s2, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("smull2.h (elem; [0])",           both(b, op( g->smull2(d->v.s, d->v.h, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("smull2.h (elem; [7])",           both(b, op( g->smull2(d->v.s, d->v.h, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("smull2.s (elem; [0])",           both(b, op( g->smull2(d->v.d, d->v.s, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("smull2.s (elem; [3])",           both(b, op( g->smull2(d->v.d, d->v.s, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));

	t.put("umull.b (vec)",                  both(b, op( g->umull(d->v.h, d->v.b8, s->v.b8) )));
	t.put("umull.h (vec)",                  both(b, op( g->umull(d->v.s, d->v.h4, s->v.h4) )));
	t.put("umull.s (vec)",                  both(b, op( g->umull(d->v.d, d->v.s2, s->v.s2) )));
	t.put("umull2.b (vec)",                 both(b, op( g->umull2(d->v.h, d->v.b, s->v.b) )));
	t.put("umull2.h (vec)",                 both(b, op( g->umull2(d->v.s, d->v.h, s->v.h) )));
	t.put("umull2.s (vec)",                 both(b, op( g->umull2(d->v.d, d->v.s, s->v.s) )));

	t.put("umull.h (elem; [0])",            both(b, op( g->umull(d->v.s, d->v.h4, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("umull.h (elem; [7])",            both(b, op( g->umull(d->v.s, d->v.h4, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("umull.s (elem; [0])",            both(b, op( g->umull(d->v.d, d->v.s2, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("umull.s (elem; [3])",            both(b, op( g->umull(d->v.d, d->v.s2, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("umull2.h (elem; [0])",           both(b, op( g->umull2(d->v.s, d->v.h, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("umull2.h (elem; [7])",           both(b, op( g->umull2(d->v.s, d->v.h, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("umull2.s (elem; [0])",           both(b, op( g->umull2(d->v.d, d->v.s, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("umull2.s (elem; [3])",           both(b, op( g->umull2(d->v.d, d->v.s, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));

	t.put("sqdmull.h (vec)",                both(b, op( g->sqdmull(d->v.s, d->v.h4, s->v.h4) )));
	t.put("sqdmull.s (vec)",                both(b, op( g->sqdmull(d->v.d, d->v.s2, s->v.s2) )));
	t.put("sqdmull2.h (vec)",               both(b, op( g->sqdmull2(d->v.s, d->v.h, s->v.h) )));
	t.put("sqdmull2.s (vec)",               both(b, op( g->sqdmull2(d->v.d, d->v.s, s->v.s) )));

	t.put("sqdmull.h (elem; v.h[0])",       both(b, op( g->sqdmull(d->v.s, d->v.h4, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmull.h (elem; v.h[7])",       both(b, op( g->sqdmull(d->v.s, d->v.h4, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmull.s (elem; v.s[0])",       both(b, op( g->sqdmull(d->v.d, d->v.s2, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmull.s (elem; v.s[3])",       both(b, op( g->sqdmull(d->v.d, d->v.s2, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmull2.h (elem; v.h[0])",      both(b, op( g->sqdmull2(d->v.s, d->v.h, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmull2.h (elem; v.h[7])",      both(b, op( g->sqdmull2(d->v.s, d->v.h, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmull2.s (elem; v.s[0])",      both(b, op( g->sqdmull2(d->v.d, d->v.s, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmull2.s (elem; v.s[3])",      both(b, op( g->sqdmull2(d->v.d, d->v.s, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));

	t.put("sqdmulh.h (vec)",                both(b, op( g->sqdmulh(d->v.h, d->v.h, s->v.h) )));
	t.put("sqdmulh.s (vec)",                both(b, op( g->sqdmulh(d->v.s, d->v.s, s->v.s) )));
	t.put("sqdmulh.h (elem; v.h[0])",       both(b, op( g->sqdmulh(d->v.h, d->v.h, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmulh.h (elem; v.h[7])",       both(b, op( g->sqdmulh(d->v.h, d->v.h, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmulh.s (elem; v.s[0])",       both(b, op( g->sqdmulh(d->v.s, d->v.s, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmulh.s (elem; v.s[3])",       both(b, op( g->sqdmulh(d->v.s, d->v.s, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));

	t.put("sqrdmulh.h (vec)",               both(b, op( g->sqrdmulh(d->v.h, d->v.h, s->v.h) )));
	t.put("sqrdmulh.s (vec)",               both(b, op( g->sqrdmulh(d->v.s, d->v.s, s->v.s) )));
	t.put("sqrdmulh.h (elem; v.h[0])",      both(b, op( g->sqrdmulh(d->v.h, d->v.h, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqrdmulh.h (elem; v.h[7])",      both(b, op( g->sqrdmulh(d->v.h, d->v.h, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqrdmulh.s (elem; v.s[0])",      both(b, op( g->sqrdmulh(d->v.s, d->v.s, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqrdmulh.s (elem; v.s[3])",      both(b, op( g->sqrdmulh(d->v.s, d->v.s, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));

	t.put("pmul.b",                         both(b, op( g->pmul(d->v.b, d->v.b, s->v.b) )));
	t.put("pmull.b",                        both(b, op( g->pmull(d->v.h, d->v.b8, s->v.b8) )));
	t.put("pmull.d",                        both(b, op( g->pmull(d->v.q, d->v.d1, s->v.d1) )));
	t.put("pmull2.b",                       both(b, op( g->pmull2(d->v.h, d->v.b, s->v.b) )));
	t.put("pmull2.d",                       both(b, op( g->pmull2(d->v.q, d->v.d, s->v.d) )));
	return;
}

static
void bench_mla_vec(bool md, double freq) {
	table t(md, "Vector integer multiply-accumulate");
	bench b(freq);

	/* mla */
	t.put("mla.b (vec)",                    both(b, op( g->mla(d->v.b, d->v.b, s->v.b) )));
	t.put("mla.h (vec)",                    both(b, op( g->mla(d->v.h, d->v.h, s->v.h) )));
	t.put("mla.s (vec)",                    both(b, op( g->mla(d->v.s, d->v.s, s->v.s) )));
	t.put("mla.b (vec; acc. fwd.)",         lat(b,  op( g->mla(g->v28.b, s->v.b, s->v.b) )));
	t.put("mla.h (vec; acc. fwd.)",         lat(b,  op( g->mla(g->v28.h, s->v.h, s->v.h) )));
	t.put("mla.s (vec; acc. fwd.)",         lat(b,  op( g->mla(g->v28.s, s->v.s, s->v.s) )));

	t.put("mla.h (elem; [0])",              both(b, op( g->mla(d->v.h, d->v.h, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("mla.h (elem; [7])",              both(b, op( g->mla(d->v.h, d->v.h, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("mla.s (elem; [0])",              both(b, op( g->mla(d->v.s, d->v.s, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("mla.s (elem; [3])",              both(b, op( g->mla(d->v.s, d->v.s, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));

	t.put("smlal.h (vec)",                  both(b, op( g->smlal(d->v.s, d->v.h4, s->v.h4) )));
	t.put("smlal.s (vec)",                  both(b, op( g->smlal(d->v.d, d->v.s2, s->v.s2) )));
	t.put("smlal2.h (vec)",                 both(b, op( g->smlal2(d->v.s, d->v.h, s->v.h) )));
	t.put("smlal2.s (vec)",                 both(b, op( g->smlal2(d->v.d, d->v.s, s->v.s) )));
	t.put("smlal.h (vec; acc. fwd.)",       lat(b,  op( g->smlal(g->v28.s, s->v.h4, s->v.h4) )));
	t.put("smlal.s (vec; acc. fwd.)",       lat(b,  op( g->smlal(g->v28.d, s->v.s2, s->v.s2) )));
	t.put("smlal2.h (vec; acc. fwd.)",      lat(b,  op( g->smlal2(g->v28.s, s->v.h, s->v.h) )));
	t.put("smlal2.s (vec; acc. fwd.)",      lat(b,  op( g->smlal2(g->v28.d, s->v.s, s->v.s) )));

	t.put("smlal.h (elem; v.h[0])",         both(b, op( g->smlal(d->v.s, d->v.h4, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("smlal.h (elem; v.h[7])",         both(b, op( g->smlal(d->v.s, d->v.h4, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("smlal.s (elem; v.s[0])",         both(b, op( g->smlal(d->v.d, d->v.s2, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("smlal.s (elem; v.s[3])",         both(b, op( g->smlal(d->v.d, d->v.s2, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("smlal2.h (elem; v.h[0])",        both(b, op( g->smlal2(d->v.s, d->v.h, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("smlal2.h (elem; v.h[7])",        both(b, op( g->smlal2(d->v.s, d->v.h, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("smlal2.s (elem; v.s[0])",        both(b, op( g->smlal2(d->v.d, d->v.s, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("smlal2.s (elem; v.s[3])",        both(b, op( g->smlal2(d->v.d, d->v.s, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));

	t.put("umlal.h (vec)",                  both(b, op( g->umlal(d->v.s, d->v.h4, s->v.h4) )));
	t.put("umlal.s (vec)",                  both(b, op( g->umlal(d->v.d, d->v.s2, s->v.s2) )));
	t.put("umlal2.h (vec)",                 both(b, op( g->umlal2(d->v.s, d->v.h, s->v.h) )));
	t.put("umlal2.s (vec)",                 both(b, op( g->umlal2(d->v.d, d->v.s, s->v.s) )));
	t.put("umlal.h (vec; acc. fwd.)",       lat(b,  op( g->umlal(g->v28.s, s->v.h4, s->v.h4) )));
	t.put("umlal.s (vec; acc. fwd.)",       lat(b,  op( g->umlal(g->v28.d, s->v.s2, s->v.s2) )));
	t.put("umlal2.h (vec; acc. fwd.)",      lat(b,  op( g->umlal2(g->v28.s, s->v.h, s->v.h) )));
	t.put("umlal2.s (vec; acc. fwd.)",      lat(b,  op( g->umlal2(g->v28.d, s->v.s, s->v.s) )));

	t.put("umlal.h (elem; v.h[0])",         both(b, op( g->umlal(d->v.s, d->v.h4, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("umlal.h (elem; v.h[7])",         both(b, op( g->umlal(d->v.s, d->v.h4, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("umlal.s (elem; v.s[0])",         both(b, op( g->umlal(d->v.d, d->v.s2, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("umlal.s (elem; v.s[3])",         both(b, op( g->umlal(d->v.d, d->v.s2, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("umlal2.h (elem; v.h[0])",        both(b, op( g->umlal2(d->v.s, d->v.h, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("umlal2.h (elem; v.h[7])",        both(b, op( g->umlal2(d->v.s, d->v.h, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("umlal2.s (elem; v.s[0])",        both(b, op( g->umlal2(d->v.d, d->v.s, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("umlal2.s (elem; v.s[3])",        both(b, op( g->umlal2(d->v.d, d->v.s, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));

	t.put("sqdmlal.h (vec)",                both(b, op( g->sqdmlal(d->v.s, d->v.h4, s->v.h4) )));
	t.put("sqdmlal.s (vec)",                both(b, op( g->sqdmlal(d->v.d, d->v.s2, s->v.s2) )));
	t.put("sqdmlal2.h (vec)",               both(b, op( g->sqdmlal2(d->v.s, d->v.h, s->v.h) )));
	t.put("sqdmlal2.s (vec)",               both(b, op( g->sqdmlal2(d->v.d, d->v.s, s->v.s) )));
	t.put("sqdmlal.h (vec; acc. fwd.)",     lat(b,  op( g->sqdmlal(g->v28.s, s->v.h4, s->v.h4) )));
	t.put("sqdmlal.s (vec; acc. fwd.)",     lat(b,  op( g->sqdmlal(g->v28.d, s->v.s2, s->v.s2) )));
	t.put("sqdmlal2.h (vec; acc. fwd.)",    lat(b,  op( g->sqdmlal2(g->v28.s, s->v.h, s->v.h) )));
	t.put("sqdmlal2.s (vec; acc. fwd.)",    lat(b,  op( g->sqdmlal2(g->v28.d, s->v.s, s->v.s) )));

	t.put("sqdmlal.h (elem; v.h[0])",       both(b, op( g->sqdmlal(d->v.s, d->v.h4, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmlal.h (elem; v.h[7])",       both(b, op( g->sqdmlal(d->v.s, d->v.h4, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmlal.s (elem; v.s[0])",       both(b, op( g->sqdmlal(d->v.d, d->v.s2, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmlal.s (elem; v.s[3])",       both(b, op( g->sqdmlal(d->v.d, d->v.s2, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmlal2.h (elem; v.h[0])",      both(b, op( g->sqdmlal2(d->v.s, d->v.h, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmlal2.h (elem; v.h[7])",      both(b, op( g->sqdmlal2(d->v.s, d->v.h, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmlal2.s (elem; v.s[0])",      both(b, op( g->sqdmlal2(d->v.d, d->v.s, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmlal2.s (elem; v.s[3])",      both(b, op( g->sqdmlal2(d->v.d, d->v.s, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));

	t.put("sqrdmlah.h (vec)",               both(b, op( g->sqrdmlah(d->v.h, d->v.h, s->v.h) )));
	t.put("sqrdmlah.s (vec)",               both(b, op( g->sqrdmlah(d->v.s, d->v.s, s->v.s) )));
	t.put("sqrdmlah.h (vec; acc. fwd.)",    lat(b,  op( g->sqrdmlah(g->v28.h, s->v.h, s->v.h) )));
	t.put("sqrdmlah.s (vec; acc. fwd.)",    lat(b,  op( g->sqrdmlah(g->v28.s, s->v.s, s->v.s) )));

	t.put("sqrdmlah.h (elem; v.h[0])",      both(b, op( g->sqrdmlah(d->v.h, d->v.h, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqrdmlah.h (elem; v.h[7])",      both(b, op( g->sqrdmlah(d->v.h, d->v.h, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqrdmlah.s (elem; v.s[0])",      both(b, op( g->sqrdmlah(d->v.s, d->v.s, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqrdmlah.s (elem; v.s[3])",      both(b, op( g->sqrdmlah(d->v.s, d->v.s, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));

	/* mls */
	t.put("mls.b (vec)",                    both(b, op( g->mls(d->v.b, d->v.b, s->v.b) )));
	t.put("mls.h (vec)",                    both(b, op( g->mls(d->v.h, d->v.h, s->v.h) )));
	t.put("mls.s (vec)",                    both(b, op( g->mls(d->v.s, d->v.s, s->v.s) )));
	t.put("mls.b (vec; acc. fwd.)",         lat(b,  op( g->mls(g->v28.b, s->v.b, s->v.b) )));
	t.put("mls.h (vec; acc. fwd.)",         lat(b,  op( g->mls(g->v28.h, s->v.h, s->v.h) )));
	t.put("mls.s (vec; acc. fwd.)",         lat(b,  op( g->mls(g->v28.s, s->v.s, s->v.s) )));

	t.put("mls.h (elem; [0])",              both(b, op( g->mls(d->v.h, d->v.h, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("mls.h (elem; [7])",              both(b, op( g->mls(d->v.h, d->v.h, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("mls.s (elem; [0])",              both(b, op( g->mls(d->v.s, d->v.s, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("mls.s (elem; [3])",              both(b, op( g->mls(d->v.s, d->v.s, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));

	t.put("smlsl.h (vec)",                  both(b, op( g->smlsl(d->v.s, d->v.h4, s->v.h4) )));
	t.put("smlsl.s (vec)",                  both(b, op( g->smlsl(d->v.d, d->v.s2, s->v.s2) )));
	t.put("smlsl2.h (vec)",                 both(b, op( g->smlsl2(d->v.s, d->v.h, s->v.h) )));
	t.put("smlsl2.s (vec)",                 both(b, op( g->smlsl2(d->v.d, d->v.s, s->v.s) )));
	t.put("smlsl.h (vec; acc. fwd.)",       lat(b,  op( g->smlsl(g->v28.s, s->v.h4, s->v.h4) )));
	t.put("smlsl.s (vec; acc. fwd.)",       lat(b,  op( g->smlsl(g->v28.d, s->v.s2, s->v.s2) )));
	t.put("smlsl2.h (vec; acc. fwd.)",      lat(b,  op( g->smlsl2(g->v28.s, s->v.h, s->v.h) )));
	t.put("smlsl2.s (vec; acc. fwd.)",      lat(b,  op( g->smlsl2(g->v28.d, s->v.s, s->v.s) )));

	t.put("smlsl.h (elem; v.h[0])",         both(b, op( g->smlsl(d->v.s, d->v.h4, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("smlsl.h (elem; v.h[7])",         both(b, op( g->smlsl(d->v.s, d->v.h4, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("smlsl.s (elem; v.s[0])",         both(b, op( g->smlsl(d->v.d, d->v.s2, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("smlsl.s (elem; v.s[3])",         both(b, op( g->smlsl(d->v.d, d->v.s2, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("smlsl2.h (elem; v.h[0])",        both(b, op( g->smlsl2(d->v.s, d->v.h, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("smlsl2.h (elem; v.h[7])",        both(b, op( g->smlsl2(d->v.s, d->v.h, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("smlsl2.s (elem; v.s[0])",        both(b, op( g->smlsl2(d->v.d, d->v.s, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("smlsl2.s (elem; v.s[3])",        both(b, op( g->smlsl2(d->v.d, d->v.s, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));

	t.put("umlsl.h (vec)",                  both(b, op( g->umlsl(d->v.s, d->v.h4, s->v.h4) )));
	t.put("umlsl.s (vec)",                  both(b, op( g->umlsl(d->v.d, d->v.s2, s->v.s2) )));
	t.put("umlsl2.h (vec)",                 both(b, op( g->umlsl2(d->v.s, d->v.h, s->v.h) )));
	t.put("umlsl2.s (vec)",                 both(b, op( g->umlsl2(d->v.d, d->v.s, s->v.s) )));
	t.put("umlsl.h (vec; acc. fwd.)",       lat(b,  op( g->umlsl(g->v28.s, s->v.h4, s->v.h4) )));
	t.put("umlsl.s (vec; acc. fwd.)",       lat(b,  op( g->umlsl(g->v28.d, s->v.s2, s->v.s2) )));
	t.put("umlsl2.h (vec; acc. fwd.)",      lat(b,  op( g->umlsl2(g->v28.s, s->v.h, s->v.h) )));
	t.put("umlsl2.s (vec; acc. fwd.)",      lat(b,  op( g->umlsl2(g->v28.d, s->v.s, s->v.s) )));

	t.put("umlsl.h (elem; v.h[0])",         both(b, op( g->umlsl(d->v.s, d->v.h4, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("umlsl.h (elem; v.h[7])",         both(b, op( g->umlsl(d->v.s, d->v.h4, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("umlsl.s (elem; v.s[0])",         both(b, op( g->umlsl(d->v.d, d->v.s2, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("umlsl.s (elem; v.s[3])",         both(b, op( g->umlsl(d->v.d, d->v.s2, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("umlsl2.h (elem; v.h[0])",        both(b, op( g->umlsl2(d->v.s, d->v.h, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("umlsl2.h (elem; v.h[7])",        both(b, op( g->umlsl2(d->v.s, d->v.h, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("umlsl2.s (elem; v.s[0])",        both(b, op( g->umlsl2(d->v.d, d->v.s, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("umlsl2.s (elem; v.s[3])",        both(b, op( g->umlsl2(d->v.d, d->v.s, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));

	t.put("sqdmlsl.h (vec)",                both(b, op( g->sqdmlsl(d->v.s, d->v.h4, s->v.h4) )));
	t.put("sqdmlsl.s (vec)",                both(b, op( g->sqdmlsl(d->v.d, d->v.s2, s->v.s2) )));
	t.put("sqdmlsl2.h (vec)",               both(b, op( g->sqdmlsl2(d->v.s, d->v.h, s->v.h) )));
	t.put("sqdmlsl2.s (vec)",               both(b, op( g->sqdmlsl2(d->v.d, d->v.s, s->v.s) )));
	t.put("sqdmlsl.h (vec; acc. fwd.)",     lat(b,  op( g->sqdmlsl(g->v28.s, s->v.h4, s->v.h4) )));
	t.put("sqdmlsl.s (vec; acc. fwd.)",     lat(b,  op( g->sqdmlsl(g->v28.d, s->v.s2, s->v.s2) )));
	t.put("sqdmlsl2.h (vec; acc. fwd.)",    lat(b,  op( g->sqdmlsl2(g->v28.s, s->v.h, s->v.h) )));
	t.put("sqdmlsl2.s (vec; acc. fwd.)",    lat(b,  op( g->sqdmlsl2(g->v28.d, s->v.s, s->v.s) )));

	t.put("sqdmlsl.h (elem; v.h[0])",       both(b, op( g->sqdmlsl(d->v.s, d->v.h4, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmlsl.h (elem; v.h[7])",       both(b, op( g->sqdmlsl(d->v.s, d->v.h4, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmlsl.s (elem; v.s[0])",       both(b, op( g->sqdmlsl(d->v.d, d->v.s2, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmlsl.s (elem; v.s[3])",       both(b, op( g->sqdmlsl(d->v.d, d->v.s2, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmlsl2.h (elem; v.h[0])",      both(b, op( g->sqdmlsl2(d->v.s, d->v.h, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmlsl2.h (elem; v.h[7])",      both(b, op( g->sqdmlsl2(d->v.s, d->v.h, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmlsl2.s (elem; v.s[0])",      both(b, op( g->sqdmlsl2(d->v.d, d->v.s, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqdmlsl2.s (elem; v.s[3])",      both(b, op( g->sqdmlsl2(d->v.d, d->v.s, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));

	t.put("sqrdmlsh.h (vec)",               both(b, op( g->sqrdmlsh(d->v.h, d->v.h, s->v.h) )));
	t.put("sqrdmlsh.s (vec)",               both(b, op( g->sqrdmlsh(d->v.s, d->v.s, s->v.s) )));
	t.put("sqrdmlsh.h (vec; acc. fwd.)",    lat(b,  op( g->sqrdmlsh(g->v28.h, s->v.h, s->v.h) )));
	t.put("sqrdmlsh.s (vec; acc. fwd.)",    lat(b,  op( g->sqrdmlsh(g->v28.s, s->v.s, s->v.s) )));
	t.put("sqrdmlsh.h (elem; v.h[0])",      both(b, op( g->sqrdmlsh(d->v.h, d->v.h, s->v.h[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqrdmlsh.h (elem; v.h[7])",      both(b, op( g->sqrdmlsh(d->v.h, d->v.h, s->v.h[7]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqrdmlsh.s (elem; v.s[0])",      both(b, op( g->sqrdmlsh(d->v.s, d->v.s, s->v.s[0]) ), 0.0, lat_patterns, thr_half_patterns));
	t.put("sqrdmlsh.s (elem; v.s[3])",      both(b, op( g->sqrdmlsh(d->v.s, d->v.s, s->v.s[3]) ), 0.0, lat_patterns, thr_half_patterns));

	t.put("sdot.b (vec)",                   both(b, op( g->sdot(d->v.s, d->v.b, s->v.b) )));
	t.put("sdot.b (elem; v.b[0])",          both(b, op( g->sdot(d->v.s, d->v.b, s->v.b4[0]) )));
	t.put("sdot.b (elem; v.b[3])",          both(b, op( g->sdot(d->v.s, d->v.b, s->v.b4[3]) )));

	/* TODO: waiting for xbyak_aarch64 implement Armv8.6 extension */
	// t.put("sudot.b (vec)",                  both(b, op( g->sudot(d->v.s, d->v.b, s->v.b) )));
	// t.put("sudot.b (elem; v.b[0])",         both(b, op( g->sudot(d->v.s, d->v.b, s->v.b4[0]) )));
	// t.put("sudot.b (elem; v.b[3])",         both(b, op( g->sudot(d->v.s, d->v.b, s->v.b4[3]) )));
	// t.put("smmla.b",                        both(b, op( g->smmla(d->v.s, d->v.b, s->v.b) )));
	// t.put("ummla.b",                        both(b, op( g->ummla(d->v.s, d->v.b, s->v.b) )));
	// t.put("usmmla.b",                       both(b, op( g->usmmla(d->v.s, d->v.b, s->v.b) )));
	// t.put("usdot.b (vec)",                  both(b, op( g->usdot(d->v.s, d->v.b, s->v.b) )));
	// t.put("usdot.b (elem; v.b[0])",         both(b, op( g->usdot(d->v.s, d->v.b, s->v.b4[0]) )));
	// t.put("usdot.b (elem; v.b[3])",         both(b, op( g->usdot(d->v.s, d->v.b, s->v.b4[3]) )));
	return;
}


static
void bench_aba_vec(bool md, double freq) {
	table t(md, "Vector integer absolute difference accumulate");
	bench b(freq);

	t.put("saba.b",                         both(b, op( g->saba(d->v.b, d->v.b, s->v.b) )));
	t.put("saba.h",                         both(b, op( g->saba(d->v.h, d->v.h, s->v.h) )));
	t.put("saba.s",                         both(b, op( g->saba(d->v.s, d->v.s, s->v.s) )));
	t.put("saba.b (acc. fwd.)",             lat(b,  op( g->saba(g->v28.b, s->v.b, s->v.b) )));
	t.put("saba.h (acc. fwd.)",             lat(b,  op( g->saba(g->v28.h, s->v.h, s->v.h) )));
	t.put("saba.s (acc. fwd.)",             lat(b,  op( g->saba(g->v28.s, s->v.s, s->v.s) )));

	t.put("uaba.b",                         both(b, op( g->uaba(d->v.b, d->v.b, s->v.b) )));
	t.put("uaba.h",                         both(b, op( g->uaba(d->v.h, d->v.h, s->v.h) )));
	t.put("uaba.s",                         both(b, op( g->uaba(d->v.s, d->v.s, s->v.s) )));
	t.put("uaba.b (acc. fwd.)",             lat(b,  op( g->uaba(g->v28.b, s->v.b, s->v.b) )));
	t.put("uaba.h (acc. fwd.)",             lat(b,  op( g->uaba(g->v28.h, s->v.h, s->v.h) )));
	t.put("uaba.s (acc. fwd.)",             lat(b,  op( g->uaba(g->v28.s, s->v.s, s->v.s) )));

	t.put("sabal.b",                        both(b, op( g->sabal(d->v.h, d->v.b8, s->v.b8) )));
	t.put("sabal.h",                        both(b, op( g->sabal(d->v.s, d->v.h4, s->v.h4) )));
	t.put("sabal.s",                        both(b, op( g->sabal(d->v.d, d->v.s2, s->v.s2) )));
	t.put("sabal2.b",                       both(b, op( g->sabal2(d->v.h, d->v.b, s->v.b) )));
	t.put("sabal2.h",                       both(b, op( g->sabal2(d->v.s, d->v.h, s->v.h) )));
	t.put("sabal2.s",                       both(b, op( g->sabal2(d->v.d, d->v.s, s->v.s) )));

	t.put("uabal.b",                        both(b, op( g->uabal(d->v.h, d->v.b8, s->v.b8) )));
	t.put("uabal.h",                        both(b, op( g->uabal(d->v.s, d->v.h4, s->v.h4) )));
	t.put("uabal.s",                        both(b, op( g->uabal(d->v.d, d->v.s2, s->v.s2) )));
	t.put("uabal2.b",                       both(b, op( g->uabal2(d->v.h, d->v.b, s->v.b) )));
	t.put("uabal2.h",                       both(b, op( g->uabal2(d->v.s, d->v.h, s->v.h) )));
	t.put("uabal2.s",                       both(b, op( g->uabal2(d->v.d, d->v.s, s->v.s) )));

	t.put("sadalp.b",                       both(b, op( g->sadalp(d->v.h, s->v.b) )));
	t.put("sadalp.h",                       both(b, op( g->sadalp(d->v.s, s->v.h) )));
	t.put("sadalp.s",                       both(b, op( g->sadalp(d->v.d, s->v.s) )));

	t.put("uadalp.b",                       both(b, op( g->uadalp(d->v.h, s->v.b) )));
	t.put("uadalp.h",                       both(b, op( g->uadalp(d->v.s, s->v.h) )));
	t.put("uadalp.s",                       both(b, op( g->uadalp(d->v.d, s->v.s) )));
	return;
}


static
void bench_recip_vec(bool md, double freq) {
	table t(md, "Vector integer divide");
	bench b(freq);

	t.put("urecpe.s",                       both(b, op( g->urecpe(d->v.s, s->v.s) )));
	t.put("ursqrte.s",                      both(b, op( g->ursqrte(d->v.s, s->v.s) )));
	return;
}

void bench_ext_arith_vec(bool md, double freq) {
	bench_mul_vec(md, freq);
	bench_mla_vec(md, freq);
	bench_aba_vec(md, freq);
	bench_recip_vec(md, freq);
	return;
}

/*
 * end of arith_vec_mul.cc
 */
