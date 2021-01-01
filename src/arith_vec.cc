
/**
 * @file arith_vec.cc
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>

static
void bench_basic_arith_vec(bool md, double freq) {
	table t(md, "Vector basic arithmetic");
	bench b(freq, (size_t)0, 0);

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

	t.put("saddl.b",                    both(b, op( g->saddl(d->v.h, d->v.b8, s->v.b8) )));
	t.put("saddl.h",                    both(b, op( g->saddl(d->v.s, d->v.h4, s->v.h4) )));
	t.put("saddl.s",                    both(b, op( g->saddl(d->v.d, d->v.s2, s->v.s2) )));
	t.put("saddl2.b",                   both(b, op( g->saddl2(d->v.h, d->v.b, s->v.b) )));
	t.put("saddl2.h",                   both(b, op( g->saddl2(d->v.s, d->v.h, s->v.h) )));
	t.put("saddl2.s",                   both(b, op( g->saddl2(d->v.d, d->v.s, s->v.s) )));

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

	t.put("addhn.h",                    both(b, op( g->addhn(d->v.b8, d->v.h, s->v.h) )));
	t.put("addhn.s",                    both(b, op( g->addhn(d->v.h4, d->v.s, s->v.s) )));
	t.put("addhn.d",                    both(b, op( g->addhn(d->v.s2, d->v.d, s->v.d) )));
	t.put("addhn2.h",                   both(b, op( g->addhn2(d->v.b16, d->v.h, s->v.h) )));
	t.put("addhn2.s",                   both(b, op( g->addhn2(d->v.h8, d->v.s, s->v.s) )));
	t.put("addhn2.d",                   both(b, op( g->addhn2(d->v.s4, d->v.d, s->v.d) )));

	t.put("raddhn.h",                   both(b, op( g->raddhn(d->v.b8, d->v.h, s->v.h) )));
	t.put("raddhn.s",                   both(b, op( g->raddhn(d->v.h4, d->v.s, s->v.s) )));
	t.put("raddhn.d",                   both(b, op( g->raddhn(d->v.s2, d->v.d, s->v.d) )));
	t.put("raddhn2.h",                  both(b, op( g->raddhn2(d->v.b16, d->v.h, s->v.h) )));
	t.put("raddhn2.s",                  both(b, op( g->raddhn2(d->v.h8, d->v.s, s->v.s) )));
	t.put("raddhn2.d",                  both(b, op( g->raddhn2(d->v.s4, d->v.d, s->v.d) )));

	t.put("shadd.b",                    both(b, op( g->shadd(d->v.b, d->v.b, s->v.b) )));
	t.put("shadd.h",                    both(b, op( g->shadd(d->v.h, d->v.h, s->v.h) )));
	t.put("shadd.s",                    both(b, op( g->shadd(d->v.s, d->v.s, s->v.s) )));

	t.put("uhadd.b",                    both(b, op( g->uhadd(d->v.b, d->v.b, s->v.b) )));
	t.put("uhadd.h",                    both(b, op( g->uhadd(d->v.h, d->v.h, s->v.h) )));
	t.put("uhadd.s",                    both(b, op( g->uhadd(d->v.s, d->v.s, s->v.s) )));

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

	t.put("subhn.h",                    both(b, op( g->subhn(d->v.b8, d->v.h, s->v.h) )));
	t.put("subhn.s",                    both(b, op( g->subhn(d->v.h4, d->v.s, s->v.s) )));
	t.put("subhn.d",                    both(b, op( g->subhn(d->v.s2, d->v.d, s->v.d) )));
	t.put("subhn2.h",                   both(b, op( g->subhn2(d->v.b16, d->v.h, s->v.h) )));
	t.put("subhn2.s",                   both(b, op( g->subhn2(d->v.h8, d->v.s, s->v.s) )));
	t.put("subhn2.d",                   both(b, op( g->subhn2(d->v.s4, d->v.d, s->v.d) )));

	t.put("rsubhn.h",                   both(b, op( g->rsubhn(d->v.b8, d->v.h, s->v.h) )));
	t.put("rsubhn.s",                   both(b, op( g->rsubhn(d->v.h4, d->v.s, s->v.s) )));
	t.put("rsubhn.d",                   both(b, op( g->rsubhn(d->v.s2, d->v.d, s->v.d) )));
	t.put("rsubhn2.h",                  both(b, op( g->rsubhn2(d->v.b16, d->v.h, s->v.h) )));
	t.put("rsubhn2.s",                  both(b, op( g->rsubhn2(d->v.h8, d->v.s, s->v.s) )));
	t.put("rsubhn2.d",                  both(b, op( g->rsubhn2(d->v.s4, d->v.d, s->v.d) )));

	t.put("shsub.b",                    both(b, op( g->shsub(d->v.b, d->v.b, s->v.b) )));
	t.put("shsub.h",                    both(b, op( g->shsub(d->v.h, d->v.h, s->v.h) )));
	t.put("shsub.s",                    both(b, op( g->shsub(d->v.s, d->v.s, s->v.s) )));

	t.put("uhsub.b",                    both(b, op( g->uhsub(d->v.b, d->v.b, s->v.b) )));
	t.put("uhsub.h",                    both(b, op( g->uhsub(d->v.h, d->v.h, s->v.h) )));
	t.put("uhsub.s",                    both(b, op( g->uhsub(d->v.s, d->v.s, s->v.s) )));

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
void bench_max_min_vec(bool md, double freq) {
	table t(md, "Vector max / min");
	bench b(freq, (size_t)0, 0);

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
void bench_mul_vec(bool md, double freq) {
	table t(md, "Vector multiplication and multiply-accumulate");
	bench b(freq, (size_t)0, 0);

	/* mul */
	t.put("mul.b",                      both(b, op( g->mul(d->v.b, d->v.b, s->v.b) )));
	t.put("mul.h",                      both(b, op( g->mul(d->v.h, d->v.h, s->v.h) )));
	t.put("mul.s",                      both(b, op( g->mul(d->v.s, d->v.s, s->v.s) )));

	t.put("smull.b (vec)",              both(b, op( g->smull(d->v.h, d->v.b8, s->v.b8) )));
	t.put("smull.h (vec)",              both(b, op( g->smull(d->v.s, d->v.h4, s->v.h4) )));
	t.put("smull.s (vec)",              both(b, op( g->smull(d->v.d, d->v.s2, s->v.s2) )));
	t.put("smull2.b (vec)",             both(b, op( g->smull2(d->v.h, d->v.b, s->v.b) )));
	t.put("smull2.h (vec)",             both(b, op( g->smull2(d->v.s, d->v.h, s->v.h) )));
	t.put("smull2.s (vec)",             both(b, op( g->smull2(d->v.d, d->v.s, s->v.s) )));

	t.put("smull.h (elem; [0])",        both(b, op( g->smull(d->v.s, d->v.h4, s->v.h[0]) )));
	t.put("smull.h (elem; [7])",        both(b, op( g->smull(d->v.s, d->v.h4, s->v.h[7]) )));
	t.put("smull.s (elem; [0])",        both(b, op( g->smull(d->v.d, d->v.s2, s->v.s[0]) )));
	t.put("smull.s (elem; [3])",        both(b, op( g->smull(d->v.d, d->v.s2, s->v.s[3]) )));
	t.put("smull2.h (elem; [0])",       both(b, op( g->smull2(d->v.s, d->v.h, s->v.h[0]) )));
	t.put("smull2.h (elem; [7])",       both(b, op( g->smull2(d->v.s, d->v.h, s->v.h[7]) )));
	t.put("smull2.s (elem; [0])",       both(b, op( g->smull2(d->v.d, d->v.s, s->v.s[0]) )));
	t.put("smull2.s (elem; [3])",       both(b, op( g->smull2(d->v.d, d->v.s, s->v.s[3]) )));

	t.put("umull.b (vec)",              both(b, op( g->umull(d->v.h, d->v.b8, s->v.b8) )));
	t.put("umull.h (vec)",              both(b, op( g->umull(d->v.s, d->v.h4, s->v.h4) )));
	t.put("umull.s (vec)",              both(b, op( g->umull(d->v.d, d->v.s2, s->v.s2) )));
	t.put("umull2.b (vec)",             both(b, op( g->umull2(d->v.h, d->v.b, s->v.b) )));
	t.put("umull2.h (vec)",             both(b, op( g->umull2(d->v.s, d->v.h, s->v.h) )));
	t.put("umull2.s (vec)",             both(b, op( g->umull2(d->v.d, d->v.s, s->v.s) )));

	t.put("umull.h (elem; [0])",        both(b, op( g->umull(d->v.s, d->v.h4, s->v.h[0]) )));
	t.put("umull.h (elem; [7])",        both(b, op( g->umull(d->v.s, d->v.h4, s->v.h[7]) )));
	t.put("umull.s (elem; [0])",        both(b, op( g->umull(d->v.d, d->v.s2, s->v.s[0]) )));
	t.put("umull.s (elem; [3])",        both(b, op( g->umull(d->v.d, d->v.s2, s->v.s[3]) )));
	t.put("umull2.h (elem; [0])",       both(b, op( g->umull2(d->v.s, d->v.h, s->v.h[0]) )));
	t.put("umull2.h (elem; [7])",       both(b, op( g->umull2(d->v.s, d->v.h, s->v.h[7]) )));
	t.put("umull2.s (elem; [0])",       both(b, op( g->umull2(d->v.d, d->v.s, s->v.s[0]) )));
	t.put("umull2.s (elem; [3])",       both(b, op( g->umull2(d->v.d, d->v.s, s->v.s[3]) )));

	t.put("sqdmull.h (vec)",            both(b, op( g->sqdmull(d->v.s, d->v.h4, s->v.h4) )));
	t.put("sqdmull.s (vec)",            both(b, op( g->sqdmull(d->v.d, d->v.s2, s->v.s2) )));
	t.put("sqdmull2.h (vec)",           both(b, op( g->sqdmull2(d->v.s, d->v.h, s->v.h) )));
	t.put("sqdmull2.s (vec)",           both(b, op( g->sqdmull2(d->v.d, d->v.s, s->v.s) )));

	t.put("sqdmull.h (elem; v.h[0])",   both(b, op( g->sqdmull(d->v.s, d->v.h4, s->v.h[0]) )));
	t.put("sqdmull.h (elem; v.h[7])",   both(b, op( g->sqdmull(d->v.s, d->v.h4, s->v.h[7]) )));
	t.put("sqdmull.s (elem; v.s[0])",   both(b, op( g->sqdmull(d->v.d, d->v.s2, s->v.s[0]) )));
	t.put("sqdmull.s (elem; v.s[3])",   both(b, op( g->sqdmull(d->v.d, d->v.s2, s->v.s[3]) )));
	t.put("sqdmull2.h (elem; v.h[0])",  both(b, op( g->sqdmull2(d->v.s, d->v.h, s->v.h[0]) )));
	t.put("sqdmull2.h (elem; v.h[7])",  both(b, op( g->sqdmull2(d->v.s, d->v.h, s->v.h[7]) )));
	t.put("sqdmull2.s (elem; v.s[0])",  both(b, op( g->sqdmull2(d->v.d, d->v.s, s->v.s[0]) )));
	t.put("sqdmull2.s (elem; v.s[3])",  both(b, op( g->sqdmull2(d->v.d, d->v.s, s->v.s[3]) )));

	t.put("sqdmulh.h (vec)",            both(b, op( g->sqdmulh(d->v.h, d->v.h, s->v.h) )));
	t.put("sqdmulh.s (vec)",            both(b, op( g->sqdmulh(d->v.s, d->v.s, s->v.s) )));
	t.put("sqdmulh.h (elem; v.h[0])",   both(b, op( g->sqdmulh(d->v.h, d->v.h, s->v.h[0]) )));
	t.put("sqdmulh.h (elem; v.h[7])",   both(b, op( g->sqdmulh(d->v.h, d->v.h, s->v.h[7]) )));
	t.put("sqdmulh.s (elem; v.s[0])",   both(b, op( g->sqdmulh(d->v.s, d->v.s, s->v.s[0]) )));
	t.put("sqdmulh.s (elem; v.s[3])",   both(b, op( g->sqdmulh(d->v.s, d->v.s, s->v.s[3]) )));

	t.put("sqrdmulh.h (vec)",           both(b, op( g->sqrdmulh(d->v.h, d->v.h, s->v.h) )));
	t.put("sqrdmulh.s (vec)",           both(b, op( g->sqrdmulh(d->v.s, d->v.s, s->v.s) )));
	t.put("sqrdmulh.h (elem; v.h[0])",  both(b, op( g->sqrdmulh(d->v.h, d->v.h, s->v.h[0]) )));
	t.put("sqrdmulh.h (elem; v.h[7])",  both(b, op( g->sqrdmulh(d->v.h, d->v.h, s->v.h[7]) )));
	t.put("sqrdmulh.s (elem; v.s[0])",  both(b, op( g->sqrdmulh(d->v.s, d->v.s, s->v.s[0]) )));
	t.put("sqrdmulh.s (elem; v.s[3])",  both(b, op( g->sqrdmulh(d->v.s, d->v.s, s->v.s[3]) )));

	/* mla */
	t.put("mla.b",                      both(b, op( g->mla(d->v.b, d->v.b, s->v.b) )));
	t.put("mla.h",                      both(b, op( g->mla(d->v.h, d->v.h, s->v.h) )));
	t.put("mla.s",                      both(b, op( g->mla(d->v.s, d->v.s, s->v.s) )));

	t.put("smlal.h (vec)",              both(b, op( g->smlal(d->v.s, d->v.h4, s->v.h4) )));
	t.put("smlal.s (vec)",              both(b, op( g->smlal(d->v.d, d->v.s2, s->v.s2) )));
	t.put("smlal2.h (vec)",             both(b, op( g->smlal2(d->v.s, d->v.h, s->v.h) )));
	t.put("smlal2.s (vec)",             both(b, op( g->smlal2(d->v.d, d->v.s, s->v.s) )));

	t.put("smlal.h (elem; v.h[0])",     both(b, op( g->smlal(d->v.s, d->v.h4, s->v.h[0]) )));
	t.put("smlal.h (elem; v.h[7])",     both(b, op( g->smlal(d->v.s, d->v.h4, s->v.h[7]) )));
	t.put("smlal.s (elem; v.s[0])",     both(b, op( g->smlal(d->v.d, d->v.s2, s->v.s[0]) )));
	t.put("smlal.s (elem; v.s[3])",     both(b, op( g->smlal(d->v.d, d->v.s2, s->v.s[3]) )));
	t.put("smlal2.h (elem; v.h[0])",    both(b, op( g->smlal2(d->v.s, d->v.h, s->v.h[0]) )));
	t.put("smlal2.h (elem; v.h[7])",    both(b, op( g->smlal2(d->v.s, d->v.h, s->v.h[7]) )));
	t.put("smlal2.s (elem; v.s[0])",    both(b, op( g->smlal2(d->v.d, d->v.s, s->v.s[0]) )));
	t.put("smlal2.s (elem; v.s[3])",    both(b, op( g->smlal2(d->v.d, d->v.s, s->v.s[3]) )));

	t.put("umlal.h (vec)",              both(b, op( g->umlal(d->v.s, d->v.h4, s->v.h4) )));
	t.put("umlal.s (vec)",              both(b, op( g->umlal(d->v.d, d->v.s2, s->v.s2) )));
	t.put("umlal2.h (vec)",             both(b, op( g->umlal2(d->v.s, d->v.h, s->v.h) )));
	t.put("umlal2.s (vec)",             both(b, op( g->umlal2(d->v.d, d->v.s, s->v.s) )));

	t.put("umlal.h (elem; v.h[0])",     both(b, op( g->umlal(d->v.s, d->v.h4, s->v.h[0]) )));
	t.put("umlal.h (elem; v.h[7])",     both(b, op( g->umlal(d->v.s, d->v.h4, s->v.h[7]) )));
	t.put("umlal.s (elem; v.s[0])",     both(b, op( g->umlal(d->v.d, d->v.s2, s->v.s[0]) )));
	t.put("umlal.s (elem; v.s[3])",     both(b, op( g->umlal(d->v.d, d->v.s2, s->v.s[3]) )));
	t.put("umlal2.h (elem; v.h[0])",    both(b, op( g->umlal2(d->v.s, d->v.h, s->v.h[0]) )));
	t.put("umlal2.h (elem; v.h[7])",    both(b, op( g->umlal2(d->v.s, d->v.h, s->v.h[7]) )));
	t.put("umlal2.s (elem; v.s[0])",    both(b, op( g->umlal2(d->v.d, d->v.s, s->v.s[0]) )));
	t.put("umlal2.s (elem; v.s[3])",    both(b, op( g->umlal2(d->v.d, d->v.s, s->v.s[3]) )));

	t.put("sqdmlal.h (vec)",            both(b, op( g->sqdmlal(d->v.s, d->v.h4, s->v.h4) )));
	t.put("sqdmlal.s (vec)",            both(b, op( g->sqdmlal(d->v.d, d->v.s2, s->v.s2) )));
	t.put("sqdmlal2.h (vec)",           both(b, op( g->sqdmlal2(d->v.s, d->v.h, s->v.h) )));
	t.put("sqdmlal2.s (vec)",           both(b, op( g->sqdmlal2(d->v.d, d->v.s, s->v.s) )));

	t.put("sqdmlal.h (elem; v.h[0])",   both(b, op( g->sqdmlal(d->v.s, d->v.h4, s->v.h[0]) )));
	t.put("sqdmlal.h (elem; v.h[7])",   both(b, op( g->sqdmlal(d->v.s, d->v.h4, s->v.h[7]) )));
	t.put("sqdmlal.s (elem; v.s[0])",   both(b, op( g->sqdmlal(d->v.d, d->v.s2, s->v.s[0]) )));
	t.put("sqdmlal.s (elem; v.s[3])",   both(b, op( g->sqdmlal(d->v.d, d->v.s2, s->v.s[3]) )));
	t.put("sqdmlal2.h (elem; v.h[0])",  both(b, op( g->sqdmlal2(d->v.s, d->v.h, s->v.h[0]) )));
	t.put("sqdmlal2.h (elem; v.h[7])",  both(b, op( g->sqdmlal2(d->v.s, d->v.h, s->v.h[7]) )));
	t.put("sqdmlal2.s (elem; v.s[0])",  both(b, op( g->sqdmlal2(d->v.d, d->v.s, s->v.s[0]) )));
	t.put("sqdmlal2.s (elem; v.s[3])",  both(b, op( g->sqdmlal2(d->v.d, d->v.s, s->v.s[3]) )));

	t.put("sqrdmlah.h (vec)",           both(b, op( g->sqrdmlah(d->v.h, d->v.h, s->v.h) )));
	t.put("sqrdmlah.s (vec)",           both(b, op( g->sqrdmlah(d->v.s, d->v.s, s->v.s) )));
	t.put("sqrdmlah.h (elem; v.h[0])",  both(b, op( g->sqrdmlah(d->v.h, d->v.h, s->v.h[0]) )));
	t.put("sqrdmlah.h (elem; v.h[7])",  both(b, op( g->sqrdmlah(d->v.h, d->v.h, s->v.h[7]) )));
	t.put("sqrdmlah.s (elem; v.s[0])",  both(b, op( g->sqrdmlah(d->v.s, d->v.s, s->v.s[0]) )));
	t.put("sqrdmlah.s (elem; v.s[3])",  both(b, op( g->sqrdmlah(d->v.s, d->v.s, s->v.s[3]) )));

	/* mls */
	t.put("mls.b",                      both(b, op( g->mls(d->v.b, d->v.b, s->v.b) )));
	t.put("mls.h",                      both(b, op( g->mls(d->v.h, d->v.h, s->v.h) )));
	t.put("mls.s",                      both(b, op( g->mls(d->v.s, d->v.s, s->v.s) )));

	t.put("smlsl.h (vec)",              both(b, op( g->smlsl(d->v.s, d->v.h4, s->v.h4) )));
	t.put("smlsl.s (vec)",              both(b, op( g->smlsl(d->v.d, d->v.s2, s->v.s2) )));
	t.put("smlsl2.h (vec)",             both(b, op( g->smlsl2(d->v.s, d->v.h, s->v.h) )));
	t.put("smlsl2.s (vec)",             both(b, op( g->smlsl2(d->v.d, d->v.s, s->v.s) )));

	t.put("smlsl.h (elem; v.h[0])",     both(b, op( g->smlsl(d->v.s, d->v.h4, s->v.h[0]) )));
	t.put("smlsl.h (elem; v.h[7])",     both(b, op( g->smlsl(d->v.s, d->v.h4, s->v.h[7]) )));
	t.put("smlsl.s (elem; v.s[0])",     both(b, op( g->smlsl(d->v.d, d->v.s2, s->v.s[0]) )));
	t.put("smlsl.s (elem; v.s[3])",     both(b, op( g->smlsl(d->v.d, d->v.s2, s->v.s[3]) )));
	t.put("smlsl2.h (elem; v.h[0])",    both(b, op( g->smlsl2(d->v.s, d->v.h, s->v.h[0]) )));
	t.put("smlsl2.h (elem; v.h[7])",    both(b, op( g->smlsl2(d->v.s, d->v.h, s->v.h[7]) )));
	t.put("smlsl2.s (elem; v.s[0])",    both(b, op( g->smlsl2(d->v.d, d->v.s, s->v.s[0]) )));
	t.put("smlsl2.s (elem; v.s[3])",    both(b, op( g->smlsl2(d->v.d, d->v.s, s->v.s[3]) )));

	t.put("umlsl.h (vec)",              both(b, op( g->umlsl(d->v.s, d->v.h4, s->v.h4) )));
	t.put("umlsl.s (vec)",              both(b, op( g->umlsl(d->v.d, d->v.s2, s->v.s2) )));
	t.put("umlsl2.h (vec)",             both(b, op( g->umlsl2(d->v.s, d->v.h, s->v.h) )));
	t.put("umlsl2.s (vec)",             both(b, op( g->umlsl2(d->v.d, d->v.s, s->v.s) )));

	t.put("umlsl.h (elem; v.h[0])",     both(b, op( g->umlsl(d->v.s, d->v.h4, s->v.h[0]) )));
	t.put("umlsl.h (elem; v.h[7])",     both(b, op( g->umlsl(d->v.s, d->v.h4, s->v.h[7]) )));
	t.put("umlsl.s (elem; v.s[0])",     both(b, op( g->umlsl(d->v.d, d->v.s2, s->v.s[0]) )));
	t.put("umlsl.s (elem; v.s[3])",     both(b, op( g->umlsl(d->v.d, d->v.s2, s->v.s[3]) )));
	t.put("umlsl2.h (elem; v.h[0])",    both(b, op( g->umlsl2(d->v.s, d->v.h, s->v.h[0]) )));
	t.put("umlsl2.h (elem; v.h[7])",    both(b, op( g->umlsl2(d->v.s, d->v.h, s->v.h[7]) )));
	t.put("umlsl2.s (elem; v.s[0])",    both(b, op( g->umlsl2(d->v.d, d->v.s, s->v.s[0]) )));
	t.put("umlsl2.s (elem; v.s[3])",    both(b, op( g->umlsl2(d->v.d, d->v.s, s->v.s[3]) )));

	t.put("sqdmlsl.h (vec)",            both(b, op( g->sqdmlsl(d->v.s, d->v.h4, s->v.h4) )));
	t.put("sqdmlsl.s (vec)",            both(b, op( g->sqdmlsl(d->v.d, d->v.s2, s->v.s2) )));
	t.put("sqdmlsl2.h (vec)",           both(b, op( g->sqdmlsl2(d->v.s, d->v.h, s->v.h) )));
	t.put("sqdmlsl2.s (vec)",           both(b, op( g->sqdmlsl2(d->v.d, d->v.s, s->v.s) )));

	t.put("sqdmlsl.h (elem; v.h[0])",   both(b, op( g->sqdmlsl(d->v.s, d->v.h4, s->v.h[0]) )));
	t.put("sqdmlsl.h (elem; v.h[7])",   both(b, op( g->sqdmlsl(d->v.s, d->v.h4, s->v.h[7]) )));
	t.put("sqdmlsl.s (elem; v.s[0])",   both(b, op( g->sqdmlsl(d->v.d, d->v.s2, s->v.s[0]) )));
	t.put("sqdmlsl.s (elem; v.s[3])",   both(b, op( g->sqdmlsl(d->v.d, d->v.s2, s->v.s[3]) )));
	t.put("sqdmlsl2.h (elem; v.h[0])",  both(b, op( g->sqdmlsl2(d->v.s, d->v.h, s->v.h[0]) )));
	t.put("sqdmlsl2.h (elem; v.h[7])",  both(b, op( g->sqdmlsl2(d->v.s, d->v.h, s->v.h[7]) )));
	t.put("sqdmlsl2.s (elem; v.s[0])",  both(b, op( g->sqdmlsl2(d->v.d, d->v.s, s->v.s[0]) )));
	t.put("sqdmlsl2.s (elem; v.s[3])",  both(b, op( g->sqdmlsl2(d->v.d, d->v.s, s->v.s[3]) )));

	t.put("sqrdmlsh.h (vec)",           both(b, op( g->sqrdmlsh(d->v.h, d->v.h, s->v.h) )));
	t.put("sqrdmlsh.s (vec)",           both(b, op( g->sqrdmlsh(d->v.s, d->v.s, s->v.s) )));
	t.put("sqrdmlsh.h (elem; v.h[0])",  both(b, op( g->sqrdmlsh(d->v.h, d->v.h, s->v.h[0]) )));
	t.put("sqrdmlsh.h (elem; v.h[7])",  both(b, op( g->sqrdmlsh(d->v.h, d->v.h, s->v.h[7]) )));
	t.put("sqrdmlsh.s (elem; v.s[0])",  both(b, op( g->sqrdmlsh(d->v.s, d->v.s, s->v.s[0]) )));
	t.put("sqrdmlsh.s (elem; v.s[3])",  both(b, op( g->sqrdmlsh(d->v.s, d->v.s, s->v.s[3]) )));

	t.put("pmul.b",                     both(b, op( g->pmul(d->v.b, d->v.b, s->v.b) )));
	t.put("pmull.b",                    both(b, op( g->pmull(d->v.h, d->v.b8, s->v.b8) )));
	t.put("pmull.d",                    both(b, op( g->pmull(d->v.q, d->v.d1, s->v.d1) )));
	t.put("pmull2.b",                   both(b, op( g->pmull2(d->v.h, d->v.b, s->v.b) )));
	t.put("pmull2.d",                   both(b, op( g->pmull2(d->v.q, d->v.d, s->v.d) )));

	t.put("sdot.b (vec)",               both(b, op( g->sdot(d->v.s, d->v.b, s->v.b) )));
	t.put("sdot.b (elem; v.b[0])",      both(b, op( g->sdot(d->v.s, d->v.b, s->v.b4[0]) )));
	t.put("sdot.b (elem; v.b[3])",      both(b, op( g->sdot(d->v.s, d->v.b, s->v.b4[3]) )));

	t.put("usdot.b (vec)",              both(b, op( g->usdot(d->v.s, d->v.b, s->v.b) )));
	t.put("usdot.b (elem; v.b[0])",     both(b, op( g->usdot(d->v.s, d->v.b, s->v.b4[0]) )));
	t.put("usdot.b (elem; v.b[3])",     both(b, op( g->usdot(d->v.s, d->v.b, s->v.b4[3]) )));

	/* TODO */
	// t.put("sudot.b (vec)",              both(b, op( g->sudot(d->v.s, d->v.b, s->v.b) )));
	// t.put("sudot.b (elem; v.b[0])",     both(b, op( g->sudot(d->v.s, d->v.b, s->v.b4[0]) )));
	// t.put("sudot.b (elem; v.b[3])",     both(b, op( g->sudot(d->v.s, d->v.b, s->v.b4[3]) )));
	// t.put("smmla.b",                    both(b, op( g->smmla(d->v.s, d->v.b, s->v.b) )));
	// t.put("ummla.b",                    both(b, op( g->ummla(d->v.s, d->v.b, s->v.b) )));
	// t.put("usmmla.b",                   both(b, op( g->usmmla(d->v.s, d->v.b, s->v.b) )));
	return;
}

static
void bench_div_vec(bool md, double freq) {
	table t(md, "Vector division");
	bench b(freq, (size_t)0, 0);

	t.put("urecpe.s",                   both(b, op( g->urecpe(d->v.s, d->v.s, s->v.s) )));
	t.put("ursqrte.s",                  both(b, op( g->ursqrte(d->v.s, d->v.s, s->v.s) )));
	return;
}

static
void bench_ext_arith_vec(bool md, double freq) {
	table t(md, "Vector extended arithmetic");
	bench b(freq, (size_t)0, 0);

	t.put("saba.b",                     both(b, op( g->saba(d->v.b, d->v.b, s->v.b) )));
	t.put("saba.h",                     both(b, op( g->saba(d->v.h, d->v.h, s->v.h) )));
	t.put("saba.s",                     both(b, op( g->saba(d->v.s, d->v.s, s->v.s) )));

	t.put("uaba.b",                     both(b, op( g->uaba(d->v.b, d->v.b, s->v.b) )));
	t.put("uaba.h",                     both(b, op( g->uaba(d->v.h, d->v.h, s->v.h) )));
	t.put("uaba.s",                     both(b, op( g->uaba(d->v.s, d->v.s, s->v.s) )));

	t.put("sabal.b",                    both(b, op( g->sabal(d->v.h, d->v.b8, s->v.b8) )));
	t.put("sabal.h",                    both(b, op( g->sabal(d->v.s, d->v.h4, s->v.h4) )));
	t.put("sabal.s",                    both(b, op( g->sabal(d->v.d, d->v.s2, s->v.s2) )));
	t.put("sabal2.b",                   both(b, op( g->sabal2(d->v.h, d->v.b, s->v.b) )));
	t.put("sabal2.h",                   both(b, op( g->sabal2(d->v.s, d->v.h, s->v.h) )));
	t.put("sabal2.s",                   both(b, op( g->sabal2(d->v.d, d->v.s, s->v.s) )));

	t.put("uabal.b",                    both(b, op( g->uabal(d->v.h, d->v.b8, s->v.b8) )));
	t.put("uabal.h",                    both(b, op( g->uabal(d->v.s, d->v.h4, s->v.h4) )));
	t.put("uabal.s",                    both(b, op( g->uabal(d->v.d, d->v.s2, s->v.s2) )));
	t.put("uabal2.b",                   both(b, op( g->uabal2(d->v.h, d->v.b, s->v.b) )));
	t.put("uabal2.h",                   both(b, op( g->uabal2(d->v.s, d->v.h, s->v.h) )));
	t.put("uabal2.s",                   both(b, op( g->uabal2(d->v.d, d->v.s, s->v.s) )));

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

	t.put("sadalp.b",                   both(b, op( g->sadalp(d->v.h, s->v.b) )));
	t.put("sadalp.h",                   both(b, op( g->sadalp(d->v.s, s->v.h) )));
	t.put("sadalp.s",                   both(b, op( g->sadalp(d->v.d, s->v.s) )));

	t.put("uadalp.b",                   both(b, op( g->uadalp(d->v.h, s->v.b) )));
	t.put("uadalp.h",                   both(b, op( g->uadalp(d->v.s, s->v.h) )));
	t.put("uadalp.s",                   both(b, op( g->uadalp(d->v.d, s->v.s) )));
	return;
}

void bench_arith_vec(bool md, double freq) {
	bench_basic_arith_vec(md, freq);
	bench_max_min_vec(md, freq);
	bench_mul_vec(md, freq);
	bench_div_vec(md, freq);
	bench_ext_arith_vec(md, freq);
	return;
}

/*
 * end of arith_vec.cc
 */
