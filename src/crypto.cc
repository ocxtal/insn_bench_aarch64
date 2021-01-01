
/**
 * @file crypto.cc
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>

void bench_crypto(bool md, double freq) {
	table t(md, "Hashing and cryptographic");
	bench b(freq, (size_t)0, 0);

	t.put("crc32x",                     both(b, op( g->crc32x(d->w, d->w, s->x) )));
	t.put("crc32cx",                    both(b, op( g->crc32cx(d->w, d->w, s->x) )));

	t.put("sha1c",                      both(b, op( g->sha1c(d->q, d->s, s->v.s) )));
	t.put("sha1h",                      both(b, op( g->sha1h(d->s, s->s) )));
	t.put("sha1m",                      both(b, op( g->sha1m(d->q, d->s, s->v.s) )));
	t.put("sha1p",                      both(b, op( g->sha1p(d->q, d->s, s->v.s) )));
	t.put("sha1su0",                    both(b, op( g->sha1su0(d->v.s, d->v.s, s->v.s) )));
	t.put("sha1su1",                    both(b, op( g->sha1su1(d->v.s, s->v.s) )));

	t.put("sha256h",                    both(b, op( g->sha256h(d->q, d->q, s->v.s) )));
	t.put("sha256h2",                   both(b, op( g->sha256h2(d->q, d->q, s->v.s) )));
	t.put("sha256su0",                  both(b, op( g->sha256su0(d->v.s, s->v.s) )));
	t.put("sha256su1",                  both(b, op( g->sha256su1(d->v.s, d->v.s, s->v.s) )));

	t.put("sha512h",                    both(b, op( g->sha512h(d->q, d->q, s->v.d) )));
	t.put("sha512h2",                   both(b, op( g->sha512h2(d->q, d->q, s->v.d) )));
	t.put("sha512su0",                  both(b, op( g->sha512su0(d->v.d, s->v.d) )));
	t.put("sha512su1",                  both(b, op( g->sha512su1(d->v.d, s->v.d, s->v.d) )));

	t.put("aese",                       both(b, op( g->aese(d->v.b, s->v.b) )));
	t.put("aesd",                       both(b, op( g->aesd(d->v.b, s->v.b) )));
	t.put("aesmc",                      both(b, op( g->aesmc(d->v.b, s->v.b) )));
	t.put("aesimc",                     both(b, op( g->aesimc(d->v.b, s->v.b) )));

	t.put("sm3partw1",                  both(b, op( g->sm3partw1(d->v.s, d->v.s, s->v.s) )));
	t.put("sm3partw2",                  both(b, op( g->sm3partw2(d->v.s, d->v.s, s->v.s) )));
	t.put("sm3ss1",                     both(b, op( g->sm3ss1(d->v.s, d->v.s, d->v.s, s->v.s) )));
	t.put("sm3tt1a ([0])",              both(b, op( g->sm3tt1a(d->v.s, d->v.s, s->v.s[0]) )));
	t.put("sm3tt1a ([3])",              both(b, op( g->sm3tt1a(d->v.s, d->v.s, s->v.s[3]) )));
	t.put("sm3tt1b ([0])",              both(b, op( g->sm3tt1b(d->v.s, d->v.s, s->v.s[0]) )));
	t.put("sm3tt1b ([3])",              both(b, op( g->sm3tt1b(d->v.s, d->v.s, s->v.s[3]) )));
	t.put("sm3tt2a ([0])",              both(b, op( g->sm3tt2a(d->v.s, d->v.s, s->v.s[0]) )));
	t.put("sm3tt2a ([3])",              both(b, op( g->sm3tt2a(d->v.s, d->v.s, s->v.s[3]) )));
	t.put("sm3tt2b ([0])",              both(b, op( g->sm3tt2b(d->v.s, d->v.s, s->v.s[0]) )));
	t.put("sm3tt2b ([3])",              both(b, op( g->sm3tt2b(d->v.s, d->v.s, s->v.s[3]) )));

	t.put("sm4e",                       both(b, op( g->sm4e(d->v.s, s->v.s) )));
	t.put("sm4ekey",                    both(b, op( g->sm4ekey(d->v.s, d->v.s, s->v.s) )));
	return;
}

/*
 * end of crypto.cc
 */
