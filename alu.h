
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
void bench_store(bool md, double freq) {
	(void)md;
	(void)freq;
	return;
}

static
void bench_load(bool md, double freq) {
	table t(md, "load instructions");

	size_t const elems = 128 / sizeof(void *);
	void **self  = (void **)malloc(sizeof(void *) * elems);
	void **loop = (void **)malloc(sizeof(void *) * elems);

	init_mems(elems, self, loop);
	bench l(freq, self);
	bench c(freq, loop);

	t.put("ldr", both(l, op( g->ldr(d->x, ptr(s->x)) )));
	t.put("ldr", both(c, op( g->ldr(d->x, ptr(s->x)) )));

	free(self);
	free(loop);
	return;
}

static
void bench_alu(bool md, double freq) {
	// printf("# measuring arithmetic and logic instruction latencies...\n");
	table t(md, "alu");
	bench b(freq, (size_t)0, 0);

	double const adc_latency = b.lat_(0, op( g->adc(d->x, s->x, s->x) )).lat;
	double const adds_adc_latency = b.lat_(0, op( g->adds(s->x, s->x, s->x); g->adc(d->x, s->x, s->x) )).lat;
	// double const fill_rate = 2.0;

	/* move */
	t.put("nop",                        thr(b, op( g->nop() )));
	t.put("mov (reg)",                  both(b, op( g->mov(d->x, s->x) )));
	t.put("mov / movz (imm)",           thr(b, op( g->mov(d->x, 0x1ffc) )));
	t.put("mov / movz (imm<<16)",       thr(b, op( g->mov(d->x, 0x1ffc<<16) )));
	t.put("mov (mask imm)",             thr(b, op( g->mov(d->x, 0x1ffffffffffc) )));
	t.put("mov / movn (imm)",           thr(b, op( g->movn(d->x, 0x1ffc) )));
	t.put("mov / movn (imm<<16)",       thr(b, op( g->movn(d->x, 0x1ffc, 16) )));
	t.put("movk",                       thr(b, op( g->movk(d->x, 0x1ffc) )));
	t.put("movk (<<16)",                thr(b, op( g->movk(d->x, 0x1ffc, 16) )));

	/* basic arith */
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

	/* not implemented in xbyak_aarch64 */
	t.put("sub",                        thr(b, op( g->sub(d->x, d->x, s->x) )));
	// t.put("subp",                       thr(b, op( g->subp(d->x, d->x, s->x) )));

	t.put("adr",                        thr(b, op( g->adr(d->x, g->L()) )));
	t.put("adrp",                       thr(b, op( g->adrp(d->x, g->L()) )));

	t.put("neg (reg)",                  both(b, op( g->neg(d->x, s->x) )));
	t.put("neg (reg<<2)",               both(b, op( g->neg(d->x, s->x, ShMod::LSL, 2) )));
	t.put("negs (reg)",                 both(b, op( g->negs(d->x, s->x) ),                          op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("negs (reg<<2)",              both(b, op( g->negs(d->x, s->x, ShMod::LSL, 2) ),           op( g->adc(d->x, d->x, s->x) ), adc_latency));

	t.put("ngc",                        both(b, op( g->ngc(d->x, s->x) )));
	t.put("ngcs",                       both(b, op( g->ngcs(d->x, s->x) ),                          op( g->adc(d->x, d->x, s->x) ), adc_latency));

	/* multiply */
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

	/* divide */
	t.put("sdiv",                       both(b, op( g->sdiv(d->x, d->x, s->x) )));
	t.put("udiv",                       both(b, op( g->udiv(d->x, d->x, s->x) )));

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


	t.put("eor (reg)",                  both(b, op( g->eor(d->x, d->x, s->x) )));
	t.put("eor (reg<<2)",               both(b, op( g->eor(d->x, d->x, s->x, ShMod::LSL, 2) )));
	t.put("eon (reg)",                  both(b, op( g->eon(d->x, d->x, s->x) )));
	t.put("eon (reg<<2)",               both(b, op( g->eon(d->x, d->x, s->x, ShMod::LSL, 2) )));

	t.put("orr (reg)",                  both(b, op( g->orr(d->x, d->x, s->x) )));
	t.put("orr (reg<<2)",               both(b, op( g->orr(d->x, d->x, s->x, ShMod::LSL, 2) )));
	t.put("orr (mask imm)",             both(b, op( g->orr(d->x, s->x, 0x1ffc) )));
	t.put("orn (reg)",                  both(b, op( g->orn(d->x, d->x, s->x) )));
	t.put("orn (reg<<2)",               both(b, op( g->orn(d->x, d->x, s->x, ShMod::LSL, 2) )));

	t.put("mvn (reg)",                  both(b, op( g->mvn(d->x, s->x) )));
	t.put("mvn (reg<<2)",               both(b, op( g->mvn(d->x, s->x, ShMod::LSL, 2) )));

	/* compare */
	t.put("ccmn (reg; eq)",             both(b, op( g->ccmn(s->x, s->x, 0x2, Cond::EQ) ),           op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("ccmn (reg; lt)",             both(b, op( g->ccmn(s->x, s->x, 0x2, Cond::LT) ),           op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("ccmn (imm; eq)",             both(b, op( g->ccmn(s->x, 17, 0x2, Cond::EQ) ),             op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("ccmn (imm; lt)",             both(b, op( g->ccmn(s->x, 17, 0x2, Cond::LT) ),             op( g->adc(d->x, d->x, s->x) ), adc_latency));

	t.put("ccmp (reg; eq)",             both(b, op( g->ccmp(s->x, s->x, 0x2, Cond::EQ) ),           op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("ccmp (reg; lt)",             both(b, op( g->ccmp(s->x, s->x, 0x2, Cond::LT) ),           op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("ccmp (imm; eq)",             both(b, op( g->ccmp(s->x, 17, 0x2, Cond::EQ) ),             op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("ccmp (imm; lt)",             both(b, op( g->ccmp(s->x, 17, 0x2, Cond::LT) ),             op( g->adc(d->x, d->x, s->x) ), adc_latency));

	/* flag manipulation */
	t.put("cfinv",                      both(b, op( g->cfinv() ),                                   op( g->adc(d->x, d->x, s->x); g->adds(d->x, d->x, s->x) ), adds_adc_latency));
	t.put("ccmp (imm; lt)",             both(b, op( g->ccmp(s->x, 17, 0x2, Cond::LT) ),             op( g->adc(d->x, d->x, s->x) ), adc_latency));
	signal(SIGILL, sigill_trap);
	t.put("cfinv",                      both(b, op( g->cfinv() ),                                   op( g->adc(d->x, d->x, s->x); g->adds(d->x, d->x, s->x) ), adds_adc_latency));

	t.put("rmif",                       both(b, op( g->rmif(s->x, 17, 0xf) ),                       op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("setf8",                      both(b, op( g->setf8(s->w) ),                               op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("setf16",                     both(b, op( g->setf16(s->w) ),                              op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("tst (reg)",                  both(b, op( g->tst(s->x, s->x) ),                           op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("tst (reg<<2)",               both(b, op( g->tst(s->x, s->x, ShMod::LSL, 2) ),            op( g->adc(d->x, d->x, s->x) ), adc_latency));
	t.put("tst (imm)",                  both(b, op( g->tst(s->x, 0x1ffc) ),                         op( g->adc(d->x, d->x, s->x) ), adc_latency));

	/* conditional arithmetic */
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

	/* bitfield manipulation */
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

	/* shift */
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

	/* sign extension / extract */
	t.put("sxtb",                       both(b, op( g->sxtb(d->x, s->w) )));
	t.put("sxth",                       both(b, op( g->sxth(d->x, s->w) )));
	t.put("sxtw",                       both(b, op( g->sxtw(d->x, s->w) )));
	t.put("uxtb",                       both(b, op( g->uxtb(d->x, s->x) )));
	t.put("uxth",                       both(b, op( g->uxth(d->x, s->x) )));

	/* reverse bits / bytes */
	t.put("rbit",                       both(b, op( g->rbit(d->x, s->x) )));
	t.put("rev (rev16)",                both(b, op( g->rev16(d->x, s->x) )));
	t.put("rev (rev32)",                both(b, op( g->rev32(d->x, s->x) )));
	t.put("rev (rev64)",                both(b, op( g->rev64(d->x, s->x) )));

	/* count bits */
	t.put("clz",                        both(b, op( g->clz(d->x, s->x) )));
	t.put("cls",                        both(b, op( g->cls(d->x, s->x) )));

	/* special */
	t.put("crc32x",                     both(b, op( g->crc32x(d->w, d->w, s->x) )));
	t.put("crc32cx",                    both(b, op( g->crc32cx(d->w, d->w, s->x) )));


	double const mov_latency = b.lat_(0, op( g->mov(d->v.d[0], s->x); g->mov(d->x, d->v.d[0]) )).lat / 2.0;
	t.put("mov (reg)",                  both(b, op( g->mov(d->v.b, s->v.b) )));
	// t.put("mov.b (v.b[0] <-> w)",       lat(b, op( g->mov(d->v.b[0], s->w); g->mov(d->x, d->v.b[0]) )));
	// t.put("mov.h (v.h[0] <-> x)",       lat(b, op( g->mov(d->v.h[0], s->x); g->mov(d->x, d->v.h[0]) )));
	t.put("mov.s (v.s[0] <-> w)",       lat(b, op( g->mov(d->v.s[0], s->w); g->mov(d->w, d->v.s[0]) )));
	t.put("mov.d (v.d[0] <-> x)",       lat(b, op( g->mov(d->v.d[0], s->x); g->mov(d->x, d->v.d[0]) )));
	// t.put("mov.b (v.b[15] <-> w)",       lat(b, op( g->mov(d->v.b[15], s->w); g->mov(d->x, d->v.b[15]) )));
	// t.put("mov.h (v.h[7] <-> x)",       lat(b, op( g->mov(d->v.h[7], s->x); g->mov(d->x, d->v.h[7]) )));
	t.put("mov.s (v.s[3] <-> w)",       lat(b, op( g->mov(d->v.s[3], s->w); g->mov(d->w, d->v.s[3]) )));
	t.put("mov.d (v.d[1] <-> x)",       lat(b, op( g->mov(d->v.d[1], s->x); g->mov(d->x, d->v.d[1]) )));
	t.put("smov.b (v.b[0] <-> x)",      lat(b, op( g->mov(d->v.b[0], s->w); g->smov(d->w, d->v.b[0]) )));
	t.put("smov.h (v.h[0] <-> x)",      lat(b, op( g->mov(d->v.h[0], s->w); g->smov(d->w, d->v.h[0]) )));
	t.put("smov.s (v.s[0] <-> x)",      lat(b, op( g->mov(d->v.s[0], s->w); g->smov(d->x, d->v.s[0]) )));
	t.put("umov.b (v.b[0] <-> w)",      lat(b, op( g->mov(d->v.b[0], s->w); g->umov(d->w, d->v.b[0]) )));
	t.put("umov.h (v.h[0] <-> w)",      lat(b, op( g->mov(d->v.h[0], s->w); g->umov(d->w, d->v.h[0]) )));
	t.put("umov.s (v.s[0] <-> w)",      lat(b, op( g->mov(d->v.s[0], s->w); g->umov(d->w, d->v.s[0]) )));
	t.put("umov.d (v.d[0] <-> x)",      lat(b, op( g->mov(d->v.d[0], s->x); g->umov(d->x, d->v.d[0]) )));

	// t.put("fmov.h (v.h[0] <-> x)",      lat(b, op( g->fmov(d->v.h[0], s->x); g->fmov(d->x, d->v.h[0]) )));
	// t.put("fmov.s (v.s[0] <-> x)",      lat(b, op( g->fmov(d->v.s[0], s->w); g->fmov(d->x, d->v.s[0]) )));
	t.put("fmov.d (v.d[0] <-> x)",      lat(b, op( g->fmov(d->v.d[0], s->x); g->fmov(d->x, d->v.d[0]) )));

	t.put("abs.b",                      both(b, op( g->abs(d->v.b, s->v.b) )));
	t.put("abs.h",                      both(b, op( g->abs(d->v.h, s->v.h) )));
	t.put("abs.s",                      both(b, op( g->abs(d->v.s, s->v.s) )));
	t.put("abs.d",                      both(b, op( g->abs(d->v.d, s->v.d) )));

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
	// t.put("addv.d",                     both(b, op( g->addv(d->d, s->v.d) )));

	t.put("aese",                       both(b, op( g->aese(d->v.b, s->v.b) )));
	t.put("aesd",                       both(b, op( g->aesd(d->v.b, s->v.b) )));
	t.put("aesmc",                      both(b, op( g->aesmc(d->v.b, s->v.b) )));
	t.put("aesimc",                     both(b, op( g->aesimc(d->v.b, s->v.b) )));

	t.put("and.b",                      both(b, op( g->and_(d->v.b, d->v.b, s->v.b) )));
	t.put("bcax.b",                     both(b, op( g->bcax(d->v.b, d->v.b, d->v.b, s->v.b) )));

	// t.put("bfcvt",                      both(b, op( g->bfcvt(d->h, d->s) )));

	t.put("bic.h (imm)",                both(b, op( g->bic(d->v.h8, 0x11, ShMod::LSL, 0) )));
	t.put("bic.h (imm)",                both(b, op( g->bic(d->v.h8, 0x11, ShMod::LSL, 8) )));
	t.put("bic.s (imm)",                both(b, op( g->bic(d->v.s4, 0x11, ShMod::LSL, 0) )));
	t.put("bic.s (imm)",                both(b, op( g->bic(d->v.s4, 0x11, ShMod::LSL, 8) )));

	t.put("bic.b (reg)",                both(b, op( g->bic(d->v.b, d->v.b, s->v.b) )));
	t.put("bif.b",                      both(b, op( g->bif(d->v.b, d->v.b, s->v.b) )));
	t.put("bit.b",                      both(b, op( g->bit(d->v.b, d->v.b, s->v.b) )));
	t.put("bsl.b",                      both(b, op( g->bsl(d->v.b, d->v.b, s->v.b) )));

	t.put("cls.b",                      both(b, op( g->cls(d->v.b, s->v.b) )));
	t.put("cls.h",                      both(b, op( g->cls(d->v.h, s->v.h) )));
	t.put("cls.s",                      both(b, op( g->cls(d->v.s, s->v.s) )));
	// t.put("cls.d",                      both(b, op( g->cls(d->v.d, s->v.d) )));
	t.put("clz.b",                      both(b, op( g->clz(d->v.b, s->v.b) )));
	t.put("clz.h",                      both(b, op( g->clz(d->v.h, s->v.h) )));
	t.put("clz.s",                      both(b, op( g->clz(d->v.s, s->v.s) )));
	// t.put("clz.d",                      both(b, op( g->clz(d->v.d, s->v.d) )));

	t.put("cmeq.b (reg)",               both(b, op( g->cmeq(d->v.b, d->v.b, s->v.b) )));
	t.put("cmeq.h (reg)",               both(b, op( g->cmeq(d->v.h, d->v.h, s->v.h) )));
	t.put("cmeq.s (reg)",               both(b, op( g->cmeq(d->v.s, d->v.s, s->v.s) )));
	t.put("cmeq.d (reg)",               both(b, op( g->cmeq(d->v.d, d->v.d, s->v.d) )));

	t.put("cmeq.b (zero)",              both(b, op( g->cmeq(d->v.b, s->v.b, 0) )));
	t.put("cmeq.h (zero)",              both(b, op( g->cmeq(d->v.h, s->v.h, 0) )));
	t.put("cmeq.s (zero)",              both(b, op( g->cmeq(d->v.s, s->v.s, 0) )));
	t.put("cmeq.d (zero)",              both(b, op( g->cmeq(d->v.d, s->v.d, 0) )));

	t.put("cmge.b (reg)",               both(b, op( g->cmge(d->v.b, d->v.b, s->v.b) )));
	t.put("cmge.h (reg)",               both(b, op( g->cmge(d->v.h, d->v.h, s->v.h) )));
	t.put("cmge.s (reg)",               both(b, op( g->cmge(d->v.s, d->v.s, s->v.s) )));
	t.put("cmge.d (reg)",               both(b, op( g->cmge(d->v.d, d->v.d, s->v.d) )));
	t.put("cmge.b (zero)",              both(b, op( g->cmge(d->v.b, s->v.b, 0) )));
	t.put("cmge.h (zero)",              both(b, op( g->cmge(d->v.h, s->v.h, 0) )));
	t.put("cmge.s (zero)",              both(b, op( g->cmge(d->v.s, s->v.s, 0) )));
	t.put("cmge.d (zero)",              both(b, op( g->cmge(d->v.d, s->v.d, 0) )));

	t.put("cmgt.b (reg)",               both(b, op( g->cmgt(d->v.b, d->v.b, s->v.b) )));
	t.put("cmgt.h (reg)",               both(b, op( g->cmgt(d->v.h, d->v.h, s->v.h) )));
	t.put("cmgt.s (reg)",               both(b, op( g->cmgt(d->v.s, d->v.s, s->v.s) )));
	t.put("cmgt.d (reg)",               both(b, op( g->cmgt(d->v.d, d->v.d, s->v.d) )));
	t.put("cmgt.b (zero)",              both(b, op( g->cmgt(d->v.b, s->v.b, 0) )));
	t.put("cmgt.h (zero)",              both(b, op( g->cmgt(d->v.h, s->v.h, 0) )));
	t.put("cmgt.s (zero)",              both(b, op( g->cmgt(d->v.s, s->v.s, 0) )));
	t.put("cmgt.d (zero)",              both(b, op( g->cmgt(d->v.d, s->v.d, 0) )));

	t.put("cmle.b (zero)",              both(b, op( g->cmle(d->v.b, s->v.b, 0) )));
	t.put("cmle.h (zero)",              both(b, op( g->cmle(d->v.h, s->v.h, 0) )));
	t.put("cmle.s (zero)",              both(b, op( g->cmle(d->v.s, s->v.s, 0) )));
	t.put("cmle.d (zero)",              both(b, op( g->cmle(d->v.d, s->v.d, 0) )));

	t.put("cmlt.b (zero)",              both(b, op( g->cmlt(d->v.b, s->v.b, 0) )));
	t.put("cmlt.h (zero)",              both(b, op( g->cmlt(d->v.h, s->v.h, 0) )));
	t.put("cmlt.s (zero)",              both(b, op( g->cmlt(d->v.s, s->v.s, 0) )));
	t.put("cmlt.d (zero)",              both(b, op( g->cmlt(d->v.d, s->v.d, 0) )));

	t.put("cmhi.b (reg)",               both(b, op( g->cmhi(d->v.b, d->v.b, s->v.b) )));
	t.put("cmhi.h (reg)",               both(b, op( g->cmhi(d->v.h, d->v.h, s->v.h) )));
	t.put("cmhi.s (reg)",               both(b, op( g->cmhi(d->v.s, d->v.s, s->v.s) )));
	t.put("cmhi.d (reg)",               both(b, op( g->cmhi(d->v.d, d->v.d, s->v.d) )));

	t.put("cmhs.b (reg)",               both(b, op( g->cmhs(d->v.b, d->v.b, s->v.b) )));
	t.put("cmhs.h (reg)",               both(b, op( g->cmhs(d->v.h, d->v.h, s->v.h) )));
	t.put("cmhs.s (reg)",               both(b, op( g->cmhs(d->v.s, d->v.s, s->v.s) )));
	t.put("cmhs.d (reg)",               both(b, op( g->cmhs(d->v.d, d->v.d, s->v.d) )));

	t.put("cmtst.b (reg)",              both(b, op( g->cmtst(d->v.b, d->v.b, s->v.b) )));
	t.put("cmtst.h (reg)",              both(b, op( g->cmtst(d->v.h, d->v.h, s->v.h) )));
	t.put("cmtst.s (reg)",              both(b, op( g->cmtst(d->v.s, d->v.s, s->v.s) )));
	t.put("cmtst.d (reg)",              both(b, op( g->cmtst(d->v.d, d->v.d, s->v.d) )));

	t.put("cnt.b (reg)",                both(b, op( g->cnt(d->v.b, s->v.b) )));

	t.put("dup.b (vec; lane = 0)",      both(b, op( g->dup(d->v.b, s->v.b[0]) )));
	t.put("dup.b (vec; lane = 15)",     both(b, op( g->dup(d->v.b, s->v.b[15]) )));
	t.put("dup.h (vec; lane = 7)",      both(b, op( g->dup(d->v.h, s->v.h[7]) )));
	t.put("dup.s (vec; lane = 3)",      both(b, op( g->dup(d->v.s, s->v.s[3]) )));
	t.put("dup.d (vec; lane = 1)",      both(b, op( g->dup(d->v.d, s->v.d[1]) )));

	// t.put("dup.b (elem)",               both(b, op( g->dup(d->v.b, s->v.b[0]) )));
	// t.put("dup.b (elem)",               both(b, op( g->dup(d->v.b, s->v.b[15]) )));
	// t.put("dup.h (elem)",               both(b, op( g->dup(d->v.h, s->v.h[7]) )));
	t.put("dup.b (elem)",               both(b, op( g->dup(d->v.b, s->w) ),                         op( g->mov(d->w, d->v.s[0]) ), mov_latency));
	t.put("dup.h (elem)",               both(b, op( g->dup(d->v.h, s->w) ),                         op( g->mov(d->w, d->v.s[0]) ), mov_latency));
	t.put("dup.s (elem)",               both(b, op( g->dup(d->v.s, s->w) ),                         op( g->mov(d->w, d->v.s[0]) ), mov_latency));
	t.put("dup.d (elem)",               both(b, op( g->dup(d->v.d, s->x) ),                         op( g->mov(d->x, d->v.d[0]) ), mov_latency));

	t.put("eor3.b",                     both(b, op( g->eor3(d->v.b, d->v.b, d->v.b, s->v.b) )));
	t.put("eor.b",                      both(b, op( g->eor(d->v.b, d->v.b, s->v.b) )));
	t.put("ext.b (>>1)",                both(b, op( g->ext(d->v.b, d->v.b, s->v.b, 1) )));
	t.put("ext.b (>>15)",               both(b, op( g->ext(d->v.b, d->v.b, s->v.b, 1) )));

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

	t.put("facge.h (scl)",              both(b, op( g->facge(d->h, d->h, s->h) )));
	t.put("facge.s (scl)",              both(b, op( g->facge(d->s, d->s, s->s) )));
	t.put("facge.d (scl)",              both(b, op( g->facge(d->d, d->d, s->d) )));
	t.put("facge.h (vec)",              both(b, op( g->facge(d->v.h, d->v.h, s->v.h) )));
	t.put("facge.s (vec)",              both(b, op( g->facge(d->v.s, d->v.s, s->v.s) )));
	t.put("facge.d (vec)",              both(b, op( g->facge(d->v.d, d->v.d, s->v.d) )));

	t.put("facgt.h (scl)",              both(b, op( g->facgt(d->h, d->h, s->h) )));
	t.put("facgt.s (scl)",              both(b, op( g->facgt(d->s, d->s, s->s) )));
	t.put("facgt.d (scl)",              both(b, op( g->facgt(d->d, d->d, s->d) )));
	t.put("facgt.h (vec)",              both(b, op( g->facgt(d->v.h, d->v.h, s->v.h) )));
	t.put("facgt.s (vec)",              both(b, op( g->facgt(d->v.s, d->v.s, s->v.s) )));
	t.put("facgt.d (vec)",              both(b, op( g->facgt(d->v.d, d->v.d, s->v.d) )));

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

	/* FIXME */
	t.put("fcmp.h (reg)",               both(b, op( g->fcmp(s->h, s->h) )));
	t.put("fcmp.h (zero)",              both(b, op( g->fcmp(s->h, 0.0) )));
	t.put("fcmp.s (reg)",               both(b, op( g->fcmp(s->s, s->s) )));
	t.put("fcmp.s (zero)",              both(b, op( g->fcmp(s->s, 0.0) )));
	t.put("fcmp.d (reg)",               both(b, op( g->fcmp(s->d, s->d) )));
	t.put("fcmp.d (zero)",              both(b, op( g->fcmp(s->d, 0.0) )));
	t.put("fcmpe.h (reg)",              both(b, op( g->fcmpe(s->h, s->h) )));
	t.put("fcmpe.h (zero)",             both(b, op( g->fcmpe(s->h, 0.0) )));
	t.put("fcmpe.s (reg)",              both(b, op( g->fcmpe(s->s, s->s) )));
	t.put("fcmpe.s (zero)",             both(b, op( g->fcmpe(s->s, 0.0) )));
	t.put("fcmpe.d (reg)",              both(b, op( g->fcmpe(s->d, s->d) )));
	t.put("fcmpe.d (zero)",             both(b, op( g->fcmpe(s->d, 0.0) )));
	t.put("fccmp.h (eq)",               both(b, op( g->fccmp(s->h, s->h, 0x2, Cond::EQ) )));
	t.put("fccmp.h (le)",               both(b, op( g->fccmp(s->h, s->h, 0x2, Cond::LE) )));
	t.put("fccmp.s (eq)",               both(b, op( g->fccmp(s->s, s->s, 0x2, Cond::EQ) )));
	t.put("fccmp.s (le)",               both(b, op( g->fccmp(s->s, s->s, 0x2, Cond::LE) )));
	t.put("fccmp.d (eq)",               both(b, op( g->fccmp(s->d, s->d, 0x2, Cond::EQ) )));
	t.put("fccmp.d (le)",               both(b, op( g->fccmp(s->d, s->d, 0x2, Cond::LE) )));
	t.put("fccmpe.h (eq)",              both(b, op( g->fccmpe(s->h, s->h, 0x2, Cond::EQ) )));
	t.put("fccmpe.h (le)",              both(b, op( g->fccmpe(s->h, s->h, 0x2, Cond::LE) )));
	t.put("fccmpe.s (eq)",              both(b, op( g->fccmpe(s->s, s->s, 0x2, Cond::EQ) )));
	t.put("fccmpe.s (le)",              both(b, op( g->fccmpe(s->s, s->s, 0x2, Cond::LE) )));
	t.put("fccmpe.d (eq)",              both(b, op( g->fccmpe(s->d, s->d, 0x2, Cond::EQ) )));
	t.put("fccmpe.d (le)",              both(b, op( g->fccmpe(s->d, s->d, 0x2, Cond::LE) )));

	t.put("fcmeq.h (scl)",              both(b, op( g->fcmeq(d->h, d->h, s->h) )));
	t.put("fcmeq.s (scl)",              both(b, op( g->fcmeq(d->s, d->s, s->s) )));
	t.put("fcmeq.d (scl)",              both(b, op( g->fcmeq(d->d, d->d, s->d) )));
	t.put("fcmeq.h (vec)",              both(b, op( g->fcmeq(d->v.h, d->v.h, s->v.h) )));
	t.put("fcmeq.s (vec)",              both(b, op( g->fcmeq(d->v.s, d->v.s, s->v.s) )));
	t.put("fcmeq.d (vec)",              both(b, op( g->fcmeq(d->v.d, d->v.d, s->v.d) )));

	t.put("fcmeq.h (scl; zero)",        both(b, op( g->fcmeq(d->h, s->h, 0.0) )));
	t.put("fcmeq.s (scl; zero)",        both(b, op( g->fcmeq(d->s, s->s, 0.0) )));
	t.put("fcmeq.d (scl; zero)",        both(b, op( g->fcmeq(d->d, s->d, 0.0) )));
	t.put("fcmeq.h (vec; zero)",        both(b, op( g->fcmeq(d->v.h, s->v.h, 0.0) )));
	t.put("fcmeq.s (vec; zero)",        both(b, op( g->fcmeq(d->v.s, s->v.s, 0.0) )));
	t.put("fcmeq.d (vec; zero)",        both(b, op( g->fcmeq(d->v.d, s->v.d, 0.0) )));

	t.put("fcmge.h (scl)",              both(b, op( g->fcmge(d->h, d->h, s->h) )));
	t.put("fcmge.s (scl)",              both(b, op( g->fcmge(d->s, d->s, s->s) )));
	t.put("fcmge.d (scl)",              both(b, op( g->fcmge(d->d, d->d, s->d) )));
	t.put("fcmge.h (vec)",              both(b, op( g->fcmge(d->v.h, d->v.h, s->v.h) )));
	t.put("fcmge.s (vec)",              both(b, op( g->fcmge(d->v.s, d->v.s, s->v.s) )));
	t.put("fcmge.d (vec)",              both(b, op( g->fcmge(d->v.d, d->v.d, s->v.d) )));

	t.put("fcmge.h (scl; zero)",        both(b, op( g->fcmge(d->h, s->h, 0.0) )));
	t.put("fcmge.s (scl; zero)",        both(b, op( g->fcmge(d->s, s->s, 0.0) )));
	t.put("fcmge.d (scl; zero)",        both(b, op( g->fcmge(d->d, s->d, 0.0) )));
	t.put("fcmge.h (vec; zero)",        both(b, op( g->fcmge(d->v.h, s->v.h, 0.0) )));
	t.put("fcmge.s (vec; zero)",        both(b, op( g->fcmge(d->v.s, s->v.s, 0.0) )));
	t.put("fcmge.d (vec; zero)",        both(b, op( g->fcmge(d->v.d, s->v.d, 0.0) )));

	t.put("fcmgt.h (scl)",              both(b, op( g->fcmgt(d->h, d->h, s->h) )));
	t.put("fcmgt.s (scl)",              both(b, op( g->fcmgt(d->s, d->s, s->s) )));
	t.put("fcmgt.d (scl)",              both(b, op( g->fcmgt(d->d, d->d, s->d) )));
	t.put("fcmgt.h (vec)",              both(b, op( g->fcmgt(d->v.h, d->v.h, s->v.h) )));
	t.put("fcmgt.s (vec)",              both(b, op( g->fcmgt(d->v.s, d->v.s, s->v.s) )));
	t.put("fcmgt.d (vec)",              both(b, op( g->fcmgt(d->v.d, d->v.d, s->v.d) )));

	t.put("fcmgt.h (scl; zero)",        both(b, op( g->fcmgt(d->h, s->h, 0.0) )));
	t.put("fcmgt.s (scl; zero)",        both(b, op( g->fcmgt(d->s, s->s, 0.0) )));
	t.put("fcmgt.d (scl; zero)",        both(b, op( g->fcmgt(d->d, s->d, 0.0) )));
	t.put("fcmgt.h (vec; zero)",        both(b, op( g->fcmgt(d->v.h, s->v.h, 0.0) )));
	t.put("fcmgt.s (vec; zero)",        both(b, op( g->fcmgt(d->v.s, s->v.s, 0.0) )));
	t.put("fcmgt.d (vec; zero)",        both(b, op( g->fcmgt(d->v.d, s->v.d, 0.0) )));

	t.put("fcmle.h (scl; zero)",        both(b, op( g->fcmle(d->h, s->h, 0.0) )));
	t.put("fcmle.s (scl; zero)",        both(b, op( g->fcmle(d->s, s->s, 0.0) )));
	t.put("fcmle.d (scl; zero)",        both(b, op( g->fcmle(d->d, s->d, 0.0) )));
	t.put("fcmle.h (vec; zero)",        both(b, op( g->fcmle(d->v.h, s->v.h, 0.0) )));
	t.put("fcmle.s (vec; zero)",        both(b, op( g->fcmle(d->v.s, s->v.s, 0.0) )));
	t.put("fcmle.d (vec; zero)",        both(b, op( g->fcmle(d->v.d, s->v.d, 0.0) )));

	t.put("fcmlt.h (scl; zero)",        both(b, op( g->fcmlt(d->h, s->h, 0.0) )));
	t.put("fcmlt.s (scl; zero)",        both(b, op( g->fcmlt(d->s, s->s, 0.0) )));
	t.put("fcmlt.d (scl; zero)",        both(b, op( g->fcmlt(d->d, s->d, 0.0) )));
	t.put("fcmlt.h (vec; zero)",        both(b, op( g->fcmlt(d->v.h, s->v.h, 0.0) )));
	t.put("fcmlt.s (vec; zero)",        both(b, op( g->fcmlt(d->v.s, s->v.s, 0.0) )));
	t.put("fcmlt.d (vec; zero)",        both(b, op( g->fcmlt(d->v.d, s->v.d, 0.0) )));

	/* select */
	t.put("fcsel.h (eq)",               both(b, op( g->fcsel(d->h, d->h, s->h, Cond::EQ) )));
	t.put("fcsel.h (le)",               both(b, op( g->fcsel(d->h, d->h, s->h, Cond::LE) )));
	t.put("fcsel.s (eq)",               both(b, op( g->fcsel(d->s, d->s, s->s, Cond::EQ) )));
	t.put("fcsel.s (le)",               both(b, op( g->fcsel(d->s, d->s, s->s, Cond::LE) )));
	t.put("fcsel.d (eq)",               both(b, op( g->fcsel(d->d, d->d, s->d, Cond::EQ) )));
	t.put("fcsel.d (le)",               both(b, op( g->fcsel(d->d, d->d, s->d, Cond::LE) )));

	/* convert */
	t.put("fcvt (h -> s)",              both(b, op( g->fcvt(d->s, s->h) )));
	t.put("fcvt (h -> d)",              both(b, op( g->fcvt(d->d, s->h) )));
	t.put("fcvt (s -> h)",              both(b, op( g->fcvt(d->h, s->s) )));
	t.put("fcvt (s -> d)",              both(b, op( g->fcvt(d->d, s->s) )));
	t.put("fcvt (d -> h)",              both(b, op( g->fcvt(d->h, s->d) )));
	t.put("fcvt (d -> s)",              both(b, op( g->fcvt(d->s, s->d) )));

	t.put("fcvtl (h -> s)",             both(b, op( g->fcvtl(d->v.s, s->v.h4) )));
	t.put("fcvtl (s -> d)",             both(b, op( g->fcvtl(d->v.d, s->v.s2) )));
	t.put("fcvtl2 (h -> s)",            both(b, op( g->fcvtl2(d->v.s, s->v.h) )));
	t.put("fcvtl2 (s -> d)",            both(b, op( g->fcvtl2(d->v.d, s->v.s) )));
	t.put("fcvtxn",                     both(b, op( g->fcvtxn(d->v.s, s->v.d) )));
	t.put("fcvtxn2",                    both(b, op( g->fcvtxn2(d->v.s, s->v.d) )));
	t.put("fjcvtzs",                    both(b, op( g->fjcvtzs(d->w, s->d) ),                       op( g->mov(d->v.s[0], d->w) ), mov_latency));

	t.put("fcvtas.h (scl)",             both(b, op( g->fcvtas(d->h, s->h) )));
	t.put("fcvtas.s (scl)",             both(b, op( g->fcvtas(d->s, s->s) )));
	t.put("fcvtas.d (scl)",             both(b, op( g->fcvtas(d->d, s->d) )));
	t.put("fcvtas.h (vec)",             both(b, op( g->fcvtas(d->v.h, s->v.h) )));
	t.put("fcvtas.s (vec)",             both(b, op( g->fcvtas(d->v.s, s->v.s) )));
	t.put("fcvtas.d (vec)",             both(b, op( g->fcvtas(d->v.d, s->v.d) )));
	t.put("fcvtas.h (scl -> reg)",      both(b, op( g->fcvtas(d->x, s->h) ),                        op( g->mov(d->v.d[0], d->x) ), mov_latency ));
	t.put("fcvtas.s (scl -> reg)",      both(b, op( g->fcvtas(d->x, s->s) ),                        op( g->mov(d->v.d[0], d->x) ), mov_latency ));
	t.put("fcvtas.d (scl -> reg)",      both(b, op( g->fcvtas(d->x, s->d) ),                        op( g->mov(d->v.d[0], d->x) ), mov_latency ));

	t.put("fcvtau.h (scl)",             both(b, op( g->fcvtau(d->h, s->h) )));
	t.put("fcvtau.s (scl)",             both(b, op( g->fcvtau(d->s, s->s) )));
	t.put("fcvtau.d (scl)",             both(b, op( g->fcvtau(d->d, s->d) )));
	t.put("fcvtau.h (vec)",             both(b, op( g->fcvtau(d->v.h, s->v.h) )));
	t.put("fcvtau.s (vec)",             both(b, op( g->fcvtau(d->v.s, s->v.s) )));
	t.put("fcvtau.d (vec)",             both(b, op( g->fcvtau(d->v.d, s->v.d) )));
	t.put("fcvtau.h (scl -> reg)",      both(b, op( g->fcvtau(d->x, s->h) ),                        op( g->mov(d->v.d[0], d->x) ), mov_latency ));
	t.put("fcvtau.s (scl -> reg)",      both(b, op( g->fcvtau(d->x, s->s) ),                        op( g->mov(d->v.d[0], d->x) ), mov_latency ));
	t.put("fcvtau.d (scl -> reg)",      both(b, op( g->fcvtau(d->x, s->d) ),                        op( g->mov(d->v.d[0], d->x) ), mov_latency ));

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

	t.put("fneg.h (scl)",               both(b, op( g->fneg(d->h, s->h) )));
	t.put("fneg.s (scl)",               both(b, op( g->fneg(d->s, s->s) )));
	t.put("fneg.d (scl)",               both(b, op( g->fneg(d->d, s->d) )));
	t.put("fneg.h (vec)",               both(b, op( g->fneg(d->v.h, s->v.h) )));
	t.put("fneg.s (vec)",               both(b, op( g->fneg(d->v.s, s->v.s) )));
	t.put("fneg.d (vec)",               both(b, op( g->fneg(d->v.d, s->v.d) )));

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

	// t.put("frint32x.s (scl)",           both(b, op( g->frint32x(d->v.s, s->v.s) )));
	// t.put("frint32x.d (scl)",           both(b, op( g->frint32x(d->v.d, s->v.d) )));
	t.put("frinta.h (scl)",             both(b, op( g->frinta(d->h, s->h) )));
	t.put("frinta.s (scl)",             both(b, op( g->frinta(d->s, s->s) )));
	t.put("frinta.d (scl)",             both(b, op( g->frinta(d->d, s->d) )));
	t.put("frinta.h (vec)",             both(b, op( g->frinta(d->v.h, s->v.h) )));
	t.put("frinta.s (vec)",             both(b, op( g->frinta(d->v.s, s->v.s) )));
	t.put("frinta.d (vec)",             both(b, op( g->frinta(d->v.d, s->v.d) )));

	t.put("frinti.h (scl)",             both(b, op( g->frinti(d->h, s->h) )));
	t.put("frinti.s (scl)",             both(b, op( g->frinti(d->s, s->s) )));
	t.put("frinti.d (scl)",             both(b, op( g->frinti(d->d, s->d) )));
	t.put("frinti.h (vec)",             both(b, op( g->frinti(d->v.h, s->v.h) )));
	t.put("frinti.s (vec)",             both(b, op( g->frinti(d->v.s, s->v.s) )));
	t.put("frinti.d (vec)",             both(b, op( g->frinti(d->v.d, s->v.d) )));

	t.put("frintm.h (scl)",             both(b, op( g->frintm(d->h, s->h) )));
	t.put("frintm.s (scl)",             both(b, op( g->frintm(d->s, s->s) )));
	t.put("frintm.d (scl)",             both(b, op( g->frintm(d->d, s->d) )));
	t.put("frintm.h (vec)",             both(b, op( g->frintm(d->v.h, s->v.h) )));
	t.put("frintm.s (vec)",             both(b, op( g->frintm(d->v.s, s->v.s) )));
	t.put("frintm.d (vec)",             both(b, op( g->frintm(d->v.d, s->v.d) )));

	t.put("frintn.h (scl)",             both(b, op( g->frintn(d->h, s->h) )));
	t.put("frintn.s (scl)",             both(b, op( g->frintn(d->s, s->s) )));
	t.put("frintn.d (scl)",             both(b, op( g->frintn(d->d, s->d) )));
	t.put("frintn.h (vec)",             both(b, op( g->frintn(d->v.h, s->v.h) )));
	t.put("frintn.s (vec)",             both(b, op( g->frintn(d->v.s, s->v.s) )));
	t.put("frintn.d (vec)",             both(b, op( g->frintn(d->v.d, s->v.d) )));

	t.put("frintp.h (scl)",             both(b, op( g->frintp(d->h, s->h) )));
	t.put("frintp.s (scl)",             both(b, op( g->frintp(d->s, s->s) )));
	t.put("frintp.d (scl)",             both(b, op( g->frintp(d->d, s->d) )));
	t.put("frintp.h (vec)",             both(b, op( g->frintp(d->v.h, s->v.h) )));
	t.put("frintp.s (vec)",             both(b, op( g->frintp(d->v.s, s->v.s) )));
	t.put("frintp.d (vec)",             both(b, op( g->frintp(d->v.d, s->v.d) )));

	t.put("frintx.h (scl)",             both(b, op( g->frintx(d->h, s->h) )));
	t.put("frintx.s (scl)",             both(b, op( g->frintx(d->s, s->s) )));
	t.put("frintx.d (scl)",             both(b, op( g->frintx(d->d, s->d) )));
	t.put("frintx.h (vec)",             both(b, op( g->frintx(d->v.h, s->v.h) )));
	t.put("frintx.s (vec)",             both(b, op( g->frintx(d->v.s, s->v.s) )));
	t.put("frintx.d (vec)",             both(b, op( g->frintx(d->v.d, s->v.d) )));

	t.put("frintz.h (scl)",             both(b, op( g->frintz(d->h, s->h) )));
	t.put("frintz.s (scl)",             both(b, op( g->frintz(d->s, s->s) )));
	t.put("frintz.d (scl)",             both(b, op( g->frintz(d->d, s->d) )));
	t.put("frintz.h (vec)",             both(b, op( g->frintz(d->v.h, s->v.h) )));
	t.put("frintz.s (vec)",             both(b, op( g->frintz(d->v.s, s->v.s) )));
	t.put("frintz.d (vec)",             both(b, op( g->frintz(d->v.d, s->v.d) )));

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

	t.put("mla.b (reg)",                both(b, op( g->mla(d->v.b, d->v.b, s->v.b) )));
	t.put("mla.h (reg)",                both(b, op( g->mla(d->v.h, d->v.h, s->v.h) )));
	t.put("mla.s (reg)",                both(b, op( g->mla(d->v.s, d->v.s, s->v.s) )));

	t.put("mul.b (reg)",                both(b, op( g->mul(d->v.b, d->v.b, s->v.b) )));
	t.put("mul.h (reg)",                both(b, op( g->mul(d->v.h, d->v.h, s->v.h) )));
	t.put("mul.s (reg)",                both(b, op( g->mul(d->v.s, d->v.s, s->v.s) )));

	t.put("pmul.b",                     both(b, op( g->pmul(d->v.b, d->v.b, s->v.b) )));
	t.put("pmull.b",                    both(b, op( g->pmull(d->v.h, d->v.b8, s->v.b8) )));
	t.put("pmull.d",                    both(b, op( g->pmull(d->v.q, d->v.d1, s->v.d1) )));
	t.put("pmull2.b",                   both(b, op( g->pmull2(d->v.h, d->v.b, s->v.b) )));
	t.put("pmull2.d",                   both(b, op( g->pmull2(d->v.q, d->v.d, s->v.d) )));

	t.put("raddhn.h",                   both(b, op( g->raddhn(d->v.b8, d->v.h, s->v.h) )));
	t.put("raddhn.s",                   both(b, op( g->raddhn(d->v.h4, d->v.s, s->v.s) )));
	t.put("raddhn.d",                   both(b, op( g->raddhn(d->v.s2, d->v.d, s->v.d) )));
	t.put("raddhn2.h",                  both(b, op( g->raddhn2(d->v.b, d->v.h, s->v.h) )));
	t.put("raddhn2.s",                  both(b, op( g->raddhn2(d->v.h, d->v.s, s->v.s) )));
	t.put("raddhn2.d",                  both(b, op( g->raddhn2(d->v.s, d->v.d, s->v.d) )));

	t.put("mvn.b",                      both(b, op( g->mvn(d->v.b, s->v.b) )));
	t.put("mvni.h",                     both(b, op( g->mvni(d->v.h, 0x11, ShMod::LSL, 0) )));
	t.put("mvni.s",                     both(b, op( g->mvni(d->v.s, 0x11, ShMod::LSL, 0) )));
	t.put("mvni.h",                     both(b, op( g->mvni(d->v.h, 0x11, ShMod::LSL, 8) )));
	t.put("mvni.s",                     both(b, op( g->mvni(d->v.s, 0x11, ShMod::LSL, 8) )));

	t.put("not.b",                      both(b, op( g->not_(d->v.b, s->v.b) )));

	t.put("neg.b",                      both(b, op( g->neg(d->v.b, s->v.b) )));
	t.put("neg.h",                      both(b, op( g->neg(d->v.h, s->v.h) )));
	t.put("neg.s",                      both(b, op( g->neg(d->v.s, s->v.s) )));
	t.put("neg.d",                      both(b, op( g->neg(d->v.d, s->v.d) )));

	t.put("orn.b",                      both(b, op( g->orn(d->v.b, d->v.b, s->v.b) )));

	t.put("orr.b",                      both(b, op( g->orr(d->v.b, d->v.b, s->v.b) )));
	t.put("orr.h",                      both(b, op( g->orr(d->v.h, 0x11, ShMod::LSL, 0) )));
	t.put("orr.s",                      both(b, op( g->orr(d->v.s, 0x11, ShMod::LSL, 0) )));
	t.put("orr.h",                      both(b, op( g->orr(d->v.h, 0x11, ShMod::LSL, 8) )));
	t.put("orr.s",                      both(b, op( g->orr(d->v.s, 0x11, ShMod::LSL, 8) )));

	t.put("rax1.d",                     both(b, op( g->rax1(d->v.d, d->v.d, s->v.d) )));

	t.put("rbit.b",                     both(b, op( g->rbit(d->v.b, s->v.b) )));
	t.put("rev16.b",                    both(b, op( g->rev16(d->v.b, s->v.b) )));
	t.put("rev32.b",                    both(b, op( g->rev32(d->v.b, s->v.b) )));
	t.put("rev32.h",                    both(b, op( g->rev32(d->v.h, s->v.h) )));
	t.put("rev64.b",                    both(b, op( g->rev64(d->v.b, s->v.b) )));
	t.put("rev64.h",                    both(b, op( g->rev64(d->v.h, s->v.h) )));
	t.put("rev64.s",                    both(b, op( g->rev64(d->v.s, s->v.s) )));

	t.put("rshrn.h (>>2)",              both(b, op( g->rshrn(d->v.b8, s->v.h, 2) )));
	t.put("rshrn.s (>>2)",              both(b, op( g->rshrn(d->v.h4, s->v.s, 2) )));
	t.put("rshrn.d (>>2)",              both(b, op( g->rshrn(d->v.s2, s->v.d, 2) )));
	t.put("rshrn2.h (>>2)",             both(b, op( g->rshrn2(d->v.b, s->v.h, 2) )));
	t.put("rshrn2.s (>>2)",             both(b, op( g->rshrn2(d->v.h, s->v.s, 2) )));
	t.put("rshrn2.d (>>2)",             both(b, op( g->rshrn2(d->v.s, s->v.d, 2) )));

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

	t.put("scvtf.h (vec; >>2)",         both(b, op( g->scvtf(d->v.h, s->v.h, 2) )));
	t.put("scvtf.s (vec; >>2)",         both(b, op( g->scvtf(d->v.s, s->v.s, 2) )));
	t.put("scvtf.d (vec; >>2)",         both(b, op( g->scvtf(d->v.d, s->v.d, 2) )));
	t.put("scvtf2.h (vec; int)",        both(b, op( g->scvtf(d->v.h, s->v.h) )));
	t.put("scvtf2.s (vec; int)",        both(b, op( g->scvtf(d->v.s, s->v.s) )));
	t.put("scvtf2.d (vec; int)",        both(b, op( g->scvtf(d->v.d, s->v.d) )));

	t.put("scvtf.h (scl; >>2)",         both(b, op( g->scvtf(d->h, s->w, 2) ), op( g->mov(d->w, d->v.s[0]) ), mov_latency));
	t.put("scvtf.s (scl; >>2)",         both(b, op( g->scvtf(d->s, s->w, 2) ), op( g->mov(d->w, d->v.s[0]) ), mov_latency));
	t.put("scvtf.d (scl; >>2)",         both(b, op( g->scvtf(d->d, s->x, 2) ), op( g->mov(d->x, d->v.d[0]) ), mov_latency));
	t.put("scvtf2.h (scl; int)",        both(b, op( g->scvtf(d->h, s->w) ), op( g->mov(d->w, d->v.s[0]) ), mov_latency));
	t.put("scvtf2.s (scl; int)",        both(b, op( g->scvtf(d->s, s->w) ), op( g->mov(d->w, d->v.s[0]) ), mov_latency));
	t.put("scvtf2.d (scl; int)",        both(b, op( g->scvtf(d->d, s->x) ), op( g->mov(d->x, d->v.d[0]) ), mov_latency));

	// t.put("cmhi.b (zero)",              both(b, op( g->cmhi(d->v.b, s->v.b, 0) )));
	// t.put("cmhi.d (zero)",              both(b, op( g->cmhi(d->v.d, s->v.d, 0) )));
}


#endif	/* _ALU_H_INCLUDED */
/*
 * end of alu.h
 */
