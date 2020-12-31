
/*
 * @file math.cc
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>

void bench_conv_fp(bool md, double freq) {
	table t(md, "Floating point math");
	bench b(freq, (size_t)0, 0);

	t.put("frsqrte.h (scl)",            both(b, op( g->frsqrte(d->h, s->h) )));
	t.put("frsqrte.s (scl)",            both(b, op( g->frsqrte(d->s, s->s) )));
	t.put("frsqrte.d (scl)",            both(b, op( g->frsqrte(d->d, s->d) )));
	t.put("frsqrte.h (vec)",            both(b, op( g->frsqrte(d->v.h, s->v.h) )));
	t.put("frsqrte.s (vec)",            both(b, op( g->frsqrte(d->v.s, s->v.s) )));
	t.put("frsqrte.d (vec)",            both(b, op( g->frsqrte(d->v.d, s->v.d) )));

	t.put("frsqrts.h (scl)",            both(b, op( g->frsqrts(d->h, d->h, s->h) )));
	t.put("frsqrts.s (scl)",            both(b, op( g->frsqrts(d->s, d->s, s->s) )));
	t.put("frsqrts.d (scl)",            both(b, op( g->frsqrts(d->d, d->d, s->d) )));
	t.put("frsqrts.h (vec)",            both(b, op( g->frsqrts(d->v.h, d->v.h, s->v.h) )));
	t.put("frsqrts.s (vec)",            both(b, op( g->frsqrts(d->v.s, d->v.s, s->v.s) )));
	t.put("frsqrts.d (vec)",            both(b, op( g->frsqrts(d->v.d, d->v.d, s->v.d) )));

	t.put("fsqrt.h (scl)",              both(b, op( g->fsqrt(d->h, s->h) )));
	t.put("fsqrt.s (scl)",              both(b, op( g->fsqrt(d->s, s->s) )));
	t.put("fsqrt.d (scl)",              both(b, op( g->fsqrt(d->d, s->d) )));
	t.put("fsqrt.h (vec)",              both(b, op( g->fsqrt(d->v.h, s->v.h) )));
	t.put("fsqrt.s (vec)",              both(b, op( g->fsqrt(d->v.s, s->v.s) )));
	t.put("fsqrt.d (vec)",              both(b, op( g->fsqrt(d->v.d, s->v.d) )));
	return;
}

/*
 * end of math.cc
 */
