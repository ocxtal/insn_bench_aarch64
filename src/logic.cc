
/**
 * @file logic.cc
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>

static
void bench_bitwise_logic(bool md, double freq, double adc_latency) {
	table t(md, "Logical instructions");
	bench b(freq, (size_t)0, 0);

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

	t.put("orr (reg)",                  both(b, op( g->orr(d->x, d->x, s->x) )));
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
void bench_bitmanip(bool md, double freq, double adc_latency) {
	table t(md, "Shift and bit-manipulation instructions");
	bench b(freq, (size_t)0, 0);

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
	bench b(freq, (size_t)0, 0);
	double const adc_latency = b.lat_(0, op( g->adc(d->x, s->x, s->x) )).lat;

	bench_bitwise_logic(md, freq, adc_latency);
	bench_bitmanip(md, freq, adc_latency);
	return;
}

static
void bench_bitwise_logic_vec(bool md, double freq) {
	table t(md, "Logical instructions");
	bench b(freq, (size_t)0, 0);

	t.put("and.b",                      both(b, op( g->and_(d->v.b, d->v.b, s->v.b) )));

	t.put("orr.b",                      both(b, op( g->orr(d->v.b, d->v.b, s->v.b) )));
	t.put("orr.h",                      both(b, op( g->orr(d->v.h, 0x11, ShMod::LSL, 0) )));
	t.put("orr.s",                      both(b, op( g->orr(d->v.s, 0x11, ShMod::LSL, 0) )));
	t.put("orr.h",                      both(b, op( g->orr(d->v.h, 0x11, ShMod::LSL, 8) )));
	t.put("orr.s",                      both(b, op( g->orr(d->v.s, 0x11, ShMod::LSL, 8) )));
	t.put("orn.b",                      both(b, op( g->orn(d->v.b, d->v.b, s->v.b) )));

	t.put("eor.b",                      both(b, op( g->eor(d->v.b, d->v.b, s->v.b) )));
	t.put("eor3.b",                     both(b, op( g->eor3(d->v.b, d->v.b, d->v.b, s->v.b) )));
	t.put("rax1.d",                     both(b, op( g->rax1(d->v.d, d->v.d, s->v.d) )));

	t.put("mvn.b",                      both(b, op( g->mvn(d->v.b, s->v.b) )));
	t.put("mvni.h",                     both(b, op( g->mvni(d->v.h, 0x11, ShMod::LSL, 0) )));
	t.put("mvni.s",                     both(b, op( g->mvni(d->v.s, 0x11, ShMod::LSL, 0) )));
	t.put("mvni.h",                     both(b, op( g->mvni(d->v.h, 0x11, ShMod::LSL, 8) )));
	t.put("mvni.s",                     both(b, op( g->mvni(d->v.s, 0x11, ShMod::LSL, 8) )));

	t.put("not.b",                      both(b, op( g->not_(d->v.b, s->v.b) )));
	t.put("mvn.b",                      both(b, op( g->mvn(d->v.b, s->v.b) )));
	t.put("mvni.h",                     both(b, op( g->mvni(d->v.h, 0x11, ShMod::LSL, 0) )));
	t.put("mvni.s",                     both(b, op( g->mvni(d->v.s, 0x11, ShMod::LSL, 0) )));
	t.put("mvni.h",                     both(b, op( g->mvni(d->v.h, 0x11, ShMod::LSL, 8) )));
	t.put("mvni.s",                     both(b, op( g->mvni(d->v.s, 0x11, ShMod::LSL, 8) )));
	return;
}

static
void bench_bitmanip_vec(bool md, double freq) {
	table t(md, "Vector shift and bit-manipulation instructions");
	bench b(freq, (size_t)0, 0);

	t.put("bcax.b",                     both(b, op( g->bcax(d->v.b, d->v.b, d->v.b, s->v.b) )));
	t.put("bic.h (imm)",                both(b, op( g->bic(d->v.h8, 0x11, ShMod::LSL, 0) )));
	t.put("bic.h (imm)",                both(b, op( g->bic(d->v.h8, 0x11, ShMod::LSL, 8) )));
	t.put("bic.s (imm)",                both(b, op( g->bic(d->v.s4, 0x11, ShMod::LSL, 0) )));
	t.put("bic.s (imm)",                both(b, op( g->bic(d->v.s4, 0x11, ShMod::LSL, 8) )));

	t.put("bic.b (reg)",                both(b, op( g->bic(d->v.b, d->v.b, s->v.b) )));
	t.put("bif.b",                      both(b, op( g->bif(d->v.b, d->v.b, s->v.b) )));
	t.put("bit.b",                      both(b, op( g->bit(d->v.b, d->v.b, s->v.b) )));
	t.put("bsl.b",                      both(b, op( g->bsl(d->v.b, d->v.b, s->v.b) )));

	t.put("rshrn.h (>>2)",              both(b, op( g->rshrn(d->v.b8, s->v.h, 2) )));
	t.put("rshrn.s (>>2)",              both(b, op( g->rshrn(d->v.h4, s->v.s, 2) )));
	t.put("rshrn.d (>>2)",              both(b, op( g->rshrn(d->v.s2, s->v.d, 2) )));
	t.put("rshrn2.h (>>2)",             both(b, op( g->rshrn2(d->v.b, s->v.h, 2) )));
	t.put("rshrn2.s (>>2)",             both(b, op( g->rshrn2(d->v.h, s->v.s, 2) )));
	t.put("rshrn2.d (>>2)",             both(b, op( g->rshrn2(d->v.s, s->v.d, 2) )));

	t.put("rbit.b",                     both(b, op( g->rbit(d->v.b, s->v.b) )));
	t.put("rev16.b",                    both(b, op( g->rev16(d->v.b, s->v.b) )));
	t.put("rev32.b",                    both(b, op( g->rev32(d->v.b, s->v.b) )));
	t.put("rev32.h",                    both(b, op( g->rev32(d->v.h, s->v.h) )));
	t.put("rev64.b",                    both(b, op( g->rev64(d->v.b, s->v.b) )));
	t.put("rev64.h",                    both(b, op( g->rev64(d->v.h, s->v.h) )));
	t.put("rev64.s",                    both(b, op( g->rev64(d->v.s, s->v.s) )));

	t.put("cls.b",                      both(b, op( g->cls(d->v.b, s->v.b) )));
	t.put("cls.h",                      both(b, op( g->cls(d->v.h, s->v.h) )));
	t.put("cls.s",                      both(b, op( g->cls(d->v.s, s->v.s) )));
	// t.put("cls.d",                      both(b, op( g->cls(d->v.d, s->v.d) )));
	t.put("clz.b",                      both(b, op( g->clz(d->v.b, s->v.b) )));
	t.put("clz.h",                      both(b, op( g->clz(d->v.h, s->v.h) )));
	t.put("clz.s",                      both(b, op( g->clz(d->v.s, s->v.s) )));
	// t.put("clz.d",                      both(b, op( g->clz(d->v.d, s->v.d) )));
	t.put("cnt.b",                      both(b, op( g->cnt(d->v.b, s->v.b) )));
	return;
}

void bench_logic_vec(bool md, double freq) {
	bench_bitwise_logic_vec(md, freq);
	bench_bitmanip_vec(md, freq);
	return;
}

/*
 * end of logic.cc
 */
