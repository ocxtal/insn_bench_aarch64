
/**
 * @file logic_vec.cc
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>

static
void bench_bitwise_logic_vec(bool md, double freq) {
	table t(md, "Vector bitwise logic instructions");
	bench b(freq);

	t.put("and.b",                      both(b, op( g->and_(d->v.b, d->v.b, s->v.b) )));

	t.put("orr.b",                      both(b, op( g->orr(d->v.b, d->v.b, s->v.b) )));
	t.put("orr.h",                      both(b, op( g->orr(d->v.h, 0x11, ShMod::LSL, 0) )));
	t.put("orr.s",                      both(b, op( g->orr(d->v.s, 0x11, ShMod::LSL, 0) )));
	t.put("orr.h",                      both(b, op( g->orr(d->v.h, 0x11, ShMod::LSL, 8) )));
	t.put("orr.s",                      both(b, op( g->orr(d->v.s, 0x11, ShMod::LSL, 8) )));
	t.put("orn.b",                      both(b, op( g->orn(d->v.b, d->v.b, s->v.b) )));

	t.put("eor.b",                      both(b, op( g->eor(d->v.b, d->v.b, s->v.b) )));
	t.put("eor3.b",                     both(b, op( g->eor3(d->v.b, d->v.b, d->v.b, s->v.b) )));

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
	table t(md, "Vector bit-manipulation instructions");
	bench b(freq);

	/* bitwise blend */
	t.put("bic.b (reg)",                both(b, op( g->bic(d->v.b, d->v.b, s->v.b) )));
	t.put("bic.h (imm)",                both(b, op( g->bic(d->v.h8, 0x11, ShMod::LSL, 0) )));
	t.put("bic.h (imm; <<8)",           both(b, op( g->bic(d->v.h8, 0x11, ShMod::LSL, 8) )));
	t.put("bic.s (imm)",                both(b, op( g->bic(d->v.s4, 0x11, ShMod::LSL, 0) )));
	t.put("bic.s (imm; <<8)",           both(b, op( g->bic(d->v.s4, 0x11, ShMod::LSL, 8) )));
	t.put("bif.b",                      both(b, op( g->bif(d->v.b, d->v.b, s->v.b) )));
	t.put("bit.b",                      both(b, op( g->bit(d->v.b, d->v.b, s->v.b) )));
	t.put("bsl.b",                      both(b, op( g->bsl(d->v.b, d->v.b, s->v.b) )));

	/* complex logic */	
	t.put("bcax.b",                     both(b, op( g->bcax(d->v.b, d->v.b, d->v.b, s->v.b) )));
	t.put("rax1.d",                     both(b, op( g->rax1(d->v.d, d->v.d, s->v.d) )));
	t.put("xar.d",                      both(b, op( g->xar(d->v.d, d->v.d, s->v.d, 0x11) )));

	/* misc */
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

static
void bench_shift_vec(bool md, double freq) {
	table t(md, "Vector shift instructions");
	bench b(freq);

	/* shift left */
	t.put("shl.b (imm; <<2)",           both(b, op( g->shl(d->v.b, s->v.b, 2) )));
	t.put("shl.h (imm; <<2)",           both(b, op( g->shl(d->v.h, s->v.h, 2) )));
	t.put("shl.s (imm; <<2)",           both(b, op( g->shl(d->v.s, s->v.s, 2) )));
	t.put("shl.d (imm; <<2)",           both(b, op( g->shl(d->v.d, s->v.d, 2) )));

	t.put("sshl.b (reg)",               both(b, op( g->sshl(d->v.b, d->v.b, s->v.b) )));
	t.put("sshl.h (reg)",               both(b, op( g->sshl(d->v.h, d->v.h, s->v.h) )));
	t.put("sshl.s (reg)",               both(b, op( g->sshl(d->v.s, d->v.s, s->v.s) )));
	t.put("sshl.d (reg)",               both(b, op( g->sshl(d->v.d, d->v.d, s->v.d) )));

	t.put("ushl.b (reg)",               both(b, op( g->ushl(d->v.b, d->v.b, s->v.b) )));
	t.put("ushl.h (reg)",               both(b, op( g->ushl(d->v.h, d->v.h, s->v.h) )));
	t.put("ushl.s (reg)",               both(b, op( g->ushl(d->v.s, d->v.s, s->v.s) )));
	t.put("ushl.d (reg)",               both(b, op( g->ushl(d->v.d, d->v.d, s->v.d) )));

	t.put("sqshl.b (imm; <<2)",         both(b, op( g->sqshl(d->v.b, s->v.b, 2) )));
	t.put("sqshl.h (imm; <<2)",         both(b, op( g->sqshl(d->v.h, s->v.h, 2) )));
	t.put("sqshl.s (imm; <<2)",         both(b, op( g->sqshl(d->v.s, s->v.s, 2) )));
	t.put("sqshl.d (imm; <<2)",         both(b, op( g->sqshl(d->v.d, s->v.d, 2) )));

	t.put("sqshl.b (reg; <<2)",         both(b, op( g->sqshl(d->v.b, s->v.b, s->v.b) )));
	t.put("sqshl.h (reg; <<2)",         both(b, op( g->sqshl(d->v.h, s->v.h, s->v.h) )));
	t.put("sqshl.s (reg; <<2)",         both(b, op( g->sqshl(d->v.s, s->v.s, s->v.s) )));
	t.put("sqshl.d (reg; <<2)",         both(b, op( g->sqshl(d->v.d, s->v.d, s->v.d) )));

	t.put("uqshl.b (imm; <<2)",         both(b, op( g->uqshl(d->v.b, s->v.b, 2) )));
	t.put("uqshl.h (imm; <<2)",         both(b, op( g->uqshl(d->v.h, s->v.h, 2) )));
	t.put("uqshl.s (imm; <<2)",         both(b, op( g->uqshl(d->v.s, s->v.s, 2) )));
	t.put("uqshl.d (imm; <<2)",         both(b, op( g->uqshl(d->v.d, s->v.d, 2) )));

	t.put("uqshl.b (reg; <<2)",         both(b, op( g->uqshl(d->v.b, s->v.b, s->v.b) )));
	t.put("uqshl.h (reg; <<2)",         both(b, op( g->uqshl(d->v.h, s->v.h, s->v.h) )));
	t.put("uqshl.s (reg; <<2)",         both(b, op( g->uqshl(d->v.s, s->v.s, s->v.s) )));
	t.put("uqshl.d (reg; <<2)",         both(b, op( g->uqshl(d->v.d, s->v.d, s->v.d) )));

	t.put("sqshlu.b (<<2)",             both(b, op( g->sqshlu(d->v.b, s->v.b, 2) )));
	t.put("sqshlu.h (<<2)",             both(b, op( g->sqshlu(d->v.h, s->v.h, 2) )));
	t.put("sqshlu.s (<<2)",             both(b, op( g->sqshlu(d->v.s, s->v.s, 2) )));
	t.put("sqshlu.d (<<2)",             both(b, op( g->sqshlu(d->v.d, s->v.d, 2) )));

	t.put("srshl.b (reg)",              both(b, op( g->srshl(d->v.b, d->v.b, s->v.b) )));
	t.put("srshl.h (reg)",              both(b, op( g->srshl(d->v.h, d->v.h, s->v.h) )));
	t.put("srshl.s (reg)",              both(b, op( g->srshl(d->v.s, d->v.s, s->v.s) )));
	t.put("srshl.d (reg)",              both(b, op( g->srshl(d->v.d, d->v.d, s->v.d) )));

	t.put("urshl.b (reg)",              both(b, op( g->urshl(d->v.b, d->v.b, s->v.b) )));
	t.put("urshl.h (reg)",              both(b, op( g->urshl(d->v.h, d->v.h, s->v.h) )));
	t.put("urshl.s (reg)",              both(b, op( g->urshl(d->v.s, d->v.s, s->v.s) )));
	t.put("urshl.d (reg)",              both(b, op( g->urshl(d->v.d, d->v.d, s->v.d) )));

	t.put("uqrshl.b (reg)",             both(b, op( g->uqrshl(d->v.b, d->v.b, s->v.b) )));
	t.put("uqrshl.h (reg)",             both(b, op( g->uqrshl(d->v.h, d->v.h, s->v.h) )));
	t.put("uqrshl.s (reg)",             both(b, op( g->uqrshl(d->v.s, d->v.s, s->v.s) )));
	t.put("uqrshl.d (reg)",             both(b, op( g->uqrshl(d->v.d, d->v.d, s->v.d) )));

	t.put("sqshlu.b (imm; <<2)",        both(b, op( g->sqshlu(d->v.b, s->v.b, 2) )));
	t.put("sqshlu.h (imm; <<2)",        both(b, op( g->sqshlu(d->v.h, s->v.h, 2) )));
	t.put("sqshlu.s (imm; <<2)",        both(b, op( g->sqshlu(d->v.s, s->v.s, 2) )));
	t.put("sqshlu.d (imm; <<2)",        both(b, op( g->sqshlu(d->v.d, s->v.d, 2) )));

	t.put("sqrshl.b",                   both(b, op( g->sqrshl(d->v.b, d->v.b, s->v.b) )));
	t.put("sqrshl.h",                   both(b, op( g->sqrshl(d->v.h, d->v.h, s->v.h) )));
	t.put("sqrshl.s",                   both(b, op( g->sqrshl(d->v.s, d->v.s, s->v.s) )));
	t.put("sqrshl.d",                   both(b, op( g->sqrshl(d->v.d, d->v.d, s->v.d) )));

	t.put("shll.b (<<8)",               both(b, op( g->shll(d->v.h, s->v.b8, 8) )));
	t.put("shll.h (<<16)",               both(b, op( g->shll(d->v.s, s->v.h4, 16) )));
	t.put("shll.s (<<32)",               both(b, op( g->shll(d->v.d, s->v.s2, 32) )));
	t.put("shll2.b (<<8)",              both(b, op( g->shll2(d->v.h, s->v.b, 8) )));
	t.put("shll2.h (<<16)",              both(b, op( g->shll2(d->v.s, s->v.h, 16) )));
	t.put("shll2.s (<<32)",              both(b, op( g->shll2(d->v.d, s->v.s, 32) )));

	t.put("sshll.b (<<2)",              both(b, op( g->sshll(d->v.h, s->v.b8, 2) )));
	t.put("sshll.h (<<2)",              both(b, op( g->sshll(d->v.s, s->v.h4, 2) )));
	t.put("sshll.s (<<2)",              both(b, op( g->sshll(d->v.d, s->v.s2, 2) )));
	t.put("sshll2.b (<<2)",             both(b, op( g->sshll2(d->v.h, s->v.b, 2) )));
	t.put("sshll2.h (<<2)",             both(b, op( g->sshll2(d->v.s, s->v.h, 2) )));
	t.put("sshll2.s (<<2)",             both(b, op( g->sshll2(d->v.d, s->v.s, 2) )));

	t.put("ushll.b (<<2)",              both(b, op( g->ushll(d->v.h, s->v.b8, 2) )));
	t.put("ushll.h (<<2)",              both(b, op( g->ushll(d->v.s, s->v.h4, 2) )));
	t.put("ushll.s (<<2)",              both(b, op( g->ushll(d->v.d, s->v.s2, 2) )));
	t.put("ushll2.b (<<2)",             both(b, op( g->ushll2(d->v.h, s->v.b, 2) )));
	t.put("ushll2.h (<<2)",             both(b, op( g->ushll2(d->v.s, s->v.h, 2) )));
	t.put("ushll2.s (<<2)",             both(b, op( g->ushll2(d->v.d, s->v.s, 2) )));

	t.put("sli.b (vec; <<2)",           both(b, op( g->sli(d->v.b, s->v.b, 2) )));
	t.put("sli.h (vec; <<2)",           both(b, op( g->sli(d->v.h, s->v.h, 2) )));
	t.put("sli.s (vec; <<2)",           both(b, op( g->sli(d->v.s, s->v.s, 2) )));
	t.put("sli.d (vec; <<2)",           both(b, op( g->sli(d->v.d, s->v.d, 2) )));

	/* shift right */
	t.put("sshr.b (imm; >>2)",          both(b, op( g->sshr(d->v.b, s->v.b, 2) )));
	t.put("sshr.h (imm; >>2)",          both(b, op( g->sshr(d->v.h, s->v.h, 2) )));
	t.put("sshr.s (imm; >>2)",          both(b, op( g->sshr(d->v.s, s->v.s, 2) )));
	t.put("sshr.d (imm; >>2)",          both(b, op( g->sshr(d->v.d, s->v.d, 2) )));

	t.put("ushr.b (imm; >>2)",          both(b, op( g->ushr(d->v.b, s->v.b, 2) )));
	t.put("ushr.h (imm; >>2)",          both(b, op( g->ushr(d->v.h, s->v.h, 2) )));
	t.put("ushr.s (imm; >>2)",          both(b, op( g->ushr(d->v.s, s->v.s, 2) )));
	t.put("ushr.d (imm; >>2)",          both(b, op( g->ushr(d->v.d, s->v.d, 2) )));

	t.put("srshr.b (imm; >>2)",         both(b, op( g->srshr(d->v.b, s->v.b, 2) )));
	t.put("srshr.h (imm; >>2)",         both(b, op( g->srshr(d->v.h, s->v.h, 2) )));
	t.put("srshr.s (imm; >>2)",         both(b, op( g->srshr(d->v.s, s->v.s, 2) )));
	t.put("srshr.d (imm; >>2)",         both(b, op( g->srshr(d->v.d, s->v.d, 2) )));

	t.put("urshr.b (imm; >>2)",         both(b, op( g->urshr(d->v.b, s->v.b, 2) )));
	t.put("urshr.h (imm; >>2)",         both(b, op( g->urshr(d->v.h, s->v.h, 2) )));
	t.put("urshr.s (imm; >>2)",         both(b, op( g->urshr(d->v.s, s->v.s, 2) )));
	t.put("urshr.d (imm; >>2)",         both(b, op( g->urshr(d->v.d, s->v.d, 2) )));

	t.put("ssra.b (imm; >>2)",          both(b, op( g->ssra(d->v.b, s->v.b, 2) )));
	t.put("ssra.h (imm; >>2)",          both(b, op( g->ssra(d->v.h, s->v.h, 2) )));
	t.put("ssra.s (imm; >>2)",          both(b, op( g->ssra(d->v.s, s->v.s, 2) )));
	t.put("ssra.d (imm; >>2)",          both(b, op( g->ssra(d->v.d, s->v.d, 2) )));

	t.put("usra.b (imm; >>2)",          both(b, op( g->usra(d->v.b, s->v.b, 2) )));
	t.put("usra.h (imm; >>2)",          both(b, op( g->usra(d->v.h, s->v.h, 2) )));
	t.put("usra.s (imm; >>2)",          both(b, op( g->usra(d->v.s, s->v.s, 2) )));
	t.put("usra.d (imm; >>2)",          both(b, op( g->usra(d->v.d, s->v.d, 2) )));

	t.put("srsra.b (imm; >>2)",         both(b, op( g->srsra(d->v.b, s->v.b, 2) )));
	t.put("srsra.h (imm; >>2)",         both(b, op( g->srsra(d->v.h, s->v.h, 2) )));
	t.put("srsra.s (imm; >>2)",         both(b, op( g->srsra(d->v.s, s->v.s, 2) )));
	t.put("srsra.d (imm; >>2)",         both(b, op( g->srsra(d->v.d, s->v.d, 2) )));

	t.put("ursra.b (imm; >>2)",         both(b, op( g->ursra(d->v.b, s->v.b, 2) )));
	t.put("ursra.h (imm; >>2)",         both(b, op( g->ursra(d->v.h, s->v.h, 2) )));
	t.put("ursra.s (imm; >>2)",         both(b, op( g->ursra(d->v.s, s->v.s, 2) )));
	t.put("ursra.d (imm; >>2)",         both(b, op( g->ursra(d->v.d, s->v.d, 2) )));

	t.put("shrn.h (>>2)",               both(b, op( g->shrn(d->v.b8, s->v.h, 2) )));
	t.put("shrn.s (>>2)",               both(b, op( g->shrn(d->v.h4, s->v.s, 2) )));
	t.put("shrn.d (>>2)",               both(b, op( g->shrn(d->v.s2, s->v.d, 2) )));
	t.put("shrn2.h (>>2)",              both(b, op( g->shrn2(d->v.b, s->v.h, 2) )));
	t.put("shrn2.s (>>2)",              both(b, op( g->shrn2(d->v.h, s->v.s, 2) )));
	t.put("shrn2.d (>>2)",              both(b, op( g->shrn2(d->v.s, s->v.d, 2) )));

	t.put("sqshrn.h (>>2)",             both(b, op( g->sqshrn(d->v.b8, s->v.h, 2) )));
	t.put("sqshrn.s (>>2)",             both(b, op( g->sqshrn(d->v.h4, s->v.s, 2) )));
	t.put("sqshrn.d (>>2)",             both(b, op( g->sqshrn(d->v.s2, s->v.d, 2) )));
	t.put("sqshrn2.h (>>2)",            both(b, op( g->sqshrn2(d->v.b, s->v.h, 2) )));
	t.put("sqshrn2.s (>>2)",            both(b, op( g->sqshrn2(d->v.h, s->v.s, 2) )));
	t.put("sqshrn2.d (>>2)",            both(b, op( g->sqshrn2(d->v.s, s->v.d, 2) )));

	t.put("uqshrn.h (>>2)",             both(b, op( g->uqshrn(d->v.b8, s->v.h, 2) )));
	t.put("uqshrn.s (>>2)",             both(b, op( g->uqshrn(d->v.h4, s->v.s, 2) )));
	t.put("uqshrn.d (>>2)",             both(b, op( g->uqshrn(d->v.s2, s->v.d, 2) )));
	t.put("uqshrn2.h (>>2)",            both(b, op( g->uqshrn2(d->v.b, s->v.h, 2) )));
	t.put("uqshrn2.s (>>2)",            both(b, op( g->uqshrn2(d->v.h, s->v.s, 2) )));
	t.put("uqshrn2.d (>>2)",            both(b, op( g->uqshrn2(d->v.s, s->v.d, 2) )));

	t.put("sqshrun.h (>>2)",            both(b, op( g->sqshrun(d->v.b8, s->v.h, 2) )));
	t.put("sqshrun.s (>>2)",            both(b, op( g->sqshrun(d->v.h4, s->v.s, 2) )));
	t.put("sqshrun.d (>>2)",            both(b, op( g->sqshrun(d->v.s2, s->v.d, 2) )));
	t.put("sqshrun2.h (>>2)",           both(b, op( g->sqshrun2(d->v.b, s->v.h, 2) )));
	t.put("sqshrun2.s (>>2)",           both(b, op( g->sqshrun2(d->v.h, s->v.s, 2) )));
	t.put("sqshrun2.d (>>2)",           both(b, op( g->sqshrun2(d->v.s, s->v.d, 2) )));

	t.put("rshrn.h (>>2)",              both(b, op( g->rshrn(d->v.b8, s->v.h, 2) )));
	t.put("rshrn.s (>>2)",              both(b, op( g->rshrn(d->v.h4, s->v.s, 2) )));
	t.put("rshrn.d (>>2)",              both(b, op( g->rshrn(d->v.s2, s->v.d, 2) )));
	t.put("rshrn2.h (>>2)",             both(b, op( g->rshrn2(d->v.b, s->v.h, 2) )));
	t.put("rshrn2.s (>>2)",             both(b, op( g->rshrn2(d->v.h, s->v.s, 2) )));
	t.put("rshrn2.d (>>2)",             both(b, op( g->rshrn2(d->v.s, s->v.d, 2) )));

	t.put("sqrshrn.h (>>2)",            both(b, op( g->sqrshrn(d->v.b8, s->v.h, 2) )));
	t.put("sqrshrn.s (>>2)",            both(b, op( g->sqrshrn(d->v.h4, s->v.s, 2) )));
	t.put("sqrshrn.d (>>2)",            both(b, op( g->sqrshrn(d->v.s2, s->v.d, 2) )));
	t.put("sqrshrn2.h (>>2)",           both(b, op( g->sqrshrn2(d->v.b, s->v.h, 2) )));
	t.put("sqrshrn2.s (>>2)",           both(b, op( g->sqrshrn2(d->v.h, s->v.s, 2) )));
	t.put("sqrshrn2.d (>>2)",           both(b, op( g->sqrshrn2(d->v.s, s->v.d, 2) )));

	t.put("uqrshrn.h (>>2)",            both(b, op( g->uqrshrn(d->v.b8, s->v.h, 2) )));
	t.put("uqrshrn.s (>>2)",            both(b, op( g->uqrshrn(d->v.h4, s->v.s, 2) )));
	t.put("uqrshrn.d (>>2)",            both(b, op( g->uqrshrn(d->v.s2, s->v.d, 2) )));
	t.put("uqrshrn2.h (>>2)",           both(b, op( g->uqrshrn2(d->v.b, s->v.h, 2) )));
	t.put("uqrshrn2.s (>>2)",           both(b, op( g->uqrshrn2(d->v.h, s->v.s, 2) )));
	t.put("uqrshrn2.d (>>2)",           both(b, op( g->uqrshrn2(d->v.s, s->v.d, 2) )));

	t.put("sqrshrun.h (>>2)",           both(b, op( g->sqrshrun(d->v.b8, s->v.h, 2) )));
	t.put("sqrshrun.s (>>2)",           both(b, op( g->sqrshrun(d->v.h4, s->v.s, 2) )));
	t.put("sqrshrun.d (>>2)",           both(b, op( g->sqrshrun(d->v.s2, s->v.d, 2) )));
	t.put("sqrshrun2.h (>>2)",          both(b, op( g->sqrshrun2(d->v.b, s->v.h, 2) )));
	t.put("sqrshrun2.s (>>2)",          both(b, op( g->sqrshrun2(d->v.h, s->v.s, 2) )));
	t.put("sqrshrun2.d (>>2)",          both(b, op( g->sqrshrun2(d->v.s, s->v.d, 2) )));

	t.put("sri.b (vec; >>2)",           both(b, op( g->sri(d->v.b, s->v.b, 2) )));
	t.put("sri.h (vec; >>2)",           both(b, op( g->sri(d->v.h, s->v.h, 2) )));
	t.put("sri.s (vec; >>2)",           both(b, op( g->sri(d->v.s, s->v.s, 2) )));
	t.put("sri.d (vec; >>2)",           both(b, op( g->sri(d->v.d, s->v.d, 2) )));
	return;
}

void bench_logic_vec(bool md, double freq) {
	bench_bitwise_logic_vec(md, freq);
	bench_bitmanip_vec(md, freq);
	bench_shift_vec(md, freq);
	return;
}

/*
 * end of logic_vec.cc
 */
