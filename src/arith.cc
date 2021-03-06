
/**
 * @file arith.cc
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>

static
void bench_basic_arith(bool md, double freq) {
	table t(md, "Scalar integer add, sub, and neg");
	bench b(freq);

	double const adc_latency  = lat_i(freq, op( g->adc(d->x, d->x, s->x) ));
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
	t.put("adcs",                       sweep(      g->adcs(s->x, s->x, g->x28) ));

	t.put("sub (reg)",                  both(b, op( g->sub(d->x, d->x, s->x) )));
	t.put("sub (reg<<2)",               both(b, op( g->sub(d->x, d->x, s->x, ShMod::LSL, 2) )));
	t.put("sub (imm)",                  both(b, op( g->sub(d->x, s->x, 1) )));
	t.put("sub (imm<<12)",              both(b, op( g->sub(d->x, s->x, 1, 12) )));

	t.put("subs (reg)",                 both(b, op( g->subs(d->x, d->x, s->x) ),                    op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("subs (reg<<2)",              both(b, op( g->subs(d->x, d->x, s->x, ShMod::LSL, 2) ),     op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("subs (imm)",                 both(b, op( g->subs(d->x, s->x, 1) ),                       op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("subs (imm<<12)",             both(b, op( g->subs(d->x, s->x, 1, 12) ),                   op( g->adc(d->x, d->x, s->x) ), adc_latency));

	t.put("sbc",                        both(b, op( g->sbc(d->x, d->x, s->x) )));
	t.put("sbcs",                       sweep(      g->sbcs(s->x, s->x, g->x28) ));

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
	t.put("ngcs",                       sweep(      g->ngcs(s->x, s->x) ));

	#undef thr_body
	#undef sweep
	return;
}

static
void bench_mul(bool md, double freq) {
	table t(md, "Scalar integer multiply and multiply-accumulate");
	bench b(freq);

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
	table t(md, "Scalar integer divide");
	bench b(freq);

	/* FIXME: vary divisor value */
	t.put("sdiv",                       both(b, op( g->sdiv(d->x, s->x, g->x28) )));
	t.put("udiv",                       both(b, op( g->udiv(d->x, s->x, g->x28) )));
	return;
}

static
void bench_signext(bool md, double freq) {
	table t(md, "Scalar integer sign extend");
	bench b(freq);

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

/*
 * end of arith.cc
 */
