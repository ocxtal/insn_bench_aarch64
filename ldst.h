
/**
 * @file ldst.h
 * @brief measure load / store latencies
 * @author Hajime Suzuki
 */
#ifndef _LDST_H_INCLUDED
#define _LDST_H_INCLUDED
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
void bench_load(double freq) {
	size_t const elems = 128 / sizeof(void *);
	void **self  = (void **)malloc(sizeof(void *) * elems);
	void **loop = (void **)malloc(sizeof(void *) * elems);

	init_mems(elems, self, loop);
	bench l(freq, self);
	bench c(freq, loop);

	print("ldr", l.both(op( g->ldr(xd, ptr(xs)) )));
	print("ldr", c.both(op( g->ldr(xd, ptr(xs)) )));

	free(self);
	free(loop);
	return;
}

static
void bench_store(double freq) {
	(void)freq;
	return;
}

#endif	/* _LDST_H_INCLUDED */

/*
 * end of ldst.h
 */
