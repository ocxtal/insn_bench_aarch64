
/**
 * @file logic.cc
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>

static
void bench_bitwise_logic(bool md, double freq, double adc_latency) {
	table t(md, "Scalar bitwise logic");
	bench b(freq);

	/* basic bitwise logic */
	t.put("and (reg)",                  both(b, op( g->and_(d->x, d->x, s->x) )));
	t.put("and (reg<<2)",               both(b, op( g->and_(d->x, d->x, s->x, ShMod::LSL, 2) )));
	t.put("and (reg<<17)",              both(b, op( g->and_(d->x, d->x, s->x, ShMod::LSL, 17) )));
	t.put("and (reg>>17)",              both(b, op( g->and_(d->x, d->x, s->x, ShMod::LSR, 17) )));
	t.put("and (reg>>17; signed)",      both(b, op( g->and_(d->x, d->x, s->x, ShMod::ASR, 17) )));
	t.put("and (reg>>17; rotate)",      both(b, op( g->and_(d->x, d->x, s->x, ShMod::ROR, 17) )));
	t.put("and (mask imm)",             both(b, op( g->and_(d->x, s->x, 0x1ffc) )));

	t.put("ands (reg)",                 both(b, op( g->ands(d->x, d->x, s->x) ),                    op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("ands (reg<<2)",              both(b, op( g->ands(d->x, d->x, s->x, ShMod::LSL, 2) ),     op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("ands (reg<<17)",             both(b, op( g->ands(d->x, d->x, s->x, ShMod::LSL, 17) ),    op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("ands (reg>>17)",             both(b, op( g->ands(d->x, d->x, s->x, ShMod::LSR, 17) ),    op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("ands (reg>>17; signed)",     both(b, op( g->ands(d->x, d->x, s->x, ShMod::ASR, 17) ),    op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("ands (reg>>17; rotate)",     both(b, op( g->ands(d->x, d->x, s->x, ShMod::ROR, 17) ),    op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("ands (mask imm)",            both(b, op( g->ands(d->x, s->x, 0x1ffc) ),                  op( g->adc(d->x, d->x, s->x) ), adc_latency));

	t.put("orr (reg)",                  both(b, op( g->orr(d->x, g->x28, s->x) )));		/* to prevent from being mov */
	t.put("orr (reg<<2)",               both(b, op( g->orr(d->x, d->x, s->x, ShMod::LSL, 2) )));
	t.put("orr (mask imm)",             both(b, op( g->orr(d->x, s->x, 0x1ffc) )));
	t.put("orn (reg)",                  both(b, op( g->orn(d->x, d->x, s->x) )));
	t.put("orn (reg<<2)",               both(b, op( g->orn(d->x, d->x, s->x, ShMod::LSL, 2) )));

	t.put("eor (reg)",                  both(b, op( g->eor(d->x, d->x, s->x) )));
	t.put("eor (reg<<2)",               both(b, op( g->eor(d->x, d->x, s->x, ShMod::LSL, 2) )));
	t.put("eon (reg)",                  both(b, op( g->eon(d->x, d->x, s->x) )));
	t.put("eon (reg<<2)",               both(b, op( g->eon(d->x, d->x, s->x, ShMod::LSL, 2) )));

	t.put("mvn (reg)",                  both(b, op( g->mvn(d->x, s->x) )));
	t.put("mvn (reg<<2)",               both(b, op( g->mvn(d->x, s->x, ShMod::LSL, 2) )));
	return;
}

static
void bench_shift_bitmanip(bool md, double freq, double adc_latency) {
	table t(md, "Scalar shift and bitmanip");
	bench b(freq);

	t.put("lsl (reg) / lslv",           both(b, op( g->lsl(d->x, d->x, s->x) )));
	t.put("lsl (imm)",                  both(b, op( g->lsl(d->x, s->x, 17) )));
	t.put("lsr (reg) / lsrv",           both(b, op( g->lsl(d->x, d->x, s->x) )));
	t.put("lsr (imm)",                  both(b, op( g->lsl(d->x, s->x, 17) )));
	t.put("asr (reg) / asrv",           both(b, op( g->asr(d->x, d->x, s->x) )));
	t.put("asr (imm)",                  both(b, op( g->asr(d->x, s->x, 17) )));
	t.put("ror (imm)",                  both(b, op( g->ror(d->x, s->x, 17) )));
	t.put("ror (reg) / rorv",           both(b, op( g->rorv(d->x, d->x, s->x) )));
	t.put("extr (imm; >>1)",            both(b, op( g->extr(d->x, d->x, s->x, 1) )));
	t.put("extr (imm; >>17)",           both(b, op( g->extr(d->x, d->x, s->x, 17) )));

	t.put("bfc",                        both(b, op( g->bfc(s->x, 17, 2) ),                          op( g->bfc(d->x, 17, 2) )));
	t.put("bfi",                        both(b, op( g->bfi(d->x, s->x, 17, 2) )));
	t.put("bfm",                        both(b, op( g->bfm(d->x, s->x, 17, 2) )));
	t.put("bfxil",                      both(b, op( g->bfxil(d->x, s->x, 17, 2) )));

	t.put("sbfm",                       both(b, op( g->sbfm(d->x, s->x, 17, 2) )));
	t.put("sbfx",                       both(b, op( g->sbfx(d->x, s->x, 2, 17) )));
	t.put("sbfiz",                      both(b, op( g->sbfiz(d->x, s->x, 2, 17) )));

	t.put("ubfm",                       both(b, op( g->ubfm(d->x, s->x, 17, 2) )));
	t.put("ubfx",                       both(b, op( g->ubfx(d->x, s->x, 2, 17) )));
	t.put("ubfiz",                      both(b, op( g->ubfiz(d->x, s->x, 2, 17) )));

	t.put("bic (reg)",                  both(b, op( g->bic(d->x, d->x, s->x) )));
	t.put("bic (reg<<2)",               both(b, op( g->bic(d->x, d->x, s->x, ShMod::LSL, 2) )));
	t.put("bics (reg)",                 both(b, op( g->bics(d->x, d->x, s->x) ),                    op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("bics (reg<<2)",              both(b, op( g->bics(d->x, d->x, s->x, ShMod::LSL, 2) ),     op( g->adc(d->x, d->x, s->x) ), adc_latency));

	t.put("rbit",                       both(b, op( g->rbit(d->x, s->x) )));
	t.put("rev (rev16)",                both(b, op( g->rev16(d->x, s->x) )));
	t.put("rev (rev32)",                both(b, op( g->rev32(d->x, s->x) )));
	t.put("rev (rev64)",                both(b, op( g->rev64(d->x, s->x) )));

	t.put("clz",                        both(b, op( g->clz(d->x, s->x) )));
	t.put("cls",                        both(b, op( g->cls(d->x, s->x) )));
	return;
}

void bench_logic(bool md, double freq) {
	double const adc_latency = lat_i(freq, op( g->adc(d->x, d->x, s->x) ));

	bench_bitwise_logic(md, freq, adc_latency);
	bench_shift_bitmanip(md, freq, adc_latency);
	return;
}

/*
 * end of logic.cc
 */
