
/*
 * @file cond.cc
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>

void bench_cond(bool md, double freq) {
	table t(md, "Conditional arithmetic and logic");
	bench b(freq);

	t.put("csinc (eq)",                 both(b, op( g->csinc(d->x, d->x, s->x, Cond::EQ) )));
	t.put("csinc (lt)",                 both(b, op( g->csinc(d->x, d->x, s->x, Cond::LT) )));
	t.put("cinc (eq)",                  both(b, op( g->cinc(d->x, s->x, Cond::EQ) )));
	t.put("cinc (lt)",                  both(b, op( g->cinc(d->x, s->x, Cond::LT) )));
	t.put("csinv (eq)",                 both(b, op( g->csinv(d->x, d->x, s->x, Cond::EQ) )));
	t.put("csinv (lt)",                 both(b, op( g->csinv(d->x, d->x, s->x, Cond::LT) )));
	t.put("cset (eq)",                  thr(b, op( g->cset(s->x, Cond::EQ) ) ));
	t.put("cset (lt)",                  thr(b, op( g->cset(s->x, Cond::LT) ) ));
	t.put("csetm (eq)",                 thr(b, op( g->csetm(s->x, Cond::EQ) ) ));
	t.put("csetm (lt)",                 thr(b, op( g->csetm(s->x, Cond::LT) ) ));
	t.put("cinv (eq)",                  both(b, op( g->cinv(d->x, s->x, Cond::EQ) )));
	t.put("cinv (lt)",                  both(b, op( g->cinv(d->x, s->x, Cond::LT) )));
	t.put("csneg (eq)",                 both(b, op( g->csneg(d->x, d->x, s->x, Cond::EQ) )));
	t.put("csneg (lt)",                 both(b, op( g->csneg(d->x, d->x, s->x, Cond::LT) )));
	t.put("cneg (eq)",                  both(b, op( g->cneg(d->x, s->x, Cond::EQ) )));
	t.put("cneg (lt)",                  both(b, op( g->cneg(d->x, s->x, Cond::LT) )));
	return;
}

void bench_cond_fp(bool md, double freq) {
	table t(md, "Floating point conditional select");
	bench b(freq);

	t.put("fcsel.h (eq)",               both(b, op( g->fcsel(d->h, d->h, s->h, Cond::EQ) )));
	t.put("fcsel.h (le)",               both(b, op( g->fcsel(d->h, d->h, s->h, Cond::LE) )));
	t.put("fcsel.s (eq)",               both(b, op( g->fcsel(d->s, d->s, s->s, Cond::EQ) )));
	t.put("fcsel.s (le)",               both(b, op( g->fcsel(d->s, d->s, s->s, Cond::LE) )));
	t.put("fcsel.d (eq)",               both(b, op( g->fcsel(d->d, d->d, s->d, Cond::EQ) )));
	t.put("fcsel.d (le)",               both(b, op( g->fcsel(d->d, d->d, s->d, Cond::LE) )));
	return;
}

/*
 * end of cond.cc
 */
