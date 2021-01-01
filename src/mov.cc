
/**
 * @file mov.cc
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>

void bench_mov(bool md, double freq) {
	table t(md, "Register move");
	bench b(freq, (size_t)0, 0);

	/* decoder / allocation throughput */
	t.put("nop",                        thr(b, op( g->nop() )));
	t.put("mov (x -> x)",               both(b, op( g->mov(d->x, s->x) )));
	t.put("mov (v.b -> v.b)",           both(b, op( g->mov(d->v.b, s->v.b) )));
	t.put("eor (reg; zeroing idiom)",   both(b, op( g->eor(d->x, s->x, s->x) )));
	t.put("eor.b (reg; zeroing idiom)", both(b, op( g->eor(d->v.b, s->v.b, s->v.b) )));
	t.put("sub (reg; zeroing idiom)",   both(b, op( g->sub(d->x, s->x, s->x) )));
	t.put("sub.b (reg; zeroing idiom)", both(b, op( g->sub(d->v.b, s->v.b, s->v.b) )));

	/* immediate */
	t.put("mov / movz (imm)",           thr(b, op( g->mov(d->x, 0x1ffc) )));
	t.put("mov / movz (imm<<16)",       thr(b, op( g->mov(d->x, 0x1ffc<<16) )));
	t.put("mov (mask imm)",             thr(b, op( g->mov(d->x, 0x1ffffffffffc) )));
	t.put("mov / movn (imm)",           thr(b, op( g->movn(d->x, 0x1ffc) )));
	t.put("mov / movn (imm<<16)",       thr(b, op( g->movn(d->x, 0x1ffc, 16) )));
	t.put("movk",                       thr(b, op( g->movk(d->x, 0x1ffc) )));
	t.put("movk (<<16)",                thr(b, op( g->movk(d->x, 0x1ffc, 16) )));
	return;
}

void bench_mov_vec(bool md, double freq) {
	table t(md, "Vector element move");
	bench b(freq, (size_t)0, 0);

	double const mov_latency = b.lat_(0, op( g->mov(d->v.d[0], s->x); g->mov(d->x, d->v.d[0]) )).lat / 2.0;

	t.put("mov.s (v.s[0] <-> w)",       lat(b, op( g->mov(d->v.s[0], s->w); g->mov(d->w, d->v.s[0]) )));
	t.put("mov.d (v.d[0] <-> x)",       lat(b, op( g->mov(d->v.d[0], s->x); g->mov(d->x, d->v.d[0]) )));
	t.put("mov.s (v.s[3] <-> w)",       lat(b, op( g->mov(d->v.s[3], s->w); g->mov(d->w, d->v.s[3]) )));
	t.put("mov.d (v.d[1] <-> x)",       lat(b, op( g->mov(d->v.d[1], s->x); g->mov(d->x, d->v.d[1]) )));
	t.put("smov.b (v.b[0] <-> x)",      lat(b, op( g->mov(d->v.b[0], s->w); g->smov(d->w, d->v.b[0]) )));
	t.put("smov.h (v.h[0] <-> x)",      lat(b, op( g->mov(d->v.h[0], s->w); g->smov(d->w, d->v.h[0]) )));
	t.put("smov.s (v.s[0] <-> x)",      lat(b, op( g->mov(d->v.s[0], s->w); g->smov(d->x, d->v.s[0]) )));
	t.put("umov.b (v.b[0] <-> w)",      lat(b, op( g->mov(d->v.b[0], s->w); g->umov(d->w, d->v.b[0]) )));
	t.put("umov.h (v.h[0] <-> w)",      lat(b, op( g->mov(d->v.h[0], s->w); g->umov(d->w, d->v.h[0]) )));
	t.put("umov.s (v.s[0] <-> w)",      lat(b, op( g->mov(d->v.s[0], s->w); g->umov(d->w, d->v.s[0]) )));
	t.put("umov.d (v.d[0] <-> x)",      lat(b, op( g->mov(d->v.d[0], s->x); g->umov(d->x, d->v.d[0]) )));
	t.put("fmov.d (v.d[0] <-> x)",      lat(b, op( g->fmov(d->v.d[0], s->x); g->fmov(d->x, d->v.d[0]) )));

	// t.put("mov.b (v.b[0] <-> w)",       lat(b, op( g->mov(d->v.b[0], s->w); g->mov(d->x, d->v.b[0]) )));
	// t.put("mov.h (v.h[0] <-> x)",       lat(b, op( g->mov(d->v.h[0], s->x); g->mov(d->x, d->v.h[0]) )));
	// t.put("mov.b (v.b[15] <-> w)",      lat(b, op( g->mov(d->v.b[15], s->w); g->mov(d->x, d->v.b[15]) )));
	// t.put("mov.h (v.h[7] <-> x)",       lat(b, op( g->mov(d->v.h[7], s->x); g->mov(d->x, d->v.h[7]) )));
	// t.put("fmov.h (v.h[0] <-> x)",      lat(b, op( g->fmov(d->v.h[0], s->x); g->fmov(d->x, d->v.h[0]) )));
	// t.put("fmov.s (v.s[0] <-> x)",      lat(b, op( g->fmov(d->v.s[0], s->w); g->fmov(d->x, d->v.s[0]) )));

	t.put("dup.b (vec; lane = 0)",      both(b, op( g->dup(d->v.b, s->v.b[0]) )));
	t.put("dup.b (vec; lane = 15)",     both(b, op( g->dup(d->v.b, s->v.b[15]) )));
	t.put("dup.h (vec; lane = 7)",      both(b, op( g->dup(d->v.h, s->v.h[7]) )));
	t.put("dup.s (vec; lane = 3)",      both(b, op( g->dup(d->v.s, s->v.s[3]) )));
	t.put("dup.d (vec; lane = 1)",      both(b, op( g->dup(d->v.d, s->v.d[1]) )));

	// t.put("dup.b (elem)",               both(b, op( g->dup(d->v.b, s->v.b[0]) )));
	// t.put("dup.b (elem)",               both(b, op( g->dup(d->v.b, s->v.b[15]) )));
	// t.put("dup.h (elem)",               both(b, op( g->dup(d->v.h, s->v.h[7]) )));
	t.put("dup.b (elem)",               both(b, op( g->dup(d->v.b, s->w) ),                         op( g->mov(d->w, d->v.s[0]) ), mov_latency));
	t.put("dup.h (elem)",               both(b, op( g->dup(d->v.h, s->w) ),                         op( g->mov(d->w, d->v.s[0]) ), mov_latency));
	t.put("dup.s (elem)",               both(b, op( g->dup(d->v.s, s->w) ),                         op( g->mov(d->w, d->v.s[0]) ), mov_latency));
	t.put("dup.d (elem)",               both(b, op( g->dup(d->v.d, s->x) ),                         op( g->mov(d->x, d->v.d[0]) ), mov_latency));

	t.put("xtn.h",                      both(b, op( g->xtn(d->v.b8, s->v.h) )));
	t.put("xtn.s",                      both(b, op( g->xtn(d->v.h4, s->v.s) )));
	t.put("xtn.d",                      both(b, op( g->xtn(d->v.s2, s->v.d) )));

	t.put("sqxtn.h (scl)",              both(b, op( g->sqxtn(d->b, s->h) )));
	t.put("sqxtn.s (scl)",              both(b, op( g->sqxtn(d->h, s->s) )));
	t.put("sqxtn.d (scl)",              both(b, op( g->sqxtn(d->s, s->d) )));

	t.put("sqxtn.h (vec)",              both(b, op( g->sqxtn(d->v.b8, s->v.h) )));
	t.put("sqxtn.s (vec)",              both(b, op( g->sqxtn(d->v.h4, s->v.s) )));
	t.put("sqxtn.d (vec)",              both(b, op( g->sqxtn(d->v.s2, s->v.d) )));
	t.put("sqxtn2.h (vec)",             both(b, op( g->sqxtn2(d->v.b, s->v.h) )));
	t.put("sqxtn2.s (vec)",             both(b, op( g->sqxtn2(d->v.h, s->v.s) )));
	t.put("sqxtn2.d (vec)",             both(b, op( g->sqxtn2(d->v.s, s->v.d) )));

	t.put("uqxtn.h (scl)",              both(b, op( g->uqxtn(d->b, s->h) )));
	t.put("uqxtn.s (scl)",              both(b, op( g->uqxtn(d->h, s->s) )));
	t.put("uqxtn.d (scl)",              both(b, op( g->uqxtn(d->s, s->d) )));

	t.put("uqxtn.h (vec)",              both(b, op( g->uqxtn(d->v.b8, s->v.h) )));
	t.put("uqxtn.s (vec)",              both(b, op( g->uqxtn(d->v.h4, s->v.s) )));
	t.put("uqxtn.d (vec)",              both(b, op( g->uqxtn(d->v.s2, s->v.d) )));
	t.put("uqxtn2.h (vec)",             both(b, op( g->uqxtn2(d->v.b, s->v.h) )));
	t.put("uqxtn2.s (vec)",             both(b, op( g->uqxtn2(d->v.h, s->v.s) )));
	t.put("uqxtn2.d (vec)",             both(b, op( g->uqxtn2(d->v.s, s->v.d) )));

	t.put("sqxtun.h (scl)",             both(b, op( g->sqxtun(d->b, s->h) )));
	t.put("sqxtun.s (scl)",             both(b, op( g->sqxtun(d->h, s->s) )));
	t.put("sqxtun.d (scl)",             both(b, op( g->sqxtun(d->s, s->d) )));

	t.put("sqxtun.h (vec)",             both(b, op( g->sqxtun(d->v.b8, s->v.h) )));
	t.put("sqxtun.s (vec)",             both(b, op( g->sqxtun(d->v.h4, s->v.s) )));
	t.put("sqxtun.d (vec)",             both(b, op( g->sqxtun(d->v.s2, s->v.d) )));
	t.put("sqxtun2.h (vec)",            both(b, op( g->sqxtun2(d->v.b, s->v.h) )));
	t.put("sqxtun2.s (vec)",            both(b, op( g->sqxtun2(d->v.h, s->v.s) )));
	t.put("sqxtun2.d (vec)",            both(b, op( g->sqxtun2(d->v.s, s->v.d) )));

	t.put("sxtl.b (vec)",               both(b, op( g->sxtl(d->v.h, s->v.b8) )));
	t.put("sxtl.h (vec)",               both(b, op( g->sxtl(d->v.s, s->v.h4) )));
	t.put("sxtl.s (vec)",               both(b, op( g->sxtl(d->v.d, s->v.s2) )));
	t.put("sxtl2.b (vec)",              both(b, op( g->sxtl2(d->v.h, s->v.b) )));
	t.put("sxtl2.h (vec)",              both(b, op( g->sxtl2(d->v.s, s->v.h) )));
	t.put("sxtl2.s (vec)",              both(b, op( g->sxtl2(d->v.d, s->v.s) )));

	t.put("uxtl.b (vec)",               both(b, op( g->uxtl(d->v.h, s->v.b8) )));
	t.put("uxtl.h (vec)",               both(b, op( g->uxtl(d->v.s, s->v.h4) )));
	t.put("uxtl.s (vec)",               both(b, op( g->uxtl(d->v.d, s->v.s2) )));
	t.put("uxtl2.b (vec)",              both(b, op( g->uxtl2(d->v.h, s->v.b) )));
	t.put("uxtl2.h (vec)",              both(b, op( g->uxtl2(d->v.s, s->v.h) )));
	t.put("uxtl2.s (vec)",              both(b, op( g->uxtl2(d->v.d, s->v.s) )));
	return;
}

void bench_perm(bool md, double freq) {
	table t(md, "Vector permutation");
	bench b(freq, (size_t)0, 0);

	// t.put("tbl (len == 1)",             both(b, op( g->tbl(d->v.b, g->v24, 1, s->v.b) ), 0.0, lat_patterns, thr_half_patterns));
	return;
}

/*
 * end of mov.cc
 */
