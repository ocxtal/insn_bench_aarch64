
/**
 * @file load.cc
 * @brief measure load latencies
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>

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

void bench_load(bool md, double freq) {
	table t(md, "Load instructions");

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

/*
 * end of load.cc
 */
