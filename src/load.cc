
/**
 * @file load.cc
 * @brief measure load latencies
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>

void bench_load(bool md, double freq) {
	table t(md, "Scalar load");

	memmgr m_h(mem_init( p ));
	memmgr m_u(mem_init( p ), 1);
	memmgr m_z(mem_init( (void *)0 ));
	memmgr m_y(mem_init( (void *)0 ), 1);

	bench h(freq, m_h.ptr(), m_h.ptr());
	bench u(freq, m_u.ptr(), m_u.ptr());
	bench z(freq, m_z.ptr(), m_z.ptr());
	bench y(freq, m_y.ptr(), m_y.ptr());

	lat_i(freq, op( g->add(d->x, d->x, s->x) ));
	lat_i(freq, op( g->add(d->x, d->x, s->x) ));
	lat_i(freq, op( g->add(d->x, d->x, s->x) ));
	double const add_latency = lat_i(freq, op( g->add(d->x, d->x, s->x) ));

	#define adj8(_g, _d)           ({ for(size_t i = 0; i < 8; i++) { _g->sub(_d->x, _d->x, 2); } })
	double const adj8_latency    = lat_i(freq, op( adj8(g, d) ));

	t.put("ldr (imm; ofs = 0)",                     both(h, op( g->ldr(d->x,   ptr(s->x,      0)) ),  0.0, lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldr (imm; ofs = 16)",                    both(h, op( g->ldr(d->x,   ptr(s->x,      16)) ), 0.0, lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldr (imm; pre, ofs = 0)",                both(h, op( g->ldr(d[1].x, pre_ptr(s->x,  0)) ),  0.0, lat_inc0_pattern, thr_skip3_patterns));
	t.put("ldr (imm; pre, ofs = 16)",               lat(h,  op( g->ldr(d->x,   pre_ptr(s->x,  16)) ), 0.0, lat_inc1_pattern));
	t.put("ldr (imm; post, ofs = 0)",               both(h, op( g->ldr(d[1].x, post_ptr(s->x, 0)) ),  0.0, lat_inc0_pattern, thr_skip3_patterns));
	t.put("ldr (imm; post, ofs = 16)",              lat(h,  op( g->ldr(d->x,   post_ptr(s->x, 16)) ), 0.0, lat_inc1_pattern));
	t.put("ldr (ptr fwd.; imm; pre, ofs = 16)",     lat(h,  op( g->ldr(g->x28, pre_ptr(s->x,  16)); adj8(g, s) ),                                         adj8_latency,     lat_flat0_pattern));
	t.put("ldr (ptr fwd.; imm; post, ofs = 16)",    lat(z,  op( g->ldr(g->x28, post_ptr(s->x, 16)); adj8(g, s) ),                                         adj8_latency,     lat_flat0_pattern));

	t.put("ldr (imm; ofs = 0; unaligned)",          both(u, op( g->ldr(d->x,   ptr(s->x,      0)) ),  0.0, lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldr (imm; ofs = 16; unaligned)",         both(u, op( g->ldr(d->x,   ptr(s->x,      16)) ), 0.0, lat_inc1_pattern, thr_skip2_patterns));

	{
	memmgr m_c(mem_init( p ), CACHE_LINE_SIZE - 1);
	bench c(freq, m_c.ptr());
	t.put("ldr (imm; ofs = 0; cross-cache)",        both(c, op( g->ldr(d->x,   ptr(s->x,      0)) ),  0.0, lat_inc1_pattern, thr_skip2_patterns));
	}

	{
	memmgr m_p(mem_init( p ), PAGE_SIZE - 1);
	bench p(freq, m_p.ptr());
	t.put("ldr (imm; ofs = 0; cross-page)",         both(p, op( g->ldr(d->x,   ptr(s->x,      0)) ),  0.0, lat_inc1_pattern, thr_skip2_patterns));
	}

	t.put("ldr (reg)",                              both(z, op( g->ldr(d->x,   ptr(g->x28, s->x)) ),                op_init( g->eor(g->x0, g->x0, g->x0) )));
	t.put("ldr (reg; <<3)",                         both(z, op( g->ldr(d->x,   ptr(g->x28, s->x, ShMod::LSL, 3)) ), op_init( g->eor(g->x0, g->x0, g->x0) )));
	t.put("ldr (literal)",                          thr(z,  op( g->ldr(d->x,   g->L()) )));
	t.put("ldr (reg; unaligned)",                   both(y, op( g->ldr(d->x,   ptr(g->x28, s->x)) ),                op_init( g->eor(g->x0, g->x0, g->x0) )));
	t.put("ldr (reg; <<3; unaligned)",              both(y, op( g->ldr(d->x,   ptr(g->x28, s->x, ShMod::LSL, 3)) ), op_init( g->eor(g->x0, g->x0, g->x0) )));

	t.put("ldrb (imm; ofs = 0)",                    both(z, op( g->ldrb(d->w,  ptr(s->x, 0));                 g->add(d->x, d->x, g->x28) ),  add_latency, lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrb (imm; ofs = 16)",                   both(z, op( g->ldrb(d->w,  ptr(s->x, 16));                g->add(d->x, d->x, g->x28) ),  add_latency, lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrb (reg)",                             both(z, op( g->ldrb(d->w,  ptr(s->x, g->x28)) ), op_init( g->eor(g->x0, g->x0, g->x0) ), 0.0,         lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrb (imm; ofs = 0; unaligned)",         both(y, op( g->ldrb(d->w,  ptr(s->x, 0));                 g->add(d->x, d->x, g->x28) ),  add_latency, lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrb (imm; ofs = 16; unaligned)",        both(y, op( g->ldrb(d->w,  ptr(s->x, 16));                g->add(d->x, d->x, g->x28) ),  add_latency, lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrb (reg; unaligned)",                  both(y, op( g->ldrb(d->w,  ptr(s->x, g->x28)) ), op_init( g->eor(g->x0, g->x0, g->x0) ), 0.0,         lat_inc1_pattern, thr_skip2_patterns));

	t.put("ldrsb (imm; ofs = 0)",                   both(z, op( g->ldrsb(d->w, ptr(s->x, 0));                 g->add(d->x, d->x, g->x28) ),  add_latency, lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrsb (imm; ofs = 16)",                  both(z, op( g->ldrsb(d->w, ptr(s->x, 16));                g->add(d->x, d->x, g->x28) ),  add_latency, lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrsb (reg)",                            both(z, op( g->ldrsb(d->w, ptr(s->x, g->x28)) ), op_init( g->eor(g->x0, g->x0, g->x0) ), 0.0,         lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrsb (imm; ofs = 0; unaligned)",        both(y, op( g->ldrsb(d->w, ptr(s->x, 0));                 g->add(d->x, d->x, g->x28) ),  add_latency, lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrsb (imm; ofs = 16; unaligned)",       both(y, op( g->ldrsb(d->w, ptr(s->x, 16));                g->add(d->x, d->x, g->x28) ),  add_latency, lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrsb (reg; unaligned)",                 both(y, op( g->ldrsb(d->w, ptr(s->x, g->x28)) ), op_init( g->eor(g->x0, g->x0, g->x0) ), 0.0,         lat_inc1_pattern, thr_skip2_patterns));

	t.put("ldrh (imm; ofs = 0)",                    both(z, op( g->ldrh(d->w,  ptr(s->x, 0));                 g->add(d->x, d->x, g->x28) ),  add_latency, lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrh (imm; ofs = 16)",                   both(z, op( g->ldrh(d->w,  ptr(s->x, 16));                g->add(d->x, d->x, g->x28) ),  add_latency, lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrh (reg)",                             both(z, op( g->ldrh(d->w,  ptr(s->x, g->x28)) ), op_init( g->eor(g->x0, g->x0, g->x0) ), 0.0,         lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrh (imm; ofs = 0; unaligned)",         both(y, op( g->ldrh(d->w,  ptr(s->x, 0));                 g->add(d->x, d->x, g->x28) ),  add_latency, lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrh (imm; ofs = 16; unaligned)",        both(y, op( g->ldrh(d->w,  ptr(s->x, 16));                g->add(d->x, d->x, g->x28) ),  add_latency, lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrh (reg; unaligned)",                  both(y, op( g->ldrh(d->w,  ptr(s->x, g->x28)) ), op_init( g->eor(g->x0, g->x0, g->x0) ), 0.0,         lat_inc1_pattern, thr_skip2_patterns));

	t.put("ldrsh (imm; ofs = 0)",                   both(z, op( g->ldrsh(d->w, ptr(s->x, 0));                 g->add(d->x, d->x, g->x28) ),  add_latency, lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrsh (imm; ofs = 16)",                  both(z, op( g->ldrsh(d->w, ptr(s->x, 16));                g->add(d->x, d->x, g->x28) ),  add_latency, lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrsh (reg)",                            both(z, op( g->ldrsh(d->w, ptr(s->x, g->x28)) ), op_init( g->eor(g->x0, g->x0, g->x0) ), 0.0,         lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrsh (imm; ofs = 0; unaligned)",        both(y, op( g->ldrsh(d->w, ptr(s->x, 0));                 g->add(d->x, d->x, g->x28) ),  add_latency, lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrsh (imm; ofs = 16; unaligned)",       both(y, op( g->ldrsh(d->w, ptr(s->x, 16));                g->add(d->x, d->x, g->x28) ),  add_latency, lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrsh (reg; unaligned)",                 both(y, op( g->ldrsh(d->w, ptr(s->x, g->x28)) ), op_init( g->eor(g->x0, g->x0, g->x0) ), 0.0,         lat_inc1_pattern, thr_skip2_patterns));

	t.put("ldrsw (imm; ofs = 0)",                   both(z, op( g->ldrsw(d->x, ptr(s->x, 0));                 g->add(d->x, d->x, g->x28) ),  add_latency, lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrsw (imm; ofs = 16)",                  both(z, op( g->ldrsw(d->x, ptr(s->x, 16));                g->add(d->x, d->x, g->x28) ),  add_latency, lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrsw (reg)",                            both(z, op( g->ldrsw(d->x, ptr(s->x, g->x28)) ), op_init( g->eor(g->x0, g->x0, g->x0) ), 0.0,         lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrsw (literal)",                        thr(h,  op( g->ldrsw(d->x, g->L()) )));
	t.put("ldrsw (imm; ofs = 0; unaligned)",        both(y, op( g->ldrsw(d->x, ptr(s->x, 0));                 g->add(d->x, d->x, g->x28) ),  add_latency, lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrsw (imm; ofs = 16; unaligned)",       both(y, op( g->ldrsw(d->x, ptr(s->x, 16));                g->add(d->x, d->x, g->x28) ),  add_latency, lat_inc1_pattern, thr_skip2_patterns));
	t.put("ldrsw (reg; unaligned)",                 both(y, op( g->ldrsw(d->x, ptr(s->x, g->x28)) ), op_init( g->eor(g->x0, g->x0, g->x0) ), 0.0,         lat_inc1_pattern, thr_skip2_patterns));

	t.put("ldur (ofs = 0)",                         both(h, op( g->ldur(d->x,  ptr(s->x, 0)) )));
	t.put("ldur (ofs = 16)",                        both(h, op( g->ldur(d->x,  ptr(s->x, 16)) )));
	t.put("ldur (ofs = 0; unaligned)",              both(u, op( g->ldur(d->x,  ptr(s->x, 0)) )));
	t.put("ldur (ofs = 16; unaligned)",             both(u, op( g->ldur(d->x,  ptr(s->x, 16)) )));

	t.put("ldurb (ofs = 0)",                        both(z, op( g->ldurb(d->w, ptr(s->x, 0));                 g->add(d->x, d->x, g->x28) ),  add_latency, lat_patterns,     thr_patterns));
	t.put("ldurb (ofs = 16)",                       both(z, op( g->ldurb(d->w, ptr(s->x, 16));                g->add(d->x, d->x, g->x28) ),  add_latency, lat_patterns,     thr_patterns));
	t.put("ldurb (ofs = 0; unaligned)",             both(y, op( g->ldurb(d->w, ptr(s->x, 0));                 g->add(d->x, d->x, g->x28) ),  add_latency, lat_patterns,     thr_patterns));
	t.put("ldurb (ofs = 16; unaligned)",            both(y, op( g->ldurb(d->w, ptr(s->x, 16));                g->add(d->x, d->x, g->x28) ),  add_latency, lat_patterns,     thr_patterns));

	t.put("ldurh (ofs = 0)",                        both(z, op( g->ldurh(d->w, ptr(s->x, 0));                 g->add(d->x, d->x, g->x28) ),  add_latency, lat_patterns,     thr_patterns));
	t.put("ldurh (ofs = 16)",                       both(z, op( g->ldurh(d->w, ptr(s->x, 16));                g->add(d->x, d->x, g->x28) ),  add_latency, lat_patterns,     thr_patterns));
	t.put("ldurh (ofs = 0; unaligned)",             both(y, op( g->ldurh(d->w, ptr(s->x, 0));                 g->add(d->x, d->x, g->x28) ),  add_latency, lat_patterns,     thr_patterns));
	t.put("ldurh (ofs = 16; unaligned)",            both(y, op( g->ldurh(d->w, ptr(s->x, 16));                g->add(d->x, d->x, g->x28) ),  add_latency, lat_patterns,     thr_patterns));

	t.put("ldursb (ofs = 0)",                       both(z, op( g->ldursb(d->w, ptr(s->x, 0));                g->add(d->x, d->x, g->x28) ),  add_latency, lat_patterns,     thr_patterns));
	t.put("ldursb (ofs = 16)",                      both(z, op( g->ldursb(d->w, ptr(s->x, 16));               g->add(d->x, d->x, g->x28) ),  add_latency, lat_patterns,     thr_patterns));
	t.put("ldursb (ofs = 0; unaligned)",            both(y, op( g->ldursb(d->w, ptr(s->x, 0));                g->add(d->x, d->x, g->x28) ),  add_latency, lat_patterns,     thr_patterns));
	t.put("ldursb (ofs = 16; unaligned)",           both(y, op( g->ldursb(d->w, ptr(s->x, 16));               g->add(d->x, d->x, g->x28) ),  add_latency, lat_patterns,     thr_patterns));

	t.put("ldursh (ofs = 0)",                       both(z, op( g->ldursh(d->w, ptr(s->x, 0));                g->add(d->x, d->x, g->x28) ),  add_latency, lat_patterns,     thr_patterns));
	t.put("ldursh (ofs = 16)",                      both(z, op( g->ldursh(d->w, ptr(s->x, 16));               g->add(d->x, d->x, g->x28) ),  add_latency, lat_patterns,     thr_patterns));
	t.put("ldursh (ofs = 0; unaligned)",            both(y, op( g->ldursh(d->w, ptr(s->x, 0));                g->add(d->x, d->x, g->x28) ),  add_latency, lat_patterns,     thr_patterns));
	t.put("ldursh (ofs = 16; unaligned)",           both(y, op( g->ldursh(d->w, ptr(s->x, 16));               g->add(d->x, d->x, g->x28) ),  add_latency, lat_patterns,     thr_patterns));

	t.put("ldursw (ofs = 0)",                       both(z, op( g->ldursw(d->x, ptr(s->x, 0));                g->add(d->x, d->x, g->x28) ),  add_latency, lat_patterns,     thr_patterns));
	t.put("ldursw (ofs = 16)",                      both(z, op( g->ldursw(d->x, ptr(s->x, 16));               g->add(d->x, d->x, g->x28) ),  add_latency, lat_patterns,     thr_patterns));
	t.put("ldursw (ofs = 0; unaligned)",            both(y, op( g->ldursw(d->x, ptr(s->x, 0));                g->add(d->x, d->x, g->x28) ),  add_latency, lat_patterns,     thr_patterns));
	t.put("ldursw (ofs = 16; unaligned)",           both(y, op( g->ldursw(d->x, ptr(s->x, 16));               g->add(d->x, d->x, g->x28) ),  add_latency, lat_patterns,     thr_patterns));

	t.put("ldp (x; ofs = 0; 1st elem)",             both(h, op(                             g->ldp(d[1].x, d[0].x,   ptr(s->x)) ),                                         0.0,         lat_inc2_pattern, thr_skip2_patterns));
	t.put("ldp (x; ofs = 0; 2nd elem)",             both(h, op(                             g->ldp(d[0].x, d[1].x,   ptr(s->x)) ),                                         0.0,         lat_inc2_pattern, thr_skip2_patterns));
	t.put("ldp (w; ofs = 0; 1st elem)",             both(z, op( g->add(s->x, s->x, g->x28); g->ldp(d[1].w, d[0].w,   ptr(s->x)) ), op_init( g->eor(g->x0, g->x0, g->x0) ), add_latency, lat_inc2_pattern, thr_skip2_patterns));
	t.put("ldp (w; ofs = 0; 2nd elem)",             both(z, op( g->add(s->x, s->x, g->x28); g->ldp(d[0].w, d[1].w,   ptr(s->x)) ), op_init( g->eor(g->x0, g->x0, g->x0) ), add_latency, lat_inc2_pattern, thr_skip2_patterns));
	t.put("ldp (x; ofs = 0; 1st elem; unaligned)",  both(u, op(                             g->ldp(d[1].x, d[0].x,   ptr(s->x)) ),                                         0.0,         lat_inc2_pattern, thr_skip2_patterns));
	t.put("ldp (x; ofs = 0; 2nd elem; unaligned)",  both(u, op(                             g->ldp(d[0].x, d[1].x,   ptr(s->x)) ),                                         0.0,         lat_inc2_pattern, thr_skip2_patterns));
	t.put("ldp (w; ofs = 0; 1st elem; unaligned)",  both(y, op( g->add(s->x, s->x, g->x28); g->ldp(d[1].w, d[0].w,   ptr(s->x)) ), op_init( g->eor(g->x0, g->x0, g->x0) ), add_latency, lat_inc2_pattern, thr_skip2_patterns));
	t.put("ldp (w; ofs = 0; 2nd elem; unaligned)",  both(y, op( g->add(s->x, s->x, g->x28); g->ldp(d[0].w, d[1].w,   ptr(s->x)) ), op_init( g->eor(g->x0, g->x0, g->x0) ), add_latency, lat_inc2_pattern, thr_skip2_patterns));

	t.put("ldpsw (ofs = 0; 1st elem)",              both(z, op( g->add(s->x, s->x, g->x28); g->ldpsw(d[1].x, d[0].x, ptr(s->x)) ), op_init( g->eor(g->x0, g->x0, g->x0) ), add_latency, lat_inc2_pattern, thr_skip2_patterns));
	t.put("ldpsw (ofs = 0; 2nd elem)",              both(z, op( g->add(s->x, s->x, g->x28); g->ldpsw(d[0].x, d[1].x, ptr(s->x)) ), op_init( g->eor(g->x0, g->x0, g->x0) ), add_latency, lat_inc2_pattern, thr_skip2_patterns));
	t.put("ldpsw (ofs = 0; 1st elem; unaligned)",   both(y, op( g->add(s->x, s->x, g->x28); g->ldpsw(d[1].x, d[0].x, ptr(s->x)) ), op_init( g->eor(g->x0, g->x0, g->x0) ), add_latency, lat_inc2_pattern, thr_skip2_patterns));
	t.put("ldpsw (ofs = 0; 2nd elem; unaligned)",   both(y, op( g->add(s->x, s->x, g->x28); g->ldpsw(d[0].x, d[1].x, ptr(s->x)) ), op_init( g->eor(g->x0, g->x0, g->x0) ), add_latency, lat_inc2_pattern, thr_skip2_patterns));
	t.put("ldnp",                                   thr(h,  op(                             g->ldnp(d[0].x, d[1].x,  ptr(s->x)) )));
	return;
}

#if 0
void bench_load_vec_2(bool md, double freq) {
	/*
	 * Measuring x -> d -> x round-trip latency on various settings:
	 * It seems several archs have x -> d and d -> x feeder paths connected just after the reservation station,
	 * resulting in a latency is hidden when an add (or similar arithmetic) operation follows the transfer.
	 * The following settings are collecting clues for that. Note that the simplest transfer from x to d is fmov,
	 * which does not insert nor duplicate the transferred element (the upper 64bit is just cleared), which shows
	 * the smallest latency (as small as 2 cycles??) among various transfer operations on the A72 uarch.
	 */
	memmgr m_h(mem_init( p ));
	memmgr m_u(mem_init( p ), 1);
	memmgr m_z(mem_init( (void *)0 ));
	memmgr m_y(mem_init( (void *)0 ), 1);

	bench h(freq, m_h.ptr(), m_h.ptr(), 0, 0);
	bench u(freq, m_u.ptr(), m_u.ptr(), 0, 0);
	bench z(freq, m_z.ptr(), m_z.ptr(), 0, 0);
	bench y(freq, m_y.ptr(), m_y.ptr(), 0, 0);

	t.put("mov-mov", lat(z, op( g->mov(g->v0.d[0], g->x0);                                           g->mov(g->x0, g->v0.d[0]) )));
	t.put("mov-mov", lat(z, op( g->mov(g->v0.d[0], g->x0);    g->add(g->v0.d, g->v0.d, g->v28.d);    g->mov(g->x0, g->v0.d[0]) )));
	t.put("mov-mov", lat(z, op( g->mov(g->v0.d[0], g->x0);    g->add(g->v0.s2, g->v0.s2, g->v28.s2); g->mov(g->x0, g->v0.d[0]) )));
	t.put("mov-mov", lat(z, op( g->mov(g->v0.d[0], g->x0);    g->ext(g->v0.b, g->v0.b, g->v28.b, 1); g->mov(g->x0, g->v0.d[0]) )));
	t.put("mov-mov", lat(z, op( g->mov(g->v0.d[0], g->x0);                                           g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x28) )));
	t.put("mov-mov", lat(z, op( g->mov(g->v0.d[0], g->x0);    g->add(g->v0.d, g->v0.d, g->v28.d);    g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x28) )));
	t.put("mov-mov", lat(z, op( g->mov(g->v0.d[0], g->x0);    g->add(g->v0.s2, g->v0.s2, g->v28.s2); g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x28) )));
	t.put("mov-mov", lat(z, op( g->mov(g->v0.d[0], g->x0);    g->ext(g->v0.b, g->v0.b, g->v28.b, 1); g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x28) )));

	t.put("fmov-mov", lat(z, op( g->fmov(g->d0, g->x0);                                              g->mov(g->x0, g->v0.d[0]) )));
	t.put("fmov-mov", lat(z, op( g->fmov(g->d0, g->x0);       g->add(g->v0.d, g->v0.d, g->v28.d);    g->mov(g->x0, g->v0.d[0]) )));
	t.put("fmov-mov", lat(z, op( g->fmov(g->d0, g->x0);       g->add(g->v0.s2, g->v0.s2, g->v28.s2); g->mov(g->x0, g->v0.d[0]) )));
	t.put("fmov-mov", lat(z, op( g->fmov(g->d0, g->x0);       g->ext(g->v0.b, g->v0.b, g->v28.b, 1); g->mov(g->x0, g->v0.d[0]) )));
	t.put("fmov-mov", lat(z, op( g->fmov(g->d0, g->x0);                                              g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x28) )));
	t.put("fmov-mov", lat(z, op( g->fmov(g->d0, g->x0);       g->add(g->v0.d, g->v0.d, g->v28.d);    g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x28) )));
	t.put("fmov-mov", lat(z, op( g->fmov(g->d0, g->x0);       g->add(g->v0.s2, g->v0.s2, g->v28.s2); g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x28) )));
	t.put("fmov-mov", lat(z, op( g->fmov(g->d0, g->x0);       g->ext(g->v0.b, g->v0.b, g->v28.b, 1); g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x28) )));

	t.put("fmov-fmov", lat(z, op( g->fmov(g->d0, g->x0);                                             g->fmov(g->x0, g->d0) )));
	t.put("fmov-fmov", lat(z, op( g->fmov(g->d0, g->x0);      g->add(g->v0.d, g->v0.d, g->v28.d);    g->fmov(g->x0, g->d0) )));
	t.put("fmov-fmov", lat(z, op( g->fmov(g->d0, g->x0);      g->add(g->v0.s2, g->v0.s2, g->v28.s2); g->fmov(g->x0, g->d0) )));
	t.put("fmov-fmov", lat(z, op( g->fmov(g->d0, g->x0);      g->ext(g->v0.b, g->v0.b, g->v28.b, 1); g->fmov(g->x0, g->d0) )));
	t.put("fmov-fmov", lat(z, op( g->fmov(g->d0, g->x0);                                             g->fmov(g->x0, g->d0); g->add(g->x0, g->x0, g->x28) )));
	t.put("fmov-fmov", lat(z, op( g->fmov(g->d0, g->x0);      g->add(g->v0.d, g->v0.d, g->v28.d);    g->fmov(g->x0, g->d0); g->add(g->x0, g->x0, g->x28) )));
	t.put("fmov-fmov", lat(z, op( g->fmov(g->d0, g->x0);      g->add(g->v0.s2, g->v0.s2, g->v28.s2); g->fmov(g->x0, g->d0); g->add(g->x0, g->x0, g->x28) )));
	t.put("fmov-fmov", lat(z, op( g->fmov(g->d0, g->x0);      g->ext(g->v0.b, g->v0.b, g->v28.b, 1); g->fmov(g->x0, g->d0); g->add(g->x0, g->x0, g->x28) )));

	t.put("dup-mov", lat(z, op( g->dup(g->v0.d, g->x0);                                              g->mov(g->x0, g->v0.d[0]) )));
	t.put("dup-mov", lat(z, op( g->dup(g->v0.d, g->x0);       g->add(g->v0.d, g->v0.d, g->v28.d);    g->mov(g->x0, g->v0.d[0]) )));
	t.put("dup-mov", lat(z, op( g->dup(g->v0.d, g->x0);       g->add(g->v0.s2, g->v0.s2, g->v28.s2); g->mov(g->x0, g->v0.d[0]) )));
	t.put("dup-mov", lat(z, op( g->dup(g->v0.d, g->x0);       g->ext(g->v0.b, g->v0.b, g->v28.b, 1); g->mov(g->x0, g->v0.d[0]) )));
	t.put("dup-mov", lat(z, op( g->dup(g->v0.d, g->x0);                                              g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x28) )));
	t.put("dup-mov", lat(z, op( g->dup(g->v0.d, g->x0);       g->add(g->v0.d, g->v0.d, g->v28.d);    g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x28) )));
	t.put("dup-mov", lat(z, op( g->dup(g->v0.d, g->x0);       g->add(g->v0.s2, g->v0.s2, g->v28.s2); g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x28) )));
	t.put("dup-mov", lat(z, op( g->dup(g->v0.d, g->x0);       g->ext(g->v0.b, g->v0.b, g->v28.b, 1); g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x28) )));

	t.put("dup-fmov", lat(z, op( g->dup(g->v0.d, g->x0);                                             g->fmov(g->x0, g->d0) )));
	t.put("dup-fmov", lat(z, op( g->dup(g->v0.d, g->x0);      g->add(g->v0.d, g->v0.d, g->v28.d);    g->fmov(g->x0, g->d0) )));
	t.put("dup-fmov", lat(z, op( g->dup(g->v0.d, g->x0);      g->add(g->v0.s2, g->v0.s2, g->v28.s2); g->fmov(g->x0, g->d0) )));
	t.put("dup-fmov", lat(z, op( g->dup(g->v0.d, g->x0);      g->ext(g->v0.b, g->v0.b, g->v28.b, 1); g->fmov(g->x0, g->d0) )));
	t.put("dup-fmov", lat(z, op( g->dup(g->v0.d, g->x0);                                             g->fmov(g->x0, g->d0); g->add(g->x0, g->x0, g->x28) )));
	t.put("dup-fmov", lat(z, op( g->dup(g->v0.d, g->x0);      g->add(g->v0.d, g->v0.d, g->v28.d);    g->fmov(g->x0, g->d0); g->add(g->x0, g->x0, g->x28) )));
	t.put("dup-fmov", lat(z, op( g->dup(g->v0.d, g->x0);      g->add(g->v0.s2, g->v0.s2, g->v28.s2); g->fmov(g->x0, g->d0); g->add(g->x0, g->x0, g->x28) )));
	t.put("dup-fmov", lat(z, op( g->dup(g->v0.d, g->x0);      g->ext(g->v0.b, g->v0.b, g->v28.b, 1); g->fmov(g->x0, g->d0); g->add(g->x0, g->x0, g->x28) )));

	t.put("ldr-mov", lat(h, op( g->ldr(g->q0, ptr(g->x0, 0));                                        g->mov(g->x0, g->v0.d[0]) )));
	t.put("ldr-mov", lat(h, op( g->ldr(g->q0, ptr(g->x0, 0)); g->add(g->v0.d, g->v0.d, g->v28.d);    g->mov(g->x0, g->v0.d[0]) )));
	t.put("ldr-mov", lat(h, op( g->ldr(g->q0, ptr(g->x0, 0)); g->add(g->v0.s2, g->v0.s2, g->v28.s2); g->mov(g->x0, g->v0.d[0]) )));
	t.put("ldr-mov", lat(h, op( g->ldr(g->q0, ptr(g->x0, 0)); g->ext(g->v0.b, g->v0.b, g->v28.b, 8); g->mov(g->x0, g->v0.d[0]) )));
	t.put("ldr-mov", lat(z, op( g->ldr(g->q0, ptr(g->x0, 0));                                        g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x1) )));
	t.put("ldr-mov", lat(z, op( g->ldr(g->q0, ptr(g->x0, 0)); g->add(g->v0.d, g->v0.d, g->v28.d);    g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x1) )));
	t.put("ldr-mov", lat(z, op( g->ldr(g->q0, ptr(g->x0, 0)); g->add(g->v0.s2, g->v0.s2, g->v28.s2); g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x1) )));
	t.put("ldr-mov", lat(z, op( g->ldr(g->q0, ptr(g->x0, 0)); g->ext(g->v0.b, g->v0.b, g->v28.b, 1); g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x1) )));

	t.put("ldr-mov", lat(u, op( g->ldr(g->q0, ptr(g->x0, 0));                                        g->mov(g->x0, g->v0.d[0]) )));
	t.put("ldr-mov", lat(u, op( g->ldr(g->q0, ptr(g->x0, 0)); g->add(g->v0.d, g->v0.d, g->v28.d);    g->mov(g->x0, g->v0.d[0]) )));
	t.put("ldr-mov", lat(u, op( g->ldr(g->q0, ptr(g->x0, 0)); g->add(g->v0.s2, g->v0.s2, g->v28.s2); g->mov(g->x0, g->v0.d[0]) )));
	t.put("ldr-mov", lat(u, op( g->ldr(g->q0, ptr(g->x0, 0)); g->ext(g->v0.b, g->v0.b, g->v28.b, 8); g->mov(g->x0, g->v0.d[0]) )));
	t.put("ldr-mov", lat(y, op( g->ldr(g->q0, ptr(g->x0, 0));                                        g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x1) )));
	t.put("ldr-mov", lat(y, op( g->ldr(g->q0, ptr(g->x0, 0)); g->add(g->v0.d, g->v0.d, g->v28.d);    g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x1) )));
	t.put("ldr-mov", lat(y, op( g->ldr(g->q0, ptr(g->x0, 0)); g->add(g->v0.s2, g->v0.s2, g->v28.s2); g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x1) )));
	t.put("ldr-mov", lat(y, op( g->ldr(g->q0, ptr(g->x0, 0)); g->ext(g->v0.b, g->v0.b, g->v28.b, 1); g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x1) )));
	return;
}
#endif

void bench_load_vec(bool md, double freq) {
	table t(md, "Vector load (might be incorrect due to a wrong d -> x latency estimation)");

	memmgr m_h(mem_init( p ));
	memmgr m_u(mem_init( p ), 1);
	memmgr m_z(mem_init( (void *)0 ));
	memmgr m_y(mem_init( (void *)0 ), 1);

	bench h(freq, m_h.ptr(), m_h.ptr(), 0, 0);
	bench u(freq, m_u.ptr(), m_u.ptr(), 0, 0);
	bench z(freq, m_z.ptr(), m_z.ptr(), 0, 0);
	bench y(freq, m_y.ptr(), m_y.ptr(), 0, 0);

	lat_i(freq, op( g->add(d->x, d->x, s->x) ));
	lat_i(freq, op( g->add(d->x, d->x, s->x) ));
	lat_i(freq, op( g->add(d->x, d->x, s->x) ));

	#define fwd_lo(_g, _s, _d)		({ g->add((_s)->v.d, (_s)->v.d, (_g)->v28.d); (_g)->mov((_d)->x, (_s)->v.d[0]); (_g)->add((_d)->x, (_d)->x, (_g)->x28); })
	#define fwd_hi(_g, _s, _d)		({ g->add((_s)->v.d, (_s)->v.d, (_g)->v28.d); (_g)->mov((_d)->x, (_s)->v.d[1]); (_g)->add((_d)->x, (_d)->x, (_g)->x28); })

	/* FIXME; would be incorrect inferrence */
	double const fmov_mov_add = lat(z, op( g->fmov(d->d, g->x0); g->mov(d->x, d->v.d[0]); g->add(d->x, d->x, g->x28) )).lat;
	double const fmov_fwd     = lat(z, op( g->fmov(d->d, s->x);  fwd_lo(g, d, s) )).lat;
	double const fwd_latency  = floor(fmov_fwd - fmov_mov_add / 2.0 + 0.5);

	t.put("ldr.q (imm; ofs = 0)",                   both(z, op( g->ldr(d->q,                ptr(s->x, 0)) ),  op( fwd_lo(g, d, s) ),     fwd_latency, lat_flat0_pattern,      thr_patterns));
	t.put("ldr.q (imm; ofs = 16)",                  both(z, op( g->ldr(d->q,                ptr(s->x, 16)) ), op( fwd_lo(g, d, s) ),     fwd_latency, lat_flat0_pattern,      thr_patterns));
	t.put("ldr.q (imm; ofs = 0; unaligned)",        both(y, op( g->ldr(d->q,                ptr(s->x, 0)) ),  op( fwd_lo(g, d, s) ),     fwd_latency, lat_flat0_pattern,      thr_patterns));
	t.put("ldr.q (imm; ofs = 16; unaligned)",       both(y, op( g->ldr(d->q,                ptr(s->x, 16)) ), op( fwd_lo(g, d, s) ),     fwd_latency, lat_flat0_pattern,      thr_patterns));
	t.put("ldur.q (imm; ofs = 0)",                  both(z, op( g->ldur(d->q,               ptr(s->x, 0)) ),  op( fwd_lo(g, d, s) ),     fwd_latency, lat_flat0_pattern,      thr_patterns));
	t.put("ldur.q (imm; ofs = 16)",                 both(z, op( g->ldur(d->q,               ptr(s->x, 16)) ), op( fwd_lo(g, d, s) ),     fwd_latency, lat_flat0_pattern,      thr_patterns));
	t.put("ldur.q (imm; ofs = 0; unaligned)",       both(y, op( g->ldur(d->q,               ptr(s->x, 0)) ),  op( fwd_lo(g, d, s) ),     fwd_latency, lat_flat0_pattern,      thr_patterns));
	t.put("ldur.q (imm; ofs = 16; unaligned)",      both(y, op( g->ldur(d->q,               ptr(s->x, 16)) ), op( fwd_lo(g, d, s) ),     fwd_latency, lat_flat0_pattern,      thr_patterns));

	t.put("ldp.q (ofs = 0; 1st elem)",              both(z, op( g->ldp(d[1].q, d[0].q,      ptr(s->x)) ),     op( fwd_lo(g, &d[1], s) ), fwd_latency, lat_flat0_pattern,      thr_skip2_patterns));
	t.put("ldp.q (ofs = 0; 2nd elem)",              both(z, op( g->ldp(d[0].q, d[1].q,      ptr(s->x)) ),     op( fwd_lo(g, &d[1], s) ), fwd_latency, lat_flat0_pattern,      thr_skip2_patterns));
	t.put("ldp.q (ofs = 0; unaligned)",             both(y, op( g->ldp(d[0].q, d[1].q,      ptr(s->x)) ),     op( fwd_lo(g, &d[1], s) ), fwd_latency, lat_flat0_pattern,      thr_skip2_patterns));
	t.put("ldnp.q",                                 thr(z,  op( g->ldnp(d[0].q, d[1].q,     ptr(s->x)) )));

	t.put("ld1.b (multi; 1 reg)",                   both(z, op( g->ld1(d[0].v.b - d[0].v.b, ptr(s->x)) ),     op( fwd_lo(g, &d[0], s) ), fwd_latency, &lat_flat_patterns[3], thr_skip1x_patterns));
	t.put("ld1.b (multi; 2 reg)",                   both(z, op( g->ld1(d[0].v.b - d[1].v.b, ptr(s->x)) ),     op( fwd_lo(g, &d[1], s) ), fwd_latency, &lat_flat_patterns[2], thr_skip2x_patterns));
	t.put("ld1.b (multi; 3 reg)",                   both(z, op( g->ld1(d[0].v.b - d[2].v.b, ptr(s->x)) ),     op( fwd_lo(g, &d[2], s) ), fwd_latency, &lat_flat_patterns[1], thr_skip3x_patterns));
	t.put("ld1.b (multi; 4 reg)",                   both(z, op( g->ld1(d[0].v.b - d[3].v.b, ptr(s->x)) ),     op( fwd_lo(g, &d[3], s) ), fwd_latency, &lat_flat_patterns[0], thr_skip4x_patterns));

	t.put("ld1.h (multi; 1 reg)",                   both(z, op( g->ld1(d[0].v.h - d[0].v.h, ptr(s->x)) ),     op( fwd_lo(g, &d[0], s) ), fwd_latency, &lat_flat_patterns[3], thr_skip1x_patterns));
	t.put("ld1.h (multi; 2 reg)",                   both(z, op( g->ld1(d[0].v.h - d[1].v.h, ptr(s->x)) ),     op( fwd_lo(g, &d[1], s) ), fwd_latency, &lat_flat_patterns[2], thr_skip2x_patterns));
	t.put("ld1.h (multi; 3 reg)",                   both(z, op( g->ld1(d[0].v.h - d[2].v.h, ptr(s->x)) ),     op( fwd_lo(g, &d[2], s) ), fwd_latency, &lat_flat_patterns[1], thr_skip3x_patterns));
	t.put("ld1.h (multi; 4 reg)",                   both(z, op( g->ld1(d[0].v.h - d[3].v.h, ptr(s->x)) ),     op( fwd_lo(g, &d[3], s) ), fwd_latency, &lat_flat_patterns[0], thr_skip4x_patterns));

	t.put("ld1.s (multi; 1 reg)",                   both(z, op( g->ld1(d[0].v.s - d[0].v.s, ptr(s->x)) ),     op( fwd_lo(g, &d[0], s) ), fwd_latency, &lat_flat_patterns[3], thr_skip1x_patterns));
	t.put("ld1.s (multi; 2 reg)",                   both(z, op( g->ld1(d[0].v.s - d[1].v.s, ptr(s->x)) ),     op( fwd_lo(g, &d[1], s) ), fwd_latency, &lat_flat_patterns[2], thr_skip2x_patterns));
	t.put("ld1.s (multi; 3 reg)",                   both(z, op( g->ld1(d[0].v.s - d[2].v.s, ptr(s->x)) ),     op( fwd_lo(g, &d[2], s) ), fwd_latency, &lat_flat_patterns[1], thr_skip3x_patterns));
	t.put("ld1.s (multi; 4 reg)",                   both(z, op( g->ld1(d[0].v.s - d[3].v.s, ptr(s->x)) ),     op( fwd_lo(g, &d[3], s) ), fwd_latency, &lat_flat_patterns[0], thr_skip4x_patterns));

	t.put("ld1.d (multi; 1 reg)",                   both(z, op( g->ld1(d[0].v.d - d[0].v.d, ptr(s->x)) ),     op( fwd_lo(g, &d[0], s) ), fwd_latency, &lat_flat_patterns[3], thr_skip1x_patterns));
	t.put("ld1.d (multi; 2 reg)",                   both(z, op( g->ld1(d[0].v.d - d[1].v.d, ptr(s->x)) ),     op( fwd_lo(g, &d[1], s) ), fwd_latency, &lat_flat_patterns[2], thr_skip2x_patterns));
	t.put("ld1.d (multi; 3 reg)",                   both(z, op( g->ld1(d[0].v.d - d[2].v.d, ptr(s->x)) ),     op( fwd_lo(g, &d[2], s) ), fwd_latency, &lat_flat_patterns[1], thr_skip3x_patterns));
	t.put("ld1.d (multi; 4 reg)",                   both(z, op( g->ld1(d[0].v.d - d[3].v.d, ptr(s->x)) ),     op( fwd_lo(g, &d[3], s) ), fwd_latency, &lat_flat_patterns[0], thr_skip4x_patterns));

	t.put("ld2.b (multi)",                          both(z, op( g->ld2(d->v.b, ptr(s->x)) ),                  op( fwd_lo(g, &d[1], s) ), fwd_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.b (multi)",                          both(z, op( g->ld3(d->v.b, ptr(s->x)) ),                  op( fwd_lo(g, &d[2], s) ), fwd_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.b (multi)",                          both(z, op( g->ld4(d->v.b, ptr(s->x)) ),                  op( fwd_lo(g, &d[3], s) ), fwd_latency, &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld2.h (multi)",                          both(z, op( g->ld2(d->v.h, ptr(s->x)) ),                  op( fwd_lo(g, &d[1], s) ), fwd_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.h (multi)",                          both(z, op( g->ld3(d->v.h, ptr(s->x)) ),                  op( fwd_lo(g, &d[2], s) ), fwd_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.h (multi)",                          both(z, op( g->ld4(d->v.h, ptr(s->x)) ),                  op( fwd_lo(g, &d[3], s) ), fwd_latency, &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld2.s (multi)",                          both(z, op( g->ld2(d->v.s, ptr(s->x)) ),                  op( fwd_lo(g, &d[1], s) ), fwd_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.s (multi)",                          both(z, op( g->ld3(d->v.s, ptr(s->x)) ),                  op( fwd_lo(g, &d[2], s) ), fwd_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.s (multi)",                          both(z, op( g->ld4(d->v.s, ptr(s->x)) ),                  op( fwd_lo(g, &d[3], s) ), fwd_latency, &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld2.d (multi)",                          both(z, op( g->ld2(d->v.d, ptr(s->x)) ),                  op( fwd_lo(g, &d[1], s) ), fwd_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.d (multi)",                          both(z, op( g->ld3(d->v.d, ptr(s->x)) ),                  op( fwd_lo(g, &d[2], s) ), fwd_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.d (multi)",                          both(z, op( g->ld4(d->v.d, ptr(s->x)) ),                  op( fwd_lo(g, &d[3], s) ), fwd_latency, &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1.b (single; [15])",                   both(z, op( g->ld1(s[3].v.b[15], ptr(s->x)) ),            op( fwd_hi(g, &s[3], s) ), fwd_latency, &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2.b (single; [15])",                   both(z, op( g->ld2(s[2].v.b[15], ptr(s->x)) ),            op( fwd_hi(g, &s[3], s) ), fwd_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.b (single; [15])",                   both(z, op( g->ld3(s[1].v.b[15], ptr(s->x)) ),            op( fwd_hi(g, &s[3], s) ), fwd_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.b (single; [15])",                   both(z, op( g->ld4(s[0].v.b[15], ptr(s->x)) ),            op( fwd_hi(g, &s[3], s) ), fwd_latency, &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1.h (single; [7])",                    both(z, op( g->ld1(s[3].v.h[7], ptr(s->x)) ),             op( fwd_hi(g, &s[3], s) ), fwd_latency, &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2.h (single; [7])",                    both(z, op( g->ld2(s[2].v.h[7], ptr(s->x)) ),             op( fwd_hi(g, &s[3], s) ), fwd_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.h (single; [7])",                    both(z, op( g->ld3(s[1].v.h[7], ptr(s->x)) ),             op( fwd_hi(g, &s[3], s) ), fwd_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.h (single; [7])",                    both(z, op( g->ld4(s[0].v.h[7], ptr(s->x)) ),             op( fwd_hi(g, &s[3], s) ), fwd_latency, &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1.s (single; [3])",                    both(z, op( g->ld1(s[3].v.s[3], ptr(s->x)) ),             op( fwd_hi(g, &s[3], s) ), fwd_latency, &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2.s (single; [3])",                    both(z, op( g->ld2(s[2].v.s[3], ptr(s->x)) ),             op( fwd_hi(g, &s[3], s) ), fwd_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.s (single; [3])",                    both(z, op( g->ld3(s[1].v.s[3], ptr(s->x)) ),             op( fwd_hi(g, &s[3], s) ), fwd_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.s (single; [3])",                    both(z, op( g->ld4(s[0].v.s[3], ptr(s->x)) ),             op( fwd_hi(g, &s[3], s) ), fwd_latency, &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1.d (single; [1])",                    both(z, op( g->ld1(s[3].v.d[1], ptr(s->x)) ),             op( fwd_hi(g, &s[3], s) ), fwd_latency, &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2.d (single; [1])",                    both(z, op( g->ld2(s[2].v.d[1], ptr(s->x)) ),             op( fwd_hi(g, &s[3], s) ), fwd_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.d (single; [1])",                    both(z, op( g->ld3(s[1].v.d[1], ptr(s->x)) ),             op( fwd_hi(g, &s[3], s) ), fwd_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.d (single; [1])",                    both(z, op( g->ld4(s[0].v.d[1], ptr(s->x)) ),             op( fwd_hi(g, &s[3], s) ), fwd_latency, &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1r.b",                                 both(z, op( g->ld1r(d->v.b, ptr(s->x)) ),                 op( fwd_lo(g, &d[0], s) ), fwd_latency, &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2r.b",                                 both(z, op( g->ld2r(d->v.b, ptr(s->x)) ),                 op( fwd_lo(g, &d[1], s) ), fwd_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3r.b",                                 both(z, op( g->ld3r(d->v.b, ptr(s->x)) ),                 op( fwd_lo(g, &d[2], s) ), fwd_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4r.b",                                 both(z, op( g->ld4r(d->v.b, ptr(s->x)) ),                 op( fwd_lo(g, &d[3], s) ), fwd_latency, &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1r.h",                                 both(z, op( g->ld1r(d->v.h, ptr(s->x)) ),                 op( fwd_lo(g, &d[0], s) ), fwd_latency, &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2r.h",                                 both(z, op( g->ld2r(d->v.h, ptr(s->x)) ),                 op( fwd_lo(g, &d[1], s) ), fwd_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3r.h",                                 both(z, op( g->ld3r(d->v.h, ptr(s->x)) ),                 op( fwd_lo(g, &d[2], s) ), fwd_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4r.h",                                 both(z, op( g->ld4r(d->v.h, ptr(s->x)) ),                 op( fwd_lo(g, &d[3], s) ), fwd_latency, &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1r.s",                                 both(z, op( g->ld1r(d->v.s, ptr(s->x)) ),                 op( fwd_lo(g, &d[0], s) ), fwd_latency, &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2r.s",                                 both(z, op( g->ld2r(d->v.s, ptr(s->x)) ),                 op( fwd_lo(g, &d[1], s) ), fwd_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3r.s",                                 both(z, op( g->ld3r(d->v.s, ptr(s->x)) ),                 op( fwd_lo(g, &d[2], s) ), fwd_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4r.s",                                 both(z, op( g->ld4r(d->v.s, ptr(s->x)) ),                 op( fwd_lo(g, &d[3], s) ), fwd_latency, &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1r.d",                                 both(z, op( g->ld1r(d->v.d, ptr(s->x)) ),                 op( fwd_lo(g, &d[0], s) ), fwd_latency, &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2r.d",                                 both(z, op( g->ld2r(d->v.d, ptr(s->x)) ),                 op( fwd_lo(g, &d[1], s) ), fwd_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3r.d",                                 both(z, op( g->ld3r(d->v.d, ptr(s->x)) ),                 op( fwd_lo(g, &d[2], s) ), fwd_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4r.d",                                 both(z, op( g->ld4r(d->v.d, ptr(s->x)) ),                 op( fwd_lo(g, &d[3], s) ), fwd_latency, &lat_flat_patterns[0], thr_skip4_patterns));
	return;
}

/*
 * end of load.cc
 */
