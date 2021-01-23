
/**
 * @file arith_vec.cc
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>

static
void bench_basic_arith_vec(bool md, double freq) {
	table t(md, "Vector integer add, sub, abs, and neg");
	bench b(freq);

	t.put("add.b",                      both(b, op( g->add(d->v.b, d->v.b, s->v.b) )));
	t.put("add.h",                      both(b, op( g->add(d->v.h, d->v.h, s->v.h) )));
	t.put("add.s",                      both(b, op( g->add(d->v.s, d->v.s, s->v.s) )));
	t.put("add.d",                      both(b, op( g->add(d->v.d, d->v.d, s->v.d) )));

	t.put("sqadd.b",                    both(b, op( g->sqadd(d->v.b, d->v.b, s->v.b) )));
	t.put("sqadd.h",                    both(b, op( g->sqadd(d->v.h, d->v.h, s->v.h) )));
	t.put("sqadd.s",                    both(b, op( g->sqadd(d->v.s, d->v.s, s->v.s) )));
	t.put("sqadd.d",                    both(b, op( g->sqadd(d->v.d, d->v.d, s->v.d) )));

	t.put("uqadd.b",                    both(b, op( g->uqadd(d->v.b, d->v.b, s->v.b) )));
	t.put("uqadd.h",                    both(b, op( g->uqadd(d->v.h, d->v.h, s->v.h) )));
	t.put("uqadd.s",                    both(b, op( g->uqadd(d->v.s, d->v.s, s->v.s) )));
	t.put("uqadd.d",                    both(b, op( g->uqadd(d->v.d, d->v.d, s->v.d) )));

	t.put("suqadd.b (scl)",             both(b, op( g->suqadd(d->b, s->b) )));
	t.put("suqadd.h (scl)",             both(b, op( g->suqadd(d->h, s->h) )));
	t.put("suqadd.s (scl)",             both(b, op( g->suqadd(d->s, s->s) )));
	t.put("suqadd.d (scl)",             both(b, op( g->suqadd(d->d, s->d) )));
	t.put("suqadd.b (vec)",             both(b, op( g->suqadd(d->v.b, s->v.b) )));
	t.put("suqadd.h (vec)",             both(b, op( g->suqadd(d->v.h, s->v.h) )));
	t.put("suqadd.s (vec)",             both(b, op( g->suqadd(d->v.s, s->v.s) )));
	t.put("suqadd.d (vec)",             both(b, op( g->suqadd(d->v.d, s->v.d) )));

	t.put("usqadd.b (scl)",             both(b, op( g->usqadd(d->b, s->b) )));
	t.put("usqadd.h (scl)",             both(b, op( g->usqadd(d->h, s->h) )));
	t.put("usqadd.s (scl)",             both(b, op( g->usqadd(d->s, s->s) )));
	t.put("usqadd.d (scl)",             both(b, op( g->usqadd(d->d, s->d) )));
	t.put("usqadd.b (vec)",             both(b, op( g->usqadd(d->v.b, s->v.b) )));
	t.put("usqadd.h (vec)",             both(b, op( g->usqadd(d->v.h, s->v.h) )));
	t.put("usqadd.s (vec)",             both(b, op( g->usqadd(d->v.s, s->v.s) )));
	t.put("usqadd.d (vec)",             both(b, op( g->usqadd(d->v.d, s->v.d) )));

	t.put("sub.b",                      both(b, op( g->sub(d->v.b, d->v.b, s->v.b) )));
	t.put("sub.h",                      both(b, op( g->sub(d->v.h, d->v.h, s->v.h) )));
	t.put("sub.s",                      both(b, op( g->sub(d->v.s, d->v.s, s->v.s) )));
	t.put("sub.d",                      both(b, op( g->sub(d->v.d, d->v.d, s->v.d) )));

	t.put("sqsub.b",                    both(b, op( g->sqsub(d->v.b, d->v.b, s->v.b) )));
	t.put("sqsub.h",                    both(b, op( g->sqsub(d->v.h, d->v.h, s->v.h) )));
	t.put("sqsub.s",                    both(b, op( g->sqsub(d->v.s, d->v.s, s->v.s) )));
	t.put("sqsub.d",                    both(b, op( g->sqsub(d->v.d, d->v.d, s->v.d) )));

	t.put("uqsub.b",                    both(b, op( g->uqsub(d->v.b, d->v.b, s->v.b) )));
	t.put("uqsub.h",                    both(b, op( g->uqsub(d->v.h, d->v.h, s->v.h) )));
	t.put("uqsub.s",                    both(b, op( g->uqsub(d->v.s, d->v.s, s->v.s) )));
	t.put("uqsub.d",                    both(b, op( g->uqsub(d->v.d, d->v.d, s->v.d) )));

	t.put("abs.b",                      both(b, op( g->abs(d->v.b, s->v.b) )));
	t.put("abs.h",                      both(b, op( g->abs(d->v.h, s->v.h) )));
	t.put("abs.s",                      both(b, op( g->abs(d->v.s, s->v.s) )));
	t.put("abs.d",                      both(b, op( g->abs(d->v.d, s->v.d) )));

	t.put("sqabs.b",                    both(b, op( g->sqabs(d->v.b, s->v.b) )));
	t.put("sqabs.h",                    both(b, op( g->sqabs(d->v.h, s->v.h) )));
	t.put("sqabs.s",                    both(b, op( g->sqabs(d->v.s, s->v.s) )));
	t.put("sqabs.d",                    both(b, op( g->sqabs(d->v.d, s->v.d) )));

	t.put("neg.b",                      both(b, op( g->neg(d->v.b, s->v.b) )));
	t.put("neg.h",                      both(b, op( g->neg(d->v.h, s->v.h) )));
	t.put("neg.s",                      both(b, op( g->neg(d->v.s, s->v.s) )));
	t.put("neg.d",                      both(b, op( g->neg(d->v.d, s->v.d) )));

	t.put("sqneg.b",                    both(b, op( g->sqneg(d->v.b, s->v.b) )));
	t.put("sqneg.h",                    both(b, op( g->sqneg(d->v.h, s->v.h) )));
	t.put("sqneg.s",                    both(b, op( g->sqneg(d->v.s, s->v.s) )));
	t.put("sqneg.d",                    both(b, op( g->sqneg(d->v.d, s->v.d) )));
	return;
}

static
void bench_extra_arith_vec(bool md, double freq) {
	table t(md, "Vector integer add and sub (widening, narrowing, and horizontal)");
	bench b(freq);

	t.put("saddl.b",                    both(b, op( g->saddl(d->v.h, d->v.b8, s->v.b8) )));
	t.put("saddl.h",                    both(b, op( g->saddl(d->v.s, d->v.h4, s->v.h4) )));
	t.put("saddl.s",                    both(b, op( g->saddl(d->v.d, d->v.s2, s->v.s2) )));
	t.put("saddl2.b",                   both(b, op( g->saddl2(d->v.h, d->v.b, s->v.b) )));
	t.put("saddl2.h",                   both(b, op( g->saddl2(d->v.s, d->v.h, s->v.h) )));
	t.put("saddl2.s",                   both(b, op( g->saddl2(d->v.d, d->v.s, s->v.s) )));

	t.put("ssubl.b",                    both(b, op( g->ssubl(d->v.h, d->v.b8, s->v.b8) )));
	t.put("ssubl.h",                    both(b, op( g->ssubl(d->v.s, d->v.h4, s->v.h4) )));
	t.put("ssubl.s",                    both(b, op( g->ssubl(d->v.d, d->v.s2, s->v.s2) )));
	t.put("ssubl2.b",                   both(b, op( g->ssubl2(d->v.h, d->v.b, s->v.b) )));
	t.put("ssubl2.h",                   both(b, op( g->ssubl2(d->v.s, d->v.h, s->v.h) )));
	t.put("ssubl2.s",                   both(b, op( g->ssubl2(d->v.d, d->v.s, s->v.s) )));

	t.put("usubl.b",                    both(b, op( g->usubl(d->v.h, d->v.b8, s->v.b8) )));
	t.put("usubl.h",                    both(b, op( g->usubl(d->v.s, d->v.h4, s->v.h4) )));
	t.put("usubl.s",                    both(b, op( g->usubl(d->v.d, d->v.s2, s->v.s2) )));
	t.put("usubl2.b",                   both(b, op( g->usubl2(d->v.h, d->v.b, s->v.b) )));
	t.put("usubl2.h",                   both(b, op( g->usubl2(d->v.s, d->v.h, s->v.h) )));
	t.put("usubl2.s",                   both(b, op( g->usubl2(d->v.d, d->v.s, s->v.s) )));

	t.put("saddlp.b",                   both(b, op( g->saddlp(d->v.h, s->v.b) )));
	t.put("saddlp.h",                   both(b, op( g->saddlp(d->v.s, s->v.h) )));
	t.put("saddlp.s",                   both(b, op( g->saddlp(d->v.d, s->v.s) )));
	t.put("saddlv.b",                   both(b, op( g->saddlv(d->h, s->v.b) )));
	t.put("saddlv.h",                   both(b, op( g->saddlv(d->s, s->v.h) )));
	t.put("saddlv.s",                   both(b, op( g->saddlv(d->d, s->v.s) )));

	t.put("saddw.h",                    both(b, op( g->saddw(d->v.h, d->v.h, s->v.b8) )));
	t.put("saddw.s",                    both(b, op( g->saddw(d->v.s, d->v.s, s->v.h4) )));
	t.put("saddw.d",                    both(b, op( g->saddw(d->v.d, d->v.d, s->v.s2) )));
	t.put("saddw2.h",                   both(b, op( g->saddw2(d->v.h, d->v.h, s->v.b) )));
	t.put("saddw2.s",                   both(b, op( g->saddw2(d->v.s, d->v.s, s->v.h) )));
	t.put("saddw2.d",                   both(b, op( g->saddw2(d->v.d, d->v.d, s->v.s) )));

	t.put("uaddw.h",                    both(b, op( g->uaddw(d->v.h, d->v.h, s->v.b8) )));
	t.put("uaddw.s",                    both(b, op( g->uaddw(d->v.s, d->v.s, s->v.h4) )));
	t.put("uaddw.d",                    both(b, op( g->uaddw(d->v.d, d->v.d, s->v.s2) )));
	t.put("uaddw2.h",                   both(b, op( g->uaddw2(d->v.h, d->v.h, s->v.b) )));
	t.put("uaddw2.s",                   both(b, op( g->uaddw2(d->v.s, d->v.s, s->v.h) )));
	t.put("uaddw2.d",                   both(b, op( g->uaddw2(d->v.d, d->v.d, s->v.s) )));

	t.put("ssubw.h",                    both(b, op( g->ssubw(d->v.h, d->v.h, s->v.b8) )));
	t.put("ssubw.s",                    both(b, op( g->ssubw(d->v.s, d->v.s, s->v.h4) )));
	t.put("ssubw.d",                    both(b, op( g->ssubw(d->v.d, d->v.d, s->v.s2) )));
	t.put("ssubw2.h",                   both(b, op( g->ssubw2(d->v.h, d->v.h, s->v.b) )));
	t.put("ssubw2.s",                   both(b, op( g->ssubw2(d->v.s, d->v.s, s->v.h) )));
	t.put("ssubw2.d",                   both(b, op( g->ssubw2(d->v.d, d->v.d, s->v.s) )));

	t.put("usubw.h",                    both(b, op( g->usubw(d->v.h, d->v.h, s->v.b8) )));
	t.put("usubw.s",                    both(b, op( g->usubw(d->v.s, d->v.s, s->v.h4) )));
	t.put("usubw.d",                    both(b, op( g->usubw(d->v.d, d->v.d, s->v.s2) )));
	t.put("usubw2.h",                   both(b, op( g->usubw2(d->v.h, d->v.h, s->v.b) )));
	t.put("usubw2.s",                   both(b, op( g->usubw2(d->v.s, d->v.s, s->v.h) )));
	t.put("usubw2.d",                   both(b, op( g->usubw2(d->v.d, d->v.d, s->v.s) )));

	t.put("addhn.h",                    both(b, op( g->addhn(d->v.b8, d->v.h, s->v.h) )));
	t.put("addhn.s",                    both(b, op( g->addhn(d->v.h4, d->v.s, s->v.s) )));
	t.put("addhn.d",                    both(b, op( g->addhn(d->v.s2, d->v.d, s->v.d) )));
	t.put("addhn2.h",                   both(b, op( g->addhn2(d->v.b16, d->v.h, s->v.h) )));
	t.put("addhn2.s",                   both(b, op( g->addhn2(d->v.h8, d->v.s, s->v.s) )));
	t.put("addhn2.d",                   both(b, op( g->addhn2(d->v.s4, d->v.d, s->v.d) )));

	t.put("subhn.h",                    both(b, op( g->subhn(d->v.b8, d->v.h, s->v.h) )));
	t.put("subhn.s",                    both(b, op( g->subhn(d->v.h4, d->v.s, s->v.s) )));
	t.put("subhn.d",                    both(b, op( g->subhn(d->v.s2, d->v.d, s->v.d) )));
	t.put("subhn2.h",                   both(b, op( g->subhn2(d->v.b16, d->v.h, s->v.h) )));
	t.put("subhn2.s",                   both(b, op( g->subhn2(d->v.h8, d->v.s, s->v.s) )));
	t.put("subhn2.d",                   both(b, op( g->subhn2(d->v.s4, d->v.d, s->v.d) )));

	t.put("raddhn.h",                   both(b, op( g->raddhn(d->v.b8, d->v.h, s->v.h) )));
	t.put("raddhn.s",                   both(b, op( g->raddhn(d->v.h4, d->v.s, s->v.s) )));
	t.put("raddhn.d",                   both(b, op( g->raddhn(d->v.s2, d->v.d, s->v.d) )));
	t.put("raddhn2.h",                  both(b, op( g->raddhn2(d->v.b16, d->v.h, s->v.h) )));
	t.put("raddhn2.s",                  both(b, op( g->raddhn2(d->v.h8, d->v.s, s->v.s) )));
	t.put("raddhn2.d",                  both(b, op( g->raddhn2(d->v.s4, d->v.d, s->v.d) )));

	t.put("rsubhn.h",                   both(b, op( g->rsubhn(d->v.b8, d->v.h, s->v.h) )));
	t.put("rsubhn.s",                   both(b, op( g->rsubhn(d->v.h4, d->v.s, s->v.s) )));
	t.put("rsubhn.d",                   both(b, op( g->rsubhn(d->v.s2, d->v.d, s->v.d) )));
	t.put("rsubhn2.h",                  both(b, op( g->rsubhn2(d->v.b16, d->v.h, s->v.h) )));
	t.put("rsubhn2.s",                  both(b, op( g->rsubhn2(d->v.h8, d->v.s, s->v.s) )));
	t.put("rsubhn2.d",                  both(b, op( g->rsubhn2(d->v.s4, d->v.d, s->v.d) )));

	t.put("shadd.b",                    both(b, op( g->shadd(d->v.b, d->v.b, s->v.b) )));
	t.put("shadd.h",                    both(b, op( g->shadd(d->v.h, d->v.h, s->v.h) )));
	t.put("shadd.s",                    both(b, op( g->shadd(d->v.s, d->v.s, s->v.s) )));

	t.put("shsub.b",                    both(b, op( g->shsub(d->v.b, d->v.b, s->v.b) )));
	t.put("shsub.h",                    both(b, op( g->shsub(d->v.h, d->v.h, s->v.h) )));
	t.put("shsub.s",                    both(b, op( g->shsub(d->v.s, d->v.s, s->v.s) )));

	t.put("uhadd.b",                    both(b, op( g->uhadd(d->v.b, d->v.b, s->v.b) )));
	t.put("uhadd.h",                    both(b, op( g->uhadd(d->v.h, d->v.h, s->v.h) )));
	t.put("uhadd.s",                    both(b, op( g->uhadd(d->v.s, d->v.s, s->v.s) )));

	t.put("uhsub.b",                    both(b, op( g->uhsub(d->v.b, d->v.b, s->v.b) )));
	t.put("uhsub.h",                    both(b, op( g->uhsub(d->v.h, d->v.h, s->v.h) )));
	t.put("uhsub.s",                    both(b, op( g->uhsub(d->v.s, d->v.s, s->v.s) )));

	t.put("srhadd.b",                   both(b, op( g->srhadd(d->v.b, d->v.b, s->v.b) )));
	t.put("srhadd.h",                   both(b, op( g->srhadd(d->v.h, d->v.h, s->v.h) )));
	t.put("srhadd.s",                   both(b, op( g->srhadd(d->v.s, d->v.s, s->v.s) )));

	t.put("urhadd.b",                   both(b, op( g->urhadd(d->v.b, d->v.b, s->v.b) )));
	t.put("urhadd.h",                   both(b, op( g->urhadd(d->v.h, d->v.h, s->v.h) )));
	t.put("urhadd.s",                   both(b, op( g->urhadd(d->v.s, d->v.s, s->v.s) )));

	t.put("addp.b",                     both(b, op( g->addp(d->v.b, d->v.b, s->v.b) )));
	t.put("addp.h",                     both(b, op( g->addp(d->v.h, d->v.h, s->v.h) )));
	t.put("addp.s",                     both(b, op( g->addp(d->v.s, d->v.s, s->v.s) )));
	t.put("addp.d",                     both(b, op( g->addp(d->v.d, d->v.d, s->v.d) )));

	t.put("addv.b",                     both(b, op( g->addv(d->b, s->v.b) )));
	t.put("addv.h",                     both(b, op( g->addv(d->h, s->v.h) )));
	t.put("addv.s",                     both(b, op( g->addv(d->s, s->v.s) )));
	// t.put("addv.d",                     both(b, op( g->addv(d->d, s->v.d) )));
	/* FIXME: addp not available */
	return;
}

static
void bench_max_min_vec(bool md, double freq) {
	table t(md, "Vector integer max and min");
	bench b(freq);

	t.put("smax.b",                     both(b, op( g->smax(d->v.b, d->v.b, s->v.b) )));
	t.put("smax.h",                     both(b, op( g->smax(d->v.h, d->v.h, s->v.h) )));
	t.put("smax.s",                     both(b, op( g->smax(d->v.s, d->v.s, s->v.s) )));
	t.put("smin.b",                     both(b, op( g->smin(d->v.b, d->v.b, s->v.b) )));
	t.put("smin.h",                     both(b, op( g->smin(d->v.h, d->v.h, s->v.h) )));
	t.put("smin.s",                     both(b, op( g->smin(d->v.s, d->v.s, s->v.s) )));

	t.put("smaxp.b",                    both(b, op( g->smaxp(d->v.b, d->v.b, s->v.b) )));
	t.put("smaxp.h",                    both(b, op( g->smaxp(d->v.h, d->v.h, s->v.h) )));
	t.put("smaxp.s",                    both(b, op( g->smaxp(d->v.s, d->v.s, s->v.s) )));
	t.put("sminp.b",                    both(b, op( g->sminp(d->v.b, d->v.b, s->v.b) )));
	t.put("sminp.h",                    both(b, op( g->sminp(d->v.h, d->v.h, s->v.h) )));
	t.put("sminp.s",                    both(b, op( g->sminp(d->v.s, d->v.s, s->v.s) )));

	t.put("smaxv.b",                    both(b, op( g->smaxv(d->b, s->v.b) )));
	t.put("smaxv.h",                    both(b, op( g->smaxv(d->h, s->v.h) )));
	t.put("smaxv.s",                    both(b, op( g->smaxv(d->s, s->v.s) )));
	t.put("sminv.b",                    both(b, op( g->sminv(d->b, s->v.b) )));
	t.put("sminv.h",                    both(b, op( g->sminv(d->h, s->v.h) )));
	t.put("sminv.s",                    both(b, op( g->sminv(d->s, s->v.s) )));

	t.put("umax.b",                     both(b, op( g->umax(d->v.b, d->v.b, s->v.b) )));
	t.put("umax.h",                     both(b, op( g->umax(d->v.h, d->v.h, s->v.h) )));
	t.put("umax.s",                     both(b, op( g->umax(d->v.s, d->v.s, s->v.s) )));
	t.put("umin.b",                     both(b, op( g->umin(d->v.b, d->v.b, s->v.b) )));
	t.put("umin.h",                     both(b, op( g->umin(d->v.h, d->v.h, s->v.h) )));
	t.put("umin.s",                     both(b, op( g->umin(d->v.s, d->v.s, s->v.s) )));

	t.put("umaxp.b",                    both(b, op( g->umaxp(d->v.b, d->v.b, s->v.b) )));
	t.put("umaxp.h",                    both(b, op( g->umaxp(d->v.h, d->v.h, s->v.h) )));
	t.put("umaxp.s",                    both(b, op( g->umaxp(d->v.s, d->v.s, s->v.s) )));
	t.put("uminp.b",                    both(b, op( g->uminp(d->v.b, d->v.b, s->v.b) )));
	t.put("uminp.h",                    both(b, op( g->uminp(d->v.h, d->v.h, s->v.h) )));
	t.put("uminp.s",                    both(b, op( g->uminp(d->v.s, d->v.s, s->v.s) )));

	t.put("umaxv.b",                    both(b, op( g->umaxv(d->b, s->v.b) )));
	t.put("umaxv.h",                    both(b, op( g->umaxv(d->h, s->v.h) )));
	t.put("umaxv.s",                    both(b, op( g->umaxv(d->s, s->v.s) )));
	t.put("uminv.b",                    both(b, op( g->uminv(d->b, s->v.b) )));
	t.put("uminv.h",                    both(b, op( g->uminv(d->h, s->v.h) )));
	t.put("uminv.s",                    both(b, op( g->uminv(d->s, s->v.s) )));
	return;
}

static
void bench_abd_vec(bool md, double freq) {
	table t(md, "Vector integer absolute difference");
	bench b(freq);

	t.put("sabd.b",                     both(b, op( g->sabd(d->v.b, d->v.b, s->v.b) )));
	t.put("sabd.h",                     both(b, op( g->sabd(d->v.h, d->v.h, s->v.h) )));
	t.put("sabd.s",                     both(b, op( g->sabd(d->v.s, d->v.s, s->v.s) )));

	t.put("uabd.b",                     both(b, op( g->uabd(d->v.b, d->v.b, s->v.b) )));
	t.put("uabd.h",                     both(b, op( g->uabd(d->v.h, d->v.h, s->v.h) )));
	t.put("uabd.s",                     both(b, op( g->uabd(d->v.s, d->v.s, s->v.s) )));

	t.put("sabdl.b",                    both(b, op( g->sabdl(d->v.h, d->v.b8, s->v.b8) )));
	t.put("sabdl.h",                    both(b, op( g->sabdl(d->v.s, d->v.h4, s->v.h4) )));
	t.put("sabdl.s",                    both(b, op( g->sabdl(d->v.d, d->v.s2, s->v.s2) )));
	t.put("sabdl2.b",                   both(b, op( g->sabdl2(d->v.h, d->v.b, s->v.b) )));
	t.put("sabdl2.h",                   both(b, op( g->sabdl2(d->v.s, d->v.h, s->v.h) )));
	t.put("sabdl2.s",                   both(b, op( g->sabdl2(d->v.d, d->v.s, s->v.s) )));

	t.put("uabdl.b",                    both(b, op( g->uabdl(d->v.h, d->v.b8, s->v.b8) )));
	t.put("uabdl.h",                    both(b, op( g->uabdl(d->v.s, d->v.h4, s->v.h4) )));
	t.put("uabdl.s",                    both(b, op( g->uabdl(d->v.d, d->v.s2, s->v.s2) )));
	t.put("uabdl2.b",                   both(b, op( g->uabdl2(d->v.h, d->v.b, s->v.b) )));
	t.put("uabdl2.h",                   both(b, op( g->uabdl2(d->v.s, d->v.h, s->v.h) )));
	t.put("uabdl2.s",                   both(b, op( g->uabdl2(d->v.d, d->v.s, s->v.s) )));
	return;
}

void bench_arith_vec(bool md, double freq) {
	bench_basic_arith_vec(md, freq);
	bench_extra_arith_vec(md, freq);
	bench_max_min_vec(md, freq);
	bench_abd_vec(md, freq);
	return;
}

/*
 * end of arith_vec.cc
 */
