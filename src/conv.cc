
/*
 * @file conv.cc
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>

void bench_conv_fp_vec(bool md, double freq) {
	table t(md, "Floating point convert");
	bench b(freq);

	double const mov_latency = lat_i(freq, op( g->mov(d->v.d[0], s->x); g->mov(d->x, d->v.d[0]) )) / 2.0;

	t.put("scvtf.h (scl; >>2)",         both(b, op( g->scvtf(d->h, s->w, 2) ), op( g->mov(d->w, d->v.s[0]) ), mov_latency));
	t.put("scvtf.s (scl; >>2)",         both(b, op( g->scvtf(d->s, s->w, 2) ), op( g->mov(d->w, d->v.s[0]) ), mov_latency));
	t.put("scvtf.d (scl; >>2)",         both(b, op( g->scvtf(d->d, s->x, 2) ), op( g->mov(d->x, d->v.d[0]) ), mov_latency));
	t.put("scvtf.h (scl; int)",         both(b, op( g->scvtf(d->h, s->w) ), op( g->mov(d->w, d->v.s[0]) ), mov_latency));
	t.put("scvtf.s (scl; int)",         both(b, op( g->scvtf(d->s, s->w) ), op( g->mov(d->w, d->v.s[0]) ), mov_latency));
	t.put("scvtf.d (scl; int)",         both(b, op( g->scvtf(d->d, s->x) ), op( g->mov(d->x, d->v.d[0]) ), mov_latency));

	t.put("scvtf.h (vec; >>2)",         both(b, op( g->scvtf(d->v.h, s->v.h, 2) )));
	t.put("scvtf.s (vec; >>2)",         both(b, op( g->scvtf(d->v.s, s->v.s, 2) )));
	t.put("scvtf.d (vec; >>2)",         both(b, op( g->scvtf(d->v.d, s->v.d, 2) )));
	t.put("scvtf.h (vec; int)",         both(b, op( g->scvtf(d->v.h, s->v.h) )));
	t.put("scvtf.s (vec; int)",         both(b, op( g->scvtf(d->v.s, s->v.s) )));
	t.put("scvtf.d (vec; int)",         both(b, op( g->scvtf(d->v.d, s->v.d) )));

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

	/* TODO: waiting for xbyak_aarch64 */
	// t.put("bfcvt",                      both(b, op( g->fcvt(d->h, s->s) )));
	// t.put("bfcvtn",                     both(b, op( g->fcvt(d->h4, s->s) )));
	// t.put("bfcvtn2",                    both(b, op( g->fcvt(d->h8, s->s) )));
	t.put("fjcvtzs",                    both(b, op( g->fjcvtzs(d->w, s->d) ),                       op( g->mov(d->v.s[0], d->w) ), mov_latency));

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
	return;
}

/*
 * end of conv.cc
 */
