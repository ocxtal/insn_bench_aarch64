
/**
 * @file atomic.cc
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>

void bench_atomic(bool md, double freq) {
	table t(md, "Atomic memory operation");

	memmgr m_z(mem_init( (void *)0 ));
	bench b(freq, NULL, m_z.ptr());

	t.put("casal",                                      both(b, op( g->casal(d->x,     s->x, ptr(g->x28)) )));
	t.put("caspal",                                     both(b, op( g->caspal(s[0].x,  s[2].x, ptr(g->x28)) ), 0.0, lat_flat0_pattern, thr_skip4x_patterns));
	t.put("casalb",                                     both(b, op( g->casalb(d->w,    s->w, ptr(g->x28)) )));
	t.put("casalh",                                     both(b, op( g->casalh(d->w,    s->w, ptr(g->x28)) )));

	t.put("ldaddal",                                    both(b, op( g->ldaddal(d->x,   s->x, ptr(g->x28)) )));
	t.put("ldaddalb",                                   both(b, op( g->ldaddalb(d->w,  s->w, ptr(g->x28)) )));
	t.put("ldaddalh",                                   both(b, op( g->ldaddalh(d->w,  s->w, ptr(g->x28)) )));

	t.put("ldclral",                                    both(b, op( g->ldclral(d->x,   s->x, ptr(g->x28)) )));
	t.put("ldclralb",                                   both(b, op( g->ldclralb(d->w,  s->w, ptr(g->x28)) )));
	t.put("ldclralh",                                   both(b, op( g->ldclralh(d->w,  s->w, ptr(g->x28)) )));

	t.put("ldsetal",                                    both(b, op( g->ldsetal(d->x,   s->x, ptr(g->x28)) )));
	t.put("ldsetalb",                                   both(b, op( g->ldsetalb(d->w,  s->w, ptr(g->x28)) )));
	t.put("ldsetalh",                                   both(b, op( g->ldsetalh(d->w,  s->w, ptr(g->x28)) )));

	t.put("ldeoral",                                    both(b, op( g->ldeoral(d->x,   s->x, ptr(g->x28)) )));
	t.put("ldeoralb",                                   both(b, op( g->ldeoralb(d->w,  s->w, ptr(g->x28)) )));
	t.put("ldeoralh",                                   both(b, op( g->ldeoralh(d->w,  s->w, ptr(g->x28)) )));

	t.put("ldumaxal",                                   both(b, op( g->ldumaxal(d->x,  s->x, ptr(g->x28)) )));
	t.put("ldumaxalb",                                  both(b, op( g->ldumaxalb(d->w, s->w, ptr(g->x28)) )));
	t.put("ldumaxalh",                                  both(b, op( g->ldumaxalh(d->w, s->w, ptr(g->x28)) )));

	t.put("ldsmaxal",                                   both(b, op( g->ldsmaxal(d->x,  s->x, ptr(g->x28)) )));
	t.put("ldsmaxalb",                                  both(b, op( g->ldsmaxalb(d->w, s->w, ptr(g->x28)) )));
	t.put("ldsmaxalh",                                  both(b, op( g->ldsmaxalh(d->w, s->w, ptr(g->x28)) )));

	t.put("lduminal",                                   both(b, op( g->lduminal(d->x,  s->x, ptr(g->x28)) )));
	t.put("lduminalb",                                  both(b, op( g->lduminalb(d->w, s->w, ptr(g->x28)) )));
	t.put("lduminalh",                                  both(b, op( g->lduminalh(d->w, s->w, ptr(g->x28)) )));

	t.put("ldsminal",                                   both(b, op( g->ldsminal(d->x,  s->x, ptr(g->x28)) )));
	t.put("ldsminalb",                                  both(b, op( g->ldsminalb(d->w, s->w, ptr(g->x28)) )));
	t.put("ldsminalh",                                  both(b, op( g->ldsminalh(d->w, s->w, ptr(g->x28)) )));
	return;
}

/*
 * end of atomic.cc
 */
