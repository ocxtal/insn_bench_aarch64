
/**
 * @file store.cc
 * @brief measure store throughput and store-to-load latencies
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>

static
void bench_store_thr(bool md, double freq) {
	table t(md, "Scalar store (throughput)");

	memmgr m_h(mem_init( p ));
	memmgr m_u(mem_init( p ), 1);
	memmgr m_z(mem_init( (void *)0 ));
	memmgr m_y(mem_init( (void *)0 ), 1);

	bench h(freq, m_h.ptr(), NULL);
	bench u(freq, m_u.ptr(), NULL);
	bench z(freq, m_z.ptr(), NULL);
	bench y(freq, m_y.ptr(), NULL);

	t.put("str (imm; ofs = 0)",                         thr(h, op( g->str(s->x,   ptr(s->x,      0)) )));
	t.put("str (imm; ofs = 16)",                        thr(h, op( g->str(s->x,   ptr(s->x,      16)) )));
	t.put("str (imm; pre, ofs = 0)",                    thr(h, op( g->str(s[1].x, pre_ptr(s->x,  0)) ),                          thr_skip2_patterns));
	t.put("str (imm; pre, ofs = 16)",                   thr(h, op( g->str(s[1].x, pre_ptr(s->x,  16)); g->sub(s->x, s->x, 16) ), thr_skip2_patterns));
	t.put("str (imm; post, ofs = 0)",                   thr(h, op( g->str(s[1].x, post_ptr(s->x, 0)) ),                          thr_skip2_patterns));
	t.put("str (imm; post, ofs = 16)",                  thr(h, op( g->str(s[1].x, post_ptr(s->x, 16)); g->sub(s->x, s->x, 16) ), thr_skip2_patterns));

	t.put("str (imm; ofs = 0; unaligned)",              thr(u, op( g->str(s->x,   ptr(s->x,      0)) )));
	t.put("str (imm; ofs = 16; unaligned)",             thr(u, op( g->str(s->x,   ptr(s->x,      16)) )));
	t.put("str (imm; pre, ofs = 0; unaligned)",         thr(u, op( g->str(s[1].x, pre_ptr(s->x,  0)) ),                          thr_skip2_patterns));
	t.put("str (imm; pre, ofs = 16; unaligned)",        thr(u, op( g->str(s[1].x, pre_ptr(s->x,  16)); g->sub(s->x, s->x, 16) ), thr_skip2_patterns));
	t.put("str (imm; post, ofs = 0; unaligned)",        thr(u, op( g->str(s[1].x, post_ptr(s->x, 0)) ),                          thr_skip2_patterns));
	t.put("str (imm; post, ofs = 16; unaligned)",       thr(u, op( g->str(s[1].x, post_ptr(s->x, 16)); g->sub(s->x, s->x, 16) ), thr_skip2_patterns));

	{
	memmgr m_c(mem_init( p ), CACHE_LINE_SIZE - 1);
	bench c(freq, m_c.ptr());
	t.put("str (imm; ofs = 0; cross-cache)",            thr(c, op( g->str(s->x,   ptr(s->x,      0)) )));
	t.put("str (imm; pre, ofs = 0; cross-cache)",       thr(c, op( g->str(s[1].x, pre_ptr(s->x,  0)) ),                          thr_skip2_patterns));
	t.put("str (imm; post, ofs = 0; cross-cache)",      thr(c, op( g->str(s[1].x, post_ptr(s->x, 0)) ),                          thr_skip2_patterns));
	}

	{
	memmgr m_p(mem_init( p ), PAGE_SIZE - 1);
	bench p(freq, m_p.ptr());
	t.put("str (imm; ofs = 0; cross-page)",             thr(p, op( g->str(s->x,   ptr(s->x,      0)) )));
	t.put("str (imm; pre, ofs = 0; cross-page)",        thr(p, op( g->str(s[1].x, pre_ptr(s->x,  0)) ),                          thr_skip2_patterns));
	t.put("str (imm; post, ofs = 0; cross-page)",       thr(p, op( g->str(s[1].x, post_ptr(s->x, 0)) ),                          thr_skip2_patterns));
	}

	t.put("str (reg)",                                  thr(h, op( g->str(s->x,   ptr(s->x, g->x28)) )));
	t.put("str (reg; <<3)",                             thr(h, op( g->str(s->x,   ptr(s->x, g->x28, ShMod::LSL, 3)) )));
	t.put("strb (reg)",                                 thr(h, op( g->strb(s->w,  ptr(s->x, g->x28)) )));
	t.put("strh (reg)",                                 thr(h, op( g->strh(s->w,  ptr(s->x, g->x28)) )));
	t.put("stur (imm; ofs = 0)",                        thr(h, op( g->stur(s->x,  ptr(s->x,      0)) )));
	t.put("sturb (imm; ofs = 0)",                       thr(h, op( g->sturb(s->w, ptr(s->x,      0)) )));
	t.put("sturh (imm; ofs = 0)",                       thr(h, op( g->sturh(s->w, ptr(s->x,      0)) )));

	t.put("stp (ofs = 0)",                              thr(h, op( g->stp(d[1].x, d[0].x, ptr(s->x)) ),                          thr_skip2_patterns));
	return;
}

static
void bench_store_to_load(bool md, double freq) {
	table t(md, "Scalar store-to-load");

	memmgr m_h(mem_init( p ));
	bench h(freq, m_h.ptr(), m_h.ptr());

	t.put("str -> ldr (ofs = 0 -> ofs = 0)",            both(h, op( g->str(s->x, ptr(g->x28, 0));  g->ldr(d->x, ptr(g->x28, 0)) )));
	t.put("str -> ldur (ofs = 0 -> ofs = 1)",           both(h, op( g->str(s->x, ptr(g->x28, 0));  g->ldur(d->x, ptr(g->x28, 1)) )));
	t.put("stur -> ldr (ofs = 1 -> ofs = 1)",           both(h, op( g->stur(s->x, ptr(g->x28, 1)); g->ldr(d->x, ptr(g->x28, 0)) )));
	t.put("stur -> ldur (ofs = 1 -> ofs = 1)",          both(h, op( g->stur(s->x, ptr(g->x28, 1)); g->ldur(d->x, ptr(g->x28, 1)) )));

	t.put("stur -> ldur (cross-cache -> aligned)",      both(h, op( g->stur(s->x, ptr(g->x28, CACHE_LINE_SIZE - 1)); g->ldur(d->x, ptr(g->x28, CACHE_LINE_SIZE - 0)) )));
	t.put("stur -> ldur (aligned -> cross-cache)",      both(h, op( g->stur(s->x, ptr(g->x28, CACHE_LINE_SIZE - 0)); g->ldur(d->x, ptr(g->x28, CACHE_LINE_SIZE - 1)) )));
	t.put("stur -> ldur (cross-cache -> cross-cache)",  both(h, op( g->stur(s->x, ptr(g->x28, CACHE_LINE_SIZE - 2)); g->ldur(d->x, ptr(g->x28, CACHE_LINE_SIZE - 1)) )));

	t.put("stp -> ldp (ofs = 0 -> ofs = 0)",            both(h, op( g->stp(d[0].x, d[1].x, ptr(g->x28, 0)); g->ldp(d[0].x, d[1].x, ptr(g->x28, 0)) ), 0.0, lat_flat0_pattern, thr_skip2_patterns));
	t.put("stp -> ldp (ofs = 0 -> ofs = 0; swap)",      both(h, op( g->stp(d[0].x, d[1].x, ptr(g->x28, 0)); g->ldp(d[1].x, d[0].x, ptr(g->x28, 0)) ), 0.0, lat_flat0_pattern, thr_skip2_patterns));
	t.put("stp -> ldp (ofs = 0 -> ofs = 8)",            both(h, op( g->stp(d[1].x, d[0].x, ptr(g->x28, 0)); g->ldp(d[1].x, d[0].x, ptr(g->x28, 8)) ), 0.0, lat_flat0_pattern, thr_skip2_patterns));
	t.put("stp -> ldp (ofs = 8 -> ofs = 0)",            both(h, op( g->stp(d[1].x, d[0].x, ptr(g->x28, 8)); g->ldp(d[1].x, d[0].x, ptr(g->x28, 0)) ), 0.0, lat_flat0_pattern, thr_skip2_patterns));
	return;
}

void bench_store(bool md, double freq) {
	bench_store_thr(md, freq);
	bench_store_to_load(md, freq);
	return;
}

static
void bench_store_vec_thr(bool md, double freq) {
	table t(md, "Vector store (throughput)");

	memmgr m_h(mem_init( p ));
	bench h(freq, m_h.ptr(), m_h.ptr());

	t.put("str.q (imm; ofs = 0)",                       thr(h, op( g->str(d->q, ptr(s->x,      0)) )));
	t.put("str.q (imm; ofs = 16)",                      thr(h, op( g->str(d->q, ptr(s->x,      16)) )));
	t.put("str.q (imm; pre, ofs = 0)",                  thr(h, op( g->str(d->q, pre_ptr(s->x,  0)) )));
	t.put("str.q (imm; pre, ofs = 16)",                 thr(h, op( g->str(d->q, pre_ptr(s->x,  16)); g->sub(s->x, s->x, 16) )));
	t.put("str.q (imm; post, ofs = 0)",                 thr(h, op( g->str(d->q, post_ptr(s->x, 0)) )));
	t.put("str.q (imm; post, ofs = 16)",                thr(h, op( g->str(d->q, post_ptr(s->x, 16)); g->sub(s->x, s->x, 16) )));

	t.put("str.q (imm; ofs = 0; unaligned)",            thr(h, op( g->str(d->q, ptr(s->x,      0)) )));
	t.put("str.q (imm; ofs = 16; unaligned)",           thr(h, op( g->str(d->q, ptr(s->x,      16)) )));
	t.put("str.q (imm; pre, ofs = 0; unaligned)",       thr(h, op( g->str(d->q, pre_ptr(s->x,  0)) )));
	t.put("str.q (imm; pre, ofs = 16; unaligned)",      thr(h, op( g->str(d->q, pre_ptr(s->x,  16)); g->sub(s->x, s->x, 16) )));
	t.put("str.q (imm; post, ofs = 0; unaligned)",      thr(h, op( g->str(d->q, post_ptr(s->x, 0)) )));
	t.put("str.q (imm; post, ofs = 16; unaligned)",     thr(h, op( g->str(d->q, post_ptr(s->x, 16)); g->sub(s->x, s->x, 16) )));

	t.put("stur.q (imm; ofs = 0)",                      thr(h, op( g->stur(d->q, ptr(s->x, 0)) )));
	t.put("stp.q (ofs = 0)",                            thr(h, op( g->stp(d[1].q, d[0].q, ptr(s->x)) )));

	t.put("st1.b (multi)",                              thr(h, op( g->st1(d->v.b, ptr(s->x)) )));
	t.put("st2.b (multi)",                              thr(h, op( g->st2(d->v.b, ptr(s->x)) )));
	t.put("st3.b (multi)",                              thr(h, op( g->st3(d->v.b, ptr(s->x)) )));
	t.put("st4.b (multi)",                              thr(h, op( g->st4(d->v.b, ptr(s->x)) )));

	t.put("st1.h (multi)",                              thr(h, op( g->st1(d->v.h, ptr(s->x)) )));
	t.put("st2.h (multi)",                              thr(h, op( g->st2(d->v.h, ptr(s->x)) )));
	t.put("st3.h (multi)",                              thr(h, op( g->st3(d->v.h, ptr(s->x)) )));
	t.put("st4.h (multi)",                              thr(h, op( g->st4(d->v.h, ptr(s->x)) )));

	t.put("st1.s (multi)",                              thr(h, op( g->st1(d->v.s, ptr(s->x)) )));
	t.put("st2.s (multi)",                              thr(h, op( g->st2(d->v.s, ptr(s->x)) )));
	t.put("st3.s (multi)",                              thr(h, op( g->st3(d->v.s, ptr(s->x)) )));
	t.put("st4.s (multi)",                              thr(h, op( g->st4(d->v.s, ptr(s->x)) )));

	t.put("st1.d (multi)",                              thr(h, op( g->st1(d->v.d, ptr(s->x)) )));
	t.put("st2.d (multi)",                              thr(h, op( g->st2(d->v.d, ptr(s->x)) )));
	t.put("st3.d (multi)",                              thr(h, op( g->st3(d->v.d, ptr(s->x)) )));
	t.put("st4.d (multi)",                              thr(h, op( g->st4(d->v.d, ptr(s->x)) )));

	t.put("st1.b (single; [15])",                       thr(h, op( g->st1(s[3].v.b[15], ptr(s->x)) )));
	t.put("st2.b (single; [15])",                       thr(h, op( g->st2(s[2].v.b[15], ptr(s->x)) )));
	t.put("st3.b (single; [15])",                       thr(h, op( g->st3(s[1].v.b[15], ptr(s->x)) )));
	t.put("st4.b (single; [15])",                       thr(h, op( g->st4(s[0].v.b[15], ptr(s->x)) )));

	t.put("st1.h (single; [7])",                        thr(h, op( g->st1(s[3].v.h[7], ptr(s->x)) )));
	t.put("st2.h (single; [7])",                        thr(h, op( g->st2(s[2].v.h[7], ptr(s->x)) )));
	t.put("st3.h (single; [7])",                        thr(h, op( g->st3(s[1].v.h[7], ptr(s->x)) )));
	t.put("st4.h (single; [7])",                        thr(h, op( g->st4(s[0].v.h[7], ptr(s->x)) )));

	t.put("st1.s (single; [3])",                        thr(h, op( g->st1(s[3].v.s[3], ptr(s->x)) )));
	t.put("st2.s (single; [3])",                        thr(h, op( g->st2(s[2].v.s[3], ptr(s->x)) )));
	t.put("st3.s (single; [3])",                        thr(h, op( g->st3(s[1].v.s[3], ptr(s->x)) )));
	t.put("st4.s (single; [3])",                        thr(h, op( g->st4(s[0].v.s[3], ptr(s->x)) )));

	t.put("st1.d (single; [1])",                        thr(h, op( g->st1(s[3].v.d[1], ptr(s->x)) )));
	t.put("st2.d (single; [1])",                        thr(h, op( g->st2(s[2].v.d[1], ptr(s->x)) )));
	t.put("st3.d (single; [1])",                        thr(h, op( g->st3(s[1].v.d[1], ptr(s->x)) )));
	t.put("st4.d (single; [1])",                        thr(h, op( g->st4(s[0].v.d[1], ptr(s->x)) )));
	return;
}

static
void bench_store_to_load_vec(bool md, double freq) {
	table t(md, "Vector store-to-load");

	memmgr m_h(mem_init( p ));
	memmgr m_z(mem_init( NULL ));

	bench h(freq, m_h.ptr(), m_h.ptr());
	bench z(freq, NULL, m_z.ptr());
	bench y(freq, NULL, (void **)(((uint8_t *)m_z.ptr()) + 1));
	bench x(freq, NULL, (void **)(((uint8_t *)m_z.ptr()) + 128 - 15));

	t.put("str.q -> ldr.q (ofs = 0 -> ofs = 0)",        both(h, op( g->str(s->q, ptr(g->x28, 0));                      g->ldr(s->q, ptr(g->x28, 0)) )));
	t.put("str.q -> ldr.q (ofs = 0 -> ofs = 1)",        both(h, op( g->str(s->q, ptr(g->x28, 0));                      g->ldur(s->q, ptr(g->x28, 1)) )));
	t.put("str.q -> ldr.q (ofs = 1 -> ofs = 0)",        both(h, op( g->stur(s->q, ptr(g->x28, 1));                     g->ldr(s->q, ptr(g->x28, 0)) )));
	t.put("str.q -> ldr.q (ofs = 1 -> ofs = 1)",        both(h, op( g->stur(s->q, ptr(g->x28, 1));                     g->ldur(s->q, ptr(g->x28, 1)) )));

	t.put("stp.s -> ldr.q (ofs = 0)",                           both(h, op( g->stp(s[0].s, s[1].s,    ptr(g->x28, 0)); g->ldr(s[0].q, ptr(g->x28, 0)) )));
	t.put("stp.d -> ldr.q (ofs = 0)",                           both(h, op( g->stp(s[0].d, s[1].d,    ptr(g->x28, 0)); g->ldr(s[0].q, ptr(g->x28, 0)) )));
	t.put("stp.q -> ldr.q (ofs = 0  -> 1st reg)",               both(h, op( g->stp(s[0].q, s[1].q,    ptr(g->x28, 0)); g->ldr(s[0].q, ptr(g->x28, 0)) )));
	t.put("stp.q -> ldr.q (ofs = 16 -> 2nd reg)",               both(h, op( g->stp(s[0].q, s[1].q,    ptr(g->x28, 0)); g->ldr(s[1].q, ptr(g->x28, 16)) )));
	t.put("stp.q -> ldr.q (false dep. ofs = 0  -> 2nd reg)",    both(h, op( g->stp(s[0].q, s[1].q,    ptr(g->x28, 0)); g->ldr(s[1].q, ptr(g->x28, 0)) )));
	t.put("stp.q -> ldr.q (false dep. ofs = 16 -> 1st reg)",    both(h, op( g->stp(s[0].q, s[1].q,    ptr(g->x28, 0)); g->ldr(s[1].q, ptr(g->x28, 0)) )));

	t.put("st1.b (single) -> ldr.q (ofs = 0)",          both(z, op( g->st1((s[0].v.b - s[0].v.b)[15], ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 0)) ),  0.0, lat_flat0_pattern, thr_skip1x_patterns));
	t.put("st2.b (single) -> ldr.q (ofs = 0)",          both(h, op( g->st2((s[0].v.b - s[1].v.b)[15], ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 0)) ),  0.0, lat_flat0_pattern, thr_skip2x_patterns));
	t.put("st3.b (single) -> ldr.q (ofs = 0)",          both(h, op( g->st3((s[0].v.b - s[2].v.b)[15], ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 0)) ),  0.0, lat_flat0_pattern, thr_skip3x_patterns));
	t.put("st4.b (single) -> ldr.q (ofs = 0)",          both(h, op( g->st4((s[0].v.b - s[3].v.b)[15], ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 0)) ),  0.0, lat_flat0_pattern, thr_skip4x_patterns));

	t.put("st1.h (single) -> ldr.q (ofs = 0)",          both(z, op( g->st1((s[0].v.h - s[0].v.h)[7],  ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 0)) ),  0.0, lat_flat0_pattern, thr_skip1x_patterns));
	t.put("st2.h (single) -> ldr.q (ofs = 0)",          both(h, op( g->st2((s[0].v.h - s[1].v.h)[7],  ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 0)) ),  0.0, lat_flat0_pattern, thr_skip2x_patterns));
	t.put("st3.h (single) -> ldr.q (ofs = 0)",          both(h, op( g->st3((s[0].v.h - s[2].v.h)[7],  ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 0)) ),  0.0, lat_flat0_pattern, thr_skip3x_patterns));
	t.put("st4.h (single) -> ldr.q (ofs = 0)",          both(h, op( g->st4((s[0].v.h - s[3].v.h)[7],  ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 0)) ),  0.0, lat_flat0_pattern, thr_skip4x_patterns));

	t.put("st1.s (single) -> ldr.q (ofs = 0)",          both(z, op( g->st1((s[0].v.s - s[0].v.s)[3],  ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 0)) ),  0.0, lat_flat0_pattern, thr_skip1x_patterns));
	t.put("st2.s (single) -> ldr.q (ofs = 0)",          both(h, op( g->st2((s[0].v.s - s[1].v.s)[3],  ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 0)) ),  0.0, lat_flat0_pattern, thr_skip2x_patterns));
	t.put("st3.s (single) -> ldr.q (ofs = 0)",          both(h, op( g->st3((s[0].v.s - s[2].v.s)[3],  ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 0)) ),  0.0, lat_flat0_pattern, thr_skip3x_patterns));
	t.put("st4.s (single) -> ldr.q (ofs = 0)",          both(h, op( g->st4((s[0].v.s - s[3].v.s)[3],  ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 0)) ),  0.0, lat_flat0_pattern, thr_skip4x_patterns));

	t.put("st1.d (single) -> ldr.q (ofs = 0)",          both(z, op( g->st1((s[0].v.d - s[0].v.d)[1],  ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 0)) ),  0.0, lat_flat0_pattern, thr_skip1x_patterns));
	t.put("st2.d (single) -> ldr.q (ofs = 0)",          both(h, op( g->st2((s[0].v.d - s[1].v.d)[1],  ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 0)) ),  0.0, lat_flat0_pattern, thr_skip2x_patterns));
	t.put("st3.d (single) -> ldr.q (ofs = 0)",          both(h, op( g->st3((s[0].v.d - s[2].v.d)[1],  ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 0)) ),  0.0, lat_flat0_pattern, thr_skip3x_patterns));
	t.put("st4.d (single) -> ldr.q (ofs = 0)",          both(h, op( g->st4((s[0].v.d - s[3].v.d)[1],  ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 0)) ),  0.0, lat_flat0_pattern, thr_skip4x_patterns));

	t.put("st1.b (multi; 1 reg)  -> ldr.q (1st reg)",   both(z, op( g->st1(s[0].v.b - s[0].v.b,       ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 0)) ),  0.0, lat_flat0_pattern, thr_skip1x_patterns));
	t.put("st1.b (multi; 2 regs) -> ldr.q (2nd reg)",   both(z, op( g->st1(s[0].v.b - s[1].v.b,       ptr(g->x28));    g->ldr(s[1].q, ptr(g->x28, 16)) ), 0.0, lat_flat0_pattern, thr_skip2x_patterns));
	t.put("st1.b (multi; 3 regs) -> ldr.q (3rd reg)",   both(z, op( g->st1(s[0].v.b - s[2].v.b,       ptr(g->x28));    g->ldr(s[2].q, ptr(g->x28, 32)) ), 0.0, lat_flat0_pattern, thr_skip3x_patterns));
	t.put("st1.b (multi; 4 regs) -> ldr.q (4th reg)",   both(z, op( g->st1(s[0].v.b - s[3].v.b,       ptr(g->x28));    g->ldr(s[3].q, ptr(g->x28, 48)) ), 0.0, lat_flat0_pattern, thr_skip4x_patterns));

	t.put("st1.b (multi; 2 regs) -> ldr.q (false dep. ofs = 16 -> 1st reg)",                          both(z, op( g->st1(s[0].v.b - s[1].v.b, ptr(g->x28)); g->ldr(s[0].q, ptr(g->x28, 16)) ), 0.0, lat_flat0_pattern, thr_skip2x_patterns));
	t.put("st1.b (multi; 3 regs) -> ldr.q (false dep. ofs = 32 -> 1st reg)",                          both(z, op( g->st1(s[0].v.b - s[2].v.b, ptr(g->x28)); g->ldr(s[0].q, ptr(g->x28, 32)) ), 0.0, lat_flat0_pattern, thr_skip3x_patterns));
	t.put("st1.b (multi; 4 regs) -> ldr.q (false dep. ofs = 48 -> 1st reg)",                          both(z, op( g->st1(s[0].v.b - s[3].v.b, ptr(g->x28)); g->ldr(s[0].q, ptr(g->x28, 48)) ), 0.0, lat_flat0_pattern, thr_skip4x_patterns));
	t.put("st1.b (multi; 2 regs) -> ldr.q (false dep. ofs = 0 -> 2nd reg)",                           both(z, op( g->st1(s[0].v.b - s[1].v.b, ptr(g->x28)); g->ldr(s[0].q, ptr(g->x28, 16)) ), 0.0, lat_flat0_pattern, thr_skip2x_patterns));
	t.put("st1.b (multi; 3 regs) -> ldr.q (false dep. ofs = 0 -> 3rd reg)",                           both(z, op( g->st1(s[0].v.b - s[2].v.b, ptr(g->x28)); g->ldr(s[0].q, ptr(g->x28, 32)) ), 0.0, lat_flat0_pattern, thr_skip3x_patterns));
	t.put("st1.b (multi; 4 regs) -> ldr.q (false dep. ofs = 0 -> 4th reg)",                           both(z, op( g->st1(s[0].v.b - s[3].v.b, ptr(g->x28)); g->ldr(s[0].q, ptr(g->x28, 48)) ), 0.0, lat_flat0_pattern, thr_skip4x_patterns));
	t.put("st1.b (multi; 2 regs) -> ldr.q (false dep. ofs = 0 -> 2nd reg; unaligned)",                both(y, op( g->st1(s[0].v.b - s[1].v.b, ptr(g->x28)); g->ldr(s[1].q, ptr(g->x28, 0)) ),  0.0, lat_flat0_pattern, thr_skip2x_patterns));
	t.put("st1.b (multi; 3 regs) -> ldr.q (false dep. ofs = 0 -> 3rd reg; unaligned)",                both(y, op( g->st1(s[0].v.b - s[2].v.b, ptr(g->x28)); g->ldr(s[2].q, ptr(g->x28, 0)) ),  0.0, lat_flat0_pattern, thr_skip3x_patterns));
	t.put("st1.b (multi; 4 regs) -> ldr.q (false dep. ofs = 0 -> 4th reg; unaligned)",                both(y, op( g->st1(s[0].v.b - s[3].v.b, ptr(g->x28)); g->ldr(s[3].q, ptr(g->x28, 0)) ),  0.0, lat_flat0_pattern, thr_skip4x_patterns));
	t.put("st1.b (multi; 2 regs) -> ldr.q (false dep. ofs = 0 -> 2nd reg; unaligned; cross-cache)",   both(x, op( g->st1(s[0].v.b - s[1].v.b, ptr(g->x28)); g->ldr(s[1].q, ptr(g->x28, 0)) ),  0.0, lat_flat0_pattern, thr_skip2x_patterns));
	t.put("st1.b (multi; 3 regs) -> ldr.q (false dep. ofs = 0 -> 3rd reg; unaligned; cross-cache)",   both(x, op( g->st1(s[0].v.b - s[2].v.b, ptr(g->x28)); g->ldr(s[2].q, ptr(g->x28, 0)) ),  0.0, lat_flat0_pattern, thr_skip3x_patterns));
	t.put("st1.b (multi; 4 regs) -> ldr.q (false dep. ofs = 0 -> 4th reg; unaligned; cross-cache)",   both(x, op( g->st1(s[0].v.b - s[3].v.b, ptr(g->x28)); g->ldr(s[3].q, ptr(g->x28, 0)) ),  0.0, lat_flat0_pattern, thr_skip4x_patterns));

	t.put("st2.b (multi) -> ldr.q (ofs = 16)",          both(h, op( g->st2(s[0].v.b - s[1].v.b,       ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 16)) ), 0.0, lat_flat0_pattern, thr_skip2x_patterns));
	t.put("st3.b (multi) -> ldr.q (ofs = 32)",          both(h, op( g->st3(s[0].v.b - s[2].v.b,       ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 32)) ), 0.0, lat_flat0_pattern, thr_skip3x_patterns));
	t.put("st4.b (multi) -> ldr.q (ofs = 48)",          both(h, op( g->st4(s[0].v.b - s[3].v.b,       ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 48)) ), 0.0, lat_flat0_pattern, thr_skip4x_patterns));

	t.put("st2.h (multi) -> ldr.q (ofs = 16)",          both(h, op( g->st2(s[0].v.h - s[1].v.h,       ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 16)) ), 0.0, lat_flat0_pattern, thr_skip2x_patterns));
	t.put("st3.h (multi) -> ldr.q (ofs = 32)",          both(h, op( g->st3(s[0].v.h - s[2].v.h,       ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 32)) ), 0.0, lat_flat0_pattern, thr_skip3x_patterns));
	t.put("st4.h (multi) -> ldr.q (ofs = 48)",          both(h, op( g->st4(s[0].v.h - s[3].v.h,       ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 48)) ), 0.0, lat_flat0_pattern, thr_skip4x_patterns));

	t.put("st2.s (multi) -> ldr.q (ofs = 16)",          both(h, op( g->st2(s[0].v.s - s[1].v.s,       ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 16)) ), 0.0, lat_flat0_pattern, thr_skip2x_patterns));
	t.put("st3.s (multi) -> ldr.q (ofs = 32)",          both(h, op( g->st3(s[0].v.s - s[2].v.s,       ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 32)) ), 0.0, lat_flat0_pattern, thr_skip3x_patterns));
	t.put("st4.s (multi) -> ldr.q (ofs = 48)",          both(h, op( g->st4(s[0].v.s - s[3].v.s,       ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 48)) ), 0.0, lat_flat0_pattern, thr_skip4x_patterns));

	t.put("st2.d (multi) -> ldr.q (ofs = 16)",          both(h, op( g->st2(s[0].v.d - s[1].v.d,       ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 16)) ), 0.0, lat_flat0_pattern, thr_skip2x_patterns));
	t.put("st3.d (multi) -> ldr.q (ofs = 32)",          both(h, op( g->st3(s[0].v.d - s[2].v.d,       ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 32)) ), 0.0, lat_flat0_pattern, thr_skip3x_patterns));
	t.put("st4.d (multi) -> ldr.q (ofs = 48)",          both(h, op( g->st4(s[0].v.d - s[3].v.d,       ptr(g->x28));    g->ldr(s[0].q, ptr(g->x28, 48)) ), 0.0, lat_flat0_pattern, thr_skip4x_patterns));
	return;
}

void bench_store_vec(bool md, double freq) {
	bench_store_vec_thr(md, freq);
	bench_store_to_load_vec(md, freq);
	return;
}

/*
 * end of store.cc
 */
