
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
void bench_alu(double freq) {
	bench b(freq);

	double const adc_latency = b.lat(op( g->adc(xd, xs, xs) )).lat;
	double const adds_adc_latency = b.lat(op( g->adds(xs, xs, xs); g->adc(xd, xs, xs) )).lat;
	// double const fill_rate = 2.0;

	print("add (reg)",              b.both(op( g->add(xd, xs, xc) )));
	print("add (reg<<2)",           b.both(op( g->add(xd, xs, xc, ShMod::LSL, 2) )));
	print("add (reg<<17)",          b.both(op( g->add(xd, xs, xc, ShMod::LSL, 17) )));
	print("add (reg>>17)",          b.both(op( g->add(xd, xs, xc, ShMod::LSR, 17) )));
	print("add (reg>>17; signed)",  b.both(op( g->add(xd, xs, xc, ShMod::ASR, 17) )));
	print("add (imm)",              b.both(op( g->add(xd, xs, 1) )));
	print("add (imm<<12)",          b.both(op( g->add(xd, xs, 1, 12) )));

	print("adds (reg)",             b.both(op( g->adds(xd, xs, xc) ),                   op( g->adc(xd, xd, xc) ), adc_latency));
	print("adds (reg<<2)",          b.both(op( g->adds(xd, xs, xc, ShMod::LSL, 2) ),    op( g->adc(xd, xd, xc) ), adc_latency));
	print("adds (reg<<17)",         b.both(op( g->adds(xd, xs, xc, ShMod::LSL, 17) ),   op( g->adc(xd, xd, xc) ), adc_latency));
	print("adds (reg>>17)",         b.both(op( g->adds(xd, xs, xc, ShMod::LSR, 17) ),   op( g->adc(xd, xd, xc) ), adc_latency));
	print("adds (imm)",             b.both(op( g->adds(xd, xs, 1) ),                    op( g->adc(xd, xd, xc) ), adc_latency));
	print("adds (imm<<12)",         b.both(op( g->adds(xd, xs, 1, 12) ),                op( g->adc(xd, xd, xc) ), adc_latency));

	print("adc",                    b.both(op( g->adc(xd, xs, xc) )));
	print("adcs",                   b.both(op( g->adcs(xd, xs, xc) )));

	print("adr",                    b.rct(op( g->adr(xd, l) )));
	print("adrp",                   b.rct(op( g->adrp(xd, l) )));

	print("and (reg)",              b.both(op( g->and_(xd, xs, xc) )));
	print("and (reg<<2)",           b.both(op( g->and_(xd, xs, xc, ShMod::LSL, 2) )));
	print("and (reg<<17)",          b.both(op( g->and_(xd, xs, xc, ShMod::LSL, 17) )));
	print("and (reg>>17)",          b.both(op( g->and_(xd, xs, xc, ShMod::LSR, 17) )));
	print("and (reg>>17; signed)",  b.both(op( g->and_(xd, xs, xc, ShMod::ASR, 17) )));
	print("and (reg>>17; rotate)",  b.both(op( g->and_(xd, xs, xc, ShMod::ROR, 17) )));
	print("and (mask imm)",         b.both(op( g->and_(xd, xs, 0x1ffc) )));

	print("ands (reg)",             b.both(op( g->ands(xd, xs, xc) ),                   op( g->adc(xd, xd, xc) ), adc_latency));
	print("ands (reg<<2)",          b.both(op( g->ands(xd, xs, xc, ShMod::LSL, 2) ),    op( g->adc(xd, xd, xc) ), adc_latency));
	print("ands (reg<<17)",         b.both(op( g->ands(xd, xs, xc, ShMod::LSL, 17) ),   op( g->adc(xd, xd, xc) ), adc_latency));
	print("ands (reg>>17)",         b.both(op( g->ands(xd, xs, xc, ShMod::LSR, 17) ),   op( g->adc(xd, xd, xc) ), adc_latency));
	print("ands (reg>>17; signed)", b.both(op( g->ands(xd, xs, xc, ShMod::ASR, 17) ),   op( g->adc(xd, xd, xc) ), adc_latency));
	print("ands (reg>>17; rotate)", b.both(op( g->ands(xd, xs, xc, ShMod::ROR, 17) ),   op( g->adc(xd, xd, xc) ), adc_latency));
	print("ands (mask imm)",        b.both(op( g->ands(xd, xs, 0x1ffc) ),               op( g->adc(xd, xd, xc) ), adc_latency));

	print("asr (reg)",              b.both(op( g->asr(xd, xs, xc) )));
	print("asr (imm)",              b.both(op( g->asr(xd, xs, 31) )));
	print("asrv",                   b.both(op( g->asrv(xd, xs, xc) )));

	print("bfc",                    b.both(op( g->bfc(xc, 17, 2) ), op( g->bfc(xd, 17, 2) )));
	print("bfi",                    b.both(op( g->bfi(xd, xs, 17, 2) )));
	print("bfm",                    b.both(op( g->bfm(xd, xs, 17, 2) )));
	print("bfxil",                  b.both(op( g->bfxil(xd, xs, 17, 2) )));

	print("bic (reg)",              b.both(op( g->bic(xd, xs, xc) )));
	print("bic (reg<<2)",           b.both(op( g->bic(xd, xs, xc, ShMod::LSL, 2) )));
	print("bics (reg)",             b.both(op( g->bics(xd, xs, xc) ),                   op( g->adc(xd, xd, xc) ), adc_latency));
	print("bics (reg<<2)",          b.both(op( g->bics(xd, xs, xc, ShMod::LSL, 2) ),    op( g->adc(xd, xd, xc) ), adc_latency));

	print("ccmn (reg; eq)",         b.both(op( g->ccmn(xs, xs, 0x2, Cond::EQ) ),        op( g->adc(xd, xs, xc) ), adc_latency));
	print("ccmn (reg; lt)",         b.both(op( g->ccmn(xs, xs, 0x2, Cond::LT) ),        op( g->adc(xd, xs, xc) ), adc_latency));
	print("ccmn (imm; eq)",         b.both(op( g->ccmn(xs, 17, 0x2, Cond::EQ) ),        op( g->adc(xd, xs, xc) ), adc_latency));
	print("ccmn (imm; lt)",         b.both(op( g->ccmn(xs, 17, 0x2, Cond::LT) ),        op( g->adc(xd, xs, xc) ), adc_latency));

	print("ccmp (reg; eq)",         b.both(op( g->ccmp(xs, xs, 0x2, Cond::EQ) ),        op( g->adc(xd, xs, xc) ), adc_latency));
	print("ccmp (reg; lt)",         b.both(op( g->ccmp(xs, xs, 0x2, Cond::LT) ),        op( g->adc(xd, xs, xc) ), adc_latency));
	print("ccmp (imm; eq)",         b.both(op( g->ccmp(xs, 17, 0x2, Cond::EQ) ),        op( g->adc(xd, xs, xc) ), adc_latency));
	print("ccmp (imm; lt)",         b.both(op( g->ccmp(xs, 17, 0x2, Cond::LT) ),        op( g->adc(xd, xs, xc) ), adc_latency));

	print("cfinv",                  b.both(op( g->cfinv() ),                            op( g->adc(xd, xs, xc); g->adds(xd, xd, xc) ), adds_adc_latency));

	print("csinc (eq)",             b.both(op( g->csinc(xd, xs, xc, Cond::EQ) )));
	print("csinc (lt)",             b.both(op( g->csinc(xd, xs, xc, Cond::LT) )));
	print("cinc (eq)",              b.both(op( g->cinc(xd, xs, Cond::EQ) )));
	print("cinc (lt)",              b.both(op( g->cinc(xd, xs, Cond::LT) )));
	print("csinv (eq)",             b.both(op( g->csinv(xd, xs, xc, Cond::EQ) )));
	print("csinv (lt)",             b.both(op( g->csinv(xd, xs, xc, Cond::LT) )));
	print("cset (eq)",              b.rct(op( g->cset(xs, Cond::EQ) ) ));
	print("cset (lt)",              b.rct(op( g->cset(xs, Cond::LT) ) ));
	print("csetm (eq)",             b.rct(op( g->csetm(xs, Cond::EQ) ) ));
	print("csetm (lt)",             b.rct(op( g->csetm(xs, Cond::LT) ) ));
	print("cinv (eq)",              b.both(op( g->cinv(xd, xs, Cond::EQ) )));
	print("cinv (lt)",              b.both(op( g->cinv(xd, xs, Cond::LT) )));
	print("csneg (eq)",             b.both(op( g->csneg(xd, xs, xc, Cond::EQ) )));
	print("csneg (lt)",             b.both(op( g->csneg(xd, xs, xc, Cond::LT) )));
	print("cneg (eq)",              b.both(op( g->cneg(xd, xs, Cond::EQ) )));
	print("cneg (lt)",              b.both(op( g->cneg(xd, xs, Cond::LT) )));

	print("eor (reg)",              b.both(op( g->eor(xd, xs, xc) )));
	print("eor (reg<<2)",           b.both(op( g->eor(xd, xs, xc, ShMod::LSL, 2) )));
	print("eon (reg)",              b.both(op( g->eon(xd, xs, xc) )));
	print("eon (reg<<2)",           b.both(op( g->eon(xd, xs, xc, ShMod::LSL, 2) )));

	print("extr (>>1)",             b.both(op( g->extr(xd, xs, xc, 1) )));
	print("extr (>>17)",            b.both(op( g->extr(xd, xs, xc, 17) )));

	print("lsl (reg) / lslv",       b.both(op( g->lsl(xd, xs, xc) )));
	print("lsl (imm)",              b.both(op( g->lsl(xd, xs, 17) )));
	print("lsr (reg) / lsrv",       b.both(op( g->lsl(xd, xs, xc) )));
	print("lsr (imm)",              b.both(op( g->lsl(xd, xs, 17) )));

	print("madd",                   b.both(op( g->madd(xd, xs, xc, xs) )));
	print("mul",                    b.both(op( g->mul(xd, xs, xc) )));
	print("msub",                   b.both(op( g->msub(xd, xs, xc, xs) )));
	print("mneg",                   b.both(op( g->mneg(xd, xs, xc) )));

	print("mov (reg)",              b.both(op( g->mov(xd, xs) )));
	print("mov / movz (imm)",       b.rct(op( g->mov(xd, 0x1ffc) )));
	print("mov / movz (imm<<16)",   b.rct(op( g->mov(xd, 0x1ffc<<16) )));
	print("mov (mask imm)",         b.rct(op( g->mov(xd, 0x1ffffffffffc) )));
	print("mov / movn (imm)",       b.rct(op( g->movn(xd, 0x1ffc) )));
	print("mov / movn (imm<<16)",   b.rct(op( g->movn(xd, 0x1ffc, 16) )));
	print("movk",                   b.rct(op( g->movk(xd, 0x1ffc) )));
	print("movk (<<16)",            b.rct(op( g->movk(xd, 0x1ffc, 16) )));

	print("mvn (reg)",              b.both(op( g->mvn(xd, xs) )));
	print("mvn (reg<<2)",           b.both(op( g->mvn(xd, xs, ShMod::LSL, 2) )));

	print("neg (reg)",              b.both(op( g->neg(xd, xs) )));
	print("neg (reg<<2)",           b.both(op( g->neg(xd, xs, ShMod::LSL, 2) )));

	print("negs (reg)",             b.both(op( g->negs(xd, xs) ),                       op( g->adc(xd, xd, xc) ), adc_latency));
	print("negs (reg<<2)",          b.both(op( g->negs(xd, xs, ShMod::LSL, 2) ),        op( g->adc(xd, xd, xc) ), adc_latency));


	print("sub",                    b.both(op( g->sub(xd, xs, xc) )));
	print("lsl",                    b.both(op( g->lsl(xd, xs, 17) )));
	print("lsr",                    b.both(op( g->lsr(xd, xs, 17) )));
	print("ror",                    b.both(op( g->ror(xd, xs, 17) )));
	print("extr",                   b.both(op( g->extr(xd, xs, xc, 17) )));
	print("uxtb",                   b.both(op( g->uxtb(xd, xs) )));
	print("uxtb",                   b.both(op( g->uxtb(xd, xs) )));
	print("udiv",                   b.both(op( g->udiv(xd, xs, xc) )));
	print("sdiv",                   b.both(op( g->sdiv(xd, xs, xc) )));
	print("crc32x",                 b.both(op( g->crc32x(wd, ws, xc) )));
	print("crc32cx",                b.both(op( g->crc32cx(wd, ws, xc) )));

}


#endif	/* _ALU_H_INCLUDED */
/*
 * end of alu.h
 */
