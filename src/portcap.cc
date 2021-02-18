
/**
 * @file portcap.cc
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>

void bench_port_cap(bool md, double freq) {
	/*
	 * port capability tests
	 */
	table t(md, "M1 port capability profiling", 1, 20);
	bench b(freq);

/*
	t.put("eor (reg)",                  both(b, op( g->eor(d->x, d->x, s->x) )));
	t.put("eor (reg)",                  both(b, op( g->eor(d->x, d->x, s->x) ), 0.0, lat_inc1_pattern));
	t.put("eor (reg)",                  both(b, op( g->eor(d->x, s->x, s->x) )));
	t.put("eor (reg)",                  both(b, op( g->eor(d->x, s->x, s->x) ), 0.0, lat_inc1_pattern));
*/

	/* load */
	#define ldr_x_s(_g, _s, _i)             { (_g)->ldr((_s)->x, ptr((_s)->x)); }
	#define ldr_x_x28(_g, _s, _i)           { (_g)->ldr((_s)->x, ptr((_g)->x28)); }
	#define ldr_x_s_x28(_g, _s, _i)         { (_g)->ldr((_s)->x, ptr((_s)->x, (_g)->x28)); }
	#define ldr_q_s(_g, _s, _i)             { (_g)->ldr((_s)->q, ptr((_s)->x)); }
	#define ldr_q_x28(_g, _s, _i)           { (_g)->ldr((_s)->q, ptr((_g)->x28)); }
	#define ldr_q_x_x28(_g, _s, _i)         { (_g)->ldr((_s)->q, ptr((_s)->x, (_g)->x28)); }

	/* store */
	#define str_x_s(_g, _s, _i)             { (_g)->str((_s)->x, ptr((_s)->x)); }
	#define str_x_x28(_g, _s, _i)           { (_g)->str((_s)->x, ptr((_g)->x28)); }
	#define str_x_s_x28(_g, _s, _i)         { (_g)->str((_s)->x, ptr((_s)->s, (_g)->x28)); }
	#define str_q_s(_g, _s, _i)             { (_g)->str((_s)->q, ptr((_s)->x)); }
	#define str_q_x28(_g, _s, _i)           { (_g)->str((_s)->q, ptr((_g)->x28)); }
	#define str_q_s_x28(_g, _s, _i)         { (_g)->str((_s)->q, ptr((_s)->s, (_g)->x28)); }

	/* load-to-store */
	#define ldr_str_x_s_0(_g, _s, _i)       { (_g)->str((_s)->x, ptr((_s)->x, (_i) * 0));   (_g)->ldr((_s)->x, ptr((_s)->x, (_i) * 0)); }
	#define ldr_str_x_s_128(_g, _s, _i)     { (_g)->str((_s)->x, ptr((_s)->x, (_i) * 128)); (_g)->ldr((_s)->x, ptr((_s)->x, (_i) * 128)); }

	/* scalar arith */
	#define add_x(_g, _s, _i)               { (_g)->add((_s)->x, (_s)->x, (_s)->x); }
	#define add_shl2_x(_g, _s, _i)          { (_g)->add((_s)->x, (_s)->x, (_s)->x, ShMod::LSL, 2); }
	#define adc_x(_g, _s, _i)               { (_g)->adc((_s)->x, (_s)->x, (_s)->x); }
	#define adds_x(_g, _s, _i)              { (_g)->adds((_s)->x, (_s)->x, (_s)->x); }
	#define mul_x(_g, _s, _i)               { (_g)->mul((_s)->x, (_s)->x, (_s)->x); }
	#define madd_x(_g, _s, _i)              { (_g)->madd((_s)->x, (_s)->x, (_s)->x, (_s)->x); }
	#define udiv_x_x28(_g, _s, _i)          { (_g)->udiv((_s)->x, (_s)->x, g->x28); }
	#define bfm_x(_g, _s, _i)               { (_g)->bfm((_s)->x, (_s)->x, 17, 2); }
	#define crc32x_x(_g, _s, _i)            { (_g)->crc32x((_s)->w, (_s)->w, (_s)->x); }

	/* floating point ops */
	#define fcmp_s_s(_g, _s, _i)            { (_g)->fcmp((_s)->s, (_s)->s); }
	#define fcmp_s_0(_g, _s, _i)            { (_g)->fcmp((_s)->s, 0.0); }
	#define fcsel_s_s_s(_g, _s, _i)         { (_g)->fcsel((_s)->s, (_s)->s, (_s)->s, Cond::EQ); }
	#define fcvtas_s_s(_g, _s, _i)          { (_g)->fcvtas((_s)->s, (_s)->s); }
	#define fcvtas_x_s(_g, _s, _i)          { (_g)->fcvtas((_s)->x, (_s)->s); }
	#define scvtf_s_w(_g, _s, _i)           { (_g)->scvtf((_s)->s, (_s)->w); }
	#define frecpe_s_s(_g, _s, _i)          { (_g)->frecpe((_s)->s, (_s)->s); }
	#define frsqrte_s_s(_g, _s, _i)         { (_g)->frsqrte((_s)->s, (_s)->s); }
	#define fmov_d_x(_g, _s, _i)            { (_g)->fmov((_s)->d, (_s)->x); }
	#define mov_x_d(_g, _s, _i)             { (_g)->mov((_s)->x, (_s)->v.d[0]); }


	#define thr_1(_b, _n, _op1)                                           { t.put(( _n ), thr(( _b ), op( _op1(g, &s[0], 0); ),                                                                                                                                      thr_skip1x_patterns)); }
	#define thr_2(_b, _n, _op1, _op2)                                     { t.put(( _n ), thr(( _b ), op( _op1(g, &s[0], 0); _op2(g, &s[1], 1); ),                                                                                                                   thr_skip2x_patterns)); }
	#define thr_3(_b, _n, _op1, _op2, _op3)                               { t.put(( _n ), thr(( _b ), op( _op1(g, &s[0], 0); _op2(g, &s[1], 1); _op3(g, &s[2], 2); ),                                                                                                thr_skip3x_patterns)); }
	#define thr_4(_b, _n, _op1, _op2, _op3, _op4)                         { t.put(( _n ), thr(( _b ), op( _op1(g, &s[0], 0); _op2(g, &s[1], 1); _op3(g, &s[2], 2); _op4(g, &s[3], 3); ),                                                                             thr_skip4x_patterns)); }
	#define thr_5(_b, _n, _op1, _op2, _op3, _op4, _op5)                   { t.put(( _n ), thr(( _b ), op( _op1(g, &s[0], 0); _op2(g, &s[1], 1); _op3(g, &s[2], 2); _op4(g, &s[3], 3); _op5(g, &s[4], 4); ),                                                          thr_skip5x_patterns)); }
	#define thr_6(_b, _n, _op1, _op2, _op3, _op4, _op5, _op6)             { t.put(( _n ), thr(( _b ), op( _op1(g, &s[0], 0); _op2(g, &s[1], 1); _op3(g, &s[2], 2); _op4(g, &s[3], 3); _op5(g, &s[4], 4); _op6(g, &s[5], 5); ),                                       thr_skip6x_patterns)); }
	#define thr_7(_b, _n, _op1, _op2, _op3, _op4, _op5, _op6, _op7)       { t.put(( _n ), thr(( _b ), op( _op1(g, &s[0], 0); _op2(g, &s[1], 1); _op3(g, &s[2], 2); _op4(g, &s[3], 3); _op5(g, &s[4], 4); _op6(g, &s[5], 5); _op7(g, &s[6], 6); ),                    thr_skip7x_patterns)); }
	#define thr_8(_b, _n, _op1, _op2, _op3, _op4, _op5, _op6, _op7, _op8) { t.put(( _n ), thr(( _b ), op( _op1(g, &s[0], 0); _op2(g, &s[1], 1); _op3(g, &s[2], 2); _op4(g, &s[3], 3); _op5(g, &s[4], 4); _op6(g, &s[5], 5); _op7(g, &s[6], 6); _op8(g, &s[7], 7); ), thr_skip8x_patterns)); }


	{
	memmgr m_h(mem_init( p ));
	bench h(freq, m_h.ptr(), m_h.ptr());
	t.put("str -> ldr (ofs = 0 -> ofs = 0)", thr(h, op(
		g->str(s[0].x, ptr(g->x28, 0));   g->ldr(s[0].x, ptr(g->x28, 0));
		g->str(s[1].x, ptr(g->x28, 128)); g->ldr(s[1].x, ptr(g->x28, 128));
		g->str(s[2].x, ptr(g->x28, 256)); g->ldr(s[2].x, ptr(g->x28, 256));
		g->str(s[3].x, ptr(g->x28, 384)); g->ldr(s[3].x, ptr(g->x28, 384));
	), thr_skip4x_patterns));
	t.put("str -> ldr (ofs = 0 -> ofs = 0)", thr(h, op(
		g->str(s[0].x, ptr(g->x28, 0)); g->ldr(s[0].x, ptr(g->x28, 0));
		g->str(s[1].x, ptr(g->x28, 0)); g->ldr(s[1].x, ptr(g->x28, 0));
		g->str(s[2].x, ptr(g->x28, 0)); g->ldr(s[2].x, ptr(g->x28, 0));
		g->str(s[3].x, ptr(g->x28, 0)); g->ldr(s[3].x, ptr(g->x28, 0));
	), thr_skip4x_patterns));

	thr_4(h, "str -> ldr (ofs = 0 -> ofs = 0)", ldr_str_x_s_0,   ldr_str_x_s_0,   ldr_str_x_s_0,   ldr_str_x_s_0);
	thr_4(h, "str -> ldr (ofs = 0 -> ofs = 0)", ldr_str_x_s_128, ldr_str_x_s_128, ldr_str_x_s_128, ldr_str_x_s_128);
	}


	double const adcs_latency = lat_i(freq, op( g->adcs(d->x, d->x, s->x) ));
	#define thr_body(_i, _body) ({ \
		thr(b, op( g->adds(s->x, s->x, g->x28); for(size_t j = 0; j < (_i); j++) { _body; } )).thr * (double)(_i); \
	})
	#define sweep(_body) ({ \
		measure_t const _l = lat(b, op( g->adcs(s->x, s->x, g->x28); _body; ), adcs_latency); \
		double const _t = std::max({ \
			thr_body(1, _body), thr_body(2, _body), thr_body(3, _body), thr_body(4, _body), \
			thr_body(5, _body), thr_body(6, _body), thr_body(7, _body), thr_body(8, _body) \
		}); \
		(measure_t){ .lat = _l.lat, .thr = _t }; \
	})
/*
	t.put("add",                        both(b, op( g->add(d->x, d->x, s->x) )));
	t.put("add (<<2)",                  both(b, op( g->add(d->x, d->x, s->x, ShMod::LSL, 2) )));
	t.put("add (<<2)",                  both(b, op( g->add(d->x, s->x, d->x, ShMod::LSL, 2) )));
	t.put("add (<<2)",                  both(b, op( g->add(d->x, g->x28, s->x, ShMod::LSL, 2) )));
	t.put("add (<<2)",                  both(b, op( g->add(d->x, s->x, g->x28, ShMod::LSL, 2) )));
	t.put("adc",                        both(b, op( g->adc(d->x, d->x, s->x) )));
	t.put("adds",                       both(b, op( g->adds(d->x, d->x, s->x) )));
	t.put("mul",                        both(b, op( g->mul(d->x, d->x, s->x) )));
	t.put("madd",                       both(b, op( g->madd(d->x, s->x, s->x, d->x) )));
	t.put("udiv",                       both(b, op( g->udiv(d->x, s->x, g->x28) )));
	t.put("bfm",                        both(b, op( g->bfm(d->x, s->x, 17, 2) )));
	t.put("crc32x",                     both(b, op( g->crc32x(d->w, d->w, s->x) )));
	// t.put("ccmn",                       sweep(      g->ccmn(s->x, g->x28, 0xf, Cond::EQ) ));
*/

	t.put("fcmp.s (reg)",               thr(b, op( g->fcmp(s->s, s->s) )));
	t.put("fcmp.s (zero)",              thr(b, op( g->fcmp(s->s, 0.0) )));
	t.put("fcsel.s",                    thr(b, op( g->fcsel(d->s, d->s, s->s, Cond::EQ) )));
	t.put("fcvtas.s (s -> s)",          thr(b, op( g->fcvtas(d->s, s->s) )));
	t.put("fcvtas.s (s -> x)",          thr(b, op( g->fcvtas(d->x, s->s) )));
	t.put("scvtf.s (x -> x)",           thr(b, op( g->scvtf(d->s, s->w) )));
	t.put("frecpe.s (scl)",             thr(b, op( g->frecpe(d->s, s->s) )));
	t.put("frsqrte.s (scl)",            thr(b, op( g->frsqrte(d->s, s->s) )));
	t.put("fmov.d (x -> d)",            thr(b, op( g->fmov(d->d, s->x); )));
	t.put("mov (d[0] -> x)",            thr(b, op( g->mov(d->x, s->v.d[0]); )));

	thr_1("fcmp.s (reg)",               thr(b, op( g->fcmp(s->s, s->s) )));
	thr_1("fcmp.s (zero)",              thr(b, op( g->fcmp(s->s, 0.0) )));
	thr_1("fcsel.s",                    thr(b, op( g->fcsel(d->s, d->s, s->s, Cond::EQ) )));
	thr_1("fcvtas.s (s -> s)",          thr(b, op( g->fcvtas(d->s, s->s) )));
	thr_1("fcvtas.s (s -> x)",          thr(b, op( g->fcvtas(d->x, s->s) )));
	thr_1("scvtf.s (x -> x)",           thr(b, op( g->scvtf(d->s, s->w) )));
	thr_1("frecpe.s (scl)",             thr(b, op( g->frecpe(d->s, s->s) )));
	thr_1("frsqrte.s (scl)",            thr(b, op( g->frsqrte(d->s, s->s) )));
	thr_1("fmov.d (x -> d)",            thr(b, op( g->fmov(d->d, s->x); )));
	thr_1("mov (d[0] -> x)",            thr(b, op( g->mov(d->x, s->v.d[0]); )));

	// double const fmov_mov_add = lat(z, op( g->fmov(d->d, g->x0); g->mov(d->x, d->v.d[0]); g->add(d->x, d->x, g->x28) )).lat;
	// double const fmov_fwd     = lat(z, op( g->fmov(d->d, s->x);  fwd_lo(g, d, s) )).lat;
	// double const fwd_latency  = floor(fmov_fwd - fmov_mov_add / 2.0 + 0.5);



	t.put("add x 3 // fmov.d (x -> d) x 3", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->fmov(s[3].d, s[3].x);
		g->fmov(s[4].d, s[4].x);
		g->fmov(s[5].d, s[5].x);
	), thr_skip6x_patterns));
	t.put("add x 4 // fmov.d (x -> d) x 3", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->fmov(s[4].d, s[4].x);
		g->fmov(s[5].d, s[5].x);
		g->fmov(s[6].d, s[6].x);
	), thr_skip7x_patterns));
	t.put("add x 5 // fmov.d (x -> d) x 3", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
		g->fmov(s[5].d, s[5].x);
		g->fmov(s[6].d, s[6].x);
		g->fmov(s[7].d, s[7].x);
	), thr_skip8x_patterns));
	t.put("adc x 3 // fmov.d (x -> d) x 3", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->fmov(s[3].d, s[3].x);
		g->fmov(s[4].d, s[4].x);
		g->fmov(s[5].d, s[5].x);
	), thr_skip6x_patterns));
	t.put("add.b x 4 // fmov.d (x -> d) x 3", thr(b, op(
		g->add(s[0].v.b, s[0].v.b, s[0].v.b);
		g->add(s[1].v.b, s[1].v.b, s[1].v.b);
		g->add(s[2].v.b, s[2].v.b, s[2].v.b);
		g->add(s[3].v.b, s[3].v.b, s[3].v.b);
		g->fmov(s[4].d, s[4].x);
		g->fmov(s[5].d, s[5].x);
		g->fmov(s[6].d, s[6].x);
	), thr_skip7x_patterns));

	t.put("add // mov.d (d -> x)", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->mov(s[1].x, s[1].v.d[0]);
	), thr_skip2x_patterns));
	t.put("add x 2 // mov.d (d -> x)", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->mov(s[2].x, s[2].v.d[0]);
	), thr_skip3x_patterns));
	t.put("add x 3 // mov.d (d -> x)", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->mov(s[3].x, s[3].v.d[0]);
	), thr_skip4x_patterns));
	t.put("add x 4 // mov.d (d -> x)", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->mov(s[4].x, s[4].v.d[0]);
	), thr_skip5x_patterns));
	t.put("add x 5 // mov.d (d -> x)", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
		g->mov(s[5].x, s[5].v.d[0]);
	), thr_skip6x_patterns));
	t.put("add x 6 // mov.d (d -> x)", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
		g->add(s[5].x, s[5].x, s[5].x);
		g->mov(s[6].x, s[6].v.d[0]);
	), thr_skip7x_patterns));

	t.put("add // mov.d (d -> x) x 2", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->mov(s[1].x, s[1].v.d[0]);
		g->mov(s[2].x, s[2].v.d[0]);
	), thr_skip3x_patterns));
	t.put("add x 2 // mov.d (d -> x) x 2", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->mov(s[2].x, s[2].v.d[0]);
		g->mov(s[3].x, s[3].v.d[0]);
	), thr_skip4x_patterns));
	t.put("add x 3 // mov.d (d -> x) x 2", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->mov(s[3].x, s[3].v.d[0]);
		g->mov(s[4].x, s[4].v.d[0]);
	), thr_skip5x_patterns));
	t.put("add x 4 // mov.d (d -> x) x 2", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->mov(s[4].x, s[4].v.d[0]);
		g->mov(s[5].x, s[5].v.d[0]);
	), thr_skip6x_patterns));
	t.put("add x 5 // mov.d (d -> x) x 2", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
		g->mov(s[5].x, s[5].v.d[0]);
		g->mov(s[6].x, s[6].v.d[0]);
	), thr_skip7x_patterns));
	t.put("add x 6 // mov.d (d -> x) x 2", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
		g->add(s[5].x, s[5].x, s[5].x);
		g->mov(s[6].x, s[6].v.d[0]);
		g->mov(s[7].x, s[7].v.d[0]);
	), thr_skip8x_patterns));
	t.put("adc // mov.d (d -> x)", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->mov(s[1].x, s[1].v.d[0]);
	), thr_skip2x_patterns));
	t.put("adc x 2 // mov.d (d -> x)", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->mov(s[2].x, s[2].v.d[0]);
	), thr_skip3x_patterns));
	t.put("adc x 3 // mov.d (d -> x)", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->mov(s[3].x, s[3].v.d[0]);
	), thr_skip4x_patterns));
	t.put("adc // mov.d (d -> x) x 2", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->mov(s[1].x, s[1].v.d[0]);
		g->mov(s[2].x, s[2].v.d[0]);
	), thr_skip3x_patterns));
	t.put("adc x 2 // mov.d (d -> x) x 2", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->mov(s[2].x, s[2].v.d[0]);
		g->mov(s[3].x, s[3].v.d[0]);
	), thr_skip4x_patterns));
	t.put("adc x 3 // mov.d (d -> x) x 2", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->mov(s[3].x, s[3].v.d[0]);
		g->mov(s[4].x, s[4].v.d[0]);
	), thr_skip5x_patterns));
	t.put("mul // mov.d (d -> x) x 2", thr(b, op(
		g->mul(s[0].x, s[0].x, s[0].x);
		g->mov(s[1].x, s[1].v.d[0]);
		g->mov(s[2].x, s[2].v.d[0]);
	), thr_skip3x_patterns));
	t.put("mul x 2 // mov.d (d -> x) x 2", thr(b, op(
		g->mul(s[0].x, s[0].x, s[0].x);
		g->mul(s[1].x, s[1].x, s[1].x);
		g->mov(s[2].x, s[2].v.d[0]);
		g->mov(s[3].x, s[3].v.d[0]);
	), thr_skip4x_patterns));
	t.put("add.b x 2 // mov.d (d -> x) x 2", thr(b, op(
		g->add(s[0].v.b, s[0].v.b, s[0].v.b);
		g->add(s[1].v.b, s[1].v.b, s[1].v.b);
		g->mov(s[2].x, s[2].v.d[0]);
		g->mov(s[3].x, s[3].v.d[0]);
	), thr_skip4x_patterns));
	t.put("add.b x 3 // mov.d (d -> x) x 2", thr(b, op(
		g->add(s[0].v.b, s[0].v.b, s[0].v.b);
		g->add(s[1].v.b, s[1].v.b, s[1].v.b);
		g->add(s[2].v.b, s[2].v.b, s[2].v.b);
		g->mov(s[3].x, s[3].v.d[0]);
		g->mov(s[4].x, s[4].v.d[0]);
	), thr_skip5x_patterns));
	t.put("add.b x 4 // mov.d (d -> x) x 2", thr(b, op(
		g->add(s[0].v.b, s[0].v.b, s[0].v.b);
		g->add(s[1].v.b, s[1].v.b, s[1].v.b);
		g->add(s[2].v.b, s[2].v.b, s[2].v.b);
		g->add(s[3].v.b, s[3].v.b, s[3].v.b);
		g->mov(s[4].x, s[4].v.d[0]);
		g->mov(s[5].x, s[5].v.d[0]);
	), thr_skip6x_patterns));

	{
	memmgr m_h(mem_init( p ));
	bench h(freq, m_h.ptr(), m_h.ptr());
	t.put("ldr x 3 // fmov.d (x -> d) x 2", thr(h, op(
		g->ldr(s[0].x, ptr(s[0].x));
		g->ldr(s[1].x, ptr(s[1].x));
		g->ldr(s[2].x, ptr(s[2].x));
		g->fmov(s[3].d, s[3].x);
		g->fmov(s[4].d, s[4].x);
	), thr_skip5x_patterns));
	t.put("ldr.q x 3 // fmov.d (x -> d) x 2", thr(h, op(
		g->ldr(s[0].q, ptr(s[0].x));
		g->ldr(s[1].q, ptr(s[1].x));
		g->ldr(s[2].q, ptr(s[2].x));
		g->fmov(s[3].d, s[3].x);
		g->fmov(s[4].d, s[4].x);
	), thr_skip5x_patterns));
	t.put("ldr x 3 // fmov.d (x -> d) x 3", thr(h, op(
		g->ldr(s[0].x, ptr(s[0].x));
		g->ldr(s[1].x, ptr(s[1].x));
		g->ldr(s[2].x, ptr(s[2].x));
		g->fmov(s[3].d, s[3].x);
		g->fmov(s[4].d, s[4].x);
		g->fmov(s[5].d, s[5].x);
	), thr_skip6x_patterns));
	t.put("ldr.q x 3 // fmov.d (x -> d) x 3", thr(h, op(
		g->ldr(s[0].q, ptr(s[0].x));
		g->ldr(s[1].q, ptr(s[1].x));
		g->ldr(s[2].q, ptr(s[2].x));
		g->fmov(s[3].d, s[3].x);
		g->fmov(s[4].d, s[4].x);
		g->fmov(s[5].d, s[5].x);
	), thr_skip6x_patterns));

	t.put("str x 2 // fmov.d (x -> d) x 2", thr(h, op(
		g->str(s[0].x, ptr(s[0].x));
		g->str(s[1].x, ptr(s[1].x));
		g->fmov(s[2].d, s[2].x);
		g->fmov(s[3].d, s[3].x);
	), thr_skip4x_patterns));
	t.put("str.q x 2 // fmov.d (x -> d) x 2", thr(h, op(
		g->str(s[0].q, ptr(s[0].x));
		g->str(s[1].q, ptr(s[1].x));
		g->fmov(s[2].d, s[2].x);
		g->fmov(s[3].d, s[3].x);
	), thr_skip4x_patterns));
	t.put("str x 2 // fmov.d (x -> d) x 3", thr(h, op(
		g->str(s[0].x, ptr(s[0].x));
		g->str(s[1].x, ptr(s[1].x));
		g->fmov(s[2].d, s[2].x);
		g->fmov(s[3].d, s[3].x);
		g->fmov(s[4].d, s[4].x);
	), thr_skip5x_patterns));
	t.put("str.q x 2 // fmov.d (x -> d) x 3", thr(h, op(
		g->str(s[0].q, ptr(s[0].x));
		g->str(s[1].q, ptr(s[1].x));
		g->fmov(s[2].d, s[2].x);
		g->fmov(s[3].d, s[3].x);
		g->fmov(s[4].d, s[4].x);
	), thr_skip5x_patterns));
	}

	{
	memmgr m_h(mem_init( p ));
	bench h(freq, m_h.ptr(), NULL);
	t.put("ldr x 3 // fmov.d (x -> d) x 2", thr(h, op(
		g->ldr(s[0].x, ptr(s[0].x, g->x28));
		g->ldr(s[1].x, ptr(s[1].x, g->x28));
		g->ldr(s[2].x, ptr(s[2].x, g->x28));
		g->fmov(s[3].d, s[3].x);
		g->fmov(s[4].d, s[4].x);
	), thr_skip5x_patterns));
	t.put("ldr.q x 3 // fmov.d (x -> d) x 2", thr(h, op(
		g->ldr(s[0].q, ptr(s[0].x, g->x28));
		g->ldr(s[1].q, ptr(s[1].x, g->x28));
		g->ldr(s[2].q, ptr(s[2].x, g->x28));
		g->fmov(s[3].d, s[3].x);
		g->fmov(s[4].d, s[4].x);
	), thr_skip5x_patterns));
	t.put("ldr x 3 // fmov.d (x -> d) x 3", thr(h, op(
		g->ldr(s[0].x, ptr(s[0].x, g->x28));
		g->ldr(s[1].x, ptr(s[1].x, g->x28));
		g->ldr(s[2].x, ptr(s[2].x, g->x28));
		g->fmov(s[3].d, s[3].x);
		g->fmov(s[4].d, s[4].x);
		g->fmov(s[5].d, s[5].x);
	), thr_skip6x_patterns));
	t.put("ldr.q x 3 // fmov.d (x -> d) x 3", thr(h, op(
		g->ldr(s[0].q, ptr(s[0].x, g->x28));
		g->ldr(s[1].q, ptr(s[1].x, g->x28));
		g->ldr(s[2].q, ptr(s[2].x, g->x28));
		g->fmov(s[3].d, s[3].x);
		g->fmov(s[4].d, s[4].x);
		g->fmov(s[5].d, s[5].x);
	), thr_skip6x_patterns));

	t.put("str x 2 // fmov.d (x -> d) x 2", thr(h, op(
		g->str(s[0].x, ptr(s[0].x, g->x28));
		g->str(s[1].x, ptr(s[1].x, g->x28));
		g->fmov(s[2].d, s[2].x);
		g->fmov(s[3].d, s[3].x);
	), thr_skip4x_patterns));
	t.put("str.q x 2 // fmov.d (x -> d) x 2", thr(h, op(
		g->str(s[0].q, ptr(s[0].x, g->x28));
		g->str(s[1].q, ptr(s[1].x, g->x28));
		g->fmov(s[2].d, s[2].x);
		g->fmov(s[3].d, s[3].x);
	), thr_skip4x_patterns));
	t.put("str x 2 // fmov.d (x -> d) x 3", thr(h, op(
		g->str(s[0].x, ptr(s[0].x, g->x28));
		g->str(s[1].x, ptr(s[1].x, g->x28));
		g->fmov(s[2].d, s[2].x);
		g->fmov(s[3].d, s[3].x);
		g->fmov(s[4].d, s[4].x);
	), thr_skip5x_patterns));
	t.put("str.q x 2 // fmov.d (x -> d) x 3", thr(h, op(
		g->str(s[0].q, ptr(s[0].x, g->x28));
		g->str(s[1].q, ptr(s[1].x, g->x28));
		g->fmov(s[2].d, s[2].x);
		g->fmov(s[3].d, s[3].x);
		g->fmov(s[4].d, s[4].x);
	), thr_skip5x_patterns));
	}

	{
	memmgr m_h(mem_init( p ));
	bench h(freq, m_h.ptr(), m_h.ptr());
	t.put("ldr x 3 // mov.d (d -> x) x 2", thr(h, op(
		g->ldr(s[0].x, ptr(s[0].x));
		g->ldr(s[1].x, ptr(s[1].x));
		g->ldr(s[2].x, ptr(s[2].x));
		g->mov(s[3].x, s[3].v.d[0]);
		g->mov(s[4].x, s[4].v.d[0]);
	), thr_skip5x_patterns));
	t.put("ldr.q x 3 // mov.d (d -> x) x 2", thr(h, op(
		g->ldr(s[0].q, ptr(s[0].x));
		g->ldr(s[1].q, ptr(s[1].x));
		g->ldr(s[2].q, ptr(s[2].x));
		g->mov(s[3].x, s[3].v.d[0]);
		g->mov(s[4].x, s[4].v.d[0]);
	), thr_skip5x_patterns));
	t.put("str x 2 // mov.d (d -> x) x 2", thr(h, op(
		g->str(s[0].x, ptr(s[0].x));
		g->str(s[1].x, ptr(s[1].x));
		g->mov(s[2].x, s[2].v.d[0]);
		g->mov(s[3].x, s[3].v.d[0]);
	), thr_skip4x_patterns));
	t.put("str.q x 2 // mov.d (d -> x) x 2", thr(h, op(
		g->str(s[0].q, ptr(s[0].x));
		g->str(s[1].q, ptr(s[1].x));
		g->mov(s[2].x, s[2].v.d[0]);
		g->mov(s[3].x, s[3].v.d[0]);
	), thr_skip4x_patterns));
	}

	/*
	 * fcvtas, fcvtau, scvtf, fcsel, fcmp, fccmp, frecpe, fsqrt
	 */

	t.put("add.b x 2 // fcvtas.s (s -> x) x 2", thr(b, op(
		g->add(s[0].v.b, s[0].v.b, s[0].v.b);
		g->add(s[1].v.b, s[1].v.b, s[1].v.b);
		g->fcvtas(s[2].x, s[2].s);
		g->fcvtas(s[3].x, s[3].s);
	), thr_skip4x_patterns));

	t.put("add.b x 3 // fcvtas.s (s -> x) x 2", thr(b, op(
		g->add(s[0].v.b, s[0].v.b, s[0].v.b);
		g->add(s[1].v.b, s[1].v.b, s[1].v.b);
		g->add(s[2].v.b, s[2].v.b, s[2].v.b);
		g->fcvtas(s[3].x, s[3].s);
		g->fcvtas(s[4].x, s[4].s);
	), thr_skip5x_patterns));

	t.put("add.b x 4 // fcvtas.s (s -> x) x 2", thr(b, op(
		g->add(s[0].v.b, s[0].v.b, s[0].v.b);
		g->add(s[1].v.b, s[1].v.b, s[1].v.b);
		g->add(s[2].v.b, s[2].v.b, s[2].v.b);
		g->add(s[3].v.b, s[3].v.b, s[3].v.b);
		g->fcvtas(s[4].x, s[4].s);
		g->fcvtas(s[5].x, s[5].s);
	), thr_skip6x_patterns));

	t.put("add.b // fcvtas.s (s -> x) x 3", thr(b, op(
		g->add(s[0].v.b, s[0].v.b, s[0].v.b);
		g->fcvtas(s[1].x, s[1].s);
		g->fcvtas(s[2].x, s[2].s);
		g->fcvtas(s[3].x, s[3].s);
	), thr_skip4x_patterns));

	t.put("add.b x 2 // fcvtas.s (s -> x) x 3", thr(b, op(
		g->add(s[0].v.b, s[0].v.b, s[0].v.b);
		g->add(s[1].v.b, s[1].v.b, s[1].v.b);
		g->fcvtas(s[2].x, s[2].s);
		g->fcvtas(s[3].x, s[3].s);
		g->fcvtas(s[4].x, s[4].s);
	), thr_skip5x_patterns));


	{
	memmgr m_h(mem_init( p ));
	bench h(freq, m_h.ptr(), m_h.ptr());
	t.put("ldr x 3 // fcvtas.s (s -> x) x 2", thr(h, op(
		g->ldr(s[0].x, ptr(s[0].x));
		g->ldr(s[1].x, ptr(s[1].x));
		g->ldr(s[2].x, ptr(s[2].x));
		g->fcvtas(s[3].x, s[3].s);
		g->fcvtas(s[4].x, s[4].s);
	), thr_skip5x_patterns));
	t.put("ldr.q x 3 // fcvtas.s (s -> x) x 2", thr(h, op(
		g->ldr(s[0].q, ptr(s[0].x));
		g->ldr(s[1].q, ptr(s[1].x));
		g->ldr(s[2].q, ptr(s[2].x));
		g->fcvtas(s[3].x, s[3].s);
		g->fcvtas(s[4].x, s[4].s);
	), thr_skip5x_patterns));
	t.put("str x 2 // fcvtas.s (s -> x) x 2", thr(h, op(
		g->str(s[0].x, ptr(s[0].x));
		g->str(s[1].x, ptr(s[1].x));
		g->fcvtas(s[2].x, s[2].s);
		g->fcvtas(s[3].x, s[3].s);
	), thr_skip4x_patterns));
	t.put("str.q x 2 // fcvtas.s (s -> x) x 2", thr(h, op(
		g->str(s[0].q, ptr(s[0].x));
		g->str(s[1].q, ptr(s[1].x));
		g->fcvtas(s[2].x, s[2].s);
		g->fcvtas(s[3].x, s[3].s);
	), thr_skip4x_patterns));
	}

	{
	memmgr m_h(mem_init( p ));
	bench h(freq, m_h.ptr(), m_h.ptr());
	t.put("ldr // scvtf.s (x -> s) x 2", thr(h, op(
		g->ldr(s[0].x, ptr(s[0].x));
		g->scvtf(s[1].s, s[1].w);
		g->scvtf(s[2].s, s[2].w);
	), thr_skip3x_patterns));
	t.put("ldr.q // scvtf.s (x -> s) x 2", thr(h, op(
		g->ldr(s[0].q, ptr(s[0].x));
		g->scvtf(s[1].s, s[1].w);
		g->scvtf(s[2].s, s[2].w);
	), thr_skip3x_patterns));
	t.put("ldr x 2 // scvtf.s (x -> s)", thr(h, op(
		g->ldr(s[0].x, ptr(s[0].x));
		g->ldr(s[1].x, ptr(s[1].x));
		g->scvtf(s[2].s, s[2].w);
	), thr_skip3x_patterns));
	t.put("ldr.q x 2 // scvtf.s (x -> s)", thr(h, op(
		g->ldr(s[0].q, ptr(s[0].x));
		g->ldr(s[1].q, ptr(s[1].x));
		g->scvtf(s[2].s, s[2].w);
	), thr_skip3x_patterns));
	t.put("ldr x 2 // scvtf.s (x -> s) x 2", thr(h, op(
		g->ldr(s[0].x, ptr(s[0].x));
		g->ldr(s[1].x, ptr(s[1].x));
		g->scvtf(s[2].s, s[2].w);
		g->scvtf(s[3].s, s[3].w);
	), thr_skip4x_patterns));
	t.put("ldr.q x 2 // scvtf.s (x -> s) x 2", thr(h, op(
		g->ldr(s[0].q, ptr(s[0].x));
		g->ldr(s[1].q, ptr(s[1].x));
		g->scvtf(s[2].s, s[2].w);
		g->scvtf(s[3].s, s[3].w);
	), thr_skip4x_patterns));
	t.put("ldr x 3 // scvtf.s (x -> s)", thr(h, op(
		g->ldr(s[0].x, ptr(s[0].x));
		g->ldr(s[1].x, ptr(s[1].x));
		g->ldr(s[2].x, ptr(s[2].x));
		g->scvtf(s[3].s, s[3].w);
	), thr_skip4x_patterns));
	t.put("ldr.q x 3 // scvtf.s (x -> s)", thr(h, op(
		g->ldr(s[0].q, ptr(s[0].x));
		g->ldr(s[1].q, ptr(s[1].x));
		g->ldr(s[2].q, ptr(s[2].x));
		g->scvtf(s[3].s, s[3].w);
	), thr_skip4x_patterns));
	t.put("ldr x 3 // scvtf.s (x -> s) x 2", thr(h, op(
		g->ldr(s[0].x, ptr(s[0].x));
		g->ldr(s[1].x, ptr(s[1].x));
		g->ldr(s[2].x, ptr(s[2].x));
		g->scvtf(s[3].s, s[3].w);
		g->scvtf(s[4].s, s[4].w);
	), thr_skip5x_patterns));
	t.put("ldr.q x 3 // scvtf.s (x -> s) x 2", thr(h, op(
		g->ldr(s[0].q, ptr(s[0].x));
		g->ldr(s[1].q, ptr(s[1].x));
		g->ldr(s[2].q, ptr(s[2].x));
		g->scvtf(s[3].s, s[3].w);
		g->scvtf(s[4].s, s[4].w);
	), thr_skip5x_patterns));

	t.put("str x 2 // scvtf.s (x -> s) x 2", thr(h, op(
		g->str(s[0].x, ptr(s[0].x));
		g->str(s[1].x, ptr(s[1].x));
		g->scvtf(s[2].s, s[2].w);
		g->scvtf(s[3].s, s[3].w);
	), thr_skip4x_patterns));
	t.put("str.q x 2 // scvtf.s (x -> s) x 2", thr(h, op(
		g->str(s[0].q, ptr(s[0].x));
		g->str(s[1].q, ptr(s[1].x));
		g->scvtf(s[2].s, s[2].w);
		g->scvtf(s[3].s, s[3].w);
	), thr_skip4x_patterns));
	t.put("str // scvtf.s (x -> s) x 3", thr(h, op(
		g->str(s[0].x, ptr(s[0].x));
		g->scvtf(s[1].s, s[1].w);
		g->scvtf(s[2].s, s[2].w);
		g->scvtf(s[3].s, s[3].w);
	), thr_skip4x_patterns));
	t.put("str.q // scvtf.s (x -> s) x 3", thr(h, op(
		g->str(s[0].q, ptr(s[0].x));
		g->scvtf(s[1].s, s[1].w);
		g->scvtf(s[2].s, s[2].w);
		g->scvtf(s[3].s, s[3].w);
	), thr_skip4x_patterns));
	t.put("str x 2 // scvtf.s (x -> s) x 3", thr(h, op(
		g->str(s[0].x, ptr(s[0].x));
		g->str(s[1].x, ptr(s[1].x));
		g->scvtf(s[2].s, s[2].w);
		g->scvtf(s[3].s, s[3].w);
		g->scvtf(s[4].s, s[4].w);
	), thr_skip5x_patterns));
	t.put("str.q x 2 // scvtf.s (x -> s) x 3", thr(h, op(
		g->str(s[0].q, ptr(s[0].x));
		g->str(s[1].q, ptr(s[1].x));
		g->scvtf(s[2].s, s[2].w);
		g->scvtf(s[3].s, s[3].w);
		g->scvtf(s[4].s, s[4].w);
	), thr_skip5x_patterns));
	}


	memmgr m_h(mem_init( p ));
	bench h(freq, m_h.ptr(), m_h.ptr());
	bench n(freq, m_h.ptr(), NULL);

	t.put("scvtf.s (x -> s) x 2 // fcvtas.s (s -> x)", thr(h, op(
		g->scvtf(s[0].h, s[0].w);
		g->scvtf(s[1].h, s[1].w);
		g->fcvtas(s[2].x, s[2].s);
	), thr_skip3x_patterns));

	t.put("scvtf.s (x -> s) // fcvtas.s (s -> x) x 2", thr(h, op(
		g->scvtf(s[0].h, s[0].w);
		g->fcvtas(s[1].x, s[1].s);
		g->fcvtas(s[2].x, s[2].s);
	), thr_skip3x_patterns));

	t.put("scvtf.s (x -> s) // fcvtas.s (s -> x) x 3", thr(h, op(
		g->scvtf(s[0].h, s[0].w);
		g->fcvtas(s[1].x, s[1].s);
		g->fcvtas(s[2].x, s[2].s);
		g->fcvtas(s[3].x, s[3].s);
	), thr_skip4x_patterns));

	t.put("scvtf.s (x -> s) x 2 // fcvtas.s (s -> x) x 2", thr(h, op(
		g->scvtf(s[0].h, s[0].w);
		g->scvtf(s[1].h, s[1].w);
		g->fcvtas(s[2].x, s[2].s);
		g->fcvtas(s[3].x, s[3].s);
	), thr_skip4x_patterns));

	t.put("scvtf.s (x -> s) x 3 // fcvtas.s (s -> x)", thr(h, op(
		g->scvtf(s[0].h, s[0].w);
		g->scvtf(s[1].h, s[1].w);
		g->scvtf(s[2].h, s[2].w);
		g->fcvtas(s[3].x, s[3].s);
	), thr_skip4x_patterns));


	// t.put("fcmp.h (reg)",               both(b, op( g->fcmp(s->h, s->h) ),  op( g->fcsel(d->h, d->h, s->h, Cond::EQ) ), fcsel_latency));
	// t.put("fcsel.h (eq)",               both(b, op( g->fcsel(d->h, d->h, s->h, Cond::EQ) )));
	// t.put("frecpe.s (scl)",             both(b, op( g->frecpe(d->s, s->s) )));
	// t.put("frsqrte.s (scl)",            both(b, op( g->frsqrte(d->s, s->s) )));

	t.put("frecpe.s (scl) // frsqrte.s (scl)", thr(h, op(
		g->frecpe(s[0].s, s[0].s);
		g->frsqrte(s[1].s, s[1].s);
	), thr_skip2x_patterns));
	t.put("frecpe.s (scl) // fcmp.s", thr(h, op(
		g->frecpe(s[0].s, s[0].s);
		g->fcmp(s[1].h, s[1].h);
	), thr_skip2x_patterns));
	t.put("frsqrte.s (scl) // fcmp.s", thr(h, op(
		g->frsqrte(s[0].s, s[0].s);
		g->fcmp(s[1].h, s[1].h);
	), thr_skip2x_patterns));


	/* mov equivalents */
	t.put("eor (reg; clearing idiom)",      both(b, op( g->eor(d->x, s->x, s->x) )));

	t.put("adds -> b.eq (pc+4; not taken)", both(b, op(
		Label l;
		g->b(Cond::EQ, l); g->L(l);
	), 0.0, lat_patterns, lat_patterns));
	t.put("adds -> b.eq (pc+4; not taken)", both(b, op(
		Label l;
		g->adds(g->x28, g->x28, g->x1);
		g->b(Cond::EQ, l); g->L(l);
	), 0.0, lat_patterns, lat_patterns));
	t.put("fcmp -> b.eq (pc+4; taken)", both(b, op(
		Label l;
		g->fcmp(g->s28, g->s28);
		g->b(Cond::EQ, l); g->L(l);
	), 0.0, lat_patterns, lat_patterns));
	t.put("fcmp -> b.ne (pc+4; not taken)", both(b, op(
		Label l;
		g->fcmp(g->s28, g->s28);
		g->b(Cond::NE, l); g->L(l);
	), 0.0, lat_patterns, lat_patterns));

	t.put("fcmp -> b.eq (pc+4; taken) // adc", both(b, op(
		Label l;
		g->fcmp(g->s28, g->s28);
		g->adc(g->x0, g->x0, g->x0);
		g->b(Cond::EQ, l); g->L(l);
	), 0.0, lat_patterns, lat_patterns));
	t.put("fcmp -> b.ne (pc+4; not taken) // adc", both(b, op(
		Label l;
		g->fcmp(g->s28, g->s28);
		g->adc(g->x0, g->x0, g->x0);
		g->b(Cond::NE, l); g->L(l);
	), 0.0, lat_patterns, lat_patterns));

	t.put("fcmp -> b.eq (pc+4; taken) // adc x 2", both(b, op(
		Label l;
		g->fcmp(g->s28, g->s28);
		g->adc(g->x0, g->x0, g->x0);
		g->adc(g->x1, g->x1, g->x1);
		g->b(Cond::EQ, l); g->L(l);
	), 0.0, lat_patterns, lat_patterns));
	t.put("fcmp -> b.ne (pc+4; not taken) // adc x 2", both(b, op(
		Label l;
		g->fcmp(g->s28, g->s28);
		g->adc(g->x0, g->x0, g->x0);
		g->adc(g->x1, g->x1, g->x1);
		g->b(Cond::NE, l); g->L(l);
	), 0.0, lat_patterns, lat_patterns));

	t.put("fcmp -> b.eq (pc+4; taken) // adc x 3", both(b, op(
		Label l;
		g->fcmp(g->s28, g->s28);
		g->adc(g->x0, g->x0, g->x0);
		g->adc(g->x1, g->x1, g->x1);
		g->adc(g->x2, g->x2, g->x2);
		g->b(Cond::EQ, l); g->L(l);
	), 0.0, lat_patterns, lat_patterns));
	t.put("fcmp -> b.ne (pc+4; not taken) // adc x 3", both(b, op(
		Label l;
		g->fcmp(g->s28, g->s28);
		g->adc(g->x0, g->x0, g->x0);
		g->adc(g->x1, g->x1, g->x1);
		g->adc(g->x2, g->x2, g->x2);
		g->b(Cond::NE, l); g->L(l);
	), 0.0, lat_patterns, lat_patterns));


	t.put("adc // b.eq (pc+4; not taken)", thr(b, op(
		Label l;
		g->adc(s[0].x, s[0].x, s[0].x);
		g->b(Cond::EQ, l); g->L(l);
	), thr_skip1x_patterns));

	t.put("adc x 2 // b.eq (pc+4; not taken)", thr(b, op(
		Label l;
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->b(Cond::EQ, l); g->L(l);
	), thr_skip2x_patterns));

	t.put("adc x 3 // b.eq (pc+4; not taken)", thr(b, op(
		Label l;
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->b(Cond::EQ, l); g->L(l);
	), thr_skip3x_patterns));



	t.put("ldr x 3 // str", thr(h, op(
		g->ldr(s[0].x, ptr(s[0].x));
		g->ldr(s[1].x, ptr(s[1].x));
		g->ldr(s[2].x, ptr(s[2].x));
		g->str(s[3].x, ptr(s[3].x));
	), thr_skip4x_patterns));

	t.put("ldr x 2 // str x 2", thr(h, op(
		g->ldr(s[0].x, ptr(s[0].x));
		g->ldr(s[1].x, ptr(s[1].x));
		g->str(s[2].x, ptr(s[2].x));
		g->str(s[3].x, ptr(s[3].x));
	), thr_skip4x_patterns));

	t.put("ldr x 3 // str x 2", thr(h, op(
		g->ldr(s[0].x, ptr(s[0].x));
		g->ldr(s[1].x, ptr(s[1].x));
		g->ldr(s[2].x, ptr(s[2].x));
		g->str(s[3].x, ptr(s[3].x));
		g->str(s[4].x, ptr(s[4].x));
	), thr_skip5x_patterns));

	#define var_offset_core(_basename, _offset, _body) { \
		memmgr m_h(mem_init( ((void *)((uint8_t const *)p - (_offset))) )); \
		bench h(freq, m_h.ptr(), m_h.ptr(), 1, 1, 1, 1, 1); \
		char buf[1024] = { 0 }; \
		size_t const len = strlen((char const *)(_basename)); \
		size_t const clip = len > 512 ? 512 : len; \
		sprintf(buf, "%.*s (ofs = %zu)", (int)clip, (char const *)(_basename), (size_t)(_offset)); \
		t.put(buf, thr(h, _body, thr_skip8x_patterns)); \
	}
	#define var_offset(_basename, _body_m) { \
		var_offset_core(_basename, 0,      ( _body_m(0) )); \
		var_offset_core(_basename, 16,     ( _body_m(16) )); \
		var_offset_core(_basename, 64,     ( _body_m(64) )); \
		var_offset_core(_basename, 1024,   ( _body_m(1024) )); \
		var_offset_core(_basename, 4096,   ( _body_m(4096) )); \
		var_offset_core(_basename, 8192,   ( _body_m(8192) )); \
		var_offset_core(_basename, 16384,  ( _body_m(16384) )); \
	}

	#define ldr1_add6(_offset) op( \
		g->add(s[0].x, s[0].x, s[0].x); \
		g->add(s[1].x, s[1].x, s[1].x); \
		g->add(s[2].x, s[2].x, s[2].x); \
		g->add(s[3].x, s[3].x, s[3].x); \
		g->add(s[4].x, s[4].x, s[4].x); \
		g->add(s[5].x, s[5].x, s[5].x); \
		g->ldr(s[6].x, ptr(s[6].x, (_offset))); \
	)
	#define ldr2_add6(_offset) op( \
		g->add(s[0].x, s[0].x, s[0].x); \
		g->add(s[1].x, s[1].x, s[1].x); \
		g->add(s[2].x, s[2].x, s[2].x); \
		g->add(s[3].x, s[3].x, s[3].x); \
		g->add(s[4].x, s[4].x, s[4].x); \
		g->add(s[5].x, s[5].x, s[5].x); \
		g->ldr(s[6].x, ptr(s[6].x, (_offset))); \
		g->ldr(s[7].x, ptr(s[7].x, (_offset))); \
	)
	#define str1_add6(_offset) op( \
		g->add(s[0].x, s[0].x, s[0].x); \
		g->add(s[1].x, s[1].x, s[1].x); \
		g->add(s[2].x, s[2].x, s[2].x); \
		g->add(s[3].x, s[3].x, s[3].x); \
		g->add(s[4].x, s[4].x, s[4].x); \
		g->add(s[5].x, s[5].x, s[5].x); \
		g->str(s[6].x, ptr(s[6].x, (_offset))); \
	)
	#define str2_add6(_offset) op( \
		g->add(s[0].x, s[0].x, s[0].x); \
		g->add(s[1].x, s[1].x, s[1].x); \
		g->add(s[2].x, s[2].x, s[2].x); \
		g->add(s[3].x, s[3].x, s[3].x); \
		g->add(s[4].x, s[4].x, s[4].x); \
		g->add(s[5].x, s[5].x, s[5].x); \
		g->str(s[6].x, ptr(s[6].x, (_offset))); \
		g->str(s[7].x, ptr(s[7].x, (_offset))); \
	)

	var_offset("add x 6 // ldr", ldr1_add6);
	var_offset("add x 6 // ldr x 2", ldr1_add6);
	var_offset("add x 6 // str", str1_add6);
	var_offset("add x 6 // str x 2", ldr1_add6);


	/* load */
	t.put("ldr.q // ldr x 2", thr(h, op(
		g->ldr(s[0].q, ptr(s[0].x));
		g->ldr(s[1].x, ptr(s[1].x));
		g->ldr(s[2].x, ptr(s[2].x));
	), thr_skip3x_patterns));
	t.put("ldr.q x 2 // ldr", thr(h, op(
		g->ldr(s[0].q, ptr(s[0].x));
		g->ldr(s[1].q, ptr(s[1].x));
		g->ldr(s[2].x, ptr(s[2].x));
	), thr_skip3x_patterns));
	t.put("ldr.q x 2 // ldr x 2", thr(h, op(
		g->ldr(s[0].q, ptr(s[0].x));
		g->ldr(s[1].q, ptr(s[1].x));
		g->ldr(s[2].x, ptr(s[2].x));
		g->ldr(s[3].x, ptr(s[3].x));
	), thr_skip4x_patterns));
	t.put("str.q // str", thr(h, op(
		g->str(s[0].q, ptr(s[0].x));
		g->str(s[1].x, ptr(s[1].x));
	), thr_skip2x_patterns));
	t.put("str.q // str x 2", thr(h, op(
		g->str(s[0].q, ptr(s[0].x));
		g->str(s[1].x, ptr(s[1].x));
		g->str(s[2].x, ptr(s[2].x));
	), thr_skip3x_patterns));
	t.put("str.q x 2 // str", thr(h, op(
		g->str(s[0].q, ptr(s[0].x));
		g->str(s[1].q, ptr(s[1].x));
		g->str(s[2].x, ptr(s[2].x));
	), thr_skip3x_patterns));



	t.put("add x 6 // ldr (ofs = 0)", thr(h, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
		g->add(s[5].x, s[5].x, s[5].x);
		g->ldr(s[6].x, ptr(s[6].x));
	), thr_skip7x_patterns));

	t.put("add x 6 // ldr (reg)", thr(n, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
		g->add(s[5].x, s[5].x, s[5].x);
		g->ldr(s[6].x, ptr(s[6].x, g->x28));
	), thr_skip7x_patterns));

	t.put("add x 6 // ldr (reg) x 2", thr(n, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
		g->add(s[5].x, s[5].x, s[5].x);
		g->ldr(s[6].x, ptr(s[6].x, g->x28));
		g->ldr(s[7].x, ptr(s[7].x, g->x28));
	), thr_skip8x_patterns));

	t.put("add x 6 // ldr (reg<<3)", thr(n, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
		g->add(s[5].x, s[5].x, s[5].x);
		g->ldr(s[6].x, ptr(s[6].x, g->x28, ShMod::LSL, 3));
	), thr_skip7x_patterns));

	t.put("add x 6 // ldr (reg<<3) x 2", thr(n, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
		g->add(s[5].x, s[5].x, s[5].x);
		g->ldr(s[6].x, ptr(s[6].x, g->x28, ShMod::LSL, 3));
		g->ldr(s[7].x, ptr(s[7].x, g->x28, ShMod::LSL, 3));
	), thr_skip8x_patterns));

	t.put("adc x 3 // ldr (ofs = 0)", thr(h, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->ldr(s[3].x, ptr(s[3].x));
	), thr_skip4x_patterns));

	t.put("adc x 3 // ldr (ofs = 0) x 2", thr(h, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->ldr(s[3].x, ptr(s[3].x));
		g->ldr(s[4].x, ptr(s[4].x));
	), thr_skip5x_patterns));

	t.put("adc x 3 // ldr (ofs = 0) x 3", thr(h, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->ldr(s[3].x, ptr(s[3].x));
		g->ldr(s[4].x, ptr(s[4].x));
		g->ldr(s[5].x, ptr(s[5].x));
	), thr_skip6x_patterns));

	t.put("mul x 2 // ldr (ofs = 0)", thr(h, op(
		g->mul(s[0].x, s[0].x, s[0].x);
		g->mul(s[1].x, s[1].x, s[1].x);
		g->ldr(s[2].x, ptr(s[2].x));
	), thr_skip3x_patterns));

	t.put("mul x 2 // ldr (ofs = 0) x 2", thr(h, op(
		g->mul(s[0].x, s[0].x, s[0].x);
		g->mul(s[1].x, s[1].x, s[1].x);
		g->ldr(s[2].x, ptr(s[2].x));
		g->ldr(s[3].x, ptr(s[3].x));
	), thr_skip4x_patterns));

	t.put("mul x 2 // ldr (ofs = 0) x 3", thr(h, op(
		g->mul(s[0].x, s[0].x, s[0].x);
		g->mul(s[1].x, s[1].x, s[1].x);
		g->ldr(s[2].x, ptr(s[2].x));
		g->ldr(s[3].x, ptr(s[3].x));
		g->ldr(s[4].x, ptr(s[4].x));
	), thr_skip5x_patterns));

	t.put("add x 6 // ldr (ofs = 16)", thr(h, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
		g->add(s[5].x, s[5].x, s[5].x);
		g->ldr(s[6].x, ptr(s[6].x, 16));
	), thr_skip7x_patterns));

	t.put("add x 6 // ldr (ofs = 16) x 2", thr(h, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
		g->add(s[5].x, s[5].x, s[5].x);
		g->ldr(s[6].x, ptr(s[6].x, 16));
		g->ldr(s[7].x, ptr(s[7].x, 16));
	), thr_skip8x_patterns));

	t.put("add x 6 // ldr (ofs = 16, 32)", thr(h, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
		g->add(s[5].x, s[5].x, s[5].x);
		g->ldr(s[6].x, ptr(s[6].x, 16));
		g->ldr(s[7].x, ptr(s[7].x, 32));
	), thr_skip8x_patterns));

	t.put("adc x 3 // ldr (ofs = 16)", thr(h, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->ldr(s[3].x, ptr(s[3].x, 16));
	), thr_skip4x_patterns));

	t.put("adc x 3 // ldr (ofs = 16) x 2", thr(h, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->ldr(s[3].x, ptr(s[3].x, 16));
		g->ldr(s[4].x, ptr(s[4].x, 16));
	), thr_skip5x_patterns));

	t.put("adc x 3 // ldr (ofs = 16) x 3", thr(h, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->ldr(s[3].x, ptr(s[3].x, 16));
		g->ldr(s[4].x, ptr(s[4].x, 16));
		g->ldr(s[5].x, ptr(s[5].x, 16));
	), thr_skip6x_patterns));

	t.put("mul x 2 // ldr (ofs = 16)", thr(h, op(
		g->mul(s[0].x, s[0].x, s[0].x);
		g->mul(s[1].x, s[1].x, s[1].x);
		g->ldr(s[2].x, ptr(s[2].x, 16));
	), thr_skip3x_patterns));

	t.put("mul x 2 // ldr (ofs = 16) x 2", thr(h, op(
		g->mul(s[0].x, s[0].x, s[0].x);
		g->mul(s[1].x, s[1].x, s[1].x);
		g->ldr(s[2].x, ptr(s[2].x, 16));
		g->ldr(s[3].x, ptr(s[3].x, 16));
	), thr_skip4x_patterns));

	t.put("mul x 2 // ldr (ofs = 16) x 3", thr(h, op(
		g->mul(s[0].x, s[0].x, s[0].x);
		g->mul(s[1].x, s[1].x, s[1].x);
		g->ldr(s[2].x, ptr(s[2].x, 16));
		g->ldr(s[3].x, ptr(s[3].x, 16));
		g->ldr(s[4].x, ptr(s[4].x, 16));
	), thr_skip5x_patterns));

	t.put("add x 6 // ldr (post)", thr(h, op(
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
		g->add(s[5].x, s[5].x, s[5].x);
		g->ldr(g->x28, pre_ptr(s[6].x, 0)); g->mov(s[6].x, g->x28);
	), thr_skip7x_patterns));

	t.put("adc x 3 // ldr (post)", thr(h, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->ldr(g->x28, pre_ptr(s[3].x, 0)); g->mov(s[3].x, g->x28);
	), thr_skip4x_patterns));

	t.put("mul x 2 // ldr (post)", thr(h, op(
		g->mul(s[0].x, s[0].x, s[0].x);
		g->mul(s[1].x, s[1].x, s[1].x);
		g->ldr(g->x28, pre_ptr(s[2].x, 0)); g->mov(s[2].x, g->x28);
	), thr_skip3x_patterns));

	/* store */
	t.put("adc x 3 // str", thr(h, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->str(s[3].x, ptr(s[3].x));
	), thr_skip4x_patterns));

	t.put("adc x 3 // str x 2", thr(h, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->str(s[3].x, ptr(s[3].x));
		g->str(s[4].x, ptr(s[4].x));
	), thr_skip5x_patterns));


	/* mov (imm) */
	t.put("mov-add x 2", thr(b, op(
		g->mov(s[0].x, 0x1ffc);
		g->add(s[0].x, s[0].x, s[0].x);
		g->mov(s[1].x, 0x1ffc);
		g->add(s[1].x, s[1].x, s[1].x);
	), thr_skip2x_patterns));

	t.put("mov-add x 3", thr(b, op(
		g->mov(s[0].x, 0x1ffc);
		g->add(s[0].x, s[0].x, s[0].x);
		g->mov(s[1].x, 0x1ffc);
		g->add(s[1].x, s[1].x, s[1].x);
		g->mov(s[2].x, 0x1ffc);
		g->add(s[2].x, s[2].x, s[2].x);
	), thr_skip3x_patterns));

	t.put("mov-add x 4", thr(b, op(
		g->mov(s[0].x, 0x1ffc);
		g->add(s[0].x, s[0].x, s[0].x);
		g->mov(s[1].x, 0x1ffc);
		g->add(s[1].x, s[1].x, s[1].x);
		g->mov(s[2].x, 0x1ffc);
		g->add(s[2].x, s[2].x, s[2].x);
		g->mov(s[3].x, 0x1ffc);
		g->add(s[3].x, s[3].x, s[3].x);
	), thr_skip4x_patterns));

	t.put("(mov,mov)-add x 2", thr(b, op(
		g->mov(s[0].x, 0x1ffc);
		g->mov(s[1].x, 0x1ffc);
		g->add(s[0].x, s[0].x, s[1].x);
		g->mov(s[2].x, 0x1ffc);
		g->mov(s[3].x, 0x1ffc);
		g->add(s[2].x, s[2].x, s[3].x);
	), thr_skip4x_patterns));

	t.put("(mov,mov)-add x 3", thr(b, op(
		g->mov(s[0].x, 0x1ffc);
		g->mov(s[1].x, 0x1ffc);
		g->add(s[0].x, s[0].x, s[1].x);
		g->mov(s[2].x, 0x1ffc);
		g->mov(s[3].x, 0x1ffc);
		g->add(s[2].x, s[2].x, s[3].x);
		g->mov(s[4].x, 0x1ffc);
		g->mov(s[5].x, 0x1ffc);
		g->add(s[4].x, s[4].x, s[5].x);
	), thr_skip6x_patterns));

	t.put("(mov,mov)-add x 4", thr(b, op(
		g->mov(s[0].x, 0x1ffc);
		g->mov(s[1].x, 0x1ffc);
		g->add(s[0].x, s[0].x, s[1].x);
		g->mov(s[2].x, 0x1ffc);
		g->mov(s[3].x, 0x1ffc);
		g->add(s[2].x, s[2].x, s[3].x);
		g->mov(s[4].x, 0x1ffc);
		g->mov(s[5].x, 0x1ffc);
		g->add(s[4].x, s[4].x, s[5].x);
		g->mov(s[6].x, 0x1ffc);
		g->mov(s[7].x, 0x1ffc);
		g->add(s[6].x, s[6].x, s[7].x);
	), thr_skip8x_patterns));


	t.put("mov-adc x 2", thr(b, op(
		g->mov(s[0].x, 0x1ffc);
		g->adc(s[0].x, s[0].x, s[0].x);
		g->mov(s[1].x, 0x1ffc);
		g->adc(s[1].x, s[1].x, s[1].x);
	), thr_skip2x_patterns));

	t.put("mov-adc x 3", thr(b, op(
		g->mov(s[0].x, 0x1ffc);
		g->adc(s[0].x, s[0].x, s[0].x);
		g->mov(s[1].x, 0x1ffc);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->mov(s[2].x, 0x1ffc);
		g->adc(s[2].x, s[2].x, s[2].x);
	), thr_skip3x_patterns));

	t.put("mov-adc x 4", thr(b, op(
		g->mov(s[0].x, 0x1ffc);
		g->adc(s[0].x, s[0].x, s[0].x);
		g->mov(s[1].x, 0x1ffc);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->mov(s[2].x, 0x1ffc);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->mov(s[3].x, 0x1ffc);
		g->adc(s[3].x, s[3].x, s[3].x);
	), thr_skip4x_patterns));


	#if 1
	/* adc // add, adds // add */
	t.put("adc x 3 // add", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
	), thr_skip4x_patterns));

	t.put("adc x 3 // add x 2", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
	), thr_skip5x_patterns));

	t.put("adc x 3 // add x 3", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
		g->add(s[5].x, s[5].x, s[5].x);
	), thr_skip6x_patterns));

	t.put("adc x 3 // add x 4", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
		g->add(s[5].x, s[5].x, s[5].x);
		g->add(s[6].x, s[6].x, s[6].x);
	), thr_skip7x_patterns));

	t.put("adds x 3 // add", thr(b, op(
		g->adds(s[0].x, s[0].x, s[0].x);
		g->adds(s[1].x, s[1].x, s[1].x);
		g->adds(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
	), thr_skip4x_patterns));

	t.put("adds x 3 // add x 2", thr(b, op(
		g->adds(s[0].x, s[0].x, s[0].x);
		g->adds(s[1].x, s[1].x, s[1].x);
		g->adds(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
	), thr_skip5x_patterns));

	t.put("adds x 3 // add x 3", thr(b, op(
		g->adds(s[0].x, s[0].x, s[0].x);
		g->adds(s[1].x, s[1].x, s[1].x);
		g->adds(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
		g->add(s[5].x, s[5].x, s[5].x);
	), thr_skip6x_patterns));

	t.put("adds x 3 // add x 4", thr(b, op(
		g->adds(s[0].x, s[0].x, s[0].x);
		g->adds(s[1].x, s[1].x, s[1].x);
		g->adds(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
		g->add(s[5].x, s[5].x, s[5].x);
		g->add(s[6].x, s[6].x, s[6].x);
	), thr_skip7x_patterns));

	/* add // add (<<2) */
	t.put("add (<<2) // add", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x, ShMod::LSL, 2);
		g->add(s[1].x, s[1].x, s[1].x);
	), thr_skip2x_patterns));

	t.put("add (<<2) // add x 2", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x, ShMod::LSL, 2);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
	), thr_skip3x_patterns));

	t.put("add (<<2) // add x 3", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x, ShMod::LSL, 2);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
	), thr_skip4x_patterns));

	t.put("add (<<2) // add x 4", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x, ShMod::LSL, 2);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
	), thr_skip5x_patterns));

	t.put("add (<<2) x 2 // add", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x, ShMod::LSL, 2);
		g->add(s[1].x, s[1].x, s[1].x, ShMod::LSL, 2);
		g->add(s[2].x, s[2].x, s[2].x);
	), thr_skip3x_patterns));

	t.put("add (<<2) x 2 // add x 2", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x, ShMod::LSL, 2);
		g->add(s[1].x, s[1].x, s[1].x, ShMod::LSL, 2);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
	), thr_skip4x_patterns));

	t.put("add (<<2) x 2 // add x 3", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x, ShMod::LSL, 2);
		g->add(s[1].x, s[1].x, s[1].x, ShMod::LSL, 2);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
	), thr_skip5x_patterns));

	t.put("add (<<2) x 3 // add", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x, ShMod::LSL, 2);
		g->add(s[1].x, s[1].x, s[1].x, ShMod::LSL, 2);
		g->add(s[2].x, s[2].x, s[2].x, ShMod::LSL, 2);
		g->add(s[3].x, s[3].x, s[3].x);
	), thr_skip4x_patterns));

	t.put("add (<<2) x 3 // add x 2", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x, ShMod::LSL, 2);
		g->add(s[1].x, s[1].x, s[1].x, ShMod::LSL, 2);
		g->add(s[2].x, s[2].x, s[2].x, ShMod::LSL, 2);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
	), thr_skip5x_patterns));

	t.put("add (<<2) x 3 // add x 3", thr(b, op(
		g->add(s[0].x, s[0].x, s[0].x, ShMod::LSL, 2);
		g->add(s[1].x, s[1].x, s[1].x, ShMod::LSL, 2);
		g->add(s[2].x, s[2].x, s[2].x, ShMod::LSL, 2);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
		g->add(s[5].x, s[5].x, s[5].x);
	), thr_skip6x_patterns));

	/* adc // add (<<2) */
	t.put("adc // add (<<2) x 2", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x, ShMod::LSL, 2);
		g->add(s[2].x, s[2].x, s[2].x, ShMod::LSL, 2);
	), thr_skip3x_patterns));

	t.put("adc // add (<<2) x 3", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x, ShMod::LSL, 2);
		g->add(s[2].x, s[2].x, s[2].x, ShMod::LSL, 2);
		g->add(s[3].x, s[3].x, s[3].x, ShMod::LSL, 2);
	), thr_skip4x_patterns));

	t.put("adc x 2 // add (<<2) x 2", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x, ShMod::LSL, 2);
		g->add(s[3].x, s[3].x, s[3].x, ShMod::LSL, 2);
	), thr_skip4x_patterns));

	t.put("adc x 2 // add (<<2) x 3", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x, ShMod::LSL, 2);
		g->add(s[3].x, s[3].x, s[3].x, ShMod::LSL, 2);
		g->add(s[3].x, s[3].x, s[4].x, ShMod::LSL, 2);
	), thr_skip5x_patterns));

	t.put("adc x 3 // add (<<2)", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x, ShMod::LSL, 2);
	), thr_skip4x_patterns));

	/* adds // add (<<2) */
	t.put("adds // add (<<2) x 2", thr(b, op(
		g->adds(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x, ShMod::LSL, 2);
		g->add(s[2].x, s[2].x, s[2].x, ShMod::LSL, 2);
	), thr_skip3x_patterns));

	t.put("adds // add (<<2) x 3", thr(b, op(
		g->adds(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x, ShMod::LSL, 2);
		g->add(s[2].x, s[2].x, s[2].x, ShMod::LSL, 2);
		g->add(s[3].x, s[3].x, s[3].x, ShMod::LSL, 2);
	), thr_skip4x_patterns));

	t.put("adds x 2 // add (<<2) x 2", thr(b, op(
		g->adds(s[0].x, s[0].x, s[0].x);
		g->adds(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x, ShMod::LSL, 2);
		g->add(s[3].x, s[3].x, s[3].x, ShMod::LSL, 2);
	), thr_skip4x_patterns));

	t.put("adds x 2 // add (<<2) x 3", thr(b, op(
		g->adds(s[0].x, s[0].x, s[0].x);
		g->adds(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x, ShMod::LSL, 2);
		g->add(s[3].x, s[3].x, s[3].x, ShMod::LSL, 2);
		g->add(s[3].x, s[3].x, s[4].x, ShMod::LSL, 2);
	), thr_skip5x_patterns));

	t.put("adds x 3 // add (<<2)", thr(b, op(
		g->adds(s[0].x, s[0].x, s[0].x);
		g->adds(s[1].x, s[1].x, s[1].x);
		g->adds(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x, ShMod::LSL, 2);
	), thr_skip4x_patterns));

	/* adc // adds */
	t.put("adc // adds x 2", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adds(s[1].x, s[1].x, s[1].x);
		g->adds(s[2].x, s[2].x, s[2].x);
	), thr_skip3x_patterns));

	t.put("adc x 2 // adds", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adds(s[2].x, s[2].x, s[2].x);
	), thr_skip3x_patterns));

	t.put("adc // adds x 3", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adds(s[1].x, s[1].x, s[1].x);
		g->adds(s[2].x, s[2].x, s[2].x);
		g->adds(s[3].x, s[3].x, s[3].x);
	), thr_skip4x_patterns));

	t.put("adc x 2 // adds x 2", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adds(s[2].x, s[2].x, s[2].x);
		g->adds(s[3].x, s[3].x, s[3].x);
	), thr_skip4x_patterns));

	t.put("adc x 3 // adds", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->adds(s[3].x, s[3].x, s[3].x);
	), thr_skip4x_patterns));

	t.put("adc // adds x 4", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adds(s[1].x, s[1].x, s[1].x);
		g->adds(s[2].x, s[2].x, s[2].x);
		g->adds(s[3].x, s[3].x, s[3].x);
		g->adds(s[4].x, s[4].x, s[4].x);
	), thr_skip5x_patterns));

	t.put("adc x 2 // adds x 3", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adds(s[2].x, s[2].x, s[2].x);
		g->adds(s[3].x, s[3].x, s[3].x);
		g->adds(s[4].x, s[4].x, s[4].x);
	), thr_skip5x_patterns));

	t.put("adc x 3 // adds x 2", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->adds(s[3].x, s[3].x, s[3].x);
		g->adds(s[4].x, s[4].x, s[4].x);
	), thr_skip5x_patterns));

	t.put("adc x 4 // adds", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->adc(s[3].x, s[3].x, s[3].x);
		g->adds(s[4].x, s[4].x, s[4].x);
	), thr_skip5x_patterns));

	t.put("adc x 3 // adds x 3", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->adds(s[3].x, s[3].x, s[3].x);
		g->adds(s[4].x, s[4].x, s[4].x);
		g->adds(s[5].x, s[5].x, s[5].x);
	), thr_skip5x_patterns));

	/* mul */
	t.put("adc x 3 // mul", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->mul(s[3].x, s[3].x, s[3].x);
	), thr_skip4x_patterns));

	t.put("adc x 3 // mul x 2", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->mul(s[3].x, s[3].x, s[3].x);
		g->mul(s[4].x, s[4].x, s[4].x);
	), thr_skip5x_patterns));

	/* madd */
	t.put("adc x 3 // madd", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->madd(s[3].x, s[3].x, s[3].x, g->x28);
	), thr_skip4x_patterns));

	t.put("mul // madd", thr(b, op(
		g->mul(s[0].x, s[0].x, s[0].x);
		g->madd(s[1].x, s[1].x, s[1].x, g->x28);
	), thr_skip2x_patterns));

	t.put("mul x 2 // madd", thr(b, op(
		g->mul(s[0].x, s[0].x, s[0].x);
		g->mul(s[1].x, s[1].x, s[1].x);
		g->madd(s[2].x, s[2].x, s[2].x, g->x28);
	), thr_skip3x_patterns));

	/* udiv */
	t.put("mul x 2 // udiv", thr(b, op(
		g->mul(s[0].x, s[0].x, s[0].x);
		g->mul(s[1].x, s[1].x, s[1].x);
		g->udiv(s[2].x, s[2].x, g->x28);
	), thr_skip3x_patterns));

	t.put("mul x 3 // udiv", thr(b, op(
		g->mul(s[0].x, s[0].x, s[0].x);
		g->mul(s[1].x, s[1].x, s[1].x);
		g->mul(s[2].x, s[2].x, s[2].x);
		g->udiv(s[3].x, s[3].x, g->x28);
	), thr_skip4x_patterns));

	t.put("mul x 4 // udiv", thr(b, op(
		g->mul(s[0].x, s[0].x, s[0].x);
		g->mul(s[1].x, s[1].x, s[1].x);
		g->mul(s[2].x, s[2].x, s[2].x);
		g->mul(s[3].x, s[3].x, s[3].x);
		g->udiv(s[4].x, s[4].x, g->x28);
	), thr_skip5x_patterns));

	t.put("madd x 2 // udiv", thr(b, op(
		g->madd(s[0].x, s[0].x, s[0].x, s[0].x);
		g->madd(s[1].x, s[1].x, s[1].x, s[1].x);
		g->udiv(s[2].x, s[2].x, g->x28);
	), thr_skip3x_patterns));

	t.put("mul // madd x 2 // udiv", thr(b, op(
		g->mul(s[0].x, s[0].x, s[0].x);
		g->madd(s[1].x, s[1].x, s[1].x, s[1].x);
		g->madd(s[2].x, s[2].x, s[2].x, s[2].x);
		g->udiv(s[3].x, s[3].x, g->x28);
	), thr_skip4x_patterns));

	t.put("mul x 2 // madd // udiv", thr(b, op(
		g->mul(s[0].x, s[0].x, s[0].x);
		g->mul(s[1].x, s[1].x, s[1].x);
		g->madd(s[2].x, s[2].x, s[2].x, s[2].x);
		g->udiv(s[3].x, s[3].x, g->x28);
	), thr_skip4x_patterns));

	/* bfm */
	t.put("adc x 3 // bfm", thr(b, op(
		g->adc(s[0].x, s[0].x, g->x4);
		g->adc(s[1].x, s[1].x, g->x9);
		g->adc(s[2].x, s[2].x, g->x14);
		g->bfm(s[3].x, s[3].x, 17, 2);
	), thr_skip5x_patterns));

	t.put("mul // bfm", thr(b, op(
		g->mul(s[0].x, s[0].x, g->x28);
		g->bfm(s[1].x, s[1].x, 17, 2);
	), thr_skip2x_patterns));

	t.put("mul x 2 // bfm", thr(b, op(
		g->mul(s[0].x, s[0].x, g->x3);
		g->mul(s[1].x, s[1].x, g->x7);
		g->bfm(s[2].x, s[2].x, 17, 2);
	), thr_skip4x_patterns));

	t.put("madd // bfm", thr(b, op(
		g->madd(s[0].x, s[0].x, s[0].x, g->x28);
		g->bfm(s[1].x, s[1].x, 17, 2);
	), thr_skip2x_patterns));

	t.put("udiv // bfm x 2", thr(b, op(
		g->udiv(s[0].x, s[0].x, g->x28);
		g->bfm(s[1].x, s[1].x, 17, 2);
		g->bfm(s[2].x, s[2].x, 17, 2);
	), thr_skip3x_patterns));

	/* crc32x */
	t.put("adc x 3 // crc32x", thr(b, op(
		g->adc(s[0].x, s[0].x, g->x4);
		g->adc(s[1].x, s[1].x, g->x9);
		g->adc(s[2].x, s[2].x, g->x14);
		g->crc32x(s[3].w, s[3].w, g->x28);
	), thr_skip5x_patterns));

	t.put("mul // crc32x", thr(b, op(
		g->mul(s[0].x, s[0].x, g->x2);
		g->crc32x(s[1].w, s[1].w, g->x28);
	), thr_skip3x_patterns));

	t.put("mul x 2 // crc32x", thr(b, op(
		g->mul(s[0].x, s[0].x, g->x3);
		g->mul(s[1].x, s[1].x, g->x7);
		g->crc32x(s[2].w, s[2].w, g->x28);
	), thr_skip4x_patterns));

	t.put("madd // crc32x", thr(b, op(
		g->madd(s[0].x, s[0].x, s[0].x, g->x2);
		g->crc32x(s[1].w, s[1].w, g->x28);
	), thr_skip3x_patterns));

	t.put("udiv // crc32x x 2", thr(b, op(
		g->udiv(s[0].x, s[0].x, g->x28);
		g->crc32x(s[1].w, s[1].w, g->x3);
		g->crc32x(s[2].w, s[2].w, g->x7);
	), thr_skip4x_patterns));

	t.put("bfm // crc32x", thr(b, op(
		g->madd(s[0].x, s[0].x, s[0].x, g->x2);
		g->crc32x(s[1].w, s[1].w, g->x28);
	), thr_skip3x_patterns));
	#endif

	/* ccmn */
	t.put("ccmn (reg; eq)",             lat(b, op( g->adcs(s->x, s->x, g->x28); g->ccmn(s->x, g->x28, 0xf, Cond::EQ) ), 0.0, lat_flat_patterns));
	t.put("ccmn (reg; eq)",             lat(b, op( g->adcs(s->x, s->x, g->x28); g->ccmn(s->x, g->x28, 0xf, Cond::EQ); g->ccmn(s->x, g->x28, 0xf, Cond::EQ) ), 0.0, lat_flat_patterns));
	t.put("ccmn (reg; eq)",             lat(b, op( g->adcs(s->x, s->x, g->x28); g->ccmn(s->x, g->x28, 0xf, Cond::EQ); g->ccmn(s->x, g->x28, 0xf, Cond::EQ); g->ccmn(s->x, g->x28, 0xf, Cond::EQ) ), 0.0, lat_flat_patterns));

	t.put("adc x 4, ccmn-adc", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->adc(s[3].x, s[3].x, s[3].x);
		g->ccmn(s[4].x, s[4].x, 0xf, Cond::EQ);
		g->adc(s[4].x, s[4].x, s[4].x);
	), thr_skip5x_patterns));

	t.put("adc x 5, ccmn-adc", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->adc(s[3].x, s[3].x, s[3].x);
		g->adc(s[4].x, s[4].x, s[4].x);
		g->ccmn(s[5].x, s[5].x, 0xf, Cond::EQ);
		g->adc(s[5].x, s[5].x, s[5].x);
	), thr_skip6x_patterns));

	t.put("adc x 3 // ccmn x 2", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->ccmn(s[1].x, s[1].x, 0xf, Cond::EQ);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->ccmn(s[2].x, s[2].x, 0xf, Cond::EQ);
	), thr_skip3x_patterns));

	t.put("adc x 3 // ccmn x 3", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->ccmn(s[0].x, s[0].x, 0xf, Cond::EQ);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->ccmn(s[1].x, s[1].x, 0xf, Cond::EQ);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->ccmn(s[2].x, s[2].x, 0xf, Cond::EQ);
	), thr_skip3x_patterns));

	t.put("adc x 4 // ccmn", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->adc(s[3].x, s[3].x, s[3].x);
		g->ccmn(s[3].x, s[3].x, 0xf, Cond::EQ);
	), thr_skip3x_patterns));

	t.put("adc x 4 // ccmn x 2", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->ccmn(s[1].x, s[1].x, 0xf, Cond::EQ);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->ccmn(s[2].x, s[2].x, 0xf, Cond::EQ);
	), thr_skip3x_patterns));

	t.put("adc x 4 // ccmn x 3", thr(b, op(
		g->adc(s[0].x, s[0].x, s[0].x);
		g->ccmn(s[0].x, s[0].x, 0xf, Cond::EQ);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->ccmn(s[1].x, s[1].x, 0xf, Cond::EQ);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->ccmn(s[2].x, s[2].x, 0xf, Cond::EQ);
	), thr_skip3x_patterns));

	t.put("mul x 2 // ccmn", thr(b, op(
		g->mul(s[0].x, s[0].x, s[0].x);
		g->mul(s[1].x, s[1].x, s[1].x);
		g->ccmn(s[2].x, s[2].x, 0xf, Cond::EQ);
	), thr_skip3x_patterns));

	t.put("madd // ccmn", thr(b, op(
		g->udiv(s[0].x, s[0].x, g->x28);
		g->ccmn(s[1].x, s[1].x, 0xf, Cond::EQ);
	), thr_skip2x_patterns));

	t.put("udiv // ccmn", thr(b, op(
		g->madd(s[0].x, s[0].x, s[0].x, g->x28);
		g->ccmn(s[1].x, s[1].x, 0xf, Cond::EQ);
	), thr_skip2x_patterns));

	t.put("bfm // ccmn", thr(b, op(
		g->bfm(s[0].x, s[0].x, 17, 2);
		g->ccmn(s[1].x, s[1].x, 0xf, Cond::EQ);
	), thr_skip2x_patterns));

	t.put("crc32x // ccmn", thr(b, op(
		g->crc32x(s[0].w, s[0].w, g->x28);
		g->ccmn(s[1].x, s[1].x, 0xf, Cond::EQ);
	), thr_skip2x_patterns));


	/* unconditional branch */
	t.put("add x 3 // b", thr(b, op(
		Label l;
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->b(l); g->L(l);
	), lat_inc3_pattern));

	t.put("add x 4 // b", thr(b, op(
		Label l;
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->b(l); g->L(l);
	), lat_inc4_pattern));

	t.put("add x 5 // b", thr(b, op(
		Label l;
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
		g->b(l); g->L(l);
	), lat_inc5_pattern));

	t.put("add x 6 // b", thr(b, op(
		Label l;
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->add(s[4].x, s[4].x, s[4].x);
		g->add(s[5].x, s[5].x, s[5].x);
		g->b(l); g->L(l);
	), lat_inc6_pattern));

	t.put("adc // b", thr(b, op(
		Label l;
		g->adc(s[0].x, s[0].x, s[0].x);
		g->b(l); g->L(l);
	), lat_inc1_pattern));

	t.put("adc x 2 // b", thr(b, op(
		Label l;
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->b(l); g->L(l);
	), lat_inc2_pattern));

	t.put("adc x 3 // b", thr(b, op(
		Label l;
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->b(l); g->L(l);
	), lat_inc3_pattern));

	t.put("mul // b", thr(b, op(
		Label l;
		g->mul(s[0].x, s[0].x, s[0].x);
		g->b(l); g->L(l);
	), lat_inc1_pattern));

	t.put("mul x 2 // b", thr(b, op(
		Label l;
		g->mul(s[0].x, s[0].x, s[0].x);
		g->mul(s[1].x, s[1].x, s[1].x);
		g->b(l); g->L(l);
	), lat_inc2_pattern));


	/* conditional branch (not taken) */
	t.put("add // cbz", thr(b, op(
		Label l;
		g->add(s[0].x, s[0].x, s[0].x);
		g->cbz(g->x28, l); g->L(l);
	), thr_skip1x_patterns));

	t.put("add x 2 // cbz", thr(b, op(
		Label l;
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->cbz(g->x28, l); g->L(l);
	), thr_skip2x_patterns));

	t.put("add x 3 // cbz", thr(b, op(
		Label l;
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->cbz(g->x28, l); g->L(l);
	), thr_skip3x_patterns));

	t.put("add x 4 // cbz", thr(b, op(
		Label l;
		g->add(s[0].x, s[0].x, s[0].x);
		g->add(s[1].x, s[1].x, s[1].x);
		g->add(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->cbz(g->x28, l); g->L(l);
	), thr_skip4x_patterns));

	t.put("adc // cbz x 2", thr(b, op(
		Label l[2];
		g->adc(s[0].x, s[0].x, s[0].x);
		g->cbz(g->x28, l[0]); g->L(l[0]);
		g->cbz(g->x28, l[1]); g->L(l[1]);
	), thr_skip1x_patterns));

	t.put("adc // cbz", thr(b, op(
		Label l;
		g->adc(s[0].x, s[0].x, s[0].x);
		g->cbz(g->x28, l); g->L(l);
	), thr_skip1x_patterns));

	t.put("adc x 2 // cbz", thr(b, op(
		Label l;
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->cbz(g->x28, l); g->L(l);
	), thr_skip2x_patterns));

	t.put("adc x 3 // cbz", thr(b, op(
		Label l;
		g->adc(s[0].x, s[0].x, s[0].x);
		g->adc(s[1].x, s[1].x, s[1].x);
		g->adc(s[2].x, s[2].x, s[2].x);
		g->cbz(g->x28, l); g->L(l);
	), thr_skip3x_patterns));


	/* conditional branch (not taken) */
	t.put("b // cbz", thr(b, op(
		Label l[2];
		g->b(l[0]); g->L(l[0]);
		g->cbz(g->x28, l[1]); g->L(l[1]);
	), lat_inc2_pattern));

	t.put("b x 2", thr(b, op(
		Label l[2];
		g->b(l[0]); g->L(l[0]);
		g->b(l[1]); g->L(l[1]);
	), lat_inc2_pattern));

	/* L1I and L1BTB size estimation, miss penalty estimation */
	#define bl_ret(_body1, _body2, _pattern) thr(b, \
		op( _body1;        g->bl(*gl) ), \
		op_init( \
			Label l; \
			               g->b(l); \
			g->align(128); g->L(*gl); _body2; g->ret(); \
			g->align(128); g->L(l); \
		), \
		_pattern \
	)
	#define blr_ret(_body1, _body2, _pattern) thr(b, \
		op( _body1;        g->blr(g->x28) ), \
		op_init( \
			Label l; \
			               g->adr(g->x1, 128); g->b(l); \
			g->align(128); _body2; g->ret(); \
			g->align(128); g->L(l); \
		), \
		_pattern \
	)
	#define blr_ret(_body1, _body2, _pattern) thr(b, \
		op( _body1;        g->blr(g->x28) ), \
		op_init( \
			Label l; \
			               g->adr(g->x1, 128); g->b(l); \
			g->align(128); _body2; g->ret(); \
			g->align(128); g->L(l); \
		), \
		_pattern \
	)

	#define var_length_core(_basename, _cnt, _body) { \
		static pattern_t const p[] = { { (_cnt), 8, 0, 16 }, { 0, 0, 0, 0 } }; \
		static pattern_t const *pp[] = { (pattern_t const *)&p, NULL }; \
		char buf[1024] = { 0 }; \
		size_t const len = strlen((char const *)(_basename)); \
		size_t const clip = len > 512 ? 512 : len; \
		sprintf(buf, "%.*s x %zu", (int)clip, (char const *)(_basename), (size_t)(_cnt)); \
		t.put(buf, thr(h, op( _body; ), pp)); \
	}
	#define var_length(_basename, _body) { \
		var_length_core(_basename, 1, ( _body )); \
		var_length_core(_basename, 4, ( _body )); \
		var_length_core(_basename, 8, ( _body )); \
		var_length_core(_basename, 16, ( _body )); \
		var_length_core(_basename, 64, ( _body )); \
		var_length_core(_basename, 128, ( _body )); \
		var_length_core(_basename, 144, ( _body )); \
		var_length_core(_basename, 160, ( _body )); \
		var_length_core(_basename, 176, ( _body )); \
		var_length_core(_basename, 192, ( _body )); \
		var_length_core(_basename, 256, ( _body )); \
		var_length_core(_basename, 512, ( _body )); \
		var_length_core(_basename, 640, ( _body )); \
		var_length_core(_basename, 1024, ( _body )); \
		var_length_core(_basename, 4096, ( _body )); \
		var_length_core(_basename, 6144, ( _body )); \
		var_length_core(_basename, 6240, ( _body )); \
		var_length_core(_basename, 6320, ( _body )); \
		var_length_core(_basename, 6400, ( _body )); \
		var_length_core(_basename, 7168, ( _body )); \
		var_length_core(_basename, 16384, ( _body )); \
		var_length_core(_basename, 32767, ( _body )); \
	}

	var_length("nop x 8", ({
		g->nop(); g->nop(); g->nop(); g->nop(); g->nop(); g->nop(); g->nop(); g->nop();
	}));
	var_length("(add, sub, eor, nop) x 2", ({
		g->add(s[0].x, s[0].x, s[0].x);
		g->sub(s[1].x, s[1].x, s[1].x);
		g->eor(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->sub(s[4].x, s[4].x, s[4].x);
		g->eor(s[5].x, s[5].x, s[5].x);
		g->nop();
		g->nop();
	}));
	var_length("(add, sub, eor)  x 2 // (nop, b)", ({
		Label l[1];
		g->add(s[0].x, s[0].x, s[0].x);
		g->sub(s[1].x, s[1].x, s[1].x);
		g->eor(s[2].x, s[2].x, s[2].x);
		g->add(s[3].x, s[3].x, s[3].x);
		g->sub(s[4].x, s[4].x, s[4].x);
		g->eor(s[5].x, s[5].x, s[5].x);
		g->nop();
		g->b(l[0]); g->L(l[0]);
	}));
	var_length("(add, sub, eor, b) x 2", ({
		Label l[2];
		g->add(s[0].x, s[0].x, s[0].x);
		g->sub(s[1].x, s[1].x, s[1].x);
		g->eor(s[2].x, s[2].x, s[2].x);
		g->b(l[0]); g->L(l[0]);
		g->add(s[3].x, s[3].x, s[3].x);
		g->sub(s[4].x, s[4].x, s[4].x);
		g->eor(s[5].x, s[5].x, s[5].x);
		g->b(l[1]); g->L(l[1]);
	}));
	var_length("b x 3", ({
		Label l[3];
		g->add(s[0].x, s[0].x, s[0].x);
		g->sub(s[1].x, s[1].x, s[1].x);
		g->eor(s[2].x, s[2].x, s[2].x);
		g->b(l[0]); g->L(l[0]);
		g->add(s[3].x, s[3].x, s[3].x);
		g->b(l[1]); g->L(l[1]);
		g->eor(s[4].x, s[4].x, s[4].x);
		g->b(l[2]); g->L(l[2]);
	}));
	var_length("b x 4", ({
		Label l[4];
		g->add(s[0].x, s[0].x, s[0].x);
		g->b(l[0]); g->L(l[0]);
		g->eor(s[1].x, s[1].x, s[1].x);
		g->b(l[1]); g->L(l[1]);
		g->add(s[2].x, s[2].x, s[2].x);
		g->b(l[2]); g->L(l[2]);
		g->eor(s[3].x, s[3].x, s[3].x);
		g->b(l[3]); g->L(l[3]);
	}));
	var_length("b x 5", ({
		Label l[5];
		g->add(s[0].x, s[0].x, s[0].x);
		g->b(l[0]); g->L(l[0]);
		g->eor(s[1].x, s[1].x, s[1].x);
		g->b(l[1]); g->L(l[1]);
		g->add(s[2].x, s[2].x, s[2].x);
		g->b(l[2]); g->L(l[2]);
		g->b(l[3]); g->L(l[3]);
		g->b(l[4]); g->L(l[4]);
	}));
	var_length("b x 6", ({
		Label l[6];
		g->add(s[0].x, s[0].x, s[0].x);
		g->b(l[0]); g->L(l[0]);
		g->b(l[1]); g->L(l[1]);
		g->b(l[2]); g->L(l[2]);
		g->add(s[1].x, s[1].x, s[1].x);
		g->b(l[3]); g->L(l[3]);
		g->b(l[4]); g->L(l[4]);
		g->b(l[5]); g->L(l[5]);
	}));
	var_length("b x 7", ({
		Label l[7];
		g->add(s[0].x, s[0].x, s[0].x);
		g->b(l[0]); g->L(l[0]);
		g->b(l[1]); g->L(l[1]);
		g->b(l[2]); g->L(l[2]);
		g->b(l[3]); g->L(l[3]);
		g->b(l[4]); g->L(l[4]);
		g->b(l[5]); g->L(l[5]);
		g->b(l[6]); g->L(l[6]);
	}));
	var_length("b x 8", ({
		Label l[8];
		g->b(l[0]); g->L(l[0]);
		g->b(l[1]); g->L(l[1]);
		g->b(l[2]); g->L(l[2]);
		g->b(l[3]); g->L(l[3]);
		g->b(l[4]); g->L(l[4]);
		g->b(l[5]); g->L(l[5]);
		g->b(l[6]); g->L(l[6]);
		g->b(l[7]); g->L(l[7]);
	}));


	/* bl-ret */
	t.put("bl-ret", bl_ret(
		{ ; },
		{ ; },
		lat_inc1_pattern
	));
	t.put("bl-ret // add // add", bl_ret(
		{ g->add(g->x0, g->x0, g->x0); },
		{ g->add(g->x6, g->x6, g->x6); },
		lat_inc1_pattern
	));
	t.put("bl-ret // add x 2 // add x 2", bl_ret(
		{ g->add(g->x0, g->x0, g->x0); g->add(g->x1, g->x1, g->x1); },
		{ g->add(g->x6, g->x6, g->x6); g->add(g->x7, g->x7, g->x7); },
		lat_inc2_pattern
	));
	t.put("bl-ret // add x 3 // add x 3", bl_ret(
		{ g->add(g->x0, g->x0, g->x0); g->add(g->x1, g->x1, g->x1); g->add(g->x2, g->x2, g->x2); },
		{ g->add(g->x6, g->x6, g->x6); g->add(g->x7, g->x7, g->x7); g->add(g->x8, g->x8, g->x8); },
		lat_inc3_pattern
	));
	t.put("bl-ret // add x 4 // add x 4", bl_ret(
		{ g->add(g->x0, g->x0, g->x0); g->add(g->x1, g->x1, g->x1); g->add(g->x2, g->x2, g->x2); g->add(g->x3, g->x3, g->x3); },
		{ g->add(g->x6, g->x6, g->x6); g->add(g->x7, g->x7, g->x7); g->add(g->x8, g->x8, g->x8); g->add(g->x9, g->x9, g->x9); },
		lat_inc4_pattern
	));
	t.put("bl-ret // add x 5 // add x 5", bl_ret(
		{ g->add(g->x0, g->x0, g->x0); g->add(g->x1, g->x1, g->x1); g->add(g->x2, g->x2, g->x2); g->add(g->x3, g->x3, g->x3); g->add(g->x4,  g->x4,  g->x4); },
		{ g->add(g->x6, g->x6, g->x6); g->add(g->x7, g->x7, g->x7); g->add(g->x8, g->x8, g->x8); g->add(g->x9, g->x9, g->x9); g->add(g->x10, g->x10, g->x10); },
		lat_inc5_pattern
	));
	t.put("bl-ret // add x 6 // add x 6", bl_ret(
		{ g->add(g->x0, g->x0, g->x0); g->add(g->x1, g->x1, g->x1); g->add(g->x2, g->x2, g->x2); g->add(g->x3, g->x3, g->x3); g->add(g->x4,  g->x4,  g->x4);  g->add(g->x5,  g->x5,  g->x5); },
		{ g->add(g->x6, g->x6, g->x6); g->add(g->x7, g->x7, g->x7); g->add(g->x8, g->x8, g->x8); g->add(g->x9, g->x9, g->x9); g->add(g->x10, g->x10, g->x10); g->add(g->x11, g->x11, g->x11); },
		lat_inc6_pattern
	));

	t.put("bl-ret // adc // adc", bl_ret(
		{ g->adc(g->x0, g->x0, g->x0); },
		{ g->adc(g->x6, g->x6, g->x6); },
		lat_inc1_pattern
	));
	t.put("bl-ret // adc x 2 // adc x 2", bl_ret(
		{ g->adc(g->x0, g->x0, g->x0); g->adc(g->x1, g->x1, g->x1); },
		{ g->adc(g->x6, g->x6, g->x6); g->adc(g->x7, g->x7, g->x7); },
		lat_inc2_pattern
	));
	t.put("bl-ret // adc x 3 // adc x 3", bl_ret(
		{ g->adc(g->x0, g->x0, g->x0); g->adc(g->x1, g->x1, g->x1); g->adc(g->x2, g->x2, g->x2); },
		{ g->adc(g->x6, g->x6, g->x6); g->adc(g->x7, g->x7, g->x7); g->adc(g->x8, g->x8, g->x8); },
		lat_inc3_pattern
	));
	t.put("bl-ret // adc x 2 // adc x 3", bl_ret(
		{ g->adc(g->x0, g->x0, g->x0); g->adc(g->x1, g->x1, g->x1); },
		{ g->adc(g->x6, g->x6, g->x6); g->adc(g->x7, g->x7, g->x7); g->adc(g->x8, g->x8, g->x8); },
		lat_inc3_pattern
	));
	t.put("bl-ret // adc x 3 // adc x 2", bl_ret(
		{ g->adc(g->x0, g->x0, g->x0); g->adc(g->x1, g->x1, g->x1); g->adc(g->x2, g->x2, g->x2); },
		{ g->adc(g->x6, g->x6, g->x6); g->adc(g->x7, g->x7, g->x7); },
		lat_inc3_pattern
	));

	t.put("bl-ret // mul // mul", bl_ret(
		{ g->mov(g->x0, g->x2); g->mul(g->x0, g->x0, g->x0); },
		{ g->mov(g->x6, g->x8); g->mul(g->x6, g->x6, g->x6); },
		lat_inc6_pattern
	));
	t.put("bl-ret // mul x 2 // mul x 2", bl_ret(
		{ g->mov(g->x0, g->x2); g->mul(g->x0, g->x0, g->x0); g->mov(g->x1, g->x3); g->mul(g->x1, g->x1, g->x1); },
		{ g->mov(g->x6, g->x8); g->mul(g->x6, g->x6, g->x6); g->mov(g->x7, g->x9); g->mul(g->x7, g->x7, g->x7); },
		lat_inc6_pattern
	));
	t.put("bl-ret // madd // madd", bl_ret(
		{ g->mov(g->x0, g->x2); g->madd(g->x0, g->x0, g->x0, g->x1); },
		{ g->mov(g->x6, g->x8); g->madd(g->x6, g->x6, g->x6, g->x7); },
		lat_inc6_pattern
	));


	/* blr-ret */
	t.put("blr-ret", blr_ret(
		{ ; },
		{ ; },
		lat_inc1_pattern
	));
	t.put("blr-ret // add // add", blr_ret(
		{ g->add(g->x0, g->x0, g->x0); },
		{ g->add(g->x6, g->x6, g->x6); },
		lat_inc1_pattern
	));
	t.put("blr-ret // add x 2 // add x 2", blr_ret(
		{ g->add(g->x0, g->x0, g->x0); g->add(g->x1, g->x1, g->x1); },
		{ g->add(g->x6, g->x6, g->x6); g->add(g->x7, g->x7, g->x7); },
		lat_inc2_pattern
	));
	t.put("blr-ret // add x 3 // add x 3", blr_ret(
		{ g->add(g->x0, g->x0, g->x0); g->add(g->x1, g->x1, g->x1); g->add(g->x2, g->x2, g->x2); },
		{ g->add(g->x6, g->x6, g->x6); g->add(g->x7, g->x7, g->x7); g->add(g->x8, g->x8, g->x8); },
		lat_inc3_pattern
	));
	t.put("blr-ret // add x 4 // add x 4", blr_ret(
		{ g->add(g->x0, g->x0, g->x0); g->add(g->x1, g->x1, g->x1); g->add(g->x2, g->x2, g->x2); g->add(g->x3, g->x3, g->x3); },
		{ g->add(g->x6, g->x6, g->x6); g->add(g->x7, g->x7, g->x7); g->add(g->x8, g->x8, g->x8); g->add(g->x9, g->x9, g->x9); },
		lat_inc4_pattern
	));
	t.put("blr-ret // add x 5 // add x 5", blr_ret(
		{ g->add(g->x0, g->x0, g->x0); g->add(g->x1, g->x1, g->x1); g->add(g->x2, g->x2, g->x2); g->add(g->x3, g->x3, g->x3); g->add(g->x4,  g->x4,  g->x4); },
		{ g->add(g->x6, g->x6, g->x6); g->add(g->x7, g->x7, g->x7); g->add(g->x8, g->x8, g->x8); g->add(g->x9, g->x9, g->x9); g->add(g->x10, g->x10, g->x10); },
		lat_inc5_pattern
	));
	t.put("blr-ret // add x 6 // add x 6", blr_ret(
		{ g->add(g->x0, g->x0, g->x0); g->add(g->x1, g->x1, g->x1); g->add(g->x2, g->x2, g->x2); g->add(g->x3, g->x3, g->x3); g->add(g->x4,  g->x4,  g->x4);  g->add(g->x5,  g->x5,  g->x5); },
		{ g->add(g->x6, g->x6, g->x6); g->add(g->x7, g->x7, g->x7); g->add(g->x8, g->x8, g->x8); g->add(g->x9, g->x9, g->x9); g->add(g->x10, g->x10, g->x10); g->add(g->x11, g->x11, g->x11); },
		lat_inc6_pattern
	));

	t.put("blr-ret // adc // adc", blr_ret(
		{ g->adc(g->x0, g->x0, g->x0); },
		{ g->adc(g->x6, g->x6, g->x6); },
		lat_inc1_pattern
	));
	t.put("blr-ret // adc x 2 // adc x 2", blr_ret(
		{ g->adc(g->x0, g->x0, g->x0); g->adc(g->x1, g->x1, g->x1); },
		{ g->adc(g->x6, g->x6, g->x6); g->adc(g->x7, g->x7, g->x7); },
		lat_inc2_pattern
	));
	t.put("blr-ret // adc x 3 // adc x 3", blr_ret(
		{ g->adc(g->x0, g->x0, g->x0); g->adc(g->x1, g->x1, g->x1); g->adc(g->x2, g->x2, g->x2); },
		{ g->adc(g->x6, g->x6, g->x6); g->adc(g->x7, g->x7, g->x7); g->adc(g->x8, g->x8, g->x8); },
		lat_inc3_pattern
	));
	t.put("blr-ret // adc x 2 // adc x 3", blr_ret(
		{ g->adc(g->x0, g->x0, g->x0); g->adc(g->x1, g->x1, g->x1); },
		{ g->adc(g->x6, g->x6, g->x6); g->adc(g->x7, g->x7, g->x7); g->adc(g->x8, g->x8, g->x8); },
		lat_inc3_pattern
	));
	t.put("blr-ret // adc x 3 // adc x 2", blr_ret(
		{ g->adc(g->x0, g->x0, g->x0); g->adc(g->x1, g->x1, g->x1); g->adc(g->x2, g->x2, g->x2); },
		{ g->adc(g->x6, g->x6, g->x6); g->adc(g->x7, g->x7, g->x7); },
		lat_inc3_pattern
	));

	t.put("blr-ret // mul // mul", blr_ret(
		{ g->mov(g->x0, g->x2); g->mul(g->x0, g->x0, g->x0); },
		{ g->mov(g->x6, g->x8); g->mul(g->x6, g->x6, g->x6); },
		lat_inc6_pattern
	));
	t.put("blr-ret // mul x 2 // mul x 2", blr_ret(
		{ g->mov(g->x0, g->x2); g->mul(g->x0, g->x0, g->x0); g->mov(g->x1, g->x3); g->mul(g->x1, g->x1, g->x1); },
		{ g->mov(g->x6, g->x8); g->mul(g->x6, g->x6, g->x6); g->mov(g->x7, g->x9); g->mul(g->x7, g->x7, g->x7); },
		lat_inc6_pattern
	));
	t.put("blr-ret // madd // madd", blr_ret(
		{ g->mov(g->x0, g->x2); g->madd(g->x0, g->x0, g->x0, g->x1); },
		{ g->mov(g->x6, g->x8); g->madd(g->x6, g->x6, g->x6, g->x7); },
		lat_inc6_pattern
	));


	return;
}

/*
 * end of portcap.cc
 */
