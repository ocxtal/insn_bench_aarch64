
/**
 * @file alu.h
 * @brief measure arithmetic and logic instruction latencies
 * @author Hajime Suzuki
 */
#ifndef _ALU_H_INCLUDED
#define _ALU_H_INCLUDED
#include <stdlib.h>
#include "utils.h"

static
void init_mems(size_t elems, void **self, void **loop) {
	for(size_t i = 0; i < elems; i++) {
		self[i] = &self[i];
	}
	for(size_t i = 0; i < elems; i++) {
		loop[i] = &loop[(i == elems - 1) ? 0 : i + 1];
	}
	return;
}

static
void bench_store(double freq) {
	(void)freq;
	return;
}

static
void bench_load(double freq) {
	size_t const elems = 128 / sizeof(void *);
	void **self  = (void **)malloc(sizeof(void *) * elems);
	void **loop = (void **)malloc(sizeof(void *) * elems);

	init_mems(elems, self, loop);
	bench l(freq, self);
	bench c(freq, loop);

	print("ldr", both(l, op( g->ldr(d->x, ptr(s->x)) )));
	print("ldr", both(c, op( g->ldr(d->x, ptr(s->x)) )));

	free(self);
	free(loop);
	return;
}

static
void bench_alu(double freq) {
	bench b(freq, (size_t)0, 0);

	double const adc_latency = b.lat_(0, op( g->adc(d->x, s->x, s->x) )).lat;
	double const adds_adc_latency = b.lat_(0, op( g->adds(s->x, s->x, s->x); g->adc(d->x, s->x, s->x) )).lat;
	// double const fill_rate = 2.0;

	/* move */
	print("nop",                        thr(b, op( g->nop() )));
	print("mov (reg)",                  both(b, op( g->mov(d->x, s->x) )));
	print("mov / movz (imm)",           thr(b, op( g->mov(d->x, 0x1ffc) )));
	print("mov / movz (imm<<16)",       thr(b, op( g->mov(d->x, 0x1ffc<<16) )));
	print("mov (mask imm)",             thr(b, op( g->mov(d->x, 0x1ffffffffffc) )));
	print("mov / movn (imm)",           thr(b, op( g->movn(d->x, 0x1ffc) )));
	print("mov / movn (imm<<16)",       thr(b, op( g->movn(d->x, 0x1ffc, 16) )));
	print("movk",                       thr(b, op( g->movk(d->x, 0x1ffc) )));
	print("movk (<<16)",                thr(b, op( g->movk(d->x, 0x1ffc, 16) )));
	#if 1
	/* basic arith */
	print("add (reg)",                  both(b, op( g->add(d->x, d->x, s->x) )));
	print("add (reg<<2)",               both(b, op( g->add(d->x, d->x, s->x, ShMod::LSL, 2) )));
	print("add (reg<<17)",              both(b, op( g->add(d->x, d->x, s->x, ShMod::LSL, 17) )));
	print("add (reg>>17)",              both(b, op( g->add(d->x, d->x, s->x, ShMod::LSR, 17) )));
	print("add (reg>>17; signed)",      both(b, op( g->add(d->x, d->x, s->x, ShMod::ASR, 17) )));
	print("add (imm)",                  both(b, op( g->add(d->x, s->x, 1) )));
	print("add (imm<<12)",              both(b, op( g->add(d->x, s->x, 1, 12) )));

	print("adds (reg)",                 both(b, op( g->adds(d->x, d->x, s->x) ),                    op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("adds (reg<<2)",              both(b, op( g->adds(d->x, d->x, s->x, ShMod::LSL, 2) ),     op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("adds (reg<<17)",             both(b, op( g->adds(d->x, d->x, s->x, ShMod::LSL, 17) ),    op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("adds (reg>>17)",             both(b, op( g->adds(d->x, d->x, s->x, ShMod::LSR, 17) ),    op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("adds (imm)",                 both(b, op( g->adds(d->x, s->x, 1) ),                       op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("adds (imm<<12)",             both(b, op( g->adds(d->x, s->x, 1, 12) ),                   op( g->adc(d->x, d->x, s->x) ), adc_latency));

	print("adc",                        both(b, op( g->adc(d->x, d->x, s->x) )));
	print("adcs",                       both(b, op( g->adcs(d->x, d->x, s->x) )));
	
	print("sub (reg)",                  both(b, op( g->sub(d->x, d->x, s->x) )));
	print("sub (reg<<2)",               both(b, op( g->sub(d->x, d->x, s->x, ShMod::LSL, 2) )));
	print("sub (imm)",                  both(b, op( g->sub(d->x, s->x, 1) )));
	print("sub (imm<<12)",              both(b, op( g->sub(d->x, s->x, 1, 12) )));

	print("subs (reg)",                 both(b, op( g->subs(d->x, d->x, s->x) ),                    op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("subs (reg<<2)",              both(b, op( g->subs(d->x, d->x, s->x, ShMod::LSL, 2) ),     op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("subs (imm)",                 both(b, op( g->subs(d->x, s->x, 1) ),                       op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("subs (imm<<12)",             both(b, op( g->subs(d->x, s->x, 1, 12) ),                   op( g->adc(d->x, d->x, s->x) ), adc_latency));

	print("sbc",                        both(b, op( g->sbc(d->x, d->x, s->x) )));
	print("sbcs",                       both(b, op( g->sbcs(d->x, d->x, s->x) )));

	/* not implemented in xbyak_aarch64 */
	print("sub",                        thr(b, op( g->sub(d->x, d->x, s->x) )));
	// print("subp",                       thr(b, op( g->subp(d->x, d->x, s->x) )));

	print("adr",                        thr(b, op( g->adr(d->x, g->L()) )));
	print("adrp",                       thr(b, op( g->adrp(d->x, g->L()) )));

	print("neg (reg)",                  both(b, op( g->neg(d->x, s->x) )));
	print("neg (reg<<2)",               both(b, op( g->neg(d->x, s->x, ShMod::LSL, 2) )));
	print("negs (reg)",                 both(b, op( g->negs(d->x, s->x) ),                          op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("negs (reg<<2)",              both(b, op( g->negs(d->x, s->x, ShMod::LSL, 2) ),           op( g->adc(d->x, d->x, s->x) ), adc_latency));

	print("ngc",                        both(b, op( g->ngc(d->x, s->x) )));
	print("ngcs",                       both(b, op( g->ngcs(d->x, s->x) ),                          op( g->adc(d->x, d->x, s->x) ), adc_latency));

	/* multiply */
	print("mul",                        both(b, op( g->mul(d->x, d->x, s->x) )));
	print("mneg",                       both(b, op( g->mneg(d->x, d->x, s->x) )));
	print("madd",                       both(b, op( g->madd(d->x, s->x, s->x, d->x) )));
	print("msub",                       both(b, op( g->msub(d->x, s->x, s->x, d->x) )));

	print("smull",                      both(b, op( g->smull(d->x, d->w, s->w) )));
	print("smnegl",                     both(b, op( g->smnegl(d->x, d->w, s->w) )));
	print("smaddl",                     both(b, op( g->smaddl(d->x, s->w, s->w, d->x) )));
	print("smsubl",                     both(b, op( g->smsubl(d->x, s->w, s->w, d->x) )));
	print("smulh",                      both(b, op( g->smulh(d->x, d->x, s->x) )));

	print("umull",                      both(b, op( g->umull(d->x, d->w, s->w) )));
	print("umnegl",                     both(b, op( g->umnegl(d->x, d->w, s->w) )));
	print("umaddl",                     both(b, op( g->umaddl(d->x, s->w, s->w, d->x) )));
	print("umsubl",                     both(b, op( g->umsubl(d->x, s->w, s->w, d->x) )));
	print("umulh",                      both(b, op( g->umulh(d->x, d->x, s->x) )));

	/* divide */
	print("sdiv",                       both(b, op( g->sdiv(d->x, d->x, s->x) )));
	print("udiv",                       both(b, op( g->udiv(d->x, d->x, s->x) )));

	/* basic bitwise logic */
	print("and (reg)",                  both(b, op( g->and_(d->x, d->x, s->x) )));
	print("and (reg<<2)",               both(b, op( g->and_(d->x, d->x, s->x, ShMod::LSL, 2) )));
	print("and (reg<<17)",              both(b, op( g->and_(d->x, d->x, s->x, ShMod::LSL, 17) )));
	print("and (reg>>17)",              both(b, op( g->and_(d->x, d->x, s->x, ShMod::LSR, 17) )));
	print("and (reg>>17; signed)",      both(b, op( g->and_(d->x, d->x, s->x, ShMod::ASR, 17) )));
	print("and (reg>>17; rotate)",      both(b, op( g->and_(d->x, d->x, s->x, ShMod::ROR, 17) )));
	print("and (mask imm)",             both(b, op( g->and_(d->x, s->x, 0x1ffc) )));

	print("ands (reg)",                 both(b, op( g->ands(d->x, d->x, s->x) ),                    op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("ands (reg<<2)",              both(b, op( g->ands(d->x, d->x, s->x, ShMod::LSL, 2) ),     op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("ands (reg<<17)",             both(b, op( g->ands(d->x, d->x, s->x, ShMod::LSL, 17) ),    op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("ands (reg>>17)",             both(b, op( g->ands(d->x, d->x, s->x, ShMod::LSR, 17) ),    op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("ands (reg>>17; signed)",     both(b, op( g->ands(d->x, d->x, s->x, ShMod::ASR, 17) ),    op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("ands (reg>>17; rotate)",     both(b, op( g->ands(d->x, d->x, s->x, ShMod::ROR, 17) ),    op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("ands (mask imm)",            both(b, op( g->ands(d->x, s->x, 0x1ffc) ),                  op( g->adc(d->x, d->x, s->x) ), adc_latency));


	print("eor (reg)",                  both(b, op( g->eor(d->x, d->x, s->x) )));
	print("eor (reg<<2)",               both(b, op( g->eor(d->x, d->x, s->x, ShMod::LSL, 2) )));
	print("eon (reg)",                  both(b, op( g->eon(d->x, d->x, s->x) )));
	print("eon (reg<<2)",               both(b, op( g->eon(d->x, d->x, s->x, ShMod::LSL, 2) )));

	print("orr (reg)",                  both(b, op( g->orr(d->x, d->x, s->x) )));
	print("orr (reg<<2)",               both(b, op( g->orr(d->x, d->x, s->x, ShMod::LSL, 2) )));
	print("orr (mask imm)",             both(b, op( g->orr(d->x, s->x, 0x1ffc) )));
	print("orn (reg)",                  both(b, op( g->orn(d->x, d->x, s->x) )));
	print("orn (reg<<2)",               both(b, op( g->orn(d->x, d->x, s->x, ShMod::LSL, 2) )));

	print("mvn (reg)",                  both(b, op( g->mvn(d->x, s->x) )));
	print("mvn (reg<<2)",               both(b, op( g->mvn(d->x, s->x, ShMod::LSL, 2) )));

	/* compare */
	print("ccmn (reg; eq)",             both(b, op( g->ccmn(s->x, s->x, 0x2, Cond::EQ) ),           op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("ccmn (reg; lt)",             both(b, op( g->ccmn(s->x, s->x, 0x2, Cond::LT) ),           op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("ccmn (imm; eq)",             both(b, op( g->ccmn(s->x, 17, 0x2, Cond::EQ) ),             op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("ccmn (imm; lt)",             both(b, op( g->ccmn(s->x, 17, 0x2, Cond::LT) ),             op( g->adc(d->x, d->x, s->x) ), adc_latency));

	print("ccmp (reg; eq)",             both(b, op( g->ccmp(s->x, s->x, 0x2, Cond::EQ) ),           op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("ccmp (reg; lt)",             both(b, op( g->ccmp(s->x, s->x, 0x2, Cond::LT) ),           op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("ccmp (imm; eq)",             both(b, op( g->ccmp(s->x, 17, 0x2, Cond::EQ) ),             op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("ccmp (imm; lt)",             both(b, op( g->ccmp(s->x, 17, 0x2, Cond::LT) ),             op( g->adc(d->x, d->x, s->x) ), adc_latency));

	/* flag manipulation */
	print("cfinv",                      both(b, op( g->cfinv() ),                                   op( g->adc(d->x, d->x, s->x); g->adds(d->x, d->x, s->x) ), adds_adc_latency));
	print("rmif",                       both(b, op( g->rmif(s->x, 17, 0xf) ),                       op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("setf8",                      both(b, op( g->setf8(s->w) ),                               op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("setf16",                     both(b, op( g->setf16(s->w) ),                              op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("tst (reg)",                  both(b, op( g->tst(s->x, s->x) ),                           op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("tst (reg<<2)",               both(b, op( g->tst(s->x, s->x, ShMod::LSL, 2) ),            op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("tst (imm)",                  both(b, op( g->tst(s->x, 0x1ffc) ),                         op( g->adc(d->x, d->x, s->x) ), adc_latency));

	/* conditional arithmetic */
	print("csinc (eq)",                 both(b, op( g->csinc(d->x, d->x, s->x, Cond::EQ) )));
	print("csinc (lt)",                 both(b, op( g->csinc(d->x, d->x, s->x, Cond::LT) )));
	print("cinc (eq)",                  both(b, op( g->cinc(d->x, s->x, Cond::EQ) )));
	print("cinc (lt)",                  both(b, op( g->cinc(d->x, s->x, Cond::LT) )));
	print("csinv (eq)",                 both(b, op( g->csinv(d->x, d->x, s->x, Cond::EQ) )));
	print("csinv (lt)",                 both(b, op( g->csinv(d->x, d->x, s->x, Cond::LT) )));
	print("cset (eq)",                  thr(b, op( g->cset(s->x, Cond::EQ) ) ));
	print("cset (lt)",                  thr(b, op( g->cset(s->x, Cond::LT) ) ));
	print("csetm (eq)",                 thr(b, op( g->csetm(s->x, Cond::EQ) ) ));
	print("csetm (lt)",                 thr(b, op( g->csetm(s->x, Cond::LT) ) ));
	print("cinv (eq)",                  both(b, op( g->cinv(d->x, s->x, Cond::EQ) )));
	print("cinv (lt)",                  both(b, op( g->cinv(d->x, s->x, Cond::LT) )));
	print("csneg (eq)",                 both(b, op( g->csneg(d->x, d->x, s->x, Cond::EQ) )));
	print("csneg (lt)",                 both(b, op( g->csneg(d->x, d->x, s->x, Cond::LT) )));
	print("cneg (eq)",                  both(b, op( g->cneg(d->x, s->x, Cond::EQ) )));
	print("cneg (lt)",                  both(b, op( g->cneg(d->x, s->x, Cond::LT) )));

	/* bitfield manipulation */
	print("bfc",                        both(b, op( g->bfc(s->x, 17, 2) ),                          op( g->bfc(d->x, 17, 2) )));
	print("bfi",                        both(b, op( g->bfi(d->x, s->x, 17, 2) )));
	print("bfm",                        both(b, op( g->bfm(d->x, s->x, 17, 2) )));
	print("bfxil",                      both(b, op( g->bfxil(d->x, s->x, 17, 2) )));

	print("sbfm",                       both(b, op( g->sbfm(d->x, s->x, 17, 2) )));
	print("sbfx",                       both(b, op( g->sbfx(d->x, s->x, 2, 17) )));
	print("sbfiz",                      both(b, op( g->sbfiz(d->x, s->x, 2, 17) )));

	print("ubfm",                       both(b, op( g->ubfm(d->x, s->x, 17, 2) )));
	print("ubfx",                       both(b, op( g->ubfx(d->x, s->x, 2, 17) )));
	print("ubfiz",                      both(b, op( g->ubfiz(d->x, s->x, 2, 17) )));

	print("bic (reg)",                  both(b, op( g->bic(d->x, d->x, s->x) )));
	print("bic (reg<<2)",               both(b, op( g->bic(d->x, d->x, s->x, ShMod::LSL, 2) )));
	print("bics (reg)",                 both(b, op( g->bics(d->x, d->x, s->x) ),                    op( g->adc(d->x, d->x, s->x) ), adc_latency));
	print("bics (reg<<2)",              both(b, op( g->bics(d->x, d->x, s->x, ShMod::LSL, 2) ),     op( g->adc(d->x, d->x, s->x) ), adc_latency));

	/* shift */
	print("lsl (reg) / lslv",           both(b, op( g->lsl(d->x, d->x, s->x) )));
	print("lsl (imm)",                  both(b, op( g->lsl(d->x, s->x, 17) )));
	print("lsr (reg) / lsrv",           both(b, op( g->lsl(d->x, d->x, s->x) )));
	print("lsr (imm)",                  both(b, op( g->lsl(d->x, s->x, 17) )));
	print("asr (reg) / asrv",           both(b, op( g->asr(d->x, d->x, s->x) )));
	print("asr (imm)",                  both(b, op( g->asr(d->x, s->x, 17) )));
	print("ror (imm)",                  both(b, op( g->ror(d->x, s->x, 17) )));
	print("ror (reg) / rorv",           both(b, op( g->rorv(d->x, d->x, s->x) )));
	print("extr (imm; >>1)",            both(b, op( g->extr(d->x, d->x, s->x, 1) )));
	print("extr (imm; >>17)",           both(b, op( g->extr(d->x, d->x, s->x, 17) )));

	/* sign extension / extract */
	print("sxtb",                       both(b, op( g->sxtb(d->x, s->w) )));
	print("sxth",                       both(b, op( g->sxth(d->x, s->w) )));
	print("sxtw",                       both(b, op( g->sxtw(d->x, s->w) )));
	print("uxtb",                       both(b, op( g->uxtb(d->x, s->x) )));
	print("uxth",                       both(b, op( g->uxth(d->x, s->x) )));

	/* reverse bits / bytes */
	print("rbit",                       both(b, op( g->rbit(d->x, s->x) )));
	print("rev (rev16)",                both(b, op( g->rev16(d->x, s->x) )));
	print("rev (rev32)",                both(b, op( g->rev32(d->x, s->x) )));
	print("rev (rev64)",                both(b, op( g->rev64(d->x, s->x) )));

	/* count bits */
	print("clz",                        both(b, op( g->clz(d->x, s->x) )));
	print("cls",                        both(b, op( g->cls(d->x, s->x) )));

	/* special */
	print("crc32x",                     both(b, op( g->crc32x(d->w, d->w, s->x) )));
	print("crc32cx",                    both(b, op( g->crc32cx(d->w, d->w, s->x) )));


	double const mov_latency = b.lat_(0, op( g->mov(d->v.d[0], s->x); g->mov(d->x, d->v.d[0]) )).lat / 2.0;
	print("mov (reg)",                  both(b, op( g->mov(d->v.b, s->v.b) )));
	// print("mov.b (v.b[0] <-> w)",       lat(b, op( g->mov(d->v.b[0], s->w); g->mov(d->x, d->v.b[0]) )));
	// print("mov.h (v.h[0] <-> x)",       lat(b, op( g->mov(d->v.h[0], s->x); g->mov(d->x, d->v.h[0]) )));
	print("mov.s (v.s[0] <-> w)",       lat(b, op( g->mov(d->v.s[0], s->w); g->mov(d->w, d->v.s[0]) )));
	print("mov.d (v.d[0] <-> x)",       lat(b, op( g->mov(d->v.d[0], s->x); g->mov(d->x, d->v.d[0]) )));
	// print("mov.b (v.b[15] <-> w)",       lat(b, op( g->mov(d->v.b[15], s->w); g->mov(d->x, d->v.b[15]) )));
	// print("mov.h (v.h[7] <-> x)",       lat(b, op( g->mov(d->v.h[7], s->x); g->mov(d->x, d->v.h[7]) )));
	print("mov.s (v.s[3] <-> w)",       lat(b, op( g->mov(d->v.s[3], s->w); g->mov(d->w, d->v.s[3]) )));
	print("mov.d (v.d[1] <-> x)",       lat(b, op( g->mov(d->v.d[1], s->x); g->mov(d->x, d->v.d[1]) )));
	print("smov.b (v.b[0] <-> x)",      lat(b, op( g->mov(d->v.b[0], s->w); g->smov(d->w, d->v.b[0]) )));
	print("smov.h (v.h[0] <-> x)",      lat(b, op( g->mov(d->v.h[0], s->w); g->smov(d->w, d->v.h[0]) )));
	print("smov.s (v.s[0] <-> x)",      lat(b, op( g->mov(d->v.s[0], s->w); g->smov(d->x, d->v.s[0]) )));
	print("umov.b (v.b[0] <-> w)",      lat(b, op( g->mov(d->v.b[0], s->w); g->umov(d->w, d->v.b[0]) )));
	print("umov.h (v.h[0] <-> w)",      lat(b, op( g->mov(d->v.h[0], s->w); g->umov(d->w, d->v.h[0]) )));
	print("umov.s (v.s[0] <-> w)",      lat(b, op( g->mov(d->v.s[0], s->w); g->umov(d->w, d->v.s[0]) )));
	print("umov.d (v.d[0] <-> x)",      lat(b, op( g->mov(d->v.d[0], s->x); g->umov(d->x, d->v.d[0]) )));

	// print("fmov.h (v.h[0] <-> x)",      lat(b, op( g->fmov(d->v.h[0], s->x); g->fmov(d->x, d->v.h[0]) )));
	// print("fmov.s (v.s[0] <-> x)",      lat(b, op( g->fmov(d->v.s[0], s->w); g->fmov(d->x, d->v.s[0]) )));
	print("fmov.d (v.d[0] <-> x)",      lat(b, op( g->fmov(d->v.d[0], s->x); g->fmov(d->x, d->v.d[0]) )));

	print("abs.b",                      both(b, op( g->abs(d->v.b, s->v.b) )));
	print("abs.h",                      both(b, op( g->abs(d->v.h, s->v.h) )));
	print("abs.s",                      both(b, op( g->abs(d->v.s, s->v.s) )));
	print("abs.d",                      both(b, op( g->abs(d->v.d, s->v.d) )));

	print("add.b",                      both(b, op( g->add(d->v.b, d->v.b, s->v.b) )));
	print("add.h",                      both(b, op( g->add(d->v.h, d->v.h, s->v.h) )));
	print("add.s",                      both(b, op( g->add(d->v.s, d->v.s, s->v.s) )));
	print("add.d",                      both(b, op( g->add(d->v.d, d->v.d, s->v.d) )));

	print("addhn.h",                    both(b, op( g->addhn(d->v.b8, d->v.h, s->v.h) )));
	print("addhn.s",                    both(b, op( g->addhn(d->v.h4, d->v.s, s->v.s) )));
	print("addhn.d",                    both(b, op( g->addhn(d->v.s2, d->v.d, s->v.d) )));
	print("addhn2.h",                   both(b, op( g->addhn2(d->v.b16, d->v.h, s->v.h) )));
	print("addhn2.s",                   both(b, op( g->addhn2(d->v.h8, d->v.s, s->v.s) )));
	print("addhn2.d",                   both(b, op( g->addhn2(d->v.s4, d->v.d, s->v.d) )));

	print("addp.b",                     both(b, op( g->addp(d->v.b, d->v.b, s->v.b) )));
	print("addp.h",                     both(b, op( g->addp(d->v.h, d->v.h, s->v.h) )));
	print("addp.s",                     both(b, op( g->addp(d->v.s, d->v.s, s->v.s) )));
	print("addp.d",                     both(b, op( g->addp(d->v.d, d->v.d, s->v.d) )));

	print("addv.b",                     both(b, op( g->addv(d->b, s->v.b) )));
	print("addv.h",                     both(b, op( g->addv(d->h, s->v.h) )));
	print("addv.s",                     both(b, op( g->addv(d->s, s->v.s) )));
	// print("addv.d",                     both(b, op( g->addv(d->d, s->v.d) )));

	print("aese",                       both(b, op( g->aese(d->v.b, s->v.b) )));
	print("aesd",                       both(b, op( g->aesd(d->v.b, s->v.b) )));
	print("aesmc",                      both(b, op( g->aesmc(d->v.b, s->v.b) )));
	print("aesimc",                     both(b, op( g->aesimc(d->v.b, s->v.b) )));

	print("and.b",                      both(b, op( g->and_(d->v.b, d->v.b, s->v.b) )));
	print("bcax.b",                     both(b, op( g->bcax(d->v.b, d->v.b, d->v.b, s->v.b) )));

	// print("bfcvt",                      both(b, op( g->bfcvt(d->h, d->s) )));

	print("bic.h (imm)",                both(b, op( g->bic(d->v.h8, 0x11, ShMod::LSL, 0) )));
	print("bic.h (imm)",                both(b, op( g->bic(d->v.h8, 0x11, ShMod::LSL, 8) )));
	print("bic.s (imm)",                both(b, op( g->bic(d->v.s4, 0x11, ShMod::LSL, 0) )));
	print("bic.s (imm)",                both(b, op( g->bic(d->v.s4, 0x11, ShMod::LSL, 8) )));

	print("bic.b (reg)",                both(b, op( g->bic(d->v.b, d->v.b, s->v.b) )));
	print("bif.b",                      both(b, op( g->bif(d->v.b, d->v.b, s->v.b) )));
	print("bit.b",                      both(b, op( g->bit(d->v.b, d->v.b, s->v.b) )));
	print("bsl.b",                      both(b, op( g->bsl(d->v.b, d->v.b, s->v.b) )));

	print("cls.b",                      both(b, op( g->cls(d->v.b, s->v.b) )));
	print("cls.h",                      both(b, op( g->cls(d->v.h, s->v.h) )));
	print("cls.s",                      both(b, op( g->cls(d->v.s, s->v.s) )));
	// print("cls.d",                      both(b, op( g->cls(d->v.d, s->v.d) )));
	print("clz.b",                      both(b, op( g->clz(d->v.b, s->v.b) )));
	print("clz.h",                      both(b, op( g->clz(d->v.h, s->v.h) )));
	print("clz.s",                      both(b, op( g->clz(d->v.s, s->v.s) )));
	// print("clz.d",                      both(b, op( g->clz(d->v.d, s->v.d) )));

	print("cmeq.b (reg)",               both(b, op( g->cmeq(d->v.b, d->v.b, s->v.b) )));
	print("cmeq.h (reg)",               both(b, op( g->cmeq(d->v.h, d->v.h, s->v.h) )));
	print("cmeq.s (reg)",               both(b, op( g->cmeq(d->v.s, d->v.s, s->v.s) )));
	print("cmeq.d (reg)",               both(b, op( g->cmeq(d->v.d, d->v.d, s->v.d) )));

	print("cmeq.b (zero)",              both(b, op( g->cmeq(d->v.b, s->v.b, 0) )));
	print("cmeq.h (zero)",              both(b, op( g->cmeq(d->v.h, s->v.h, 0) )));
	print("cmeq.s (zero)",              both(b, op( g->cmeq(d->v.s, s->v.s, 0) )));
	print("cmeq.d (zero)",              both(b, op( g->cmeq(d->v.d, s->v.d, 0) )));

	print("cmge.b (reg)",               both(b, op( g->cmge(d->v.b, d->v.b, s->v.b) )));
	print("cmge.h (reg)",               both(b, op( g->cmge(d->v.h, d->v.h, s->v.h) )));
	print("cmge.s (reg)",               both(b, op( g->cmge(d->v.s, d->v.s, s->v.s) )));
	print("cmge.d (reg)",               both(b, op( g->cmge(d->v.d, d->v.d, s->v.d) )));
	print("cmge.b (zero)",              both(b, op( g->cmge(d->v.b, s->v.b, 0) )));
	print("cmge.h (zero)",              both(b, op( g->cmge(d->v.h, s->v.h, 0) )));
	print("cmge.s (zero)",              both(b, op( g->cmge(d->v.s, s->v.s, 0) )));
	print("cmge.d (zero)",              both(b, op( g->cmge(d->v.d, s->v.d, 0) )));

	print("cmgt.b (reg)",               both(b, op( g->cmgt(d->v.b, d->v.b, s->v.b) )));
	print("cmgt.h (reg)",               both(b, op( g->cmgt(d->v.h, d->v.h, s->v.h) )));
	print("cmgt.s (reg)",               both(b, op( g->cmgt(d->v.s, d->v.s, s->v.s) )));
	print("cmgt.d (reg)",               both(b, op( g->cmgt(d->v.d, d->v.d, s->v.d) )));
	print("cmgt.b (zero)",              both(b, op( g->cmgt(d->v.b, s->v.b, 0) )));
	print("cmgt.h (zero)",              both(b, op( g->cmgt(d->v.h, s->v.h, 0) )));
	print("cmgt.s (zero)",              both(b, op( g->cmgt(d->v.s, s->v.s, 0) )));
	print("cmgt.d (zero)",              both(b, op( g->cmgt(d->v.d, s->v.d, 0) )));

	print("cmle.b (zero)",              both(b, op( g->cmle(d->v.b, s->v.b, 0) )));
	print("cmle.h (zero)",              both(b, op( g->cmle(d->v.h, s->v.h, 0) )));
	print("cmle.s (zero)",              both(b, op( g->cmle(d->v.s, s->v.s, 0) )));
	print("cmle.d (zero)",              both(b, op( g->cmle(d->v.d, s->v.d, 0) )));

	print("cmlt.b (zero)",              both(b, op( g->cmlt(d->v.b, s->v.b, 0) )));
	print("cmlt.h (zero)",              both(b, op( g->cmlt(d->v.h, s->v.h, 0) )));
	print("cmlt.s (zero)",              both(b, op( g->cmlt(d->v.s, s->v.s, 0) )));
	print("cmlt.d (zero)",              both(b, op( g->cmlt(d->v.d, s->v.d, 0) )));

	print("cmhi.b (reg)",               both(b, op( g->cmhi(d->v.b, d->v.b, s->v.b) )));
	print("cmhi.h (reg)",               both(b, op( g->cmhi(d->v.h, d->v.h, s->v.h) )));
	print("cmhi.s (reg)",               both(b, op( g->cmhi(d->v.s, d->v.s, s->v.s) )));
	print("cmhi.d (reg)",               both(b, op( g->cmhi(d->v.d, d->v.d, s->v.d) )));

	print("cmhs.b (reg)",               both(b, op( g->cmhs(d->v.b, d->v.b, s->v.b) )));
	print("cmhs.h (reg)",               both(b, op( g->cmhs(d->v.h, d->v.h, s->v.h) )));
	print("cmhs.s (reg)",               both(b, op( g->cmhs(d->v.s, d->v.s, s->v.s) )));
	print("cmhs.d (reg)",               both(b, op( g->cmhs(d->v.d, d->v.d, s->v.d) )));

	print("cmtst.b (reg)",              both(b, op( g->cmtst(d->v.b, d->v.b, s->v.b) )));
	print("cmtst.h (reg)",              both(b, op( g->cmtst(d->v.h, d->v.h, s->v.h) )));
	print("cmtst.s (reg)",              both(b, op( g->cmtst(d->v.s, d->v.s, s->v.s) )));
	print("cmtst.d (reg)",              both(b, op( g->cmtst(d->v.d, d->v.d, s->v.d) )));

	print("cnt.b (reg)",                both(b, op( g->cnt(d->v.b, s->v.b) )));

	print("dup.b (vec; lane = 0)",      both(b, op( g->dup(d->v.b, s->v.b[0]) )));
	print("dup.b (vec; lane = 15)",     both(b, op( g->dup(d->v.b, s->v.b[15]) )));
	print("dup.h (vec; lane = 7)",      both(b, op( g->dup(d->v.h, s->v.h[7]) )));
	print("dup.s (vec; lane = 3)",      both(b, op( g->dup(d->v.s, s->v.s[3]) )));
	print("dup.d (vec; lane = 1)",      both(b, op( g->dup(d->v.d, s->v.d[1]) )));

	// print("dup.b (elem)",               both(b, op( g->dup(d->v.b, s->v.b[0]) )));
	// print("dup.b (elem)",               both(b, op( g->dup(d->v.b, s->v.b[15]) )));
	// print("dup.h (elem)",               both(b, op( g->dup(d->v.h, s->v.h[7]) )));
	print("dup.b (elem)",               both(b, op( g->dup(d->v.b, s->w) ),                         op( g->mov(d->w, d->v.s[0]) ), mov_latency));
	print("dup.h (elem)",               both(b, op( g->dup(d->v.h, s->w) ),                         op( g->mov(d->w, d->v.s[0]) ), mov_latency));
	print("dup.s (elem)",               both(b, op( g->dup(d->v.s, s->w) ),                         op( g->mov(d->w, d->v.s[0]) ), mov_latency));
	print("dup.d (elem)",               both(b, op( g->dup(d->v.d, s->x) ),                         op( g->mov(d->x, d->v.d[0]) ), mov_latency));

	print("eor3.b",                     both(b, op( g->eor3(d->v.b, d->v.b, d->v.b, s->v.b) )));
	print("eor.b",                      both(b, op( g->eor(d->v.b, d->v.b, s->v.b) )));
	print("ext.b (>>1)",                both(b, op( g->ext(d->v.b, d->v.b, s->v.b, 1) )));
	print("ext.b (>>15)",               both(b, op( g->ext(d->v.b, d->v.b, s->v.b, 1) )));

	print("fabd.h (scl)",               both(b, op( g->fabd(d->h, d->h, s->h) )));
	print("fabd.s (scl)",               both(b, op( g->fabd(d->s, d->s, s->s) )));
	print("fabd.d (scl)",               both(b, op( g->fabd(d->d, d->d, s->d) )));
	print("fabd.h (vec)",               both(b, op( g->fabd(d->v.h, d->v.h, s->v.h) )));
	print("fabd.s (vec)",               both(b, op( g->fabd(d->v.s, d->v.s, s->v.s) )));
	print("fabd.d (vec)",               both(b, op( g->fabd(d->v.d, d->v.d, s->v.d) )));

	print("fabs.h (scl)",               both(b, op( g->fabs(d->h, s->h) )));
	print("fabs.s (scl)",               both(b, op( g->fabs(d->s, s->s) )));
	print("fabs.d (scl)",               both(b, op( g->fabs(d->d, s->d) )));
	print("fabs.h (vec)",               both(b, op( g->fabs(d->v.h, s->v.h) )));
	print("fabs.s (vec)",               both(b, op( g->fabs(d->v.s, s->v.s) )));
	print("fabs.d (vec)",               both(b, op( g->fabs(d->v.d, s->v.d) )));

	print("facge.h (scl)",              both(b, op( g->facge(d->h, d->h, s->h) )));
	print("facge.s (scl)",              both(b, op( g->facge(d->s, d->s, s->s) )));
	print("facge.d (scl)",              both(b, op( g->facge(d->d, d->d, s->d) )));
	print("facge.h (vec)",              both(b, op( g->facge(d->v.h, d->v.h, s->v.h) )));
	print("facge.s (vec)",              both(b, op( g->facge(d->v.s, d->v.s, s->v.s) )));
	print("facge.d (vec)",              both(b, op( g->facge(d->v.d, d->v.d, s->v.d) )));

	print("facgt.h (scl)",              both(b, op( g->facgt(d->h, d->h, s->h) )));
	print("facgt.s (scl)",              both(b, op( g->facgt(d->s, d->s, s->s) )));
	print("facgt.d (scl)",              both(b, op( g->facgt(d->d, d->d, s->d) )));
	print("facgt.h (vec)",              both(b, op( g->facgt(d->v.h, d->v.h, s->v.h) )));
	print("facgt.s (vec)",              both(b, op( g->facgt(d->v.s, d->v.s, s->v.s) )));
	print("facgt.d (vec)",              both(b, op( g->facgt(d->v.d, d->v.d, s->v.d) )));

	print("fadd.h (scl)",               both(b, op( g->fadd(d->h, d->h, s->h) )));
	print("fadd.s (scl)",               both(b, op( g->fadd(d->s, d->s, s->s) )));
	print("fadd.d (scl)",               both(b, op( g->fadd(d->d, d->d, s->d) )));
	print("fadd.h (vec)",               both(b, op( g->fadd(d->v.h, d->v.h, s->v.h) )));
	print("fadd.s (vec)",               both(b, op( g->fadd(d->v.s, d->v.s, s->v.s) )));
	print("fadd.d (vec)",               both(b, op( g->fadd(d->v.d, d->v.d, s->v.d) )));

	print("faddp.h (scl)",              both(b, op( g->faddp(d->h, s->v.h2) )));
	print("faddp.s (scl)",              both(b, op( g->faddp(d->s, s->v.s2) )));
	print("faddp.d (scl)",              both(b, op( g->faddp(d->d, s->v.d2) )));
	print("faddp.h (vec)",              both(b, op( g->faddp(d->v.h, d->v.h, s->v.h) )));
	print("faddp.s (vec)",              both(b, op( g->faddp(d->v.s, d->v.s, s->v.s) )));
	print("faddp.d (vec)",              both(b, op( g->faddp(d->v.d, d->v.d, s->v.d) )));

	print("fcadd.h (deg = 90)",         both(b, op( g->fcadd(d->v.h, d->v.h, s->v.h, 90) )));
	print("fcadd.s (deg = 90)",         both(b, op( g->fcadd(d->v.s, d->v.s, s->v.s, 90) )));
	print("fcadd.d (deg = 90)",         both(b, op( g->fcadd(d->v.d, d->v.d, s->v.d, 90) )));

	/* FIXME */
	print("fcmp.h (reg)",               both(b, op( g->fcmp(s->h, s->h) )));
	print("fcmp.h (zero)",              both(b, op( g->fcmp(s->h, 0.0) )));
	print("fcmp.s (reg)",               both(b, op( g->fcmp(s->s, s->s) )));
	print("fcmp.s (zero)",              both(b, op( g->fcmp(s->s, 0.0) )));
	print("fcmp.d (reg)",               both(b, op( g->fcmp(s->d, s->d) )));
	print("fcmp.d (zero)",              both(b, op( g->fcmp(s->d, 0.0) )));
	print("fcmpe.h (reg)",              both(b, op( g->fcmpe(s->h, s->h) )));
	print("fcmpe.h (zero)",             both(b, op( g->fcmpe(s->h, 0.0) )));
	print("fcmpe.s (reg)",              both(b, op( g->fcmpe(s->s, s->s) )));
	print("fcmpe.s (zero)",             both(b, op( g->fcmpe(s->s, 0.0) )));
	print("fcmpe.d (reg)",              both(b, op( g->fcmpe(s->d, s->d) )));
	print("fcmpe.d (zero)",             both(b, op( g->fcmpe(s->d, 0.0) )));
	print("fccmp.h (eq)",               both(b, op( g->fccmp(s->h, s->h, 0x2, Cond::EQ) )));
	print("fccmp.h (le)",               both(b, op( g->fccmp(s->h, s->h, 0x2, Cond::LE) )));
	print("fccmp.s (eq)",               both(b, op( g->fccmp(s->s, s->s, 0x2, Cond::EQ) )));
	print("fccmp.s (le)",               both(b, op( g->fccmp(s->s, s->s, 0x2, Cond::LE) )));
	print("fccmp.d (eq)",               both(b, op( g->fccmp(s->d, s->d, 0x2, Cond::EQ) )));
	print("fccmp.d (le)",               both(b, op( g->fccmp(s->d, s->d, 0x2, Cond::LE) )));
	print("fccmpe.h (eq)",              both(b, op( g->fccmpe(s->h, s->h, 0x2, Cond::EQ) )));
	print("fccmpe.h (le)",              both(b, op( g->fccmpe(s->h, s->h, 0x2, Cond::LE) )));
	print("fccmpe.s (eq)",              both(b, op( g->fccmpe(s->s, s->s, 0x2, Cond::EQ) )));
	print("fccmpe.s (le)",              both(b, op( g->fccmpe(s->s, s->s, 0x2, Cond::LE) )));
	print("fccmpe.d (eq)",              both(b, op( g->fccmpe(s->d, s->d, 0x2, Cond::EQ) )));
	print("fccmpe.d (le)",              both(b, op( g->fccmpe(s->d, s->d, 0x2, Cond::LE) )));

	print("fcmeq.h (scl)",              both(b, op( g->fcmeq(d->h, d->h, s->h) )));
	print("fcmeq.s (scl)",              both(b, op( g->fcmeq(d->s, d->s, s->s) )));
	print("fcmeq.d (scl)",              both(b, op( g->fcmeq(d->d, d->d, s->d) )));
	print("fcmeq.h (vec)",              both(b, op( g->fcmeq(d->v.h, d->v.h, s->v.h) )));
	print("fcmeq.s (vec)",              both(b, op( g->fcmeq(d->v.s, d->v.s, s->v.s) )));
	print("fcmeq.d (vec)",              both(b, op( g->fcmeq(d->v.d, d->v.d, s->v.d) )));

	print("fcmeq.h (scl; zero)",        both(b, op( g->fcmeq(d->h, s->h, 0.0) )));
	print("fcmeq.s (scl; zero)",        both(b, op( g->fcmeq(d->s, s->s, 0.0) )));
	print("fcmeq.d (scl; zero)",        both(b, op( g->fcmeq(d->d, s->d, 0.0) )));
	print("fcmeq.h (vec; zero)",        both(b, op( g->fcmeq(d->v.h, s->v.h, 0.0) )));
	print("fcmeq.s (vec; zero)",        both(b, op( g->fcmeq(d->v.s, s->v.s, 0.0) )));
	print("fcmeq.d (vec; zero)",        both(b, op( g->fcmeq(d->v.d, s->v.d, 0.0) )));

	print("fcmge.h (scl)",              both(b, op( g->fcmge(d->h, d->h, s->h) )));
	print("fcmge.s (scl)",              both(b, op( g->fcmge(d->s, d->s, s->s) )));
	print("fcmge.d (scl)",              both(b, op( g->fcmge(d->d, d->d, s->d) )));
	print("fcmge.h (vec)",              both(b, op( g->fcmge(d->v.h, d->v.h, s->v.h) )));
	print("fcmge.s (vec)",              both(b, op( g->fcmge(d->v.s, d->v.s, s->v.s) )));
	print("fcmge.d (vec)",              both(b, op( g->fcmge(d->v.d, d->v.d, s->v.d) )));

	print("fcmge.h (scl; zero)",        both(b, op( g->fcmge(d->h, s->h, 0.0) )));
	print("fcmge.s (scl; zero)",        both(b, op( g->fcmge(d->s, s->s, 0.0) )));
	print("fcmge.d (scl; zero)",        both(b, op( g->fcmge(d->d, s->d, 0.0) )));
	print("fcmge.h (vec; zero)",        both(b, op( g->fcmge(d->v.h, s->v.h, 0.0) )));
	print("fcmge.s (vec; zero)",        both(b, op( g->fcmge(d->v.s, s->v.s, 0.0) )));
	print("fcmge.d (vec; zero)",        both(b, op( g->fcmge(d->v.d, s->v.d, 0.0) )));

	print("fcmgt.h (scl)",              both(b, op( g->fcmgt(d->h, d->h, s->h) )));
	print("fcmgt.s (scl)",              both(b, op( g->fcmgt(d->s, d->s, s->s) )));
	print("fcmgt.d (scl)",              both(b, op( g->fcmgt(d->d, d->d, s->d) )));
	print("fcmgt.h (vec)",              both(b, op( g->fcmgt(d->v.h, d->v.h, s->v.h) )));
	print("fcmgt.s (vec)",              both(b, op( g->fcmgt(d->v.s, d->v.s, s->v.s) )));
	print("fcmgt.d (vec)",              both(b, op( g->fcmgt(d->v.d, d->v.d, s->v.d) )));

	print("fcmgt.h (scl; zero)",        both(b, op( g->fcmgt(d->h, s->h, 0.0) )));
	print("fcmgt.s (scl; zero)",        both(b, op( g->fcmgt(d->s, s->s, 0.0) )));
	print("fcmgt.d (scl; zero)",        both(b, op( g->fcmgt(d->d, s->d, 0.0) )));
	print("fcmgt.h (vec; zero)",        both(b, op( g->fcmgt(d->v.h, s->v.h, 0.0) )));
	print("fcmgt.s (vec; zero)",        both(b, op( g->fcmgt(d->v.s, s->v.s, 0.0) )));
	print("fcmgt.d (vec; zero)",        both(b, op( g->fcmgt(d->v.d, s->v.d, 0.0) )));

	print("fcmle.h (scl; zero)",        both(b, op( g->fcmle(d->h, s->h, 0.0) )));
	print("fcmle.s (scl; zero)",        both(b, op( g->fcmle(d->s, s->s, 0.0) )));
	print("fcmle.d (scl; zero)",        both(b, op( g->fcmle(d->d, s->d, 0.0) )));
	print("fcmle.h (vec; zero)",        both(b, op( g->fcmle(d->v.h, s->v.h, 0.0) )));
	print("fcmle.s (vec; zero)",        both(b, op( g->fcmle(d->v.s, s->v.s, 0.0) )));
	print("fcmle.d (vec; zero)",        both(b, op( g->fcmle(d->v.d, s->v.d, 0.0) )));

	print("fcmlt.h (scl; zero)",        both(b, op( g->fcmlt(d->h, s->h, 0.0) )));
	print("fcmlt.s (scl; zero)",        both(b, op( g->fcmlt(d->s, s->s, 0.0) )));
	print("fcmlt.d (scl; zero)",        both(b, op( g->fcmlt(d->d, s->d, 0.0) )));
	print("fcmlt.h (vec; zero)",        both(b, op( g->fcmlt(d->v.h, s->v.h, 0.0) )));
	print("fcmlt.s (vec; zero)",        both(b, op( g->fcmlt(d->v.s, s->v.s, 0.0) )));
	print("fcmlt.d (vec; zero)",        both(b, op( g->fcmlt(d->v.d, s->v.d, 0.0) )));

	/* select */
	print("fcsel.h (eq)",               both(b, op( g->fcsel(d->h, d->h, s->h, Cond::EQ) )));
	print("fcsel.h (le)",               both(b, op( g->fcsel(d->h, d->h, s->h, Cond::LE) )));
	print("fcsel.s (eq)",               both(b, op( g->fcsel(d->s, d->s, s->s, Cond::EQ) )));
	print("fcsel.s (le)",               both(b, op( g->fcsel(d->s, d->s, s->s, Cond::LE) )));
	print("fcsel.d (eq)",               both(b, op( g->fcsel(d->d, d->d, s->d, Cond::EQ) )));
	print("fcsel.d (le)",               both(b, op( g->fcsel(d->d, d->d, s->d, Cond::LE) )));

	/* convert */
	print("fcvt (h -> s)",              both(b, op( g->fcvt(d->s, s->h) )));
	print("fcvt (h -> d)",              both(b, op( g->fcvt(d->d, s->h) )));
	print("fcvt (s -> h)",              both(b, op( g->fcvt(d->h, s->s) )));
	print("fcvt (s -> d)",              both(b, op( g->fcvt(d->d, s->s) )));
	print("fcvt (d -> h)",              both(b, op( g->fcvt(d->h, s->d) )));
	print("fcvt (d -> s)",              both(b, op( g->fcvt(d->s, s->d) )));

	print("fcvtl (h -> s)",             both(b, op( g->fcvtl(d->v.s, s->v.h4) )));
	print("fcvtl (s -> d)",             both(b, op( g->fcvtl(d->v.d, s->v.s2) )));
	print("fcvtl2 (h -> s)",            both(b, op( g->fcvtl2(d->v.s, s->v.h) )));
	print("fcvtl2 (s -> d)",            both(b, op( g->fcvtl2(d->v.d, s->v.s) )));
	print("fcvtxn",                     both(b, op( g->fcvtxn(d->v.s, s->v.d) )));
	print("fcvtxn2",                    both(b, op( g->fcvtxn2(d->v.s, s->v.d) )));
	print("fjcvtzs",                    both(b, op( g->fjcvtzs(d->w, s->d) ),                       op( g->mov(d->v.s[0], d->w) ), mov_latency));

	print("fcvtas.h (scl)",             both(b, op( g->fcvtas(d->h, s->h) )));
	print("fcvtas.s (scl)",             both(b, op( g->fcvtas(d->s, s->s) )));
	print("fcvtas.d (scl)",             both(b, op( g->fcvtas(d->d, s->d) )));
	print("fcvtas.h (vec)",             both(b, op( g->fcvtas(d->v.h, s->v.h) )));
	print("fcvtas.s (vec)",             both(b, op( g->fcvtas(d->v.s, s->v.s) )));
	print("fcvtas.d (vec)",             both(b, op( g->fcvtas(d->v.d, s->v.d) )));
	print("fcvtas.h (scl -> reg)",      both(b, op( g->fcvtas(d->x, s->h) ),                        op( g->mov(d->v.d[0], d->x) ), mov_latency ));
	print("fcvtas.s (scl -> reg)",      both(b, op( g->fcvtas(d->x, s->s) ),                        op( g->mov(d->v.d[0], d->x) ), mov_latency ));
	print("fcvtas.d (scl -> reg)",      both(b, op( g->fcvtas(d->x, s->d) ),                        op( g->mov(d->v.d[0], d->x) ), mov_latency ));

	print("fcvtau.h (scl)",             both(b, op( g->fcvtau(d->h, s->h) )));
	print("fcvtau.s (scl)",             both(b, op( g->fcvtau(d->s, s->s) )));
	print("fcvtau.d (scl)",             both(b, op( g->fcvtau(d->d, s->d) )));
	print("fcvtau.h (vec)",             both(b, op( g->fcvtau(d->v.h, s->v.h) )));
	print("fcvtau.s (vec)",             both(b, op( g->fcvtau(d->v.s, s->v.s) )));
	print("fcvtau.d (vec)",             both(b, op( g->fcvtau(d->v.d, s->v.d) )));
	print("fcvtau.h (scl -> reg)",      both(b, op( g->fcvtau(d->x, s->h) ),                        op( g->mov(d->v.d[0], d->x) ), mov_latency ));
	print("fcvtau.s (scl -> reg)",      both(b, op( g->fcvtau(d->x, s->s) ),                        op( g->mov(d->v.d[0], d->x) ), mov_latency ));
	print("fcvtau.d (scl -> reg)",      both(b, op( g->fcvtau(d->x, s->d) ),                        op( g->mov(d->v.d[0], d->x) ), mov_latency ));

	print("fmax.h (scl)",               both(b, op( g->fmax(d->h, d->h, s->h) )));
	print("fmax.s (scl)",               both(b, op( g->fmax(d->s, d->s, s->s) )));
	print("fmax.d (scl)",               both(b, op( g->fmax(d->d, d->d, s->d) )));
	print("fmax.h (vec)",               both(b, op( g->fmax(d->v.h, d->v.h, s->v.h) )));
	print("fmax.s (vec)",               both(b, op( g->fmax(d->v.s, d->v.s, s->v.s) )));
	print("fmax.d (vec)",               both(b, op( g->fmax(d->v.d, d->v.d, s->v.d) )));

	print("fmaxp.h (scl)",              both(b, op( g->fmaxp(d->h, s->v.h2) )));
	print("fmaxp.s (scl)",              both(b, op( g->fmaxp(d->s, s->v.s2) )));
	print("fmaxp.d (scl)",              both(b, op( g->fmaxp(d->d, s->v.d2) )));
	print("fmaxp.h (vec)",              both(b, op( g->fmaxp(d->v.h, d->v.h, s->v.h) )));
	print("fmaxp.s (vec)",              both(b, op( g->fmaxp(d->v.s, d->v.s, s->v.s) )));
	print("fmaxp.d (vec)",              both(b, op( g->fmaxp(d->v.d, d->v.d, s->v.d) )));

	print("fmaxv.h",                    both(b, op( g->fmaxv(d->h, s->v.h) )));
	print("fmaxv.s",                    both(b, op( g->fmaxv(d->s, s->v.s) )));

	print("fmaxnm.h (scl)",             both(b, op( g->fmaxnm(d->h, d->h, s->h) )));
	print("fmaxnm.s (scl)",             both(b, op( g->fmaxnm(d->s, d->s, s->s) )));
	print("fmaxnm.d (scl)",             both(b, op( g->fmaxnm(d->d, d->d, s->d) )));
	print("fmaxnm.h (vec)",             both(b, op( g->fmaxnm(d->v.h, d->v.h, s->v.h) )));
	print("fmaxnm.s (vec)",             both(b, op( g->fmaxnm(d->v.s, d->v.s, s->v.s) )));
	print("fmaxnm.d (vec)",             both(b, op( g->fmaxnm(d->v.d, d->v.d, s->v.d) )));

	print("fmaxnmp.h (scl)",            both(b, op( g->fmaxnmp(d->h, s->v.h2) )));
	print("fmaxnmp.s (scl)",            both(b, op( g->fmaxnmp(d->s, s->v.s2) )));
	print("fmaxnmp.d (scl)",            both(b, op( g->fmaxnmp(d->d, s->v.d2) )));
	print("fmaxnmp.h (vec)",            both(b, op( g->fmaxnmp(d->v.h, d->v.h, s->v.h) )));
	print("fmaxnmp.s (vec)",            both(b, op( g->fmaxnmp(d->v.s, d->v.s, s->v.s) )));
	print("fmaxnmp.d (vec)",            both(b, op( g->fmaxnmp(d->v.d, d->v.d, s->v.d) )));

	print("fmaxnmv.h",                  both(b, op( g->fmaxnmv(d->h, s->v.h) )));
	print("fmaxnmv.s",                  both(b, op( g->fmaxnmv(d->s, s->v.s) )));

	print("fmin.h (scl)",               both(b, op( g->fmin(d->h, d->h, s->h) )));
	print("fmin.s (scl)",               both(b, op( g->fmin(d->s, d->s, s->s) )));
	print("fmin.d (scl)",               both(b, op( g->fmin(d->d, d->d, s->d) )));
	print("fmin.h (vec)",               both(b, op( g->fmin(d->v.h, d->v.h, s->v.h) )));
	print("fmin.s (vec)",               both(b, op( g->fmin(d->v.s, d->v.s, s->v.s) )));
	print("fmin.d (vec)",               both(b, op( g->fmin(d->v.d, d->v.d, s->v.d) )));

	print("fminp.h (scl)",              both(b, op( g->fminp(d->h, s->v.h2) )));
	print("fminp.s (scl)",              both(b, op( g->fminp(d->s, s->v.s2) )));
	print("fminp.d (scl)",              both(b, op( g->fminp(d->d, s->v.d2) )));
	print("fminp.h (vec)",              both(b, op( g->fminp(d->v.h, d->v.h, s->v.h) )));
	print("fminp.s (vec)",              both(b, op( g->fminp(d->v.s, d->v.s, s->v.s) )));
	print("fminp.d (vec)",              both(b, op( g->fminp(d->v.d, d->v.d, s->v.d) )));

	print("fminv.h",                    both(b, op( g->fminv(d->h, s->v.h) )));
	print("fminv.s",                    both(b, op( g->fminv(d->s, s->v.s) )));

	print("fminnm.h (scl)",             both(b, op( g->fminnm(d->h, d->h, s->h) )));
	print("fminnm.s (scl)",             both(b, op( g->fminnm(d->s, d->s, s->s) )));
	print("fminnm.d (scl)",             both(b, op( g->fminnm(d->d, d->d, s->d) )));
	print("fminnm.h (vec)",             both(b, op( g->fminnm(d->v.h, d->v.h, s->v.h) )));
	print("fminnm.s (vec)",             both(b, op( g->fminnm(d->v.s, d->v.s, s->v.s) )));
	print("fminnm.d (vec)",             both(b, op( g->fminnm(d->v.d, d->v.d, s->v.d) )));

	print("fminnmp.h (scl)",            both(b, op( g->fminnmp(d->h, s->v.h2) )));
	print("fminnmp.s (scl)",            both(b, op( g->fminnmp(d->s, s->v.s2) )));
	print("fminnmp.d (scl)",            both(b, op( g->fminnmp(d->d, s->v.d2) )));
	print("fminnmp.h (vec)",            both(b, op( g->fminnmp(d->v.h, d->v.h, s->v.h) )));
	print("fminnmp.s (vec)",            both(b, op( g->fminnmp(d->v.s, d->v.s, s->v.s) )));
	print("fminnmp.d (vec)",            both(b, op( g->fminnmp(d->v.d, d->v.d, s->v.d) )));

	print("fminnmv.h",                  both(b, op( g->fminnmv(d->h, s->v.h) )));
	print("fminnmv.s",                  both(b, op( g->fminnmv(d->s, s->v.s) )));

	/* source register range limited */
	// print("fmla.h (elem)",              both(b, op( g->fmla(d->v.h, d->v.h, s->v.h[0]) )));
	// print("fmla.s (elem)",              both(b, op( g->fmla(d->v.s, d->v.s, s->v.s[0]) )));
	// print("fmla.d (elem)",              both(b, op( g->fmla(d->v.d, d->v.d, s->v.d[0]) )));
	print("fmla.h (vec)",               both(b, op( g->fmla(d->v.h, d->v.h, s->v.h) )));
	print("fmla.s (vec)",               both(b, op( g->fmla(d->v.s, d->v.s, s->v.s) )));
	print("fmla.d (vec)",               both(b, op( g->fmla(d->v.d, d->v.d, s->v.d) )));
	print("fmlal.h (vec)",              both(b, op( g->fmlal(d->v.s, d->v.h4, s->v.h4) )));
	print("fmlal2.h (vec)",             both(b, op( g->fmlal2(d->v.s, d->v.h4, s->v.h4) )));

	print("fmls.h (vec)",               both(b, op( g->fmls(d->v.h, d->v.h, s->v.h) )));
	print("fmls.s (vec)",               both(b, op( g->fmls(d->v.s, d->v.s, s->v.s) )));
	print("fmls.d (vec)",               both(b, op( g->fmls(d->v.d, d->v.d, s->v.d) )));
	print("fmlsl.h (vec)",              both(b, op( g->fmlsl(d->v.s, d->v.h4, s->v.h4) )));
	print("fmlsl2.h (vec)",             both(b, op( g->fmlsl2(d->v.s, d->v.h4, s->v.h4) )));

	print("fmadd.h",                    both(b, op( g->fmadd(d->h, d->h, d->h, s->h) )));
	print("fmadd.s",                    both(b, op( g->fmadd(d->s, d->s, d->s, s->s) )));
	print("fmadd.d",                    both(b, op( g->fmadd(d->d, d->d, d->d, s->d) )));

	print("fmsub.h",                    both(b, op( g->fmsub(d->h, d->h, d->h, s->h) )));
	print("fmsub.s",                    both(b, op( g->fmsub(d->s, d->s, d->s, s->s) )));
	print("fmsub.d",                    both(b, op( g->fmsub(d->d, d->d, d->d, s->d) )));

	print("fnmadd.h",                   both(b, op( g->fnmadd(d->h, d->h, d->h, s->h) )));
	print("fnmadd.s",                   both(b, op( g->fnmadd(d->s, d->s, d->s, s->s) )));
	print("fnmadd.d",                   both(b, op( g->fnmadd(d->d, d->d, d->d, s->d) )));

	print("fnmsub.h",                   both(b, op( g->fnmsub(d->h, d->h, d->h, s->h) )));
	print("fnmsub.s",                   both(b, op( g->fnmsub(d->s, d->s, d->s, s->s) )));
	print("fnmsub.d",                   both(b, op( g->fnmsub(d->d, d->d, d->d, s->d) )));

	print("fmul.h (scl)",               both(b, op( g->fmul(d->h, d->h, s->h) )));
	print("fmul.s (scl)",               both(b, op( g->fmul(d->s, d->s, s->s) )));
	print("fmul.d (scl)",               both(b, op( g->fmul(d->d, d->d, s->d) )));
	print("fmul.h (vec)",               both(b, op( g->fmul(d->v.h, d->v.h, s->v.h) )));
	print("fmul.s (vec)",               both(b, op( g->fmul(d->v.s, d->v.s, s->v.s) )));
	print("fmul.d (vec)",               both(b, op( g->fmul(d->v.d, d->v.d, s->v.d) )));

	print("fmulx.h (scl)",              both(b, op( g->fmulx(d->h, d->h, s->h) )));
	print("fmulx.s (scl)",              both(b, op( g->fmulx(d->s, d->s, s->s) )));
	print("fmulx.d (scl)",              both(b, op( g->fmulx(d->d, d->d, s->d) )));
	print("fmulx.h (vec)",              both(b, op( g->fmulx(d->v.h, d->v.h, s->v.h) )));
	print("fmulx.s (vec)",              both(b, op( g->fmulx(d->v.s, d->v.s, s->v.s) )));
	print("fmulx.d (vec)",              both(b, op( g->fmulx(d->v.d, d->v.d, s->v.d) )));

	print("fnmul.h (scl)",              both(b, op( g->fnmul(d->h, d->h, s->h) )));
	print("fnmul.s (scl)",              both(b, op( g->fnmul(d->s, d->s, s->s) )));
	print("fnmul.d (scl)",              both(b, op( g->fnmul(d->d, d->d, s->d) )));

	print("fcmla.h (elem; deg = 0)",    both(b, op( g->fcmla(d->v.h, d->v.h, s->v.h[0], 0) )));
	print("fcmla.s (elem; deg = 0)",    both(b, op( g->fcmla(d->v.s, d->v.s, s->v.s[0], 0) )));
	// print("fcmla.d (elem; deg = 0)",    both(b, op( g->fcmla(d->v.d, d->v.d, s->v.d[0], 0) )));
	print("fcmla.h (elem; deg = 90)",   both(b, op( g->fcmla(d->v.h, d->v.h, s->v.h[0], 90) )));
	print("fcmla.s (elem; deg = 90)",   both(b, op( g->fcmla(d->v.s, d->v.s, s->v.s[0], 90) )));
	// print("fcmla.d (elem; deg = 90)",   both(b, op( g->fcmla(d->v.d, d->v.d, s->v.d[0], 90) )));
	print("fcmla.h (vec; deg = 0)",     both(b, op( g->fcmla(d->v.h, d->v.h, s->v.h, 0) )));
	print("fcmla.s (vec; deg = 0)",     both(b, op( g->fcmla(d->v.s, d->v.s, s->v.s, 0) )));
	print("fcmla.d (vec; deg = 0)",     both(b, op( g->fcmla(d->v.d, d->v.d, s->v.d, 0) )));
	print("fcmla.h (vec; deg = 90)",    both(b, op( g->fcmla(d->v.h, d->v.h, s->v.h, 90) )));
	print("fcmla.s (vec; deg = 90)",    both(b, op( g->fcmla(d->v.s, d->v.s, s->v.s, 90) )));
	print("fcmla.d (vec; deg = 90)",    both(b, op( g->fcmla(d->v.d, d->v.d, s->v.d, 90) )));

	print("fneg.h (scl)",               both(b, op( g->fneg(d->h, s->h) )));
	print("fneg.s (scl)",               both(b, op( g->fneg(d->s, s->s) )));
	print("fneg.d (scl)",               both(b, op( g->fneg(d->d, s->d) )));
	print("fneg.h (vec)",               both(b, op( g->fneg(d->v.h, s->v.h) )));
	print("fneg.s (vec)",               both(b, op( g->fneg(d->v.s, s->v.s) )));
	print("fneg.d (vec)",               both(b, op( g->fneg(d->v.d, s->v.d) )));

	print("frecpe.h (scl)",             both(b, op( g->frecpe(d->h, s->h) )));
	print("frecpe.s (scl)",             both(b, op( g->frecpe(d->s, s->s) )));
	print("frecpe.d (scl)",             both(b, op( g->frecpe(d->d, s->d) )));
	print("frecpe.h (vec)",             both(b, op( g->frecpe(d->v.h, s->v.h) )));
	print("frecpe.s (vec)",             both(b, op( g->frecpe(d->v.s, s->v.s) )));
	print("frecpe.d (vec)",             both(b, op( g->frecpe(d->v.d, s->v.d) )));

	print("frecps.h (scl)",             both(b, op( g->frecps(d->h, d->h, s->h) )));
	print("frecps.s (scl)",             both(b, op( g->frecps(d->s, d->s, s->s) )));
	print("frecps.d (scl)",             both(b, op( g->frecps(d->d, d->d, s->d) )));
	print("frecps.h (vec)",             both(b, op( g->frecps(d->v.h, d->v.h, s->v.h) )));
	print("frecps.s (vec)",             both(b, op( g->frecps(d->v.s, d->v.s, s->v.s) )));
	print("frecps.d (vec)",             both(b, op( g->frecps(d->v.d, d->v.d, s->v.d) )));

	print("frecpx.h (scl)",             both(b, op( g->frecpx(d->h, s->h) )));
	print("frecpx.s (scl)",             both(b, op( g->frecpx(d->s, s->s) )));
	print("frecpx.d (scl)",             both(b, op( g->frecpx(d->d, s->d) )));

	// print("frint32x.s (scl)",           both(b, op( g->frint32x(d->v.s, s->v.s) )));
	// print("frint32x.d (scl)",           both(b, op( g->frint32x(d->v.d, s->v.d) )));
	print("frinta.h (scl)",             both(b, op( g->frinta(d->h, s->h) )));
	print("frinta.s (scl)",             both(b, op( g->frinta(d->s, s->s) )));
	print("frinta.d (scl)",             both(b, op( g->frinta(d->d, s->d) )));
	print("frinta.h (vec)",             both(b, op( g->frinta(d->v.h, s->v.h) )));
	print("frinta.s (vec)",             both(b, op( g->frinta(d->v.s, s->v.s) )));
	print("frinta.d (vec)",             both(b, op( g->frinta(d->v.d, s->v.d) )));

	print("frinti.h (scl)",             both(b, op( g->frinti(d->h, s->h) )));
	print("frinti.s (scl)",             both(b, op( g->frinti(d->s, s->s) )));
	print("frinti.d (scl)",             both(b, op( g->frinti(d->d, s->d) )));
	print("frinti.h (vec)",             both(b, op( g->frinti(d->v.h, s->v.h) )));
	print("frinti.s (vec)",             both(b, op( g->frinti(d->v.s, s->v.s) )));
	print("frinti.d (vec)",             both(b, op( g->frinti(d->v.d, s->v.d) )));

	print("frintm.h (scl)",             both(b, op( g->frintm(d->h, s->h) )));
	print("frintm.s (scl)",             both(b, op( g->frintm(d->s, s->s) )));
	print("frintm.d (scl)",             both(b, op( g->frintm(d->d, s->d) )));
	print("frintm.h (vec)",             both(b, op( g->frintm(d->v.h, s->v.h) )));
	print("frintm.s (vec)",             both(b, op( g->frintm(d->v.s, s->v.s) )));
	print("frintm.d (vec)",             both(b, op( g->frintm(d->v.d, s->v.d) )));

	print("frintn.h (scl)",             both(b, op( g->frintn(d->h, s->h) )));
	print("frintn.s (scl)",             both(b, op( g->frintn(d->s, s->s) )));
	print("frintn.d (scl)",             both(b, op( g->frintn(d->d, s->d) )));
	print("frintn.h (vec)",             both(b, op( g->frintn(d->v.h, s->v.h) )));
	print("frintn.s (vec)",             both(b, op( g->frintn(d->v.s, s->v.s) )));
	print("frintn.d (vec)",             both(b, op( g->frintn(d->v.d, s->v.d) )));

	print("frintp.h (scl)",             both(b, op( g->frintp(d->h, s->h) )));
	print("frintp.s (scl)",             both(b, op( g->frintp(d->s, s->s) )));
	print("frintp.d (scl)",             both(b, op( g->frintp(d->d, s->d) )));
	print("frintp.h (vec)",             both(b, op( g->frintp(d->v.h, s->v.h) )));
	print("frintp.s (vec)",             both(b, op( g->frintp(d->v.s, s->v.s) )));
	print("frintp.d (vec)",             both(b, op( g->frintp(d->v.d, s->v.d) )));

	print("frintx.h (scl)",             both(b, op( g->frintx(d->h, s->h) )));
	print("frintx.s (scl)",             both(b, op( g->frintx(d->s, s->s) )));
	print("frintx.d (scl)",             both(b, op( g->frintx(d->d, s->d) )));
	print("frintx.h (vec)",             both(b, op( g->frintx(d->v.h, s->v.h) )));
	print("frintx.s (vec)",             both(b, op( g->frintx(d->v.s, s->v.s) )));
	print("frintx.d (vec)",             both(b, op( g->frintx(d->v.d, s->v.d) )));

	print("frintz.h (scl)",             both(b, op( g->frintz(d->h, s->h) )));
	print("frintz.s (scl)",             both(b, op( g->frintz(d->s, s->s) )));
	print("frintz.d (scl)",             both(b, op( g->frintz(d->d, s->d) )));
	print("frintz.h (vec)",             both(b, op( g->frintz(d->v.h, s->v.h) )));
	print("frintz.s (vec)",             both(b, op( g->frintz(d->v.s, s->v.s) )));
	print("frintz.d (vec)",             both(b, op( g->frintz(d->v.d, s->v.d) )));

	print("frsqrte.h (scl)",            both(b, op( g->frsqrte(d->h, s->h) )));
	print("frsqrte.s (scl)",            both(b, op( g->frsqrte(d->s, s->s) )));
	print("frsqrte.d (scl)",            both(b, op( g->frsqrte(d->d, s->d) )));
	print("frsqrte.h (vec)",            both(b, op( g->frsqrte(d->v.h, s->v.h) )));
	print("frsqrte.s (vec)",            both(b, op( g->frsqrte(d->v.s, s->v.s) )));
	print("frsqrte.d (vec)",            both(b, op( g->frsqrte(d->v.d, s->v.d) )));

	print("frsqrts.h (scl)",            both(b, op( g->frsqrts(d->h, d->h, s->h) )));
	print("frsqrts.s (scl)",            both(b, op( g->frsqrts(d->s, d->s, s->s) )));
	print("frsqrts.d (scl)",            both(b, op( g->frsqrts(d->d, d->d, s->d) )));
	print("frsqrts.h (vec)",            both(b, op( g->frsqrts(d->v.h, d->v.h, s->v.h) )));
	print("frsqrts.s (vec)",            both(b, op( g->frsqrts(d->v.s, d->v.s, s->v.s) )));
	print("frsqrts.d (vec)",            both(b, op( g->frsqrts(d->v.d, d->v.d, s->v.d) )));

	print("fsqrt.h (scl)",              both(b, op( g->fsqrt(d->h, s->h) )));
	print("fsqrt.s (scl)",              both(b, op( g->fsqrt(d->s, s->s) )));
	print("fsqrt.d (scl)",              both(b, op( g->fsqrt(d->d, s->d) )));
	print("fsqrt.h (vec)",              both(b, op( g->fsqrt(d->v.h, s->v.h) )));
	print("fsqrt.s (vec)",              both(b, op( g->fsqrt(d->v.s, s->v.s) )));
	print("fsqrt.d (vec)",              both(b, op( g->fsqrt(d->v.d, s->v.d) )));

	print("mla.b (reg)",                both(b, op( g->mla(d->v.b, d->v.b, s->v.b) )));
	print("mla.h (reg)",                both(b, op( g->mla(d->v.h, d->v.h, s->v.h) )));
	print("mla.s (reg)",                both(b, op( g->mla(d->v.s, d->v.s, s->v.s) )));

	print("mul.b (reg)",                both(b, op( g->mul(d->v.b, d->v.b, s->v.b) )));
	print("mul.h (reg)",                both(b, op( g->mul(d->v.h, d->v.h, s->v.h) )));
	print("mul.s (reg)",                both(b, op( g->mul(d->v.s, d->v.s, s->v.s) )));

	print("pmul.b",                     both(b, op( g->pmul(d->v.b, d->v.b, s->v.b) )));
	print("pmull.b",                    both(b, op( g->pmull(d->v.h, d->v.b8, s->v.b8) )));
	print("pmull.d",                    both(b, op( g->pmull(d->v.q, d->v.d1, s->v.d1) )));
	print("pmull2.b",                   both(b, op( g->pmull2(d->v.h, d->v.b, s->v.b) )));
	print("pmull2.d",                   both(b, op( g->pmull2(d->v.q, d->v.d, s->v.d) )));

	print("raddhn.h",                   both(b, op( g->raddhn(d->v.b8, d->v.h, s->v.h) )));
	print("raddhn.s",                   both(b, op( g->raddhn(d->v.h4, d->v.s, s->v.s) )));
	print("raddhn.d",                   both(b, op( g->raddhn(d->v.s2, d->v.d, s->v.d) )));
	print("raddhn2.h",                  both(b, op( g->raddhn2(d->v.b, d->v.h, s->v.h) )));
	print("raddhn2.s",                  both(b, op( g->raddhn2(d->v.h, d->v.s, s->v.s) )));
	print("raddhn2.d",                  both(b, op( g->raddhn2(d->v.s, d->v.d, s->v.d) )));

	print("mvn.b",                      both(b, op( g->mvn(d->v.b, s->v.b) )));
	print("mvni.h",                     both(b, op( g->mvni(d->v.h, 0x11, ShMod::LSL, 0) )));
	print("mvni.s",                     both(b, op( g->mvni(d->v.s, 0x11, ShMod::LSL, 0) )));
	print("mvni.h",                     both(b, op( g->mvni(d->v.h, 0x11, ShMod::LSL, 8) )));
	print("mvni.s",                     both(b, op( g->mvni(d->v.s, 0x11, ShMod::LSL, 8) )));

	print("not.b",                      both(b, op( g->not_(d->v.b, s->v.b) )));

	print("neg.b",                      both(b, op( g->neg(d->v.b, s->v.b) )));
	print("neg.h",                      both(b, op( g->neg(d->v.h, s->v.h) )));
	print("neg.s",                      both(b, op( g->neg(d->v.s, s->v.s) )));
	print("neg.d",                      both(b, op( g->neg(d->v.d, s->v.d) )));

	print("orn.b",                      both(b, op( g->orn(d->v.b, d->v.b, s->v.b) )));

	print("orr.b",                      both(b, op( g->orr(d->v.b, d->v.b, s->v.b) )));
	print("orr.h",                      both(b, op( g->orr(d->v.h, 0x11, ShMod::LSL, 0) )));
	print("orr.s",                      both(b, op( g->orr(d->v.s, 0x11, ShMod::LSL, 0) )));
	print("orr.h",                      both(b, op( g->orr(d->v.h, 0x11, ShMod::LSL, 8) )));
	print("orr.s",                      both(b, op( g->orr(d->v.s, 0x11, ShMod::LSL, 8) )));

	print("rax1.d",                     both(b, op( g->rax1(d->v.d, d->v.d, s->v.d) )));

	print("rbit.b",                     both(b, op( g->rbit(d->v.b, s->v.b) )));
	print("rev16.b",                    both(b, op( g->rev16(d->v.b, s->v.b) )));
	print("rev32.b",                    both(b, op( g->rev32(d->v.b, s->v.b) )));
	print("rev32.h",                    both(b, op( g->rev32(d->v.h, s->v.h) )));
	print("rev64.b",                    both(b, op( g->rev64(d->v.b, s->v.b) )));
	print("rev64.h",                    both(b, op( g->rev64(d->v.h, s->v.h) )));
	print("rev64.s",                    both(b, op( g->rev64(d->v.s, s->v.s) )));

	print("rshrn.h (>>2)",              both(b, op( g->rshrn(d->v.b8, s->v.h, 2) )));
	print("rshrn.s (>>2)",              both(b, op( g->rshrn(d->v.h4, s->v.s, 2) )));
	print("rshrn.d (>>2)",              both(b, op( g->rshrn(d->v.s2, s->v.d, 2) )));
	print("rshrn2.h (>>2)",             both(b, op( g->rshrn2(d->v.b, s->v.h, 2) )));
	print("rshrn2.s (>>2)",             both(b, op( g->rshrn2(d->v.h, s->v.s, 2) )));
	print("rshrn2.d (>>2)",             both(b, op( g->rshrn2(d->v.s, s->v.d, 2) )));

	print("rsubhn.h",                   both(b, op( g->rsubhn(d->v.b8, d->v.h, s->v.h) )));
	print("rsubhn.s",                   both(b, op( g->rsubhn(d->v.h4, d->v.s, s->v.s) )));
	print("rsubhn.d",                   both(b, op( g->rsubhn(d->v.s2, d->v.d, s->v.d) )));
	print("rsubhn2.h",                  both(b, op( g->rsubhn2(d->v.b, d->v.h, s->v.h) )));
	print("rsubhn2.s",                  both(b, op( g->rsubhn2(d->v.h, d->v.s, s->v.s) )));
	print("rsubhn2.d",                  both(b, op( g->rsubhn2(d->v.s, d->v.d, s->v.d) )));

	print("saba.b",                     both(b, op( g->saba(d->v.b, d->v.b, s->v.b) )));
	print("saba.h",                     both(b, op( g->saba(d->v.h, d->v.h, s->v.h) )));
	print("saba.s",                     both(b, op( g->saba(d->v.s, d->v.s, s->v.s) )));

	print("sabal.b",                    both(b, op( g->sabal(d->v.h, d->v.b8, s->v.b8) )));
	print("sabal.h",                    both(b, op( g->sabal(d->v.s, d->v.h4, s->v.h4) )));
	print("sabal.s",                    both(b, op( g->sabal(d->v.d, d->v.s2, s->v.s2) )));

	print("sabal2.b",                   both(b, op( g->sabal2(d->v.h, d->v.b, s->v.b) )));
	print("sabal2.h",                   both(b, op( g->sabal2(d->v.s, d->v.h, s->v.h) )));
	print("sabal2.s",                   both(b, op( g->sabal2(d->v.d, d->v.s, s->v.s) )));

	print("sabd.b",                     both(b, op( g->sabd(d->v.b, d->v.b, s->v.b) )));
	print("sabd.h",                     both(b, op( g->sabd(d->v.h, d->v.h, s->v.h) )));
	print("sabd.s",                     both(b, op( g->sabd(d->v.s, d->v.s, s->v.s) )));

	print("sabdl.b",                    both(b, op( g->sabdl(d->v.h, d->v.b8, s->v.b8) )));
	print("sabdl.h",                    both(b, op( g->sabdl(d->v.s, d->v.h4, s->v.h4) )));
	print("sabdl.s",                    both(b, op( g->sabdl(d->v.d, d->v.s2, s->v.s2) )));

	print("sabdl2.b",                   both(b, op( g->sabdl2(d->v.h, d->v.b, s->v.b) )));
	print("sabdl2.h",                   both(b, op( g->sabdl2(d->v.s, d->v.h, s->v.h) )));
	print("sabdl2.s",                   both(b, op( g->sabdl2(d->v.d, d->v.s, s->v.s) )));

	print("sadalp.b",                   both(b, op( g->sadalp(d->v.h, s->v.b) )));
	print("sadalp.h",                   both(b, op( g->sadalp(d->v.s, s->v.h) )));
	print("sadalp.s",                   both(b, op( g->sadalp(d->v.d, s->v.s) )));

	print("saddl.b",                    both(b, op( g->saddl(d->v.h, d->v.b8, s->v.b8) )));
	print("saddl.h",                    both(b, op( g->saddl(d->v.s, d->v.h4, s->v.h4) )));
	print("saddl.s",                    both(b, op( g->saddl(d->v.d, d->v.s2, s->v.s2) )));

	print("saddl2.b",                   both(b, op( g->saddl2(d->v.h, d->v.b, s->v.b) )));
	print("saddl2.h",                   both(b, op( g->saddl2(d->v.s, d->v.h, s->v.h) )));
	print("saddl2.s",                   both(b, op( g->saddl2(d->v.d, d->v.s, s->v.s) )));

	print("saddlp.b",                   both(b, op( g->saddlp(d->v.h, s->v.b) )));
	print("saddlp.h",                   both(b, op( g->saddlp(d->v.s, s->v.h) )));
	print("saddlp.s",                   both(b, op( g->saddlp(d->v.d, s->v.s) )));

	print("saddlv.b",                   both(b, op( g->saddlv(d->h, s->v.b) )));
	print("saddlv.h",                   both(b, op( g->saddlv(d->s, s->v.h) )));
	print("saddlv.s",                   both(b, op( g->saddlv(d->d, s->v.s) )));

	print("saddw.b",                    both(b, op( g->saddw(d->v.h, d->v.h, s->v.b8) )));
	print("saddw.h",                    both(b, op( g->saddw(d->v.s, d->v.s, s->v.h4) )));
	print("saddw.s",                    both(b, op( g->saddw(d->v.d, d->v.d, s->v.s2) )));

	print("saddw2.b",                   both(b, op( g->saddw2(d->v.h, d->v.h, s->v.b) )));
	print("saddw2.h",                   both(b, op( g->saddw2(d->v.s, d->v.s, s->v.h) )));
	print("saddw2.s",                   both(b, op( g->saddw2(d->v.d, d->v.d, s->v.s) )));

	print("scvtf.h (vec; >>2)",         both(b, op( g->scvtf(d->v.h, s->v.h, 2) )));
	print("scvtf.s (vec; >>2)",         both(b, op( g->scvtf(d->v.s, s->v.s, 2) )));
	print("scvtf.d (vec; >>2)",         both(b, op( g->scvtf(d->v.d, s->v.d, 2) )));
	print("scvtf2.h (vec; int)",        both(b, op( g->scvtf(d->v.h, s->v.h) )));
	print("scvtf2.s (vec; int)",        both(b, op( g->scvtf(d->v.s, s->v.s) )));
	print("scvtf2.d (vec; int)",        both(b, op( g->scvtf(d->v.d, s->v.d) )));

	print("scvtf.h (scl; >>2)",         both(b, op( g->scvtf(d->h, s->w, 2) ), op( g->mov(d->w, d->v.s[0]) ), mov_latency));
	print("scvtf.s (scl; >>2)",         both(b, op( g->scvtf(d->s, s->w, 2) ), op( g->mov(d->w, d->v.s[0]) ), mov_latency));
	print("scvtf.d (scl; >>2)",         both(b, op( g->scvtf(d->d, s->x, 2) ), op( g->mov(d->x, d->v.d[0]) ), mov_latency));
	print("scvtf2.h (scl; int)",        both(b, op( g->scvtf(d->h, s->w) ), op( g->mov(d->w, d->v.s[0]) ), mov_latency));
	print("scvtf2.s (scl; int)",        both(b, op( g->scvtf(d->s, s->w) ), op( g->mov(d->w, d->v.s[0]) ), mov_latency));
	print("scvtf2.d (scl; int)",        both(b, op( g->scvtf(d->d, s->x) ), op( g->mov(d->x, d->v.d[0]) ), mov_latency));

	// print("cmhi.b (zero)",              both(b, op( g->cmhi(d->v.b, s->v.b, 0) )));
	// print("cmhi.d (zero)",              both(b, op( g->cmhi(d->v.d, s->v.d, 0) )));



	#endif
}


#endif	/* _ALU_H_INCLUDED */
/*
 * end of alu.h
 */
