
/**
 * @file perm.cc
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>

void bench_prem(bool md, double freq) {
	table t(md, "Vector permutation");
	bench b(freq, (size_t)0, 0);

	t.put("ext.b (>>1)",                both(b, op( g->ext(d->v.b, d->v.b, s->v.b, 1) )));
	t.put("ext.b (>>15)",               both(b, op( g->ext(d->v.b, d->v.b, s->v.b, 1) )));
	return;
}

/*
 * end of perm.cc
 */
