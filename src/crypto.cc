
/**
 * @file crypto.cc
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>

void bench_crypto(bool md, double freq) {
	table t(md, "Hashing and cryptographic");
	bench b(freq, (size_t)0, 0);

	t.put("aese",                       both(b, op( g->aese(d->v.b, s->v.b) )));
	t.put("aesd",                       both(b, op( g->aesd(d->v.b, s->v.b) )));
	t.put("aesmc",                      both(b, op( g->aesmc(d->v.b, s->v.b) )));
	t.put("aesimc",                     both(b, op( g->aesimc(d->v.b, s->v.b) )));

	t.put("crc32x",                     both(b, op( g->crc32x(d->w, d->w, s->x) )));
	t.put("crc32cx",                    both(b, op( g->crc32cx(d->w, d->w, s->x) )));
	return;
}

/*
 * end of crypto.cc
 */
