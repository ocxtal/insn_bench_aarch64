
/**
 * @file arith.cc
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>

static
void bench_basic_arith(bool md, double freq, double adc_latency) {
	table t(md, "Addition, subtraction, and negation");
	bench b(freq, (size_t)0, 0);

	t.put("add (reg)",                  both(b, op( g->add(d->x, d->x, s->x) )));
	t.put("add (reg<<2)",               both(b, op( g->add(d->x, d->x, s->x, ShMod::LSL, 2) )));
	t.put("add (reg<<17)",              both(b, op( g->add(d->x, d->x, s->x, ShMod::LSL, 17) )));
	t.put("add (reg>>17)",              both(b, op( g->add(d->x, d->x, s->x, ShMod::LSR, 17) )));
	t.put("add (reg>>17; signed)",      both(b, op( g->add(d->x, d->x, s->x, ShMod::ASR, 17) )));
	t.put("add (imm)",                  both(b, op( g->add(d->x, s->x, 1) )));
	t.put("add (imm<<12)",              both(b, op( g->add(d->x, s->x, 1, 12) )));

	t.put("adds (reg)",                 both(b, op( g->adds(d->x, d->x, s->x) ),                    op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("adds (reg<<2)",              both(b, op( g->adds(d->x, d->x, s->x, ShMod::LSL, 2) ),     op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("adds (reg<<17)",             both(b, op( g->adds(d->x, d->x, s->x, ShMod::LSL, 17) ),    op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("adds (reg>>17)",             both(b, op( g->adds(d->x, d->x, s->x, ShMod::LSR, 17) ),    op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("adds (imm)",                 both(b, op( g->adds(d->x, s->x, 1) ),                       op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("adds (imm<<12)",             both(b, op( g->adds(d->x, s->x, 1, 12) ),                   op( g->adc(d->x, d->x, s->x) ), adc_latency));

	t.put("adc",                        both(b, op( g->adc(d->x, d->x, s->x) )));
	t.put("adcs",                       both(b, op( g->adcs(d->x, d->x, s->x) )));
	
	t.put("sub (reg)",                  both(b, op( g->sub(d->x, d->x, s->x) )));
	t.put("sub (reg<<2)",               both(b, op( g->sub(d->x, d->x, s->x, ShMod::LSL, 2) )));
	t.put("sub (imm)",                  both(b, op( g->sub(d->x, s->x, 1) )));
	t.put("sub (imm<<12)",              both(b, op( g->sub(d->x, s->x, 1, 12) )));

	t.put("subs (reg)",                 both(b, op( g->subs(d->x, d->x, s->x) ),                    op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("subs (reg<<2)",              both(b, op( g->subs(d->x, d->x, s->x, ShMod::LSL, 2) ),     op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("subs (imm)",                 both(b, op( g->subs(d->x, s->x, 1) ),                       op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("subs (imm<<12)",             both(b, op( g->subs(d->x, s->x, 1, 12) ),                   op( g->adc(d->x, d->x, s->x) ), adc_latency));

	t.put("sbc",                        both(b, op( g->sbc(d->x, d->x, s->x) )));
	t.put("sbcs",                       both(b, op( g->sbcs(d->x, d->x, s->x) )));

	t.put("sub",                        thr(b, op( g->sub(d->x, d->x, s->x) )));
	/* not implemented in xbyak_aarch64 */
	// t.put("subp",                       thr(b, op( g->subp(d->x, d->x, s->x) )));

	t.put("adr",                        thr(b, op( g->adr(d->x, g->L()) )));
	t.put("adrp",                       thr(b, op( g->adrp(d->x, g->L()) )));

	t.put("neg (reg)",                  both(b, op( g->neg(d->x, s->x) )));
	t.put("neg (reg<<2)",               both(b, op( g->neg(d->x, s->x, ShMod::LSL, 2) )));
	t.put("negs (reg)",                 both(b, op( g->negs(d->x, s->x) ),                          op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("negs (reg<<2)",              both(b, op( g->negs(d->x, s->x, ShMod::LSL, 2) ),           op( g->adc(d->x, d->x, s->x) ), adc_latency));

	t.put("ngc",                        both(b, op( g->ngc(d->x, s->x) )));
	t.put("ngcs",                       both(b, op( g->ngcs(d->x, s->x) ),                          op( g->adc(d->x, d->x, s->x) ), adc_latency));
	return;
}

static
void bench_mul(bool md, double freq) {
	table t(md, "Multiplication and multiply-accumulate");
	bench b(freq, (size_t)0, 0);

	t.put("mul",                        both(b, op( g->mul(d->x, d->x, s->x) )));
	t.put("mneg",                       both(b, op( g->mneg(d->x, d->x, s->x) )));
	t.put("madd",                       both(b, op( g->madd(d->x, s->x, s->x, d->x) )));
	t.put("msub",                       both(b, op( g->msub(d->x, s->x, s->x, d->x) )));

	t.put("smull",                      both(b, op( g->smull(d->x, d->w, s->w) )));
	t.put("smnegl",                     both(b, op( g->smnegl(d->x, d->w, s->w) )));
	t.put("smaddl",                     both(b, op( g->smaddl(d->x, s->w, s->w, d->x) )));
	t.put("smsubl",                     both(b, op( g->smsubl(d->x, s->w, s->w, d->x) )));
	t.put("smulh",                      both(b, op( g->smulh(d->x, d->x, s->x) )));

	t.put("umull",                      both(b, op( g->umull(d->x, d->w, s->w) )));
	t.put("umnegl",                     both(b, op( g->umnegl(d->x, d->w, s->w) )));
	t.put("umaddl",                     both(b, op( g->umaddl(d->x, s->w, s->w, d->x) )));
	t.put("umsubl",                     both(b, op( g->umsubl(d->x, s->w, s->w, d->x) )));
	t.put("umulh",                      both(b, op( g->umulh(d->x, d->x, s->x) )));
	return;
}

static
void bench_div(bool md, double freq) {
	table t(md, "Division");
	bench b(freq, (size_t)0, 0);

	/* FIXME: vary divisor value */
	t.put("sdiv",                       both(b, op( g->sdiv(d->x, d->x, s->x) )));
	t.put("udiv",                       both(b, op( g->udiv(d->x, d->x, s->x) )));
	return;
}

static
void bench_signext(bool md, double freq) {
	table t(md, "Sign extension");
	bench b(freq, (size_t)0, 0);

	t.put("sxtb",                       both(b, op( g->sxtb(d->x, s->w) )));
	t.put("sxth",                       both(b, op( g->sxth(d->x, s->w) )));
	t.put("sxtw",                       both(b, op( g->sxtw(d->x, s->w) )));
	t.put("uxtb",                       both(b, op( g->uxtb(d->x, s->x) )));
	t.put("uxth",                       both(b, op( g->uxth(d->x, s->x) )));
	return;
}

void bench_arith(bool md, double freq) {
	bench_basic_arith(md, freq);
	bench_mul(md, freq);
	bench_div(md, freq);
	bench_signext(md, freq);
	return;
}

static
void bench_basic_arith_vec(bool md, double freq) {
	table t(md, "Vector basic arithmetic");
	bench b(freq, (size_t)0, 0);

	t.put("add.b",                      both(b, op( g->add(d->v.b, d->v.b, s->v.b) )));
	t.put("add.h",                      both(b, op( g->add(d->v.h, d->v.h, s->v.h) )));
	t.put("add.s",                      both(b, op( g->add(d->v.s, d->v.s, s->v.s) )));
	t.put("add.d",                      both(b, op( g->add(d->v.d, d->v.d, s->v.d) )));

	t.put("addhn.h",                    both(b, op( g->addhn(d->v.b8, d->v.h, s->v.h) )));
	t.put("addhn.s",                    both(b, op( g->addhn(d->v.h4, d->v.s, s->v.s) )));
	t.put("addhn.d",                    both(b, op( g->addhn(d->v.s2, d->v.d, s->v.d) )));
	t.put("addhn2.h",                   both(b, op( g->addhn2(d->v.b16, d->v.h, s->v.h) )));
	t.put("addhn2.s",                   both(b, op( g->addhn2(d->v.h8, d->v.s, s->v.s) )));
	t.put("addhn2.d",                   both(b, op( g->addhn2(d->v.s4, d->v.d, s->v.d) )));

	t.put("addp.b",                     both(b, op( g->addp(d->v.b, d->v.b, s->v.b) )));
	t.put("addp.h",                     both(b, op( g->addp(d->v.h, d->v.h, s->v.h) )));
	t.put("addp.s",                     both(b, op( g->addp(d->v.s, d->v.s, s->v.s) )));
	t.put("addp.d",                     both(b, op( g->addp(d->v.d, d->v.d, s->v.d) )));

	t.put("addv.b",                     both(b, op( g->addv(d->b, s->v.b) )));
	t.put("addv.h",                     both(b, op( g->addv(d->h, s->v.h) )));
	t.put("addv.s",                     both(b, op( g->addv(d->s, s->v.s) )));

	/* not available; addp */
	// t.put("addv.d",                     both(b, op( g->addv(d->d, s->v.d) )));

	t.put("abs.b",                      both(b, op( g->abs(d->v.b, s->v.b) )));
	t.put("abs.h",                      both(b, op( g->abs(d->v.h, s->v.h) )));
	t.put("abs.s",                      both(b, op( g->abs(d->v.s, s->v.s) )));
	t.put("abs.d",                      both(b, op( g->abs(d->v.d, s->v.d) )));

	t.put("neg.b",                      both(b, op( g->neg(d->v.b, s->v.b) )));
	t.put("neg.h",                      both(b, op( g->neg(d->v.h, s->v.h) )));
	t.put("neg.s",                      both(b, op( g->neg(d->v.s, s->v.s) )));
	t.put("neg.d",                      both(b, op( g->neg(d->v.d, s->v.d) )));
	return;
}

static
void bench_mul_vec(bool md, double freq) {
	table t(md, "Vector multiplication and multiply-accumulate");
	bench b(freq, (size_t)0, 0);

	t.put("mul.b (reg)",                both(b, op( g->mul(d->v.b, d->v.b, s->v.b) )));
	t.put("mul.h (reg)",                both(b, op( g->mul(d->v.h, d->v.h, s->v.h) )));
	t.put("mul.s (reg)",                both(b, op( g->mul(d->v.s, d->v.s, s->v.s) )));

	t.put("mla.b (reg)",                both(b, op( g->mla(d->v.b, d->v.b, s->v.b) )));
	t.put("mla.h (reg)",                both(b, op( g->mla(d->v.h, d->v.h, s->v.h) )));
	t.put("mla.s (reg)",                both(b, op( g->mla(d->v.s, d->v.s, s->v.s) )));

	t.put("mls.b (reg)",                both(b, op( g->mls(d->v.b, d->v.b, s->v.b) )));
	t.put("mls.h (reg)",                both(b, op( g->mls(d->v.h, d->v.h, s->v.h) )));
	t.put("mls.s (reg)",                both(b, op( g->mls(d->v.s, d->v.s, s->v.s) )));

	t.put("pmul.b",                     both(b, op( g->pmul(d->v.b, d->v.b, s->v.b) )));
	t.put("pmull.b",                    both(b, op( g->pmull(d->v.h, d->v.b8, s->v.b8) )));
	t.put("pmull.d",                    both(b, op( g->pmull(d->v.q, d->v.d1, s->v.d1) )));
	t.put("pmull2.b",                   both(b, op( g->pmull2(d->v.h, d->v.b, s->v.b) )));
	t.put("pmull2.d",                   both(b, op( g->pmull2(d->v.q, d->v.d, s->v.d) )));
	return;
}

static
void bench_ext_arith_vec(bool md, double freq) {
	table t(md, "Vector extended arithmetic");
	bench b(freq, (size_t)0, 0);

	t.put("raddhn.h",                   both(b, op( g->raddhn(d->v.b8, d->v.h, s->v.h) )));
	t.put("raddhn.s",                   both(b, op( g->raddhn(d->v.h4, d->v.s, s->v.s) )));
	t.put("raddhn.d",                   both(b, op( g->raddhn(d->v.s2, d->v.d, s->v.d) )));
	t.put("raddhn2.h",                  both(b, op( g->raddhn2(d->v.b, d->v.h, s->v.h) )));
	t.put("raddhn2.s",                  both(b, op( g->raddhn2(d->v.h, d->v.s, s->v.s) )));
	t.put("raddhn2.d",                  both(b, op( g->raddhn2(d->v.s, d->v.d, s->v.d) )));

	t.put("rsubhn.h",                   both(b, op( g->rsubhn(d->v.b8, d->v.h, s->v.h) )));
	t.put("rsubhn.s",                   both(b, op( g->rsubhn(d->v.h4, d->v.s, s->v.s) )));
	t.put("rsubhn.d",                   both(b, op( g->rsubhn(d->v.s2, d->v.d, s->v.d) )));
	t.put("rsubhn2.h",                  both(b, op( g->rsubhn2(d->v.b, d->v.h, s->v.h) )));
	t.put("rsubhn2.s",                  both(b, op( g->rsubhn2(d->v.h, d->v.s, s->v.s) )));
	t.put("rsubhn2.d",                  both(b, op( g->rsubhn2(d->v.s, d->v.d, s->v.d) )));

	t.put("saba.b",                     both(b, op( g->saba(d->v.b, d->v.b, s->v.b) )));
	t.put("saba.h",                     both(b, op( g->saba(d->v.h, d->v.h, s->v.h) )));
	t.put("saba.s",                     both(b, op( g->saba(d->v.s, d->v.s, s->v.s) )));

	t.put("sabal.b",                    both(b, op( g->sabal(d->v.h, d->v.b8, s->v.b8) )));
	t.put("sabal.h",                    both(b, op( g->sabal(d->v.s, d->v.h4, s->v.h4) )));
	t.put("sabal.s",                    both(b, op( g->sabal(d->v.d, d->v.s2, s->v.s2) )));

	t.put("sabal2.b",                   both(b, op( g->sabal2(d->v.h, d->v.b, s->v.b) )));
	t.put("sabal2.h",                   both(b, op( g->sabal2(d->v.s, d->v.h, s->v.h) )));
	t.put("sabal2.s",                   both(b, op( g->sabal2(d->v.d, d->v.s, s->v.s) )));

	t.put("sabd.b",                     both(b, op( g->sabd(d->v.b, d->v.b, s->v.b) )));
	t.put("sabd.h",                     both(b, op( g->sabd(d->v.h, d->v.h, s->v.h) )));
	t.put("sabd.s",                     both(b, op( g->sabd(d->v.s, d->v.s, s->v.s) )));

	t.put("sabdl.b",                    both(b, op( g->sabdl(d->v.h, d->v.b8, s->v.b8) )));
	t.put("sabdl.h",                    both(b, op( g->sabdl(d->v.s, d->v.h4, s->v.h4) )));
	t.put("sabdl.s",                    both(b, op( g->sabdl(d->v.d, d->v.s2, s->v.s2) )));

	t.put("sabdl2.b",                   both(b, op( g->sabdl2(d->v.h, d->v.b, s->v.b) )));
	t.put("sabdl2.h",                   both(b, op( g->sabdl2(d->v.s, d->v.h, s->v.h) )));
	t.put("sabdl2.s",                   both(b, op( g->sabdl2(d->v.d, d->v.s, s->v.s) )));

	t.put("sadalp.b",                   both(b, op( g->sadalp(d->v.h, s->v.b) )));
	t.put("sadalp.h",                   both(b, op( g->sadalp(d->v.s, s->v.h) )));
	t.put("sadalp.s",                   both(b, op( g->sadalp(d->v.d, s->v.s) )));

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

	t.put("saddw.b",                    both(b, op( g->saddw(d->v.h, d->v.h, s->v.b8) )));
	t.put("saddw.h",                    both(b, op( g->saddw(d->v.s, d->v.s, s->v.h4) )));
	t.put("saddw.s",                    both(b, op( g->saddw(d->v.d, d->v.d, s->v.s2) )));

	t.put("saddw2.b",                   both(b, op( g->saddw2(d->v.h, d->v.h, s->v.b) )));
	t.put("saddw2.h",                   both(b, op( g->saddw2(d->v.s, d->v.s, s->v.h) )));
	t.put("saddw2.s",                   both(b, op( g->saddw2(d->v.d, d->v.d, s->v.s) )));
	return;
}

void bench_arith_vec(bool md, double freq) {
	bench_basic_arith_vec(md, freq);
	bench_mul_vec(md, freq);
	bench_ext_arith_vec(md, freq);
	return;
}

static
void bench_basic_arith_fp_vec(bool md, double freq) {
	table t(md, "Floating point basic arithmetic");
	bench b(freq, (size_t)0, 0);

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

	t.put("fcadd.h (deg = 90)",         both(b, op( g->fcadd(d->v.h, d->v.h, s->v.h, 90) )));
	t.put("fcadd.s (deg = 90)",         both(b, op( g->fcadd(d->v.s, d->v.s, s->v.s, 90) )));
	t.put("fcadd.d (deg = 90)",         both(b, op( g->fcadd(d->v.d, d->v.d, s->v.d, 90) )));

	t.put("fabd.h (scl)",               both(b, op( g->fabd(d->h, d->h, s->h) )));
	t.put("fabd.s (scl)",               both(b, op( g->fabd(d->s, d->s, s->s) )));
	t.put("fabd.d (scl)",               both(b, op( g->fabd(d->d, d->d, s->d) )));
	t.put("fabd.h (vec)",               both(b, op( g->fabd(d->v.h, d->v.h, s->v.h) )));
	t.put("fabd.s (vec)",               both(b, op( g->fabd(d->v.s, d->v.s, s->v.s) )));
	t.put("fabd.d (vec)",               both(b, op( g->fabd(d->v.d, d->v.d, s->v.d) )));

	t.put("fabs.h (scl)",               both(b, op( g->fabs(d->h, s->h) )));
	t.put("fabs.s (scl)",               both(b, op( g->fabs(d->s, s->s) )));
	t.put("fabs.d (scl)",               both(b, op( g->fabs(d->d, s->d) )));
	t.put("fabs.h (vec)",               both(b, op( g->fabs(d->v.h, s->v.h) )));
	t.put("fabs.s (vec)",               both(b, op( g->fabs(d->v.s, s->v.s) )));
	t.put("fabs.d (vec)",               both(b, op( g->fabs(d->v.d, s->v.d) )));

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
	bench b(freq, (size_t)0, 0);

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
	bench b(freq, (size_t)0, 0);

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

	t.put("fcmla.h (elem; deg = 0)",    both(b, op( g->fcmla(d->v.h, d->v.h, s->v.h[0], 0) )));
	t.put("fcmla.s (elem; deg = 0)",    both(b, op( g->fcmla(d->v.s, d->v.s, s->v.s[0], 0) )));
	// t.put("fcmla.d (elem; deg = 0)",    both(b, op( g->fcmla(d->v.d, d->v.d, s->v.d[0], 0) )));
	t.put("fcmla.h (elem; deg = 90)",   both(b, op( g->fcmla(d->v.h, d->v.h, s->v.h[0], 90) )));
	t.put("fcmla.s (elem; deg = 90)",   both(b, op( g->fcmla(d->v.s, d->v.s, s->v.s[0], 90) )));
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
	bench b(freq, (size_t)0, 0);

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

	// printf("# measuring arithmetic and logic instruction latencies...\n");
	// table t(md, "alu");
	// bench b(freq, (size_t)0, 0);

	// double const adc_latency = b.lat_(0, op( g->adc(d->x, s->x, s->x) )).lat;
	// double const adds_adc_latency = b.lat_(0, op( g->adds(s->x, s->x, s->x); g->adc(d->x, s->x, s->x) )).lat;
	// double const fill_rate = 2.0;

	// t.put("cmhi.b (zero)",              both(b, op( g->cmhi(d->v.b, s->v.b, 0) )));
	// t.put("cmhi.d (zero)",              both(b, op( g->cmhi(d->v.d, s->v.d, 0) )));
}

/*
 * end of arith.cc
 */
