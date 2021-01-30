
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
	fprintf(stderr, "%f\n", adj8_latency);

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

void bench_load_vec(bool md, double freq) {
	table t(md, "Vector load");

	memmgr m_h(mem_init( p ));
	memmgr m_u(mem_init( p ), 1);
	memmgr m_z(mem_init( (void *)0 ));

	bench h(freq, m_h.ptr(), m_h.ptr(), 0, 0);
	bench u(freq, m_u.ptr(), m_u.ptr(), 0, 0);
	bench z(freq, m_z.ptr(), 0, 0, 0);

	// memmgr m_m(mem_init( &p[i - 2] ));
	// memmgr m_n(mem_init( &p[i - 2] ), 1);
	// bench m(freq, m_m.ptr() + 2);
	// bench n(freq, m_n.ptr() + 2);
	// bench y(freq, ((uint8_t *)m_z.ptr()) + 1);

	// printf("%p, %p, %p\n", head, head_m16, zero);

	lat_i(freq, op( g->add(d->x, d->x, s->x) ));
	lat_i(freq, op( g->add(d->x, d->x, s->x) ));
	lat_i(freq, op( g->add(d->x, d->x, s->x) ));


	t.put("mov-mov", lat(z, op( g->mov(g->v0.d[0], g->x0);                                     g->mov(g->x0, g->v0.d[0]) )));
	t.put("mov-mov", lat(z, op( g->mov(g->v0.d[0], g->x0); g->add(g->v0.d, g->v0.d, g->v28.d); g->mov(g->x0, g->v0.d[0]) )));
	t.put("mov-mov", lat(z, op( g->mov(g->v0.d[0], g->x0); g->ext(g->v0.b, g->v0.b, g->v28.b, 1); g->mov(g->x0, g->v0.d[0]) )));
	t.put("mov-mov", lat(z, op( g->mov(g->v0.d[0], g->x0);                                     g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x28) )));
	t.put("mov-mov", lat(z, op( g->mov(g->v0.d[0], g->x0); g->add(g->v0.d, g->v0.d, g->v28.d); g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x28) )));
	t.put("mov-mov", lat(z, op( g->mov(g->v0.d[0], g->x0); g->ext(g->v0.b, g->v0.b, g->v28.b, 1); g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x28) )));

	t.put("fmov-mov", lat(z, op( g->fmov(g->d0, g->x0);                                     g->mov(g->x0, g->v0.d[0]) )));
	t.put("fmov-mov", lat(z, op( g->fmov(g->d0, g->x0); g->add(g->v0.d, g->v0.d, g->v28.d); g->mov(g->x0, g->v0.d[0]) )));
	t.put("fmov-mov", lat(z, op( g->fmov(g->d0, g->x0); g->ext(g->v0.b, g->v0.b, g->v28.b, 1); g->mov(g->x0, g->v0.d[0]) )));
	t.put("fmov-mov", lat(z, op( g->fmov(g->d0, g->x0);                                     g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x28) )));
	t.put("fmov-mov", lat(z, op( g->fmov(g->d0, g->x0); g->add(g->v0.d, g->v0.d, g->v28.d); g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x28) )));
	t.put("fmov-mov", lat(z, op( g->fmov(g->d0, g->x0); g->ext(g->v0.b, g->v0.b, g->v28.b, 1); g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x28) )));

	t.put("fmov-fmov", lat(z, op( g->fmov(g->d0, g->x0);                                     g->fmov(g->x0, g->d0) )));
	t.put("fmov-fmov", lat(z, op( g->fmov(g->d0, g->x0); g->add(g->v0.d, g->v0.d, g->v28.d); g->fmov(g->x0, g->d0) )));
	t.put("fmov-fmov", lat(z, op( g->fmov(g->d0, g->x0); g->ext(g->v0.b, g->v0.b, g->v28.b, 1); g->fmov(g->x0, g->d0) )));
	t.put("fmov-fmov", lat(z, op( g->fmov(g->d0, g->x0);                                     g->fmov(g->x0, g->d0); g->add(g->x0, g->x0, g->x28) )));
	t.put("fmov-fmov", lat(z, op( g->fmov(g->d0, g->x0); g->add(g->v0.d, g->v0.d, g->v28.d); g->fmov(g->x0, g->d0); g->add(g->x0, g->x0, g->x28) )));
	t.put("fmov-fmov", lat(z, op( g->fmov(g->d0, g->x0); g->ext(g->v0.b, g->v0.b, g->v28.b, 1); g->fmov(g->x0, g->d0); g->add(g->x0, g->x0, g->x28) )));

	t.put("dup-mov", lat(z, op( g->dup(g->v0.d, g->x0);                                     g->mov(g->x0, g->v0.d[0]) )));
	t.put("dup-mov", lat(z, op( g->dup(g->v0.d, g->x0); g->add(g->v0.d, g->v0.d, g->v28.d); g->mov(g->x0, g->v0.d[0]) )));
	t.put("dup-mov", lat(z, op( g->dup(g->v0.d, g->x0); g->ext(g->v0.b, g->v0.b, g->v28.b, 1); g->mov(g->x0, g->v0.d[0]) )));
	t.put("dup-mov", lat(z, op( g->dup(g->v0.d, g->x0);                                     g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x28) )));
	t.put("dup-mov", lat(z, op( g->dup(g->v0.d, g->x0); g->add(g->v0.d, g->v0.d, g->v28.d); g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x28) )));
	t.put("dup-mov", lat(z, op( g->dup(g->v0.d, g->x0); g->ext(g->v0.b, g->v0.b, g->v28.b, 1); g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x28) )));

	t.put("dup-fmov", lat(z, op( g->dup(g->v0.d, g->x0);                                     g->fmov(g->x0, g->d0) )));
	t.put("dup-fmov", lat(z, op( g->dup(g->v0.d, g->x0); g->add(g->v0.d, g->v0.d, g->v28.d); g->fmov(g->x0, g->d0) )));
	t.put("dup-fmov", lat(z, op( g->dup(g->v0.d, g->x0); g->ext(g->v0.b, g->v0.b, g->v28.b, 1); g->fmov(g->x0, g->d0) )));
	t.put("dup-fmov", lat(z, op( g->dup(g->v0.d, g->x0);                                     g->fmov(g->x0, g->d0); g->add(g->x0, g->x0, g->x28) )));
	t.put("dup-fmov", lat(z, op( g->dup(g->v0.d, g->x0); g->add(g->v0.d, g->v0.d, g->v28.d); g->fmov(g->x0, g->d0); g->add(g->x0, g->x0, g->x28) )));
	t.put("dup-fmov", lat(z, op( g->dup(g->v0.d, g->x0); g->ext(g->v0.b, g->v0.b, g->v28.b, 1); g->fmov(g->x0, g->d0); g->add(g->x0, g->x0, g->x28) )));

	t.put("ldr-mov", lat(h, op( g->ldr(g->q0, ptr(g->x0, 0));                                     g->mov(g->x0, g->v0.d[0]) )));
	t.put("ldr-mov", lat(h, op( g->ldr(g->q0, ptr(g->x0, 0)); g->add(g->v0.d, g->v0.d, g->v28.d); g->mov(g->x0, g->v0.d[0]) )));
	t.put("ldr-mov", lat(h, op( g->ldr(g->q0, ptr(g->x0, 0)); g->add(g->v0.d, g->v0.d, g->v28.d); g->mov(g->x0, g->v0.d[0]) )));
	t.put("ldr-mov", lat(z, op( g->ldr(g->q0, ptr(g->x0, 0));                                     g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x1) )));
	t.put("ldr-mov", lat(z, op( g->ldr(g->q0, ptr(g->x0, 0)); g->add(g->v0.d, g->v0.d, g->v28.d); g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x1) )));
	t.put("ldr-mov", lat(z, op( g->ldr(g->q0, ptr(g->x0, 0)); g->add(g->v0.d, g->v0.d, g->v28.d); g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x1) )));
/*
	t.put("0", lat(z, op( g->ldur(g->q0, ptr(g->x0, 0)); g->add(g->v0.d, g->v0.d, g->v28.d); g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x1) )));
	t.put("0", lat(z, op( g->ldur(g->q0, ptr(g->x0, 1)); g->add(g->v0.d, g->v0.d, g->v28.d); g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x1) )));
	t.put("0", lat(z, op( g->ldur(g->q0, ptr(g->x0, 63)); g->add(g->v0.d, g->v0.d, g->v28.d); g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x1) )));
	t.put("0", lat(z, op( g->ldur(g->q0, ptr(g->x0, 16)); g->add(g->v0.d, g->v0.d, g->v28.d); g->mov(g->x0, g->v0.d[0]); g->add(g->x0, g->x0, g->x1) )));
	t.put("0", lat(z, op( g->mov(d->v.s[0], s->w);                                   g->mov(d->w, d->v.s[0]) )));
	t.put("0", lat(z, op( g->dup(d->v.d, s->x);                                      g->mov(d->x, d->v.d[0]) )));
	t.put("0", lat(z, op( g->dup(d->v.s, s->w);                                      g->mov(d->w, d->v.s[0]) )));
*/
	t.put("1", lat(z, op(                                                                g->fcsel(d->d, d->d, s->d, Cond::NE) )));
	t.put("1", lat(z, op(                          g->fccmp(s->d, s->d, 0x00, Cond::NE); g->fcsel(d->d, d->d, s->d, Cond::NE) )));
	t.put("1", lat(z, op( g->ldr(d->d, ptr(s->x)); g->fccmp(d->d, s->d, 0x00, Cond::NE); g->adc(d->x, d->x, g->x28) )));
	t.put("1", lat(z, op(                          g->fccmp(s->d, s->d, 0x00, Cond::NE); g->adcs(d->x, d->x, g->x28) )));
	t.put("1", lat(z, op( g->ldr(g->d0, ptr(g->x0)); g->adcs(g->x0, g->x0, g->x28); g->fccmp(g->d0, g->d1, 0x00, Cond::NE) )));
	t.put("1", lat(z, op( g->ldr(g->x1, ptr(g->x0)); g->add(g->x0, g->x0, g->x1); g->ldr(g->d0, ptr(g->x0)); g->adcs(g->x0, g->x0, g->x28); g->fccmp(g->d0, g->d1, 0x00, Cond::NE) )));
	t.put("1", lat(z, op( g->ldr(g->x1, ptr(g->x0)); g->add(g->x0, g->x0, g->x1); g->ldr(g->d0, ptr(g->x0)); g->adc(g->x0, g->x0, g->x28); g->adds(g->x0, g->x0, g->x28); g->fccmp(g->d0, g->d1, 0x00, Cond::NE) )));
	t.put("1", lat(z, op( g->ldr(g->x1, ptr(g->x0)); g->add(g->x0, g->x0, g->x1); g->ldr(g->d0, ptr(g->x0)); g->fccmp(g->d0, g->d1, 0x00, Cond::NE); g->adc(g->x0, g->x0, g->x28); g->add(g->x0, g->x0, g->x28); g->adds(g->x0, g->x0, g->x28) )));
	t.put("1", lat(z, op( g->ldr(g->x1, ptr(g->x0)); g->add(g->x0, g->x0, g->x1); g->ldr(g->d0, ptr(g->x0)); g->fccmp(g->d0, g->d1, 0x00, Cond::NE); g->adc(g->x0, g->x0, g->x28); g->add(g->x0, g->x0, g->x28); g->add(g->x0, g->x0, g->x28); g->adds(g->x0, g->x0, g->x28) )));
	t.put("1", lat(z, op( g->ldr(g->x1, ptr(g->x0)); g->add(g->x0, g->x0, g->x1); g->ldr(g->d0, ptr(g->x0)); g->fccmp(g->d0, g->d1, 0x00, Cond::NE); g->adc(g->x0, g->x0, g->x28); g->add(g->x0, g->x0, g->x28); g->add(g->x0, g->x0, g->x28); g->add(g->x0, g->x0, g->x28); g->adds(g->x0, g->x0, g->x28) )));
	// fprintf(stderr, "%f, %f, %f, %f, %f, %f, %f, %f, %f\n", lat0, lat1, lat2, lat3, lat4, lat5, lat6, lat7, lat8);


	double const lat10 = lat(z, op( g->ldr(g->q1, ptr(g->x0)); g->ldadd(g->x1, g->x1, ptr(g->x0)) )).lat;
	fprintf(stderr, "%f\n", lat10);

	double const mov1 = lat_i(freq, op( g->mov(d->v.d[0], s->x);                                   g->mov(d->x, d->v.d[0]) ));
	double const mov2 = lat_i(freq, op( g->mov(d->v.s[0], s->w); g->mov(d->v.s[1], s->w);          g->mov(d->x, d->v.d[0]) ));
	double const mov6 = lat_i(freq, op( g->mov(d->v.d[0], s->x); g->mov(d->v.d[1], s->x);          g->mov(d->x, d->v.d[0]) ));
	double const mov7 = lat_i(freq, op( g->mov(d->v.d[1], s->x); g->mov(d->v.d[0], s->x);          g->mov(d->x, d->v.d[0]) ));
	double const mov3 = lat_i(freq, op( g->mov(d->v.d[0], s->x); g->add(d->v.s, d->v.s, g->v28.s); g->mov(d->x, d->v.d[0]); ));
	double const mov4 = lat_i(freq, op( g->mov(d->v.d[0], s->x);                                   g->mov(d->x, d->v.d[0]); g->add(d->x, d->x, g->x28) ));
	double const mov5 = lat_i(freq, op( g->mov(d->v.d[0], s->x); g->add(d->v.s, d->v.s, g->v28.s); g->mov(d->x, d->v.d[0]); g->add(d->x, d->x, g->x28) ));
	fprintf(stderr, "%f, %f, %f, %f, %f, %f, %f\n", mov1, mov2, mov6, mov7, mov3, mov4, mov5);

	/* note: this assumes ldr (simd; 64bit) and ldr (scalar; 64bit) have the same issue-to-arrive latency */
	#define fwd_x28(_g, _d)		({ (_g)->add((_d)->v.s2, (_d)->v.s2, (_g)->v28.s2); (_g)->mov((_d)->x, (_d)->v.d[0]); (_g)->add((_d)->x, (_d)->x, (_g)->x28); })
	#define fwd_zero(_g, _d)	({ (_g)->add((_d)->v.s2, (_d)->v.s2, (_g)->v28.s2); (_g)->mov((_d)->x, (_d)->v.d[0]); (_g)->add((_d)->x, (_d)->x, 0); })

	double const ldrx_latency     = lat(h, op( g->ldr(d->x, ptr(s->x)) ),                 0.0,          lat_inc1_pattern).lat;
	double const fwd_zero_latency = lat(h, op( g->ldr(d->d, ptr(s->x)); fwd_zero(g, d) ), ldrx_latency, lat_inc1_pattern).lat;
	double const fwd_x28_latency  = lat(z, op( g->ldr(d->d, ptr(s->x)); fwd_x28(g, d) ),  ldrx_latency, lat_inc1_pattern).lat;
	fprintf(stderr, "%f, %f, %f\n", ldrx_latency, fwd_x28_latency, fwd_zero_latency);

	// double const mov_latency     = lat_i(freq, op( g->mov(d->v.d[0], s->x); g->mov(d->x, d->v.d[0]) )) / 2.0;

	t.put("ldr.q (imm; ofs = 0)",                   both(h, op( g->ldr(d->q,                ptr(s->x, 0)) ),  op( fwd_zero(g, d) ), fwd_zero_latency, lat_inc1_pattern,      thr_patterns));
	t.put("ldr.q (imm; ofs = 16)",                  both(h, op( g->ldr(d->q,                ptr(s->x, 16)) ), op( fwd_zero(g, d) ), fwd_zero_latency, lat_inc1_pattern,      thr_patterns));
	t.put("ldr.q (imm; ofs = 0; unaligned)",        both(u, op( g->ldr(d->q,                ptr(s->x, 0)) ),  op( fwd_zero(g, d) ), fwd_zero_latency, lat_inc1_pattern,      thr_patterns));
	t.put("ldr.q (imm; ofs = 16; unaligned)",       both(u, op( g->ldr(d->q,                ptr(s->x, 16)) ), op( fwd_zero(g, d) ), fwd_zero_latency, lat_inc1_pattern,      thr_patterns));
	t.put("ldur.q (imm; ofs = 0)",                  both(h, op( g->ldur(d->q,               ptr(s->x, 0)) ),  op( fwd_zero(g, d) ), fwd_zero_latency, lat_inc1_pattern,      thr_patterns));
	t.put("ldur.q (imm; ofs = 16)",                 both(h, op( g->ldur(d->q,               ptr(s->x, 16)) ), op( fwd_zero(g, d) ), fwd_zero_latency, lat_inc1_pattern,      thr_patterns));
	t.put("ldur.q (imm; ofs = 0; unaligned)",       both(u, op( g->ldur(d->q,               ptr(s->x, 0)) ),  op( fwd_zero(g, d) ), fwd_zero_latency, lat_inc1_pattern,      thr_patterns));
	t.put("ldur.q (imm; ofs = 16; unaligned)",      both(u, op( g->ldur(d->q,               ptr(s->x, 16)) ), op( fwd_zero(g, d) ), fwd_zero_latency, lat_inc1_pattern,      thr_patterns));

	t.put("ldp.q (ofs = 0; 1st elem)",              both(h, op( g->ldp(d[1].q, d[0].q,      ptr(s->x)) ),     op( fwd_zero(g, &d[1]) ), fwd_zero_latency, lat_inc2_pattern,      thr_skip2_patterns));
	t.put("ldp.q (ofs = 0; 2nd elem)",              both(h, op( g->ldp(d[0].q, d[1].q,      ptr(s->x)) ),     op( fwd_zero(g, &d[1]) ), fwd_zero_latency, lat_inc2_pattern,      thr_skip2_patterns));
	t.put("ldp.q (ofs = 0; unaligned)",             both(u, op( g->ldp(d[0].q, d[1].q,      ptr(s->x)) ),     op( fwd_zero(g, &d[1]) ), fwd_zero_latency, lat_inc2_pattern,      thr_skip2_patterns));
	t.put("ldnp.q",                                 thr(h,  op( g->ldnp(d[0].q, d[1].q,     ptr(s->x)) )));

	t.put("ldp.q (ofs = 0; 1st elem)",              both(h, op( g->ldp(d[1].q, d[0].q,      ptr(s->x)) ),     op( g->mov(d[1].x, d[1].v.d[0]) ),                             0.0,     lat_inc2_pattern,      thr_skip2_patterns));
	t.put("ldp.q (ofs = 0; 2nd elem)",              both(h, op( g->ldp(d[0].q, d[1].q,      ptr(s->x)) ),     op( g->mov(d[1].x, d[1].v.d[0]) ),                             0.0,     lat_inc2_pattern,      thr_skip2_patterns));
	t.put("ldp.q (ofs = 0; unaligned)",             both(u, op( g->ldp(d[0].q, d[1].q,      ptr(s->x)) ),     op( g->mov(d[1].x, d[1].v.d[0]) ),                             0.0,     lat_inc2_pattern,      thr_skip2_patterns));
	t.put("ldnp.q",                                 thr(h,  op( g->ldnp(d[0].q, d[1].q,     ptr(s->x)) )));

	t.put("ld1.b (multi; 1 reg)",                   both(z, op( g->ld1(d[0].v.b - d[0].v.b, ptr(s->x)) ),     op( fwd_x28(g, d) ), fwd_x28_latency,  &lat_flat_patterns[3], thr_skip1x_patterns));
	t.put("ld1.b (multi; 2 reg)",                   both(z, op( g->ld1(d[0].v.b - d[1].v.b, ptr(s->x)) ),     op( fwd_x28(g, d) ), fwd_x28_latency,  &lat_flat_patterns[2], thr_skip2x_patterns));
	t.put("ld1.b (multi; 3 reg)",                   both(z, op( g->ld1(d[0].v.b - d[2].v.b, ptr(s->x)) ),     op( fwd_x28(g, d) ), fwd_x28_latency,  &lat_flat_patterns[1], thr_skip3x_patterns));
	t.put("ld1.b (multi; 4 reg)",                   both(z, op( g->ld1(d[0].v.b - d[3].v.b, ptr(s->x)) ),     op( fwd_x28(g, d) ), fwd_x28_latency,  &lat_flat_patterns[0], thr_skip4x_patterns));

	t.put("ld1.h (multi; 1 reg)",                   both(z, op( g->ld1(d[0].v.h - d[0].v.h, ptr(s->x)) ),     op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[3], thr_skip1x_patterns));
	t.put("ld1.h (multi; 2 reg)",                   both(z, op( g->ld1(d[0].v.h - d[1].v.h, ptr(s->x)) ),     op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[2], thr_skip2x_patterns));
	t.put("ld1.h (multi; 3 reg)",                   both(z, op( g->ld1(d[0].v.h - d[2].v.h, ptr(s->x)) ),     op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[1], thr_skip3x_patterns));
	t.put("ld1.h (multi; 4 reg)",                   both(z, op( g->ld1(d[0].v.h - d[3].v.h, ptr(s->x)) ),     op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[0], thr_skip4x_patterns));

	t.put("ld1.s (multi; 1 reg)",                   both(z, op( g->ld1(d[0].v.s - d[0].v.s, ptr(s->x)) ),     op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[3], thr_skip1x_patterns));
	t.put("ld1.s (multi; 2 reg)",                   both(z, op( g->ld1(d[0].v.s - d[1].v.s, ptr(s->x)) ),     op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[2], thr_skip2x_patterns));
	t.put("ld1.s (multi; 3 reg)",                   both(z, op( g->ld1(d[0].v.s - d[2].v.s, ptr(s->x)) ),     op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[1], thr_skip3x_patterns));
	t.put("ld1.s (multi; 4 reg)",                   both(z, op( g->ld1(d[0].v.s - d[3].v.s, ptr(s->x)) ),     op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[0], thr_skip4x_patterns));

	t.put("ld1.d (multi; 1 reg)",                   both(h, op( g->ld1(d[0].v.d - d[0].v.d, ptr(s->x)) ),     op( fwd_zero(g, d) ), fwd_zero_latency, &lat_flat_patterns[3], thr_skip1x_patterns));
	t.put("ld1.d (multi; 2 reg)",                   both(h, op( g->ld1(d[0].v.d - d[1].v.d, ptr(s->x)) ),     op( fwd_zero(g, d) ), fwd_zero_latency, &lat_flat_patterns[2], thr_skip2x_patterns));
	t.put("ld1.d (multi; 3 reg)",                   both(h, op( g->ld1(d[0].v.d - d[2].v.d, ptr(s->x)) ),     op( fwd_zero(g, d) ), fwd_zero_latency, &lat_flat_patterns[1], thr_skip3x_patterns));
	t.put("ld1.d (multi; 4 reg)",                   both(h, op( g->ld1(d[0].v.d - d[3].v.d, ptr(s->x)) ),     op( fwd_zero(g, d) ), fwd_zero_latency, &lat_flat_patterns[0], thr_skip4x_patterns));

	t.put("ld2.b (multi)",                          both(z, op( g->ld2(d->v.b, ptr(s->x)) ),                  op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.b (multi)",                          both(z, op( g->ld3(d->v.b, ptr(s->x)) ),                  op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.b (multi)",                          both(z, op( g->ld4(d->v.b, ptr(s->x)) ),                  op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld2.h (multi)",                          both(z, op( g->ld2(d->v.h, ptr(s->x)) ),                  op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.h (multi)",                          both(z, op( g->ld3(d->v.h, ptr(s->x)) ),                  op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.h (multi)",                          both(z, op( g->ld4(d->v.h, ptr(s->x)) ),                  op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld2.s (multi)",                          both(z, op( g->ld2(d->v.s, ptr(s->x)) ),                  op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.s (multi)",                          both(z, op( g->ld3(d->v.s, ptr(s->x)) ),                  op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.s (multi)",                          both(z, op( g->ld4(d->v.s, ptr(s->x)) ),                  op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld2.d (multi)",                          both(h, op( g->ld2(d->v.d, ptr(s->x)) ),                  op( fwd_zero(g, d) ), fwd_zero_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.d (multi)",                          both(h, op( g->ld3(d->v.d, ptr(s->x)) ),                  op( fwd_zero(g, d) ), fwd_zero_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.d (multi)",                          both(h, op( g->ld4(d->v.d, ptr(s->x)) ),                  op( fwd_zero(g, d) ), fwd_zero_latency, &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1.b (single; [15])",                   both(z, op( g->ld1(s[3].v.b[15], ptr(s->x)) ),            op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2.b (single; [15])",                   both(z, op( g->ld2(s[2].v.b[15], ptr(s->x)) ),            op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.b (single; [15])",                   both(z, op( g->ld3(s[1].v.b[15], ptr(s->x)) ),            op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.b (single; [15])",                   both(z, op( g->ld4(s[0].v.b[15], ptr(s->x)) ),            op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1.h (single; [7])",                    both(z, op( g->ld1(s[3].v.h[7], ptr(s->x)) ),             op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2.h (single; [7])",                    both(z, op( g->ld2(s[2].v.h[7], ptr(s->x)) ),             op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.h (single; [7])",                    both(z, op( g->ld3(s[1].v.h[7], ptr(s->x)) ),             op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.h (single; [7])",                    both(z, op( g->ld4(s[0].v.h[7], ptr(s->x)) ),             op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1.s (single; [3])",                    both(z, op( g->ld1(s[3].v.s[3], ptr(s->x)) ),             op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2.s (single; [3])",                    both(z, op( g->ld2(s[2].v.s[3], ptr(s->x)) ),             op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.s (single; [3])",                    both(z, op( g->ld3(s[1].v.s[3], ptr(s->x)) ),             op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.s (single; [3])",                    both(z, op( g->ld4(s[0].v.s[3], ptr(s->x)) ),             op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1.d (single; [1])",                    both(h, op( g->ld1(s[3].v.d[1], ptr(s->x)) ),             op( fwd_zero(g, d) ), fwd_zero_latency, &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2.d (single; [1])",                    both(h, op( g->ld2(s[2].v.d[1], ptr(s->x)) ),             op( fwd_zero(g, d) ), fwd_zero_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.d (single; [1])",                    both(h, op( g->ld3(s[1].v.d[1], ptr(s->x)) ),             op( fwd_zero(g, d) ), fwd_zero_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.d (single; [1])",                    both(h, op( g->ld4(s[0].v.d[1], ptr(s->x)) ),             op( fwd_zero(g, d) ), fwd_zero_latency, &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1r.b",                                 both(z, op( g->ld1r(d->v.b, ptr(s->x)) ),                 op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2r.b",                                 both(z, op( g->ld2r(d->v.b, ptr(s->x)) ),                 op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3r.b",                                 both(z, op( g->ld3r(d->v.b, ptr(s->x)) ),                 op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4r.b",                                 both(z, op( g->ld4r(d->v.b, ptr(s->x)) ),                 op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1r.h",                                 both(z, op( g->ld1r(d->v.h, ptr(s->x)) ),                 op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2r.h",                                 both(z, op( g->ld2r(d->v.h, ptr(s->x)) ),                 op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3r.h",                                 both(z, op( g->ld3r(d->v.h, ptr(s->x)) ),                 op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4r.h",                                 both(z, op( g->ld4r(d->v.h, ptr(s->x)) ),                 op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1r.s",                                 both(z, op( g->ld1r(d->v.s, ptr(s->x)) ),                 op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2r.s",                                 both(z, op( g->ld2r(d->v.s, ptr(s->x)) ),                 op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3r.s",                                 both(z, op( g->ld3r(d->v.s, ptr(s->x)) ),                 op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4r.s",                                 both(z, op( g->ld4r(d->v.s, ptr(s->x)) ),                 op( fwd_x28(g, d) ),  fwd_x28_latency,  &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1r.d",                                 both(h, op( g->ld1r(d->v.d, ptr(s->x)) ),                 op( fwd_zero(g, d) ), fwd_zero_latency, &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2r.d",                                 both(h, op( g->ld2r(d->v.d, ptr(s->x)) ),                 op( fwd_zero(g, d) ), fwd_zero_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3r.d",                                 both(h, op( g->ld3r(d->v.d, ptr(s->x)) ),                 op( fwd_zero(g, d) ), fwd_zero_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4r.d",                                 both(h, op( g->ld4r(d->v.d, ptr(s->x)) ),                 op( fwd_zero(g, d) ), fwd_zero_latency, &lat_flat_patterns[0], thr_skip4_patterns));

/*
	t.put("ldr.q (imm; ofs = 0)",                   both(h, op( g->ldr(d->q, ptr(s->x,      0)) ),            op( g->add(d->v.b, d->v.b, g->v28.b); g->mov(d->x, d->v.d[0]) ), 0.0,     lat_inc1_pattern,      thr_patterns));
	t.put("ldr.q (imm; ofs = 0)",                   both(h, op( g->ldr(d->q, ptr(s->x,      0)) ),            op( g->mov(d->x, d->v.d[0]) ), 0.0,     lat_inc1_pattern,      thr_patterns));

	t.put("ldr.q (imm; ofs = 0)",                   both(h, op( g->ldr(d->q, ptr(s->x,      0)) ),            op( g->mov(d->x, d->v.d[0]) ),                                 mov_latency,     lat_inc1_pattern,      thr_patterns));
	t.put("ldr.q (imm; ofs = 16)",                  both(h, op( g->ldr(d->q, ptr(s->x,      16)) ),           op( g->mov(d->x, d->v.d[0]) ),                                 mov_latency,     lat_inc1_pattern,      thr_patterns));

	t.put("ldr.q (imm; ofs = 0)",                   both(h, op( g->ldr(d->q,                ptr(s->x, 0)) ),  op( g->mov(d->x, d->v.d[0]) ),                                 mov_latency,     lat_inc1_pattern,      thr_patterns));
	t.put("ldr.q (imm; ofs = 16)",                  both(h, op( g->ldr(d->q,                ptr(s->x, 16)) ), op( g->mov(d->x, d->v.d[0]) ),                                 mov_latency,     lat_inc1_pattern,      thr_patterns));
	t.put("ldr.q (imm; ofs = 0; unaligned)",        both(u, op( g->ldr(d->q,                ptr(s->x, 0)) ),  op( g->mov(d->x, d->v.d[0]) ),                                 mov_latency,     lat_inc1_pattern,      thr_patterns));
	t.put("ldr.q (imm; ofs = 16; unaligned)",       both(u, op( g->ldr(d->q,                ptr(s->x, 16)) ), op( g->mov(d->x, d->v.d[0]) ),                                 mov_latency,     lat_inc1_pattern,      thr_patterns));
	t.put("ldur.q (imm; ofs = 0)",                  both(h, op( g->ldur(d->q,               ptr(s->x, 0)) ),  op( g->mov(d->x, d->v.d[0]) ),                                 mov_latency,     lat_inc1_pattern,      thr_patterns));
	t.put("ldur.q (imm; ofs = 16)",                 both(h, op( g->ldur(d->q,               ptr(s->x, 16)) ), op( g->mov(d->x, d->v.d[0]) ),                                 mov_latency,     lat_inc1_pattern,      thr_patterns));
	t.put("ldur.q (imm; ofs = 0; unaligned)",       both(u, op( g->ldur(d->q,               ptr(s->x, 0)) ),  op( g->mov(d->x, d->v.d[0]) ),                                 mov_latency,     lat_inc1_pattern,      thr_patterns));
	t.put("ldur.q (imm; ofs = 16; unaligned)",      both(u, op( g->ldur(d->q,               ptr(s->x, 16)) ), op( g->mov(d->x, d->v.d[0]) ),                                 mov_latency,     lat_inc1_pattern,      thr_patterns));

	t.put("ldp.q (ofs = 0; 1st elem)",              both(h, op( g->ldp(d[1].q, d[0].q,      ptr(s->x)) ),     op( g->mov(d[1].x, d[1].v.d[0]) ),                             mov_latency,     lat_inc2_pattern,      thr_skip2_patterns));
	t.put("ldp.q (ofs = 0; 2nd elem)",              both(h, op( g->ldp(d[0].q, d[1].q,      ptr(s->x)) ),     op( g->mov(d[1].x, d[1].v.d[0]) ),                             mov_latency,     lat_inc2_pattern,      thr_skip2_patterns));
	t.put("ldp.q (ofs = 0; unaligned)",             both(u, op( g->ldp(d[0].q, d[1].q,      ptr(s->x)) ),     op( g->mov(d[1].x, d[1].v.d[0]) ),                             mov_latency,     lat_inc2_pattern,      thr_skip2_patterns));
	t.put("ldnp.q",                                 thr(h,  op( g->ldnp(d[0].q, d[1].q,     ptr(s->x)) )));

	t.put("ld1.b (multi; 1 reg)",                   both(z, op( g->ld1(d[0].v.b - d[0].v.b, ptr(s->x)) ),     op( g->umov(s->w, d[0].v.b[15]); g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[3], thr_skip1x_patterns));
	t.put("ld1.b (multi; 2 reg)",                   both(z, op( g->ld1(d[0].v.b - d[1].v.b, ptr(s->x)) ),     op( g->umov(s->w, d[1].v.b[15]); g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[2], thr_skip2x_patterns));
	t.put("ld1.b (multi; 3 reg)",                   both(z, op( g->ld1(d[0].v.b - d[2].v.b, ptr(s->x)) ),     op( g->umov(s->w, d[2].v.b[15]); g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[1], thr_skip3x_patterns));
	t.put("ld1.b (multi; 4 reg)",                   both(z, op( g->ld1(d[0].v.b - d[3].v.b, ptr(s->x)) ),     op( g->umov(s->w, d[3].v.b[15]); g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[0], thr_skip4x_patterns));

	t.put("ld1.h (multi; 1 reg)",                   both(z, op( g->ld1(d[0].v.h - d[0].v.h, ptr(s->x)) ),     op( g->umov(s->w, d[0].v.h[7]);  g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[3], thr_skip1x_patterns));
	t.put("ld1.h (multi; 2 reg)",                   both(z, op( g->ld1(d[0].v.h - d[1].v.h, ptr(s->x)) ),     op( g->umov(s->w, d[1].v.h[7]);  g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[2], thr_skip2x_patterns));
	t.put("ld1.h (multi; 3 reg)",                   both(z, op( g->ld1(d[0].v.h - d[2].v.h, ptr(s->x)) ),     op( g->umov(s->w, d[2].v.h[7]);  g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[1], thr_skip3x_patterns));
	t.put("ld1.h (multi; 4 reg)",                   both(z, op( g->ld1(d[0].v.h - d[3].v.h, ptr(s->x)) ),     op( g->umov(s->w, d[3].v.h[7]);  g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[0], thr_skip4x_patterns));

	t.put("ld1.s (multi; 1 reg)",                   both(z, op( g->ld1(d[0].v.s - d[0].v.s, ptr(s->x)) ),     op( g->mov(s->w, d[0].v.s[3]);   g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[3], thr_skip1x_patterns));
	t.put("ld1.s (multi; 2 reg)",                   both(z, op( g->ld1(d[0].v.s - d[1].v.s, ptr(s->x)) ),     op( g->mov(s->w, d[1].v.s[3]);   g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[2], thr_skip2x_patterns));
	t.put("ld1.s (multi; 3 reg)",                   both(z, op( g->ld1(d[0].v.s - d[2].v.s, ptr(s->x)) ),     op( g->mov(s->w, d[2].v.s[3]);   g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[1], thr_skip3x_patterns));
	t.put("ld1.s (multi; 4 reg)",                   both(z, op( g->ld1(d[0].v.s - d[3].v.s, ptr(s->x)) ),     op( g->mov(s->w, d[3].v.s[3]);   g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[0], thr_skip4x_patterns));

	t.put("ld1.d (multi; 1 reg)",                   both(h, op( g->ld1(d[0].v.d - d[0].v.d, ptr(s->x)) ),     op( g->mov(s->x, d[0].v.d[1]) ),                               mov_latency,     &lat_flat_patterns[3], thr_skip1x_patterns));
	t.put("ld1.d (multi; 2 reg)",                   both(h, op( g->ld1(d[0].v.d - d[1].v.d, ptr(s->x)) ),     op( g->mov(s->x, d[1].v.d[1]) ),                               mov_latency,     &lat_flat_patterns[2], thr_skip2x_patterns));
	t.put("ld1.d (multi; 3 reg)",                   both(h, op( g->ld1(d[0].v.d - d[2].v.d, ptr(s->x)) ),     op( g->mov(s->x, d[2].v.d[1]) ),                               mov_latency,     &lat_flat_patterns[1], thr_skip3x_patterns));
	t.put("ld1.d (multi; 4 reg)",                   both(h, op( g->ld1(d[0].v.d - d[3].v.d, ptr(s->x)) ),     op( g->mov(s->x, d[3].v.d[1]) ),                               mov_latency,     &lat_flat_patterns[0], thr_skip4x_patterns));

	t.put("ld2.b (multi)",                          both(z, op( g->ld2(d->v.b, ptr(s->x)) ),                  op( g->umov(s->w, d->v.b[15]);   g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.b (multi)",                          both(z, op( g->ld3(d->v.b, ptr(s->x)) ),                  op( g->umov(s->w, d->v.b[15]);   g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.b (multi)",                          both(z, op( g->ld4(d->v.b, ptr(s->x)) ),                  op( g->umov(s->w, d->v.b[15]);   g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld2.h (multi)",                          both(z, op( g->ld2(d->v.h, ptr(s->x)) ),                  op( g->umov(s->w, d->v.h[7]);    g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.h (multi)",                          both(z, op( g->ld3(d->v.h, ptr(s->x)) ),                  op( g->umov(s->w, d->v.h[7]);    g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.h (multi)",                          both(z, op( g->ld4(d->v.h, ptr(s->x)) ),                  op( g->umov(s->w, d->v.h[7]);    g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld2.s (multi)",                          both(z, op( g->ld2(d->v.s, ptr(s->x)) ),                  op( g->mov(s->w, d->v.s[3]);     g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.s (multi)",                          both(z, op( g->ld3(d->v.s, ptr(s->x)) ),                  op( g->mov(s->w, d->v.s[3]);     g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.s (multi)",                          both(z, op( g->ld4(d->v.s, ptr(s->x)) ),                  op( g->mov(s->w, d->v.s[3]);     g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld2.d (multi)",                          both(h, op( g->ld2(d->v.d, ptr(s->x)) ),                  op( g->mov(s->x, d->v.d[1]) ),                                 mov_latency,     &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.d (multi)",                          both(h, op( g->ld3(d->v.d, ptr(s->x)) ),                  op( g->mov(s->x, d->v.d[1]) ),                                 mov_latency,     &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.d (multi)",                          both(h, op( g->ld4(d->v.d, ptr(s->x)) ),                  op( g->mov(s->x, d->v.d[1]) ),                                 mov_latency,     &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1.b (single; [15])",                   both(z, op( g->ld1(s[3].v.b[15], ptr(s->x)) ),            op( g->umov(s->w, d->v.b[15]);   g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2.b (single; [15])",                   both(z, op( g->ld2(s[2].v.b[15], ptr(s->x)) ),            op( g->umov(s->w, d->v.b[15]);   g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.b (single; [15])",                   both(z, op( g->ld3(s[1].v.b[15], ptr(s->x)) ),            op( g->umov(s->w, d->v.b[15]);   g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.b (single; [15])",                   both(z, op( g->ld4(s[0].v.b[15], ptr(s->x)) ),            op( g->umov(s->w, d->v.b[15]);   g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1.h (single; [7])",                    both(z, op( g->ld1(s[3].v.h[7], ptr(s->x)) ),             op( g->umov(s->w, d->v.h[7]);    g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2.h (single; [7])",                    both(z, op( g->ld2(s[2].v.h[7], ptr(s->x)) ),             op( g->umov(s->w, d->v.h[7]);    g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.h (single; [7])",                    both(z, op( g->ld3(s[1].v.h[7], ptr(s->x)) ),             op( g->umov(s->w, d->v.h[7]);    g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.h (single; [7])",                    both(z, op( g->ld4(s[0].v.h[7], ptr(s->x)) ),             op( g->umov(s->w, d->v.h[7]);    g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1.s (single; [3])",                    both(z, op( g->ld1(s[3].v.s[3], ptr(s->x)) ),             op( g->mov(s->w, d->v.s[3]);     g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2.s (single; [3])",                    both(z, op( g->ld2(s[2].v.s[3], ptr(s->x)) ),             op( g->mov(s->w, d->v.s[3]);     g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.s (single; [3])",                    both(z, op( g->ld3(s[1].v.s[3], ptr(s->x)) ),             op( g->mov(s->w, d->v.s[3]);     g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.s (single; [3])",                    both(z, op( g->ld4(s[0].v.s[3], ptr(s->x)) ),             op( g->mov(s->w, d->v.s[3]);     g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1.d (single; [1])",                    both(h, op( g->ld1(s[3].v.d[1], ptr(s->x)) ),             op( g->mov(s->x, d->v.d[1]) ),                                 mov_latency,     &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2.d (single; [1])",                    both(h, op( g->ld2(s[2].v.d[1], ptr(s->x)) ),             op( g->mov(s->x, d->v.d[1]) ),                                 mov_latency,     &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3.d (single; [1])",                    both(h, op( g->ld3(s[1].v.d[1], ptr(s->x)) ),             op( g->mov(s->x, d->v.d[1]) ),                                 mov_latency,     &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4.d (single; [1])",                    both(h, op( g->ld4(s[0].v.d[1], ptr(s->x)) ),             op( g->mov(s->x, d->v.d[1]) ),                                 mov_latency,     &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1r.b",                                 both(z, op( g->ld1r(d->v.b, ptr(s->x)) ),                 op( g->umov(s->w, d->v.b[15]);   g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2r.b",                                 both(z, op( g->ld2r(d->v.b, ptr(s->x)) ),                 op( g->umov(s->w, d->v.b[15]);   g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3r.b",                                 both(z, op( g->ld3r(d->v.b, ptr(s->x)) ),                 op( g->umov(s->w, d->v.b[15]);   g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4r.b",                                 both(z, op( g->ld4r(d->v.b, ptr(s->x)) ),                 op( g->umov(s->w, d->v.b[15]);   g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1r.h",                                 both(z, op( g->ld1r(d->v.h, ptr(s->x)) ),                 op( g->umov(s->w, d->v.h[7]);    g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2r.h",                                 both(z, op( g->ld2r(d->v.h, ptr(s->x)) ),                 op( g->umov(s->w, d->v.h[7]);    g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3r.h",                                 both(z, op( g->ld3r(d->v.h, ptr(s->x)) ),                 op( g->umov(s->w, d->v.h[7]);    g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4r.h",                                 both(z, op( g->ld4r(d->v.h, ptr(s->x)) ),                 op( g->umov(s->w, d->v.h[7]);    g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1r.s",                                 both(z, op( g->ld1r(d->v.s, ptr(s->x)) ),                 op( g->mov(s->w, d->v.s[3]);     g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2r.s",                                 both(z, op( g->ld2r(d->v.s, ptr(s->x)) ),                 op( g->mov(s->w, d->v.s[3]);     g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3r.s",                                 both(z, op( g->ld3r(d->v.s, ptr(s->x)) ),                 op( g->mov(s->w, d->v.s[3]);     g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4r.s",                                 both(z, op( g->ld4r(d->v.s, ptr(s->x)) ),                 op( g->mov(s->w, d->v.s[3]);     g->add(s->x, s->x, g->x28) ), mov_add_latency, &lat_flat_patterns[0], thr_skip4_patterns));

	t.put("ld1r.d",                                 both(h, op( g->ld1r(d->v.d, ptr(s->x)) ),                 op( g->mov(s->x, d->v.d[1]) ),                                 mov_latency,     &lat_flat_patterns[3], thr_skip1_patterns));
	t.put("ld2r.d",                                 both(h, op( g->ld2r(d->v.d, ptr(s->x)) ),                 op( g->mov(s->x, d->v.d[1]) ),                                 mov_latency,     &lat_flat_patterns[2], thr_skip2_patterns));
	t.put("ld3r.d",                                 both(h, op( g->ld3r(d->v.d, ptr(s->x)) ),                 op( g->mov(s->x, d->v.d[1]) ),                                 mov_latency,     &lat_flat_patterns[1], thr_skip3_patterns));
	t.put("ld4r.d",                                 both(h, op( g->ld4r(d->v.d, ptr(s->x)) ),                 op( g->mov(s->x, d->v.d[1]) ),                                 mov_latency,     &lat_flat_patterns[0], thr_skip4_patterns));
*/
	return;
}

/*
 * end of load.cc
 */
