
/**
 * @file portcap.cc
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>
#include <functional>

/*
 * collision bench for port capability estimation
 */
typedef std::function<void (CodeGenerator *, AReg const *, size_t)> op_col_t;
#define col(_b, ...)			try_setjmp((_b).col_(0, __VA_ARGS__))

#define op_col_fn(_id, _body)	void _id(CodeGenerator *g, AReg const *s, size_t i) { (void)g; (void)s; (void)i; _body; }

/* nop */
op_col_fn(nop,                 ({ g->nop(); }) )

/* load */
op_col_fn(ldr_x_s,             ({ g->ldr(s->x, ptr(s->x)); }) )
op_col_fn(ldr_x_x28,           ({ g->ldr(s->x, ptr(g->x28)); }) )
op_col_fn(ldr_x_s_x28,         ({ g->ldr(s->x, ptr(s->x, g->x28)); }) )
op_col_fn(ldr_x_s_16,          ({ g->ldr(s->x, ptr(s->x, 16)); }) )
op_col_fn(ldr_x_s_32,          ({ g->ldr(s->x, ptr(s->x, 32)); }) )
op_col_fn(ldr_x_s_k,           ({ g->ldr(s->x, ptr(s->x, (uint32_t)i)); }) )
op_col_fn(ldr_x_s_x28_lsl3,    ({ g->ldr(s->x, ptr(s->x, g->x28, ShMod::LSL, 3)); }) )
op_col_fn(ldr_x_pre_s,         ({ g->ldr(g->x28, pre_ptr(s->x, 0)); g->mov(s->x, g->x28); }) )

op_col_fn(ldr_q_s,             ({ g->ldr(s->q, ptr(s->x)); }) )
op_col_fn(ldr_q_x28,           ({ g->ldr(s->q, ptr(g->x28)); }) )
op_col_fn(ldr_q_s_x28,         ({ g->ldr(s->q, ptr(s->x, g->x28)); }) )
op_col_fn(ldr_q_s_16,          ({ g->ldr(s->q, ptr(s->x, 16)); }) )
op_col_fn(ldr_q_s_32,          ({ g->ldr(s->q, ptr(s->x, 32)); }) )
op_col_fn(ldr_q_s_k,           ({ g->ldr(s->q, ptr(s->x, (uint32_t)i)); }) )
op_col_fn(ldr_q_s_x28_lsl3,    ({ g->ldr(s->q, ptr(s->x, g->x28, ShMod::LSL, 3)); }) )
op_col_fn(ldr_q_pre_s,         ({ g->ldr(s->q, pre_ptr(s->x, 0)); }) )

/* store */
op_col_fn(str_x_s,             ({ g->str(s->x, ptr(s->x)); }) )
op_col_fn(str_x_x28,           ({ g->str(s->x, ptr(g->x28)); }) )
op_col_fn(str_x_s_x28,         ({ g->str(s->x, ptr(s->x, g->x28)); }) )
op_col_fn(str_x_s_16,          ({ g->str(s->x, ptr(s->x, 16)); }) )
op_col_fn(str_x_s_32,          ({ g->str(s->x, ptr(s->x, 32)); }) )
op_col_fn(str_x_s_k,           ({ g->str(s->x, ptr(s->x, (uint32_t)i)); }) )
op_col_fn(str_x_s_x28_lsl3,    ({ g->str(s->x, ptr(s->x, g->x28, ShMod::LSL, 3)); }) )
op_col_fn(str_x_pre_s,         ({ g->str(g->x28, pre_ptr(s->x, 0)); }) )

op_col_fn(str_q_s,             ({ g->str(s->q, ptr(s->x)); }) )
op_col_fn(str_q_x28,           ({ g->str(s->q, ptr(g->x28)); }) )
op_col_fn(str_q_s_x28,         ({ g->str(s->q, ptr(s->x, g->x28)); }) )
op_col_fn(str_q_s_16,          ({ g->str(s->q, ptr(s->x, 16)); }) )
op_col_fn(str_q_s_32,          ({ g->str(s->q, ptr(s->x, 32)); }) )
op_col_fn(str_q_s_k,           ({ g->str(s->q, ptr(s->x, (uint32_t)i)); }) )
op_col_fn(str_q_s_x28_lsl3,    ({ g->str(s->q, ptr(s->x, g->x28, ShMod::LSL, 3)); }) )
op_col_fn(str_q_pre_s,         ({ g->str(s->q, pre_ptr(s->x, 0)); }) )

/* load-to-store */
op_col_fn(ldr_str_x_s_0,       ({ g->str(s->x, ptr(s->x, (uint32_t)i * 0));   g->ldr(s->x, ptr(s->x, (uint32_t)i * 0)); }) )
op_col_fn(ldr_str_x_s_128,     ({ g->str(s->x, ptr(s->x, (uint32_t)i * 128)); g->ldr(s->x, ptr(s->x, (uint32_t)i * 128)); }) )

/* scalar arith */
op_col_fn(add_x,               ({ g->add(s->x, s->x, s->x); }) )
op_col_fn(add_shl2_x,          ({ g->add(s->x, s->x, s->x, ShMod::LSL, 2); }) )
op_col_fn(adc_x,               ({ g->adc(s->x, s->x, s->x); }) )
op_col_fn(adds_x,              ({ g->adds(s->x, s->x, s->x); }) )
op_col_fn(adcs_x,              ({ g->adcs(s->x, s->x, g->x28); }) )
op_col_fn(mul_x,               ({ g->mul(s->x, s->x, s->x); }) )
op_col_fn(mul_x0,              ({ g->mul(g->x0, g->x0, g->x0); }) )
op_col_fn(mul_x1,              ({ g->mul(g->x1, g->x1, g->x1); }) )
op_col_fn(madd_x,              ({ g->madd(s->x, s->x, s->x, g->x28); }) )
op_col_fn(udiv_x_x28,          ({ g->udiv(s->x, s->x, g->x28); }) )
op_col_fn(bfm_x,               ({ g->bfm(s->x, s->x, 17, 2); }) )
op_col_fn(crc32x_w,            ({ g->crc32x(s->w, s->w, g->x28); }) )
op_col_fn(ccmn_eq_x,           ({ g->ccmn(s->x, s->x, 0xf, Cond::EQ); }) )

/* movi */
op_col_fn(movi_x,              ({ g->mov(s->x, 0x1ffc); }) )
op_col_fn(movi_add_x,          ({ g->mov(s->x, 0x1ffc); g->add(s->x, s->x, s->x); }) )
op_col_fn(movi_add_x_prev,     ({ g->mov(s->x, 0x1ffc); g->add(s->x, s[-1].x, s->x); }) )
op_col_fn(movi_adc_x,          ({ g->mov(s->x, 0x1ffc); g->adc(s->x, s->x, s->x); }) )
op_col_fn(movi_adc_x_prev,     ({ g->mov(s->x, 0x1ffc); g->adc(s->x, s[-1].x, s->x); }) )

/* floating point arith and misc */
op_col_fn(fcmp_s,              ({ g->fcmp(s->s, s->s); }) )
op_col_fn(fcmp_0,              ({ g->fcmp(s->s, 0.0); }) )
op_col_fn(fcmp_0_adc_x,        ({ g->fcmp(s->s, 0.0); g->adc(s->x, s->x, s->x); }) )
op_col_fn(fcsel_s,             ({ g->fcsel(s->s, s->s, s->s, Cond::EQ); }) )
op_col_fn(adds_fcsel_s,        ({ g->adds(s->x, s->x, s->x); g->fcsel(s->s, s->s, s->s, Cond::EQ); }) )
op_col_fn(fcvtas_s,            ({ g->fcvtas(s->s, s->s); }) )
op_col_fn(fcvtas_x,            ({ g->fcvtas(s->x, s->s); }) )
op_col_fn(fjcvtzs_d,           ({ g->fjcvtzs(s->w, s->d); }) )
op_col_fn(scvtf_w,             ({ g->scvtf(s->s, s->w); }) )
op_col_fn(frecpe_s,            ({ g->frecpe(s->s, s->s); }) )
op_col_fn(frsqrte_s,           ({ g->frsqrte(s->s, s->s); }) )
op_col_fn(fmov_d_x,            ({ g->fmov(s->d, s->x); }) )
op_col_fn(fmov_x_d,            ({ g->fmov(s->x, s->d); }) )
op_col_fn(dup_d_x,             ({ g->dup(s->v.d, s->x); }) )
op_col_fn(mov_x_d,             ({ g->mov(s->x, s->v.d[0]); }) )
op_col_fn(mov_x_d1,            ({ g->mov(s->x, s->v.d[1]); }) )

/* SIMD arith */
op_col_fn(add_v,               ({ g->add(s->v.b, s->v.b, s->v.b); }) )
op_col_fn(eor_v,               ({ g->eor(s->v.b, s->v.b, s->v.b); }) )

/* crypto */
op_col_fn(sha1c_v,             ({ g->sha1c(s->q, s->s, s->v.s); }) )
op_col_fn(sha1h_v,             ({ g->sha1h(s->s, s->s); }) )
op_col_fn(sha1m_v,             ({ g->sha1m(s->q, s->s, s->v.s); }) )
op_col_fn(sha1p_v,             ({ g->sha1p(s->q, s->s, s->v.s); }) )
op_col_fn(sha1su0_v,           ({ g->sha1su0(s->v.s, s->v.s, s->v.s); }) )
op_col_fn(sha1su1_v,           ({ g->sha1su1(s->v.s, s->v.s); }) )

op_col_fn(sha256h_v,           ({ g->sha256h(s->q, s->q, s->v.s); }) )
op_col_fn(sha256h2_v,          ({ g->sha256h2(s->q, s->q, s->v.s); }) )
op_col_fn(sha256su0_v,         ({ g->sha256su0(s->v.s, s->v.s); }) )
op_col_fn(sha256su1_v,         ({ g->sha256su1(s->v.s, s->v.s, s->v.s); }) )

/* branch */
op_col_fn(b_pc4,               ({ Label l; g->b(l); g->L(l); }) )
op_col_fn(beq_pc4,             ({ Label l; g->b(Cond::EQ, l); g->L(l); }) )
op_col_fn(bne_pc4,             ({ Label l; g->b(Cond::NE, l); g->L(l); }) )
op_col_fn(cbz_x28_pc4,         ({ Label l; g->cbz(g->x28, l); g->L(l); }) )


class bench_col : public bench {
public:
	bench_col(
		double const &_freq,
		size_t const &_xseed = 1,
		size_t const &_xconst = 1,
		uint8_t const &_vseed = 1,
		uint8_t const &_vconst = 1,
		size_t const &_rep = 1,
		size_t const &_div = 1,
		size_t const &_n_insns_body = 1
	) : bench(_freq, _xseed, _xconst, vdupq_n_u8(_vseed), vdupq_n_u8(_vconst), _rep, _div, _n_insns_body) {
	}
	bench_col(
		double const &_freq,
		void *const &_xseed,
		void *const &_xconst = NULL,
		uint8_t const &_vseed = 1,
		uint8_t const &_vconst = 1,
		size_t const &_rep = 1,
		size_t const &_div = 1,
		size_t const &_n_insns_body = 1
	) : bench(_freq, (size_t)_xseed, (size_t)_xconst, vdupq_n_u8(_vseed), vdupq_n_u8(_vconst), _rep, _div, _n_insns_body) {
	}

	/* base */
	measure_t col_(size_t line, op_col_t op1,                                                                                                   pattern_t const **q = thr_skip1x_patterns)           { return(thr_(line,  op_cap( op1(this, &s[0], 0); ),                                                                                                                                                    q)); }
	measure_t col_(size_t line, op_col_t op1, op_col_t op2,                                                                                     pattern_t const **q = thr_skip2x_patterns)           { return(thr_(line,  op_cap( op1(this, &s[0], 0); op2(this, &s[1], 1); ),                                                                                                                               q)); }
	measure_t col_(size_t line, op_col_t op1, op_col_t op2, op_col_t op3,                                                                       pattern_t const **q = thr_skip3x_patterns)           { return(thr_(line,  op_cap( op1(this, &s[0], 0); op2(this, &s[1], 1); op3(this, &s[2], 2); ),                                                                                                          q)); }
	measure_t col_(size_t line, op_col_t op1, op_col_t op2, op_col_t op3, op_col_t op4,                                                         pattern_t const **q = thr_skip4x_patterns)           { return(thr_(line,  op_cap( op1(this, &s[0], 0); op2(this, &s[1], 1); op3(this, &s[2], 2); op4(this, &s[3], 3); ),                                                                                     q)); }
	measure_t col_(size_t line, op_col_t op1, op_col_t op2, op_col_t op3, op_col_t op4, op_col_t op5,                                           pattern_t const **q = thr_skip5x_patterns)           { return(thr_(line,  op_cap( op1(this, &s[0], 0); op2(this, &s[1], 1); op3(this, &s[2], 2); op4(this, &s[3], 3); op5(this, &s[4], 4); ),                                                                q)); }
	measure_t col_(size_t line, op_col_t op1, op_col_t op2, op_col_t op3, op_col_t op4, op_col_t op5, op_col_t op6,                             pattern_t const **q = thr_skip6x_patterns)           { return(thr_(line,  op_cap( op1(this, &s[0], 0); op2(this, &s[1], 1); op3(this, &s[2], 2); op4(this, &s[3], 3); op5(this, &s[4], 4); op6(this, &s[5], 5); ),                                           q)); }
	measure_t col_(size_t line, op_col_t op1, op_col_t op2, op_col_t op3, op_col_t op4, op_col_t op5, op_col_t op6, op_col_t op7,               pattern_t const **q = thr_skip7x_patterns)           { return(thr_(line,  op_cap( op1(this, &s[0], 0); op2(this, &s[1], 1); op3(this, &s[2], 2); op4(this, &s[3], 3); op5(this, &s[4], 4); op6(this, &s[5], 5); op7(this, &s[6], 6); ),                      q)); }
	measure_t col_(size_t line, op_col_t op1, op_col_t op2, op_col_t op3, op_col_t op4, op_col_t op5, op_col_t op6, op_col_t op7, op_col_t op8, pattern_t const **q = thr_skip8x_patterns)           { return(thr_(line,  op_cap( op1(this, &s[0], 0); op2(this, &s[1], 1); op3(this, &s[2], 2); op4(this, &s[3], 3); op5(this, &s[4], 4); op6(this, &s[5], 5); op7(this, &s[6], 6); op8(this, &s[7], 7); ), q)); }

	/* w/ latency */
	measure_t col_(size_t line, op_col_t op1,                                                                                                   pattern_t const **q, pattern_t const **r)            { return(both_(line, op_cap( op1(this, &s[0], 0); ),                                                                                                                                                    0.0, q, r)); }
	measure_t col_(size_t line, op_col_t op1, op_col_t op2,                                                                                     pattern_t const **q, pattern_t const **r)            { return(both_(line, op_cap( op1(this, &s[0], 0); op2(this, &s[1], 1); ),                                                                                                                               0.0, q, r)); }
	measure_t col_(size_t line, op_col_t op1, op_col_t op2, op_col_t op3,                                                                       pattern_t const **q, pattern_t const **r)            { return(both_(line, op_cap( op1(this, &s[0], 0); op2(this, &s[1], 1); op3(this, &s[2], 2); ),                                                                                                          0.0, q, r)); }
	measure_t col_(size_t line, op_col_t op1, op_col_t op2, op_col_t op3, op_col_t op4,                                                         pattern_t const **q, pattern_t const **r)            { return(both_(line, op_cap( op1(this, &s[0], 0); op2(this, &s[1], 1); op3(this, &s[2], 2); op4(this, &s[3], 3); ),                                                                                     0.0, q, r)); }
	measure_t col_(size_t line, op_col_t op1, op_col_t op2, op_col_t op3, op_col_t op4, op_col_t op5,                                           pattern_t const **q, pattern_t const **r)            { return(both_(line, op_cap( op1(this, &s[0], 0); op2(this, &s[1], 1); op3(this, &s[2], 2); op4(this, &s[3], 3); op5(this, &s[4], 4); ),                                                                0.0, q, r)); }
	measure_t col_(size_t line, op_col_t op1, op_col_t op2, op_col_t op3, op_col_t op4, op_col_t op5, op_col_t op6,                             pattern_t const **q, pattern_t const **r)            { return(both_(line, op_cap( op1(this, &s[0], 0); op2(this, &s[1], 1); op3(this, &s[2], 2); op4(this, &s[3], 3); op5(this, &s[4], 4); op6(this, &s[5], 5); ),                                           0.0, q, r)); }
	measure_t col_(size_t line, op_col_t op1, op_col_t op2, op_col_t op3, op_col_t op4, op_col_t op5, op_col_t op6, op_col_t op7,               pattern_t const **q, pattern_t const **r)            { return(both_(line, op_cap( op1(this, &s[0], 0); op2(this, &s[1], 1); op3(this, &s[2], 2); op4(this, &s[3], 3); op5(this, &s[4], 4); op6(this, &s[5], 5); op7(this, &s[6], 6); ),                      0.0, q, r)); }
	measure_t col_(size_t line, op_col_t op1, op_col_t op2, op_col_t op3, op_col_t op4, op_col_t op5, op_col_t op6, op_col_t op7, op_col_t op8, pattern_t const **q, pattern_t const **r)            { return(both_(line, op_cap( op1(this, &s[0], 0); op2(this, &s[1], 1); op3(this, &s[2], 2); op4(this, &s[3], 3); op5(this, &s[4], 4); op6(this, &s[5], 5); op7(this, &s[6], 6); op8(this, &s[7], 7); ), 0.0, q, r)); }

	/* w/ a parameter */
	measure_t col_(size_t line, op_col_t op1,                                                                                                   size_t k, pattern_t const **q = thr_skip1x_patterns) { return(thr_(line,  op_cap( op1(this, &s[0], k); ),                                                                                                                                                    q)); }
	measure_t col_(size_t line, op_col_t op1, op_col_t op2,                                                                                     size_t k, pattern_t const **q = thr_skip2x_patterns) { return(thr_(line,  op_cap( op1(this, &s[0], k); op2(this, &s[1], k); ),                                                                                                                               q)); }
	measure_t col_(size_t line, op_col_t op1, op_col_t op2, op_col_t op3,                                                                       size_t k, pattern_t const **q = thr_skip3x_patterns) { return(thr_(line,  op_cap( op1(this, &s[0], k); op2(this, &s[1], k); op3(this, &s[2], k); ),                                                                                                          q)); }
	measure_t col_(size_t line, op_col_t op1, op_col_t op2, op_col_t op3, op_col_t op4,                                                         size_t k, pattern_t const **q = thr_skip4x_patterns) { return(thr_(line,  op_cap( op1(this, &s[0], k); op2(this, &s[1], k); op3(this, &s[2], k); op4(this, &s[3], k); ),                                                                                     q)); }
	measure_t col_(size_t line, op_col_t op1, op_col_t op2, op_col_t op3, op_col_t op4, op_col_t op5,                                           size_t k, pattern_t const **q = thr_skip5x_patterns) { return(thr_(line,  op_cap( op1(this, &s[0], k); op2(this, &s[1], k); op3(this, &s[2], k); op4(this, &s[3], k); op5(this, &s[4], k); ),                                                                q)); }
	measure_t col_(size_t line, op_col_t op1, op_col_t op2, op_col_t op3, op_col_t op4, op_col_t op5, op_col_t op6,                             size_t k, pattern_t const **q = thr_skip6x_patterns) { return(thr_(line,  op_cap( op1(this, &s[0], k); op2(this, &s[1], k); op3(this, &s[2], k); op4(this, &s[3], k); op5(this, &s[4], k); op6(this, &s[5], k); ),                                           q)); }
	measure_t col_(size_t line, op_col_t op1, op_col_t op2, op_col_t op3, op_col_t op4, op_col_t op5, op_col_t op6, op_col_t op7,               size_t k, pattern_t const **q = thr_skip7x_patterns) { return(thr_(line,  op_cap( op1(this, &s[0], k); op2(this, &s[1], k); op3(this, &s[2], k); op4(this, &s[3], k); op5(this, &s[4], k); op6(this, &s[5], k); op7(this, &s[6], k); ),                      q)); }
	measure_t col_(size_t line, op_col_t op1, op_col_t op2, op_col_t op3, op_col_t op4, op_col_t op5, op_col_t op6, op_col_t op7, op_col_t op8, size_t k, pattern_t const **q = thr_skip8x_patterns) { return(thr_(line,  op_cap( op1(this, &s[0], k); op2(this, &s[1], k); op3(this, &s[2], k); op4(this, &s[3], k); op5(this, &s[4], k); op6(this, &s[5], k); op7(this, &s[6], k); op8(this, &s[7], k); ), q)); }
};


static
void bench_cap_base(bool md, double freq) {
	table t(md, "base latency / throughput", 1, 20);
	bench_col b(freq);

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

	t.put("mul (chain) // div", col(b, mul_x0, udiv_x_x28));
	t.put("mul (chain) x 2 // div", col(b, mul_x0, mul_x1, udiv_x_x28));
	t.put("mul (chain) x 3 // div", col(b, mul_x0, mul_x1, mul_x0, udiv_x_x28));
	t.put("mul (chain) x 4 // div", col(b, mul_x0, mul_x1, mul_x0, mul_x1, udiv_x_x28));


	t.put("add",                                            both(b, op( g->add(d->x, d->x, s->x) )));
	t.put("add (<<2)",                                      both(b, op( g->add(d->x, d->x, s->x, ShMod::LSL, 2) )));
	t.put("add (<<2)",                                      both(b, op( g->add(d->x, s->x, d->x, ShMod::LSL, 2) )));
	t.put("add (<<2)",                                      both(b, op( g->add(d->x, g->x28, s->x, ShMod::LSL, 2) )));
	t.put("add (<<2)",                                      both(b, op( g->add(d->x, s->x, g->x28, ShMod::LSL, 2) )));
	t.put("adc",                                            both(b, op( g->adc(d->x, d->x, s->x) )));
	t.put("adds",                                           both(b, op( g->adds(d->x, d->x, s->x) )));
	t.put("mul",                                            both(b, op( g->mul(d->x, d->x, s->x) )));
	t.put("madd",                                           both(b, op( g->madd(d->x, s->x, s->x, d->x) )));
	t.put("udiv",                                           both(b, op( g->udiv(d->x, s->x, g->x28) )));
	t.put("bfm",                                            both(b, op( g->bfm(d->x, s->x, 17, 2) )));
	t.put("crc32x",                                         both(b, op( g->crc32x(d->w, d->w, s->x) )));
	t.put("ccmn",                                           sweep(      g->ccmn(s->x, g->x28, 0xf, Cond::EQ) ));

	t.put("fcmp.s (reg)",                                   thr(b,  op( g->fcmp(s->s, s->s) )));
	t.put("fcmp.s (zero)",                                  thr(b,  op( g->fcmp(s->s, 0.0) )));
	t.put("fcsel.s",                                        thr(b,  op( g->fcsel(d->s, d->s, s->s, Cond::EQ) )));
	t.put("fcvtas.s (s -> s)",                              thr(b,  op( g->fcvtas(d->s, s->s) )));
	t.put("fcvtas.s (s -> x)",                              thr(b,  op( g->fcvtas(d->x, s->s) )));
	t.put("scvtf.s (x -> s)",                               thr(b,  op( g->scvtf(d->s, s->w) )));
	t.put("frecpe.s (scl)",                                 thr(b,  op( g->frecpe(d->s, s->s) )));
	t.put("frsqrte.s (scl)",                                thr(b,  op( g->frsqrte(d->s, s->s) )));
	t.put("fmov.d (x -> d)",                                thr(b,  op( g->fmov(d->d, s->x); )));
	t.put("mov (d[0] -> x)",                                thr(b,  op( g->mov(d->x, s->v.d[0]); )));

	t.put("add.b",                                          thr(b,  op( g->add(d->v.b, s->v.b, s->v.b) )));

	t.put("sha1c",                                          thr(b,  op( g->sha1c(d->q, d->s, s->v.s) )));
	t.put("sha1h",                                          thr(b,  op( g->sha1h(d->s, s->s) )));
	t.put("sha1m",                                          thr(b,  op( g->sha1m(d->q, d->s, s->v.s) )));
	t.put("sha1p",                                          thr(b,  op( g->sha1p(d->q, d->s, s->v.s) )));
	t.put("sha1su0",                                        thr(b,  op( g->sha1su0(d->v.s, d->v.s, s->v.s) )));
	t.put("sha1su1",                                        thr(b,  op( g->sha1su1(d->v.s, s->v.s) )));

	t.put("sha256h",                                        thr(b,  op( g->sha256h(d->q, d->q, s->v.s) )));
	t.put("sha256h2",                                       thr(b,  op( g->sha256h2(d->q, d->q, s->v.s) )));
	t.put("sha256su0",                                      thr(b,  op( g->sha256su0(d->v.s, s->v.s) )));
	t.put("sha256su1",                                      thr(b,  op( g->sha256su1(d->v.s, d->v.s, s->v.s) )));

	t.put("eor (reg; clearing idiom)",                      thr(b,  op( g->eor(d->x, s->x, s->x) )));
	t.put("eor.b (reg; clearing idiom)",                    thr(b,  op( g->eor(d->v.b, s->v.b, s->v.b) )));
	return;
}

static
void bench_cap_load_store(bool md, double freq) {
	table t(md, "load / store units", 1, 20);
	bench_col b(freq);

	memmgr m_h(mem_init( p ));
	bench_col h(freq, m_h.ptr(), m_h.ptr());
	bench_col n(freq, m_h.ptr(), NULL);

	t.put("str -> ldr (ofs = 0 -> ofs = 0)",                col(h, ldr_str_x_s_0,   ldr_str_x_s_0,   ldr_str_x_s_0,   ldr_str_x_s_0));
	t.put("str -> ldr (ofs = 0 -> ofs = 0)",                col(h, ldr_str_x_s_128, ldr_str_x_s_128, ldr_str_x_s_128, ldr_str_x_s_128));

	/*
	 * load / store base throughput
	 */
	t.put("ldr",                                            col(h, ldr_x_s));
	t.put("ldr x 2",                                        col(h, ldr_x_s, ldr_x_s));
	t.put("ldr x 3",                                        col(h, ldr_x_s, ldr_x_s, ldr_x_s));
	t.put("ldr x 4",                                        col(h, ldr_x_s, ldr_x_s, ldr_x_s, ldr_x_s));
	t.put("ldr x 5",                                        col(h, ldr_x_s, ldr_x_s, ldr_x_s, ldr_x_s, ldr_x_s));

	t.put("str",                                            col(h, str_x_s));
	t.put("str x 2",                                        col(h, str_x_s, str_x_s));
	t.put("str x 3",                                        col(h, str_x_s, str_x_s, str_x_s));
	t.put("str x 4",                                        col(h, str_x_s, str_x_s, str_x_s, str_x_s));
	t.put("str x 5",                                        col(h, str_x_s, str_x_s, str_x_s, str_x_s, str_x_s));

	/*
	 * if load / store ports are shared
	 */
	t.put("ldr x 3 // str",                                 col(h, ldr_x_s, ldr_x_s, ldr_x_s,                   str_x_s));
	t.put("ldr x 2 // str x 2",                             col(h, ldr_x_s, ldr_x_s,                            str_x_s, str_x_s));
	t.put("ldr x 3 // str x 2",                             col(h, ldr_x_s, ldr_x_s, ldr_x_s,                   str_x_s, str_x_s));

	/*
	 * if ports are shared between scalar and SIMD/fp domains
	 */
	t.put("ldr.q // ldr x 2",                               col(h, ldr_q_s,                                     ldr_x_s, ldr_x_s));
	t.put("ldr.q x 2 // ldr",                               col(h, ldr_q_s, ldr_q_s,                            ldr_x_s));
	t.put("ldr.q x 2 // ldr x 2",                           col(h, ldr_q_s, ldr_q_s,                            ldr_x_s, ldr_x_s));
	t.put("str.q // str",                                   col(h, str_q_s,                                     str_x_s));
	t.put("str.q // str x 2",                               col(h, str_q_s,                                     str_x_s, str_x_s));
	t.put("str.q x 2 // str",                               col(h, str_q_s, str_q_s,                            str_x_s));

	/*
	 * if ldr consumes a scalar port
	 */
	t.put("add x 6 // ldr (ofs = 0)",                       col(h, add_x, add_x, add_x, add_x, add_x, add_x,    ldr_x_s));
	t.put("add x 6 // ldr (reg)",                           col(n, add_x, add_x, add_x, add_x, add_x, add_x,    ldr_x_s_x28));
	t.put("add x 6 // ldr (reg) x 2",                       col(n, add_x, add_x, add_x, add_x, add_x, add_x,    ldr_x_s_x28, ldr_x_s_x28));
	t.put("add x 6 // ldr (reg<<3)",                        col(n, add_x, add_x, add_x, add_x, add_x, add_x,    ldr_x_s_x28_lsl3));
	t.put("add x 6 // ldr (reg<<3) x 2",                    col(n, add_x, add_x, add_x, add_x, add_x, add_x,    ldr_x_s_x28_lsl3, ldr_x_s_x28_lsl3));
	t.put("adc x 3 // ldr (ofs = 0)",                       col(h, adc_x, adc_x, adc_x,                         ldr_x_s));
	t.put("adc x 3 // ldr (ofs = 0) x 2",                   col(h, adc_x, adc_x, adc_x,                         ldr_x_s, ldr_x_s));
	t.put("adc x 3 // ldr (ofs = 0) x 3",                   col(h, adc_x, adc_x, adc_x,                         ldr_x_s, ldr_x_s, ldr_x_s));
	t.put("mul x 2 // ldr (ofs = 0)",                       col(h, mul_x, mul_x,                                ldr_x_s));
	t.put("mul x 2 // ldr (ofs = 0) x 2",                   col(h, mul_x, mul_x,                                ldr_x_s, ldr_x_s));
	t.put("mul x 2 // ldr (ofs = 0) x 3",                   col(h, mul_x, mul_x,                                ldr_x_s, ldr_x_s, ldr_x_s));
	t.put("add x 6 // ldr (ofs = 16)",                      col(h, add_x, add_x, add_x, add_x, add_x, add_x,    ldr_x_s_16));
	t.put("add x 6 // ldr (ofs = 16) x 2",                  col(h, add_x, add_x, add_x, add_x, add_x, add_x,    ldr_x_s_16, ldr_x_s_16));
	t.put("add x 6 // ldr (ofs = 16, 32)",                  col(h, add_x, add_x, add_x, add_x, add_x, add_x,    ldr_x_s_16, ldr_x_s_32));

	t.put("adc x 3 // ldr (ofs = 16)",                      col(h, adc_x, adc_x, adc_x,                         ldr_x_s_16));
	t.put("adc x 3 // ldr (ofs = 16) x 2",                  col(h, adc_x, adc_x, adc_x,                         ldr_x_s_16, ldr_x_s_16));
	t.put("adc x 3 // ldr (ofs = 16) x 3",                  col(h, adc_x, adc_x, adc_x,                         ldr_x_s_16, ldr_x_s_16, ldr_x_s_16));
	t.put("mul x 2 // ldr (ofs = 16)",                      col(h, mul_x, mul_x,                                ldr_x_s_16));
	t.put("mul x 2 // ldr (ofs = 16) x 2",                  col(h, mul_x, mul_x,                                ldr_x_s_16, ldr_x_s_16));
	t.put("mul x 2 // ldr (ofs = 16) x 3",                  col(h, mul_x, mul_x,                                ldr_x_s_16, ldr_x_s_16, ldr_x_s_16));

	t.put("add x 6 // ldr (pre)",                           col(h, add_x, add_x, add_x, add_x, add_x, add_x,    ldr_x_pre_s));
	t.put("adc x 3 // ldr (pre)",                           col(h, adc_x, adc_x, adc_x,                         ldr_x_pre_s));
	t.put("mul x 2 // ldr (pre)",                           col(h, mul_x, mul_x,                                ldr_x_pre_s));

	/*
	 * if str consumes a scalar port
	 */
	t.put("add x 6 // str (ofs = 0)",                       col(h, add_x, add_x, add_x, add_x, add_x, add_x,    str_x_s));
	t.put("add x 6 // str (reg)",                           col(n, add_x, add_x, add_x, add_x, add_x, add_x,    str_x_s_x28));
	t.put("add x 6 // str (reg) x 2",                       col(n, add_x, add_x, add_x, add_x, add_x, add_x,    str_x_s_x28, str_x_s_x28));
	t.put("add x 6 // str (reg<<3)",                        col(n, add_x, add_x, add_x, add_x, add_x, add_x,    str_x_s_x28_lsl3));
	t.put("add x 6 // str (reg<<3) x 2",                    col(n, add_x, add_x, add_x, add_x, add_x, add_x,    str_x_s_x28_lsl3, str_x_s_x28_lsl3));

	t.put("adc x 3 // str",                                 col(h, adc_x, adc_x, adc_x,                         str_x_s));
	t.put("adc x 3 // str x 2",                             col(h, adc_x, adc_x, adc_x,                         str_x_s, str_x_s));

	t.put("add x 6 // str (ofs = 16)",                      col(h, add_x, add_x, add_x, add_x, add_x, add_x,    str_x_s_16));
	t.put("add x 6 // str (ofs = 16) x 2",                  col(h, add_x, add_x, add_x, add_x, add_x, add_x,    str_x_s_16, str_x_s_16));
	t.put("add x 6 // str (ofs = 16, 32)",                  col(h, add_x, add_x, add_x, add_x, add_x, add_x,    str_x_s_16, str_x_s_32));

	t.put("add x 6 // str (pre)",                           col(h, add_x, add_x, add_x, add_x, add_x, add_x,    str_x_pre_s));
	t.put("adc x 3 // str (pre)",                           col(h, adc_x, adc_x, adc_x,                         str_x_pre_s));

	/*
	 * if the load / store units consumes scalar port when offset is large
	 */
	#define var_offset_core(_basename, _offset, ...) { \
		memmgr m_h(mem_init( ((void *)((uint8_t const *)p - (_offset))) )); \
		bench_col h(freq, m_h.ptr(), m_h.ptr(), 1, 1, 1, 1, 1); \
		char buf[1024] = { 0 }; \
		size_t const len = strlen((char const *)(_basename)); \
		size_t const clip = len > 512 ? 512 : len; \
		snprintf(buf, 1023, "%.*s (ofs = %zu)", (int)clip, (char const *)(_basename), (size_t)(_offset)); \
		t.put(buf, col(h, __VA_ARGS__, (size_t)(_offset))); \
	}
	#define var_offset(_basename, ...) { \
		var_offset_core(_basename, 0,      __VA_ARGS__); \
		var_offset_core(_basename, 16,     __VA_ARGS__); \
		var_offset_core(_basename, 64,     __VA_ARGS__); \
		var_offset_core(_basename, 1024,   __VA_ARGS__); \
		var_offset_core(_basename, 4096,   __VA_ARGS__); \
		var_offset_core(_basename, 8192,   __VA_ARGS__); \
		var_offset_core(_basename, 16384,  __VA_ARGS__); \
	}

	var_offset("add x 6 // ldr",                                   add_x, add_x, add_x, add_x, add_x, add_x,    ldr_x_s_k);
	var_offset("add x 6 // ldr x 2",                               add_x, add_x, add_x, add_x, add_x, add_x,    ldr_x_s_k, ldr_x_s_k);
	var_offset("add x 6 // str",                                   add_x, add_x, add_x, add_x, add_x, add_x,    str_x_s_k);
	var_offset("add x 6 // str x 2",                               add_x, add_x, add_x, add_x, add_x, add_x,    str_x_s_k, str_x_s_k);

	#undef var_offset_core
	#undef var_offset
	return;
}

static
void bench_cap_branch(bool md, double freq) {
	table t(md, "branch units", 1, 20);
	bench_col b(freq);

	/*
	 * base branch throughput
	 */
	t.put("b",                                              col(b,                                              b_pc4,                    lat_patterns, lat_patterns));
	t.put("cbq (pc+4; not taken)",                          col(b,                                              cbz_x28_pc4,              lat_patterns, lat_patterns));
	t.put("adds -> b.eq (pc+4; not taken)",                 col(b, adds_x,                                      beq_pc4,                  lat_patterns, lat_patterns));
	t.put("b x 2",                                          col(b,                                              b_pc4,       b_pc4,       lat_inc2_pattern));
	t.put("b // cbz (pc+4; not taken)",                     col(b,                                              b_pc4,       cbz_x28_pc4, lat_inc2_pattern));

	/*
	 * if branch consumes scalar ports
	 */
	t.put("add x 3 // b",                                   col(b, add_x, add_x, add_x,                         b_pc4,                    lat_inc3_pattern));
	t.put("add x 4 // b",                                   col(b, add_x, add_x, add_x, add_x,                  b_pc4,                    lat_inc4_pattern));
	t.put("add x 5 // b",                                   col(b, add_x, add_x, add_x, add_x, add_x,           b_pc4,                    lat_inc5_pattern));
	t.put("add x 6 // b",                                   col(b, add_x, add_x, add_x, add_x, add_x, add_x,    b_pc4,                    lat_inc6_pattern));
	t.put("adc // b",                                       col(b, adc_x,                                       b_pc4,                    lat_inc1_pattern));
	t.put("adc x 2 // b",                                   col(b, adc_x, adc_x,                                b_pc4,                    lat_inc2_pattern));
	t.put("adc x 3 // b",                                   col(b, adc_x, adc_x, adc_x,                         b_pc4,                    lat_inc3_pattern));
	t.put("mul // b",                                       col(b, mul_x,                                       b_pc4,                    lat_inc1_pattern));
	t.put("mul x 2 // b",                                   col(b, mul_x, mul_x,                                b_pc4,                    lat_inc2_pattern));

	t.put("adc // b.eq (pc+4; not taken)",                  col(b, adc_x,                                       beq_pc4,                  lat_inc1_pattern));
	t.put("adc x 2 // b.eq (pc+4; not taken)",              col(b, adc_x, adc_x,                                beq_pc4,                  lat_inc2_pattern));
	t.put("adc x 3 // b.eq (pc+4; not taken)",              col(b, adc_x, adc_x, adc_x,                         beq_pc4,                  lat_inc3_pattern));

	t.put("add // cbz (pc+4; not taken)",                   col(b, add_x,                                       cbz_x28_pc4,              lat_inc1_pattern));
	t.put("add x 2 // cbz (pc+4; not taken)",               col(b, add_x, add_x,                                cbz_x28_pc4,              lat_inc2_pattern));
	t.put("add x 3 // cbz (pc+4; not taken)",               col(b, add_x, add_x, add_x,                         cbz_x28_pc4,              lat_inc3_pattern));
	t.put("add x 4 // cbz (pc+4; not taken)",               col(b, add_x, add_x, add_x, add_x,                  cbz_x28_pc4,              lat_inc4_pattern));
	t.put("adc // cbz (pc+4; not taken) x 2",               col(b, adc_x,                                       cbz_x28_pc4, cbz_x28_pc4, lat_inc1_pattern));
	t.put("adc // cbz (pc+4; not taken)",                   col(b, adc_x,                                       cbz_x28_pc4,              lat_inc1_pattern));
	t.put("adc x 2 // cbz (pc+4; not taken)",               col(b, adc_x, adc_x,                                cbz_x28_pc4,              lat_inc2_pattern));
	t.put("adc x 3 // cbz (pc+4; not taken)",               col(b, adc_x, adc_x, adc_x,                         cbz_x28_pc4,              lat_inc3_pattern));

	/*
	 * if fp domain compare delays; if fp domain compare consumes additional scalar ports
	 */
	t.put("fcmp -> b.eq (pc+4; taken)",                     col(b, fcmp_s,                                      beq_pc4,                  lat_patterns, lat_patterns));
	t.put("fcmp -> b.ne (pc+4; not taken)",                 col(b, fcmp_s,                                      bne_pc4,                  lat_patterns, lat_patterns));
	t.put("fcmp -> b.eq (pc+4; taken) // adc",              col(b, fcmp_s, adc_x,                               beq_pc4,                  lat_patterns, lat_patterns));
	t.put("fcmp -> b.ne (pc+4; not taken) // adc",          col(b, fcmp_s, adc_x,                               bne_pc4,                  lat_patterns, lat_patterns));
	t.put("fcmp -> b.eq (pc+4; taken) // adc x 2",          col(b, fcmp_s, adc_x, adc_x,                        beq_pc4,                  lat_patterns, lat_patterns));
	t.put("fcmp -> b.ne (pc+4; not taken) // adc x 2",      col(b, fcmp_s, adc_x, adc_x,                        bne_pc4,                  lat_patterns, lat_patterns));
	t.put("fcmp -> b.eq (pc+4; taken) // adc x 3",          col(b, fcmp_s, adc_x, adc_x, adc_x,                 beq_pc4,                  lat_patterns, lat_patterns));
	t.put("fcmp -> b.ne (pc+4; not taken) // adc x 3",      col(b, fcmp_s, adc_x, adc_x, adc_x,                 bne_pc4,                  lat_patterns, lat_patterns));

	/*
	 * branch-and-link (call) and return (ret) throughput and port consumption
	 */
	#define _ex_op(_op, _n)		{ g->_op(g->x##_n, g->x##_n, g->x##_n); }
	#define ops_1a(_op)			{ _ex_op(_op, 0); }
	#define ops_2a(_op)			{ _ex_op(_op, 0); _ex_op(_op, 1); }
	#define ops_3a(_op)			{ _ex_op(_op, 0); _ex_op(_op, 1); _ex_op(_op, 2); }
	#define ops_4a(_op)			{ _ex_op(_op, 0); _ex_op(_op, 1); _ex_op(_op, 2); _ex_op(_op, 3); }
	#define ops_5a(_op)			{ _ex_op(_op, 0); _ex_op(_op, 1); _ex_op(_op, 2); _ex_op(_op, 3); _ex_op(_op, 4); }
	#define ops_6a(_op)			{ _ex_op(_op, 0); _ex_op(_op, 1); _ex_op(_op, 2); _ex_op(_op, 3); _ex_op(_op, 4); _ex_op(_op, 5); }
	#define ops_1b(_op)			{ _ex_op(_op, 6); }
	#define ops_2b(_op)			{ _ex_op(_op, 6); _ex_op(_op, 7); }
	#define ops_3b(_op)			{ _ex_op(_op, 6); _ex_op(_op, 7); _ex_op(_op, 8); }
	#define ops_4b(_op)			{ _ex_op(_op, 6); _ex_op(_op, 7); _ex_op(_op, 8); _ex_op(_op, 9); }
	#define ops_5b(_op)			{ _ex_op(_op, 6); _ex_op(_op, 7); _ex_op(_op, 8); _ex_op(_op, 9); _ex_op(_op, 10); }
	#define ops_6b(_op)			{ _ex_op(_op, 6); _ex_op(_op, 7); _ex_op(_op, 8); _ex_op(_op, 9); _ex_op(_op, 10); _ex_op(_op, 11); }

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

	/* bl-ret */
	t.put("bl-ret",                         bl_ret({ ; },       { ; },       lat_inc1_pattern));
	t.put("bl-ret // add // add",           bl_ret(ops_1a(add), ops_1b(add), lat_inc1_pattern));
	t.put("bl-ret // add x 2 // add x 2",   bl_ret(ops_2a(add), ops_2b(add), lat_inc2_pattern));
	t.put("bl-ret // add x 3 // add x 3",   bl_ret(ops_3a(add), ops_3b(add), lat_inc3_pattern));
	t.put("bl-ret // add x 4 // add x 4",   bl_ret(ops_4a(add), ops_4b(add), lat_inc4_pattern));
	t.put("bl-ret // add x 5 // add x 5",   bl_ret(ops_5a(add), ops_5b(add), lat_inc5_pattern));
	t.put("bl-ret // add x 6 // add x 6",   bl_ret(ops_6a(add), ops_6b(add), lat_inc6_pattern));

	t.put("bl-ret // adc // adc",           bl_ret(ops_1a(adc), ops_1b(adc), lat_inc1_pattern));
	t.put("bl-ret // adc x 2 // adc x 2",   bl_ret(ops_2a(adc), ops_2b(adc), lat_inc2_pattern));
	t.put("bl-ret // adc x 3 // adc x 3",   bl_ret(ops_3a(adc), ops_3b(adc), lat_inc3_pattern));
	t.put("bl-ret // adc x 2 // adc x 3",   bl_ret(ops_2a(adc), ops_3b(adc), lat_inc3_pattern));
	t.put("bl-ret // adc x 3 // adc x 2",   bl_ret(ops_3a(adc), ops_2b(adc), lat_inc3_pattern));

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
	t.put("blr-ret",                        blr_ret({ ; },       { ; },       lat_inc1_pattern));
	t.put("blr-ret // add // add",          blr_ret(ops_1a(add), ops_1b(add), lat_inc1_pattern));
	t.put("blr-ret // add x 2 // add x 2",  blr_ret(ops_2a(add), ops_2b(add), lat_inc2_pattern));
	t.put("blr-ret // add x 3 // add x 3",  blr_ret(ops_3a(add), ops_3b(add), lat_inc3_pattern));
	t.put("blr-ret // add x 4 // add x 4",  blr_ret(ops_4a(add), ops_4b(add), lat_inc4_pattern));
	t.put("blr-ret // add x 5 // add x 5",  blr_ret(ops_5a(add), ops_5b(add), lat_inc5_pattern));
	t.put("blr-ret // add x 6 // add x 6",  blr_ret(ops_6a(add), ops_6b(add), lat_inc6_pattern));

	t.put("blr-ret // adc // adc",          blr_ret(ops_1a(adc), ops_1b(adc), lat_inc1_pattern));
	t.put("blr-ret // adc x 2 // adc x 2",  blr_ret(ops_2a(adc), ops_2b(adc), lat_inc2_pattern));
	t.put("blr-ret // adc x 3 // adc x 3",  blr_ret(ops_3a(adc), ops_3b(adc), lat_inc3_pattern));
	t.put("blr-ret // adc x 2 // adc x 3",  blr_ret(ops_2a(adc), ops_3b(adc), lat_inc3_pattern));
	t.put("blr-ret // adc x 3 // adc x 2",  blr_ret(ops_3a(adc), ops_2b(adc), lat_inc3_pattern));

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

	/*
	 * L1I and L1BTB size estimation, miss penalty estimation
	 */
	#define var_length_core(_basename, _cnt, ...) { \
		static pattern_t const p[] = { { (_cnt), 8, 0, 16 }, { 0, 0, 0, 0 } }; \
		static pattern_t const *pp[] = { (pattern_t const *)&p, NULL }; \
		char buf[1024] = { 0 }; \
		size_t const len = strlen((char const *)(_basename)); \
		size_t const clip = len > 512 ? 512 : len; \
		snprintf(buf, 1023, "%.*s x %zu", (int)clip, (char const *)(_basename), (size_t)(_cnt)); \
		t.put(buf, col(b, __VA_ARGS__, pp)); \
	}
	#define var_length(_basename, ...) { \
		var_length_core(_basename, 1, __VA_ARGS__); \
		var_length_core(_basename, 4, __VA_ARGS__); \
		var_length_core(_basename, 8, __VA_ARGS__); \
		var_length_core(_basename, 16, __VA_ARGS__); \
		var_length_core(_basename, 64, __VA_ARGS__); \
		var_length_core(_basename, 128, __VA_ARGS__); \
		var_length_core(_basename, 144, __VA_ARGS__); \
		var_length_core(_basename, 160, __VA_ARGS__); \
		var_length_core(_basename, 176, __VA_ARGS__); \
		var_length_core(_basename, 192, __VA_ARGS__); \
		var_length_core(_basename, 256, __VA_ARGS__); \
		var_length_core(_basename, 512, __VA_ARGS__); \
		var_length_core(_basename, 640, __VA_ARGS__); \
		var_length_core(_basename, 1024, __VA_ARGS__); \
		var_length_core(_basename, 4096, __VA_ARGS__); \
		var_length_core(_basename, 6144, __VA_ARGS__); \
		var_length_core(_basename, 6240, __VA_ARGS__); \
		var_length_core(_basename, 6320, __VA_ARGS__); \
		var_length_core(_basename, 6400, __VA_ARGS__); \
		var_length_core(_basename, 7168, __VA_ARGS__); \
		var_length_core(_basename, 16384, __VA_ARGS__); \
		var_length_core(_basename, 32767, __VA_ARGS__); \
	}

	var_length("nop x 8",                                          nop,   nop,   nop,   nop,   nop,   nop,   nop,   nop);
	var_length("add x 6 // nop x 2",                               add_x, add_x, add_x, add_x, add_x, add_x, nop,   nop);
	var_length("add x 6 // nop // b",                              add_x, add_x, add_x, add_x, add_x, add_x, nop,   b_pc4);
	var_length("add x 6 // b x 2",                                 add_x, add_x, add_x, b_pc4, add_x, add_x, add_x, b_pc4);
	var_length("add x 5 // b x 3",                                 add_x, add_x, add_x, b_pc4, add_x, b_pc4, add_x, b_pc4);
	var_length("add x 4 // b x 4",                                 add_x, b_pc4, add_x, b_pc4, add_x, b_pc4, add_x, b_pc4);
	var_length("add x 3 // b x 5",                                 add_x, b_pc4, add_x, b_pc4, add_x, b_pc4, b_pc4, b_pc4);
	var_length("add x 2 // b x 6",                                 add_x, b_pc4, b_pc4, b_pc4, add_x, b_pc4, b_pc4, b_pc4);
	var_length("add // b x 7",                                     add_x, b_pc4, b_pc4, b_pc4, b_pc4, b_pc4, b_pc4, b_pc4);
	var_length("b x 8",                                            b_pc4, b_pc4, b_pc4, b_pc4, b_pc4, b_pc4, b_pc4, b_pc4);

	#undef var_length_core
	#undef var_length
	return;
}

static
void bench_cap_scalar(bool md, double freq) {
	table t(md, "scalar ports", 1, 20);
	bench_col b(freq);

	/* mov (imm) */
	t.put("mov(imm)-add x 2",                               col(b, movi_add_x, movi_add_x));
	t.put("mov(imm)-add x 3",                               col(b, movi_add_x, movi_add_x, movi_add_x));
	t.put("mov(imm)-add x 4",                               col(b, movi_add_x, movi_add_x, movi_add_x, movi_add_x));
	t.put("(mov(imm),mov(imm))-add x 2",                    col(b, movi_x, movi_add_x_prev, movi_x, movi_add_x_prev));
	t.put("(mov(imm),mov(imm))-add x 3",                    col(b, movi_x, movi_add_x_prev, movi_x, movi_add_x_prev, movi_x, movi_add_x_prev));
	t.put("(mov(imm),mov(imm))-add x 4",                    col(b, movi_x, movi_add_x_prev, movi_x, movi_add_x_prev, movi_x, movi_add_x_prev, movi_x, movi_add_x_prev));
	t.put("mov(imm)-adc x 2",                               col(b, movi_adc_x, movi_adc_x));
	t.put("mov(imm)-adc x 3",                               col(b, movi_adc_x, movi_adc_x, movi_adc_x));
	t.put("mov(imm)-adc x 4",                               col(b, movi_adc_x, movi_adc_x, movi_adc_x, movi_adc_x));

	/* adc // add, adds // add */
	t.put("adc x 3 // add",                                 col(b, adc_x, adc_x, adc_x,                         add_x));
	t.put("adc x 3 // add x 2",                             col(b, adc_x, adc_x, adc_x,                         add_x, add_x));
	t.put("adc x 3 // add x 3",                             col(b, adc_x, adc_x, adc_x,                         add_x, add_x, add_x));
	t.put("adc x 3 // add x 4",                             col(b, adc_x, adc_x, adc_x,                         add_x, add_x, add_x, add_x));
	t.put("adds x 3 // add",                                col(b, adds_x, adds_x, adds_x,                      add_x));
	t.put("adds x 3 // add x 2",                            col(b, adds_x, adds_x, adds_x,                      add_x, add_x));
	t.put("adds x 3 // add x 3",                            col(b, adds_x, adds_x, adds_x,                      add_x, add_x, add_x));
	t.put("adds x 3 // add x 4",                            col(b, adds_x, adds_x, adds_x,                      add_x, add_x, add_x, add_x));

	/* add // add (<<2) */
	t.put("add (<<2) // add",                               col(b, add_shl2_x,                                  add_x));
	t.put("add (<<2) // add x 2",                           col(b, add_shl2_x,                                  add_x, add_x));
	t.put("add (<<2) // add x 3",                           col(b, add_shl2_x,                                  add_x, add_x, add_x));
	t.put("add (<<2) // add x 4",                           col(b, add_shl2_x,                                  add_x, add_x, add_x, add_x));
	t.put("add (<<2) x 2 // add",                           col(b, add_shl2_x, add_shl2_x,                      add_x));
	t.put("add (<<2) x 2 // add x 2",                       col(b, add_shl2_x, add_shl2_x,                      add_x, add_x));
	t.put("add (<<2) x 2 // add x 3",                       col(b, add_shl2_x, add_shl2_x,                      add_x, add_x, add_x));
	t.put("add (<<2) x 3 // add",                           col(b, add_shl2_x, add_shl2_x, add_shl2_x,          add_x));
	t.put("add (<<2) x 3 // add x 2",                       col(b, add_shl2_x, add_shl2_x, add_shl2_x,          add_x, add_x));
	t.put("add (<<2) x 3 // add x 3",                       col(b, add_shl2_x, add_shl2_x, add_shl2_x,          add_x, add_x, add_x));

	/* adc // add (<<2) */
	t.put("adc // add (<<2) x 2",                           col(b, adc_x,                                       add_shl2_x, add_shl2_x));
	t.put("adc // add (<<2) x 3",                           col(b, adc_x,                                       add_shl2_x, add_shl2_x, add_shl2_x));
	t.put("adc x 2 // add (<<2) x 2",                       col(b, adc_x, adc_x,                                add_shl2_x, add_shl2_x));
	t.put("adc x 2 // add (<<2) x 3",                       col(b, adc_x, adc_x,                                add_shl2_x, add_shl2_x, add_shl2_x));
	t.put("adc x 3 // add (<<2)",                           col(b, adc_x, adc_x, adc_x,                         add_shl2_x));

	/* adds // add (<<2) */
	t.put("adds // add (<<2) x 2",                          col(b, adds_x,                                      add_shl2_x, add_shl2_x));
	t.put("adds // add (<<2) x 3",                          col(b, adds_x,                                      add_shl2_x, add_shl2_x, add_shl2_x));
	t.put("adds x 2 // add (<<2) x 2",                      col(b, adds_x, adds_x,                              add_shl2_x, add_shl2_x));
	t.put("adds x 2 // add (<<2) x 3",                      col(b, adds_x, adds_x,                              add_shl2_x, add_shl2_x, add_shl2_x));
	t.put("adds x 3 // add (<<2)",                          col(b, adds_x, adds_x, adds_x,                      add_shl2_x));

	/* adc // adds */
	t.put("adc // adds x 2",                                col(b, adc_x,                                       adds_x, adds_x));
	t.put("adc x 2 // adds",                                col(b, adc_x, adc_x,                                adds_x));
	t.put("adc // adds x 3",                                col(b, adc_x,                                       adds_x, adds_x, adds_x));
	t.put("adc x 2 // adds x 2",                            col(b, adc_x, adc_x,                                adds_x, adds_x));
	t.put("adc x 3 // adds",                                col(b, adc_x, adc_x, adc_x,                         adds_x));
	t.put("adc // adds x 4",                                col(b, adc_x,                                       adds_x, adds_x, adds_x, adds_x));
	t.put("adc x 2 // adds x 3",                            col(b, adc_x, adc_x,                                adds_x, adds_x, adds_x));
	t.put("adc x 3 // adds x 2",                            col(b, adc_x, adc_x, adc_x,                         adds_x, adds_x));
	t.put("adc x 4 // adds",                                col(b, adc_x, adc_x, adc_x, adc_x,                  adds_x));
	t.put("adc x 3 // adds x 3",                            col(b, adc_x, adc_x, adc_x,                         adds_x, adds_x, adds_x));

	/* mul */
	t.put("adc x 3 // mul",                                 col(b, adc_x, adc_x, adc_x,                         mul_x));
	t.put("adc x 3 // mul x 2",                             col(b, adc_x, adc_x, adc_x,                         mul_x, mul_x));

	/* madd */
	t.put("adc x 3 // madd",                                col(b, adc_x, adc_x, adc_x,                         madd_x));
	t.put("mul // madd",                                    col(b, mul_x,                                       madd_x));
	t.put("mul x 2 // madd",                                col(b, mul_x, mul_x,                                madd_x));

	/* udiv */
	t.put("mul x 2 // udiv",                                col(b, mul_x, mul_x,                                udiv_x_x28));
	t.put("mul x 3 // udiv",                                col(b, mul_x, mul_x, mul_x,                         udiv_x_x28));
	t.put("mul x 4 // udiv",                                col(b, mul_x, mul_x, mul_x, mul_x,                  udiv_x_x28));
	t.put("madd x 2 // udiv",                               col(b,                             madd_x, madd_x,  udiv_x_x28));
	t.put("mul // madd x 2 // udiv",                        col(b, mul_x,                      madd_x, madd_x,  udiv_x_x28));
	t.put("mul x 2 // madd // udiv",                        col(b, mul_x, mul_x,               madd_x,          udiv_x_x28));

	/* bfm */
	t.put("adc x 3 // bfm",                                 col(b, adc_x, adc_x, adc_x,                         bfm_x));
	t.put("mul // bfm",                                     col(b, mul_x,                                       bfm_x));
	t.put("mul x 2 // bfm",                                 col(b, mul_x, mul_x,                                bfm_x));
	t.put("madd // bfm",                                    col(b, madd_x,                                      bfm_x));
	t.put("udiv // bfm x 2",                                col(b, udiv_x_x28,                                  bfm_x, bfm_x));

	/* crc32x */
	t.put("adc x 3 // crc32x",                              col(b, adc_x, adc_x, adc_x,                         crc32x_w));
	t.put("mul // crc32x",                                  col(b, mul_x,                                       crc32x_w));
	t.put("mul x 2 // crc32x",                              col(b, mul_x, mul_x,                                crc32x_w));
	t.put("madd // crc32x",                                 col(b, madd_x,                                      crc32x_w));
	t.put("udiv // crc32x x 2",                             col(b, udiv_x_x28,                                  crc32x_w, crc32x_w));
	t.put("bfm // crc32x",                                  col(b, madd_x,                                      crc32x_w));

	/* ccmn */
	t.put("adcs -> ccmn",                                   col(b, adcs_x, ccmn_eq_x,                                                     lat_flat_patterns, lat_flat_patterns));
	t.put("adcs -> ccmn -> ccmn",                           col(b, adcs_x, ccmn_eq_x, ccmn_eq_x,                                          lat_flat_patterns, lat_flat_patterns));
	t.put("adcs -> ccmn -> ccmn -> ccmn",                   col(b, adcs_x, ccmn_eq_x, ccmn_eq_x, ccmn_eq_x,                               lat_flat_patterns, lat_flat_patterns));

	t.put("adc x 3 // ccmn",                                col(b, ccmn_eq_x, adc_x,                                      adc_x, adc_x));
	t.put("adc x 3 // ccmn x 2",                            col(b, ccmn_eq_x, adc_x, ccmn_eq_x, adc_x,                    adc_x));
	t.put("adc x 3 // ccmn x 3",                            col(b, ccmn_eq_x, adc_x, ccmn_eq_x, adc_x, ccmn_eq_x, adc_x));
	t.put("adc x 4 // ccmn",                                col(b, ccmn_eq_x, adc_x,                                      adc_x, adc_x, adc_x));
	t.put("adc x 4 // ccmn x 2",                            col(b, ccmn_eq_x, adc_x, ccmn_eq_x, adc_x,                    adc_x, adc_x));
	t.put("adc x 4 // ccmn x 3",                            col(b, ccmn_eq_x, adc_x, ccmn_eq_x, adc_x, ccmn_eq_x, adc_x,  adc_x));
	t.put("adc x 5 // ccmn",                                col(b, ccmn_eq_x, adc_x,                                      adc_x, adc_x, adc_x, adc_x));
	t.put("adc x 5 // ccmn x 2",                            col(b, ccmn_eq_x, adc_x, ccmn_eq_x, adc_x,                    adc_x, adc_x, adc_x));
	t.put("adc x 6 // ccmn",                                col(b, ccmn_eq_x, adc_x,                                      adc_x, adc_x, adc_x, adc_x, adc_x));
	t.put("adc x 6 // ccmn x 2",                            col(b, ccmn_eq_x, adc_x, ccmn_eq_x, adc_x,                    adc_x, adc_x, adc_x, adc_x));

	t.put("mul x 2 // ccmn",                                col(b, ccmn_eq_x,                                   mul_x, mul_x));
	t.put("madd // ccmn",                                   col(b, ccmn_eq_x,                                   udiv_x_x28));
	t.put("udiv // ccmn",                                   col(b, ccmn_eq_x,                                   madd_x));
	t.put("bfm // ccmn",                                    col(b, ccmn_eq_x,                                   bfm_x));
	t.put("crc32x // ccmn",                                 col(b, ccmn_eq_x,                                   crc32x_w));
	return;
}

static
void bench_cap_transfer(bool md, double freq) {
	table t(md, "scalar <-> SIMD / fp", 1, 20);
	bench_col b(freq);

	t.put("fcmp.s (reg)",                                   col(b, fcmp_s));
	t.put("fcmp.s (zero)",                                  col(b, fcmp_0));
	t.put("fcsel.s",                                        col(b, fcsel_s));
	t.put("adds -> fcsel.s",                                col(b, adds_fcsel_s));
	t.put("fcvtas.s (s -> s)",                              col(b, fcvtas_s));
	t.put("fcvtas.s (s -> x)",                              col(b, fcvtas_x));
	t.put("scvtf.s (x -> x)",                               col(b, scvtf_w));
	t.put("frecpe.s (scl)",                                 col(b, frecpe_s));
	t.put("frsqrte.s (scl)",                                col(b, frsqrte_s));
	t.put("fmov.d (x -> d)",                                col(b, fmov_d_x));
	t.put("mov (d[0] -> x)",                                col(b, mov_x_d));

	/* x -> d w/o broadcasting */
	t.put("add x 3 // fmov.d (x -> d) x 3",                 col(b, add_x, add_x, add_x,                         fmov_d_x, fmov_d_x, fmov_d_x));
	t.put("add x 4 // fmov.d (x -> d) x 3",                 col(b, add_x, add_x, add_x, add_x,                  fmov_d_x, fmov_d_x, fmov_d_x));
	t.put("add x 5 // fmov.d (x -> d) x 3",                 col(b, add_x, add_x, add_x, add_x, add_x,           fmov_d_x, fmov_d_x, fmov_d_x));
	t.put("adc x 3 // fmov.d (x -> d) x 3",                 col(b, adc_x, adc_x, adc_x,                         fmov_d_x, fmov_d_x, fmov_d_x));

	t.put("add.b x 2 // fmov.d (x -> d) x 3",               col(b, add_v, add_v,                                fmov_d_x, fmov_d_x, fmov_d_x));
	t.put("add.b x 2 // fmov.d (x -> d) x 4",               col(b, add_v, add_v,                                fmov_d_x, fmov_d_x, fmov_d_x, fmov_d_x));
	t.put("add.b x 3 // fmov.d (x -> d) x 3",               col(b, add_v, add_v, add_v,                         fmov_d_x, fmov_d_x, fmov_d_x));
	t.put("add.b x 3 // fmov.d (x -> d) x 4",               col(b, add_v, add_v, add_v,                         fmov_d_x, fmov_d_x, fmov_d_x, fmov_d_x));
	t.put("add.b x 4 // fmov.d (x -> d) x 3",               col(b, add_v, add_v, add_v, add_v,                  fmov_d_x, fmov_d_x, fmov_d_x));
	t.put("add.b x 4 // fmov.d (x -> d) x 4",               col(b, add_v, add_v, add_v, add_v,                  fmov_d_x, fmov_d_x, fmov_d_x, fmov_d_x));

	{
	memmgr m_h(mem_init( p ));
	bench_col h(freq, m_h.ptr(), m_h.ptr());
	t.put("ldr x 3 // fmov.d (x -> d) x 2",                 col(h, ldr_x_s, ldr_x_s, ldr_x_s,                   fmov_d_x, fmov_d_x));
	t.put("ldr.q x 3 // fmov.d (x -> d) x 2",               col(h, ldr_q_s, ldr_q_s, ldr_q_s,                   fmov_d_x, fmov_d_x));
	t.put("ldr x 3 // fmov.d (x -> d) x 3",                 col(h, ldr_x_s, ldr_x_s, ldr_x_s,                   fmov_d_x, fmov_d_x, fmov_d_x));
	t.put("ldr.q x 3 // fmov.d (x -> d) x 3",               col(h, ldr_q_s, ldr_q_s, ldr_q_s,                   fmov_d_x, fmov_d_x, fmov_d_x));

	t.put("str x 2 // fmov.d (x -> d) x 2",                 col(h, str_x_s, str_x_s,                            fmov_d_x, fmov_d_x));
	t.put("str.q x 2 // fmov.d (x -> d) x 2",               col(h, str_q_s, str_q_s,                            fmov_d_x, fmov_d_x));
	t.put("str x 2 // fmov.d (x -> d) x 3",                 col(h, str_x_s, str_x_s,                            fmov_d_x, fmov_d_x, fmov_d_x));
	t.put("str.q x 2 // fmov.d (x -> d) x 3",               col(h, str_q_s, str_q_s,                            fmov_d_x, fmov_d_x, fmov_d_x));
	}

	{
	memmgr m_h(mem_init( p ));
	bench_col h(freq, m_h.ptr(), NULL);
	t.put("ldr x 3 // fmov.d (x -> d) x 2",                 col(h, ldr_x_s_x28, ldr_x_s_x28, ldr_x_s_x28,       fmov_d_x, fmov_d_x));
	t.put("ldr.q x 3 // fmov.d (x -> d) x 2",               col(h, ldr_q_s_x28, ldr_q_s_x28, ldr_q_s_x28,       fmov_d_x, fmov_d_x));
	t.put("ldr x 3 // fmov.d (x -> d) x 3",                 col(h, ldr_x_s_x28, ldr_x_s_x28, ldr_x_s_x28,       fmov_d_x, fmov_d_x, fmov_d_x));
	t.put("ldr.q x 3 // fmov.d (x -> d) x 3",               col(h, ldr_q_s_x28, ldr_q_s_x28, ldr_q_s_x28,       fmov_d_x, fmov_d_x, fmov_d_x));

	t.put("str x 2 // fmov.d (x -> d) x 2",                 col(h, str_x_s_x28, str_x_s_x28,                    fmov_d_x, fmov_d_x));
	t.put("str.q x 2 // fmov.d (x -> d) x 2",               col(h, str_q_s_x28, str_q_s_x28,                    fmov_d_x, fmov_d_x));
	t.put("str x 2 // fmov.d (x -> d) x 3",                 col(h, str_x_s_x28, str_x_s_x28,                    fmov_d_x, fmov_d_x, fmov_d_x));
	t.put("str.q x 2 // fmov.d (x -> d) x 3",               col(h, str_q_s_x28, str_q_s_x28,                    fmov_d_x, fmov_d_x, fmov_d_x));
	}

	/* x -> d w/ broadcasting */
	t.put("add.b x 2 // dup.d (x -> v.d) x 3",              col(b, add_v, add_v,                                dup_d_x, dup_d_x, dup_d_x));
	t.put("add.b x 2 // dup.d (x -> v.d) x 4",              col(b, add_v, add_v,                                dup_d_x, dup_d_x, dup_d_x, dup_d_x));
	t.put("add.b x 3 // dup.d (x -> v.d) x 3",              col(b, add_v, add_v, add_v,                         dup_d_x, dup_d_x, dup_d_x));
	t.put("add.b x 3 // dup.d (x -> v.d) x 4",              col(b, add_v, add_v, add_v,                         dup_d_x, dup_d_x, dup_d_x, dup_d_x));
	t.put("add.b x 4 // dup.d (x -> v.d) x 3",              col(b, add_v, add_v, add_v, add_v,                  dup_d_x, dup_d_x, dup_d_x));
	t.put("add.b x 4 // dup.d (x -> v.d) x 4",              col(b, add_v, add_v, add_v, add_v,                  dup_d_x, dup_d_x, dup_d_x, dup_d_x));

	/* flag x -> d */
	{
	memmgr m_h(mem_init( p ));
	bench_col h(freq, m_h.ptr(), m_h.ptr());
	t.put("ldr x 3 // (adds -> fcsel.s) x 2",               col(h, ldr_x_s, ldr_x_s, ldr_x_s,                   adds_fcsel_s, adds_fcsel_s));
	t.put("ldr.q x 3 // (adds -> fcsel.s) x 2",             col(h, ldr_q_s, ldr_q_s, ldr_q_s,                   adds_fcsel_s, adds_fcsel_s));
	t.put("ldr x 3 // (adds -> fcsel.s) x 3",               col(h, ldr_x_s, ldr_x_s, ldr_x_s,                   adds_fcsel_s, adds_fcsel_s, adds_fcsel_s));
	t.put("ldr.q x 3 // (adds -> fcsel.s) x 3",             col(h, ldr_q_s, ldr_q_s, ldr_q_s,                   adds_fcsel_s, adds_fcsel_s, adds_fcsel_s));

	t.put("str x 2 // (adds -> fcsel.s) x 2",               col(h, str_x_s, str_x_s,                            adds_fcsel_s, adds_fcsel_s));
	t.put("str.q x 2 // (adds -> fcsel.s) x 2",             col(h, str_q_s, str_q_s,                            adds_fcsel_s, adds_fcsel_s));
	t.put("str x 2 // (adds -> fcsel.s) x 3",               col(h, str_x_s, str_x_s,                            adds_fcsel_s, adds_fcsel_s, adds_fcsel_s));
	t.put("str.q x 2 // (adds -> fcsel.s) x 3",             col(h, str_q_s, str_q_s,                            adds_fcsel_s, adds_fcsel_s, adds_fcsel_s));
	}

	t.put("add x 3 // (adds -> fcsel.s) x 2",               col(b, add_x, add_x, add_x,                         adds_fcsel_s, adds_fcsel_s));
	t.put("add x 3 // (adds -> fcsel.s) x 3",               col(b, add_x, add_x, add_x,                         adds_fcsel_s, adds_fcsel_s, adds_fcsel_s));
	t.put("add x 4 // (adds -> fcsel.s) x 2",               col(b, add_x, add_x, add_x, add_x,                  adds_fcsel_s, adds_fcsel_s));
	t.put("add x 4 // (adds -> fcsel.s) x 3",               col(b, add_x, add_x, add_x, add_x,                  adds_fcsel_s, adds_fcsel_s, adds_fcsel_s));

	t.put("add.b x 3 // (adds -> fcsel.s) x 2",             col(b, add_v, add_v, add_v,                         adds_fcsel_s, adds_fcsel_s));
	t.put("add.b x 3 // (adds -> fcsel.s) x 3",             col(b, add_v, add_v, add_v,                         adds_fcsel_s, adds_fcsel_s, adds_fcsel_s));
	t.put("add.b x 4 // (adds -> fcsel.s) x 2",             col(b, add_v, add_v, add_v, add_v,                  adds_fcsel_s, adds_fcsel_s));
	t.put("add.b x 4 // (adds -> fcsel.s) x 3",             col(b, add_v, add_v, add_v, add_v,                  adds_fcsel_s, adds_fcsel_s, adds_fcsel_s));

	t.put("fmov.d (x -> d) x 3 // (adds -> fcsel.s) x 2",   col(b, fmov_d_x, fmov_d_x, fmov_d_x,                adds_fcsel_s, adds_fcsel_s));
	t.put("fmov.d (x -> d) x 3 // (adds -> fcsel.s) x 3",   col(b, fmov_d_x, fmov_d_x, fmov_d_x,                adds_fcsel_s, adds_fcsel_s, adds_fcsel_s));
	t.put("fmov.d (x -> d) x 4 // (adds -> fcsel.s) x 2",   col(b, fmov_d_x, fmov_d_x, fmov_d_x, fmov_d_x,      adds_fcsel_s, adds_fcsel_s));
	t.put("fmov.d (x -> d) x 4 // (adds -> fcsel.s) x 3",   col(b, fmov_d_x, fmov_d_x, fmov_d_x, fmov_d_x,      adds_fcsel_s, adds_fcsel_s, adds_fcsel_s));

	/* d -> x */
	t.put("add // mov.d (d -> x)",                          col(b, add_x,                                       mov_x_d));
	t.put("add x 2 // mov.d (d -> x)",                      col(b, add_x, add_x,                                mov_x_d));
	t.put("add x 3 // mov.d (d -> x)",                      col(b, add_x, add_x, add_x,                         mov_x_d));
	t.put("add x 4 // mov.d (d -> x)",                      col(b, add_x, add_x, add_x, add_x,                  mov_x_d));
	t.put("add x 5 // mov.d (d -> x)",                      col(b, add_x, add_x, add_x, add_x, add_x,           mov_x_d));
	t.put("add x 6 // mov.d (d -> x)",                      col(b, add_x, add_x, add_x, add_x, add_x, add_x,    mov_x_d));

	t.put("add // mov.d (d -> x) x 2",                      col(b, add_x,                                       mov_x_d, mov_x_d));
	t.put("add x 2 // mov.d (d -> x) x 2",                  col(b, add_x, add_x,                                mov_x_d, mov_x_d));
	t.put("add x 3 // mov.d (d -> x) x 2",                  col(b, add_x, add_x, add_x,                         mov_x_d, mov_x_d));
	t.put("add x 4 // mov.d (d -> x) x 2",                  col(b, add_x, add_x, add_x, add_x,                  mov_x_d, mov_x_d));
	t.put("add x 5 // mov.d (d -> x) x 2",                  col(b, add_x, add_x, add_x, add_x, add_x,           mov_x_d, mov_x_d));
	t.put("add x 6 // mov.d (d -> x) x 2",                  col(b, add_x, add_x, add_x, add_x, add_x, add_x,    mov_x_d, mov_x_d));

	t.put("add // mov.d (d -> x) x 3",                      col(b, add_x,                                       mov_x_d, mov_x_d, mov_x_d));
	t.put("add x 2 // mov.d (d -> x) x 3",                  col(b, add_x, add_x,                                mov_x_d, mov_x_d, mov_x_d));
	t.put("add x 3 // mov.d (d -> x) x 3",                  col(b, add_x, add_x, add_x,                         mov_x_d, mov_x_d, mov_x_d));
	t.put("add x 4 // mov.d (d -> x) x 3",                  col(b, add_x, add_x, add_x, add_x,                  mov_x_d, mov_x_d, mov_x_d));
	t.put("add x 5 // mov.d (d -> x) x 3",                  col(b, add_x, add_x, add_x, add_x, add_x,           mov_x_d, mov_x_d, mov_x_d));

	t.put("adc // mov.d (d -> x)",                          col(b, adc_x,                                       mov_x_d));
	t.put("adc x 2 // mov.d (d -> x)",                      col(b, adc_x, adc_x,                                mov_x_d));
	t.put("adc x 3 // mov.d (d -> x)",                      col(b, adc_x, adc_x, adc_x,                         mov_x_d));
	t.put("adc // mov.d (d -> x) x 2",                      col(b, adc_x,                                       mov_x_d, mov_x_d));
	t.put("adc x 2 // mov.d (d -> x) x 2",                  col(b, adc_x, adc_x,                                mov_x_d, mov_x_d));
	t.put("adc x 3 // mov.d (d -> x) x 2",                  col(b, adc_x, adc_x, adc_x,                         mov_x_d, mov_x_d));
	t.put("mul // mov.d (d -> x) x 2",                      col(b, mul_x,                                       mov_x_d, mov_x_d));
	t.put("mul x 2 // mov.d (d -> x) x 2",                  col(b, mul_x, mul_x,                                mov_x_d, mov_x_d));

	t.put("add.b x 2 // mov.d (d -> x) x 2",                col(b, add_v, add_v,                                mov_x_d, mov_x_d));
	t.put("add.b x 3 // mov.d (d -> x) x 2",                col(b, add_v, add_v, add_v,                         mov_x_d, mov_x_d));
	t.put("add.b x 4 // mov.d (d -> x) x 2",                col(b, add_v, add_v, add_v, add_v,                  mov_x_d, mov_x_d));

	t.put("add.b x 2 // fmov.d (d -> x) x 2",               col(b, add_v, add_v,                                fmov_x_d, fmov_x_d));
	t.put("add.b x 3 // fmov.d (d -> x) x 2",               col(b, add_v, add_v, add_v,                         fmov_x_d, fmov_x_d));
	t.put("add.b x 4 // fmov.d (d -> x) x 2",               col(b, add_v, add_v, add_v, add_v,                  fmov_x_d, fmov_x_d));

	t.put("add.b x 2 // mov.d (d[1] -> x) x 2",             col(b, add_v, add_v,                                mov_x_d1, mov_x_d1));
	t.put("add.b x 3 // mov.d (d[1] -> x) x 2",             col(b, add_v, add_v, add_v,                         mov_x_d1, mov_x_d1));
	t.put("add.b x 4 // mov.d (d[1] -> x) x 2",             col(b, add_v, add_v, add_v, add_v,                  mov_x_d1, mov_x_d1));

	{
	memmgr m_h(mem_init( p ));
	bench_col h(freq, m_h.ptr(), m_h.ptr());
	t.put("ldr x 3 // mov.d (d -> x) x 2",                  col(h, ldr_x_s, ldr_x_s, ldr_x_s,                   mov_x_d, mov_x_d));
	t.put("ldr.q x 3 // mov.d (d -> x) x 2",                col(h, ldr_q_s, ldr_q_s, ldr_q_s,                   mov_x_d, mov_x_d));
	t.put("str x 2 // mov.d (d -> x) x 2",                  col(h, str_x_s, str_x_s,                            mov_x_d, mov_x_d));
	t.put("str.q x 2 // mov.d (d -> x) x 2",                col(h, str_q_s, str_q_s,                            mov_x_d, mov_x_d));
	}

	/* flag d -> x */
	t.put("(fcmp -> adc)",                                  col(b, fcmp_0_adc_x));
	t.put("(fcmp -> adc) // mov.d (d -> x)",                col(b, fcmp_0_adc_x,                                mov_x_d));
	t.put("(fcmp -> adc) x 2 // mov.d (d -> x)",            col(b, fcmp_0_adc_x, fcmp_0_adc_x,                  mov_x_d));
	t.put("(fcmp -> adc) x 3 // mov.d (d -> x)",            col(b, fcmp_0_adc_x, fcmp_0_adc_x, fcmp_0_adc_x,    mov_x_d));
	t.put("(fcmp -> adc) // mov.d (d -> x) x 2",            col(b, fcmp_0_adc_x,                                mov_x_d, mov_x_d));
	t.put("(fcmp -> adc) x 2 // mov.d (d -> x) x 2",        col(b, fcmp_0_adc_x, fcmp_0_adc_x,                  mov_x_d, mov_x_d));
	t.put("(fcmp -> adc) x 3 // mov.d (d -> x) x 2",        col(b, fcmp_0_adc_x, fcmp_0_adc_x, fcmp_0_adc_x,    mov_x_d, mov_x_d));

	/*
	 * fcvtas, fcvtau, scvtf, fcsel, fcmp, fccmp, frecpe, fsqrt
	 */
	t.put("add.b x 2 // fcvtas.s (s -> s) x 2",             col(b, add_v, add_v,                                fcvtas_s, fcvtas_s));
	t.put("add.b x 3 // fcvtas.s (s -> s) x 2",             col(b, add_v, add_v, add_v,                         fcvtas_s, fcvtas_s));
	t.put("add.b x 4 // fcvtas.s (s -> s) x 2",             col(b, add_v, add_v, add_v, add_v,                  fcvtas_s, fcvtas_s));
	t.put("add.b // fcvtas.s (s -> s) x 3",                 col(b, add_v,                                       fcvtas_s, fcvtas_s, fcvtas_s));
	t.put("add.b x 2 // fcvtas.s (s -> s) x 3",             col(b, add_v, add_v,                                fcvtas_s, fcvtas_s, fcvtas_s));

	t.put("add.b x 2 // fcvtas.s (s -> x)",                 col(b, add_v, add_v,                                fcvtas_x));
	t.put("add.b x 3 // fcvtas.s (s -> x)",                 col(b, add_v, add_v, add_v,                         fcvtas_x));
	t.put("add.b x 4 // fcvtas.s (s -> x)",                 col(b, add_v, add_v, add_v, add_v,                  fcvtas_x));

	t.put("add.b x 2 // fcvtas.s (s -> x) x 2",             col(b, add_v, add_v,                                fcvtas_x, fcvtas_x));
	t.put("add.b x 3 // fcvtas.s (s -> x) x 2",             col(b, add_v, add_v, add_v,                         fcvtas_x, fcvtas_x));
	t.put("add.b x 4 // fcvtas.s (s -> x) x 2",             col(b, add_v, add_v, add_v, add_v,                  fcvtas_x, fcvtas_x));

	t.put("add x 4 // fcvtas.s (s -> x)",                   col(b, add_x, add_x, add_x, add_x,                  fcvtas_x));
	t.put("add x 5 // fcvtas.s (s -> x)",                   col(b, add_x, add_x, add_x, add_x, add_x,           fcvtas_x));
	t.put("add x 6 // fcvtas.s (s -> x)",                   col(b, add_x, add_x, add_x, add_x, add_x, add_x,    fcvtas_x));

	t.put("add x 4 // fcvtas.s (s -> x) x 2",               col(b, add_x, add_x, add_x, add_x,                  fcvtas_x, fcvtas_x));
	t.put("add x 5 // fcvtas.s (s -> x) x 2",               col(b, add_x, add_x, add_x, add_x, add_x,           fcvtas_x, fcvtas_x));
	t.put("add x 6 // fcvtas.s (s -> x) x 2",               col(b, add_x, add_x, add_x, add_x, add_x, add_x,    fcvtas_x, fcvtas_x));

	t.put("adc // fcvtas.s (s -> x)",                       col(b, adc_x,                                       fcvtas_x));
	t.put("adc x 2 // fcvtas.s (s -> x)",                   col(b, adc_x, adc_x,                                fcvtas_x));
	t.put("adc x 3 // fcvtas.s (s -> x)",                   col(b, adc_x, adc_x, adc_x,                         fcvtas_x));

	t.put("adc // fcvtas.s (s -> x) x 2",                   col(b, adc_x,                                       fcvtas_x, fcvtas_x));
	t.put("adc x 2 // fcvtas.s (s -> x) x 2",               col(b, adc_x, adc_x,                                fcvtas_x, fcvtas_x));
	t.put("adc x 3 // fcvtas.s (s -> x) x 2",               col(b, adc_x, adc_x, adc_x,                         fcvtas_x, fcvtas_x));

	{
	memmgr m_h(mem_init( p ));
	bench_col h(freq, m_h.ptr(), m_h.ptr());
	t.put("ldr x 3 // fcvtas.s (s -> s) x 2",               col(h, ldr_x_s, ldr_x_s, ldr_x_s,                   fcvtas_s, fcvtas_s));
	t.put("ldr.q x 3 // fcvtas.s (s -> s) x 2",             col(h, ldr_q_s, ldr_q_s, ldr_q_s,                   fcvtas_s, fcvtas_s));
	t.put("str x 2 // fcvtas.s (s -> s) x 2",               col(h, str_x_s, str_x_s,                            fcvtas_s, fcvtas_s));
	t.put("str.q x 2 // fcvtas.s (s -> s) x 2",             col(h, str_q_s, str_q_s,                            fcvtas_s, fcvtas_s));
	}

	{
	memmgr m_h(mem_init( p ));
	bench_col h(freq, m_h.ptr(), m_h.ptr());
	t.put("ldr x 3 // fcvtas.s (s -> x) x 2",               col(h, ldr_x_s, ldr_x_s, ldr_x_s,                   fcvtas_s, fcvtas_s));
	t.put("ldr.q x 3 // fcvtas.s (s -> x) x 2",             col(h, ldr_q_s, ldr_q_s, ldr_q_s,                   fcvtas_s, fcvtas_s));
	t.put("str x 2 // fcvtas.s (s -> x) x 2",               col(h, str_x_s, str_x_s,                            fcvtas_s, fcvtas_s));
	t.put("str.q x 2 // fcvtas.s (s -> x) x 2",             col(h, str_q_s, str_q_s,                            fcvtas_s, fcvtas_s));
	}

	t.put("add.b x 2 // scvtf.s (x -> s)",                  col(b, add_v, add_v,                                scvtf_w));
	t.put("add.b x 3 // scvtf.s (x -> s)",                  col(b, add_v, add_v, add_v,                         scvtf_w));
	t.put("add.b x 4 // scvtf.s (x -> s)",                  col(b, add_v, add_v, add_v, add_v,                  scvtf_w));

	t.put("add.b x 2 // scvtf.s (x -> s) x 2",              col(b, add_v, add_v,                                scvtf_w, scvtf_w));
	t.put("add.b x 3 // scvtf.s (x -> s) x 2",              col(b, add_v, add_v, add_v,                         scvtf_w, scvtf_w));
	t.put("add.b x 4 // scvtf.s (x -> s) x 2",              col(b, add_v, add_v, add_v, add_v,                  scvtf_w, scvtf_w));

	t.put("add x 4 // scvtf.s (x -> s)",                    col(b, add_x, add_x, add_x, add_x,                  scvtf_w));
	t.put("add x 5 // scvtf.s (x -> s)",                    col(b, add_x, add_x, add_x, add_x, add_x,           scvtf_w));
	t.put("add x 6 // scvtf.s (x -> s)",                    col(b, add_x, add_x, add_x, add_x, add_x, add_x,    scvtf_w));

	t.put("add x 4 // scvtf.s (x -> s) x 2",                col(b, add_x, add_x, add_x, add_x,                  scvtf_w, scvtf_w));
	t.put("add x 5 // scvtf.s (x -> s) x 2",                col(b, add_x, add_x, add_x, add_x, add_x,           scvtf_w, scvtf_w));
	t.put("add x 6 // scvtf.s (x -> s) x 2",                col(b, add_x, add_x, add_x, add_x, add_x, add_x,    scvtf_w, scvtf_w));

	t.put("adc // scvtf.s (x -> s)",                        col(b, adc_x,                                       scvtf_w));
	t.put("adc x 2 // scvtf.s (x -> s)",                    col(b, adc_x, adc_x,                                scvtf_w));
	t.put("adc x 3 // scvtf.s (x -> s)",                    col(b, adc_x, adc_x, adc_x,                         scvtf_w));

	t.put("adc // scvtf.s (x -> s) x 2",                    col(b, adc_x,                                       scvtf_w, scvtf_w));
	t.put("adc x 2 // scvtf.s (x -> s) x 2",                col(b, adc_x, adc_x,                                scvtf_w, scvtf_w));
	t.put("adc x 3 // scvtf.s (x -> s) x 2",                col(b, adc_x, adc_x, adc_x,                         scvtf_w, scvtf_w));

	{
	memmgr m_h(mem_init( p ));
	bench_col h(freq, m_h.ptr(), m_h.ptr());
	t.put("ldr // scvtf.s (x -> s) x 2",                    col(h, ldr_x_s,                                     scvtf_w, scvtf_w));
	t.put("ldr.q // scvtf.s (x -> s) x 2",                  col(h, ldr_q_s,                                     scvtf_w, scvtf_w));
	t.put("ldr x 2 // scvtf.s (x -> s)",                    col(h, ldr_x_s, ldr_x_s,                            scvtf_w));
	t.put("ldr.q x 2 // scvtf.s (x -> s)",                  col(h, ldr_q_s, ldr_q_s,                            scvtf_w));
	t.put("ldr x 2 // scvtf.s (x -> s) x 2",                col(h, ldr_x_s, ldr_x_s,                            scvtf_w, scvtf_w));
	t.put("ldr.q x 2 // scvtf.s (x -> s) x 2",              col(h, ldr_q_s, ldr_q_s,                            scvtf_w, scvtf_w));
	t.put("ldr x 3 // scvtf.s (x -> s)",                    col(h, ldr_x_s, ldr_x_s, ldr_x_s,                   scvtf_w));
	t.put("ldr.q x 3 // scvtf.s (x -> s)",                  col(h, ldr_q_s, ldr_q_s, ldr_q_s,                   scvtf_w));
	t.put("ldr x 3 // scvtf.s (x -> s) x 2",                col(h, ldr_x_s, ldr_x_s, ldr_x_s,                   scvtf_w, scvtf_w));
	t.put("ldr.q x 3 // scvtf.s (x -> s) x 2",              col(h, ldr_q_s, ldr_q_s, ldr_q_s,                   scvtf_w, scvtf_w));

	t.put("str x 2 // scvtf.s (x -> s) x 2",                col(h, str_x_s, str_x_s,                            scvtf_w, scvtf_w));
	t.put("str.q x 2 // scvtf.s (x -> s) x 2",              col(h, str_q_s, str_q_s,                            scvtf_w, scvtf_w));
	t.put("str // scvtf.s (x -> s) x 3",                    col(h, str_x_s,                                     scvtf_w, scvtf_w, scvtf_w));
	t.put("str.q // scvtf.s (x -> s) x 3",                  col(h, str_q_s,                                     scvtf_w, scvtf_w, scvtf_w));
	t.put("str x 2 // scvtf.s (x -> s) x 3",                col(h, str_x_s, str_x_s,                            scvtf_w, scvtf_w, scvtf_w));
	t.put("str.q x 2 // scvtf.s (x -> s) x 3",              col(h, str_q_s, str_q_s,                            scvtf_w, scvtf_w, scvtf_w));
	}

	{
	memmgr m_h(mem_init( p ));
	bench_col h(freq, m_h.ptr(), m_h.ptr());
	bench_col n(freq, m_h.ptr(), NULL);

	t.put("scvtf.s (x -> s) // fcvtas.s (s -> x) x 2",      col(h, scvtf_w,                                     fcvtas_s, fcvtas_s));
	t.put("scvtf.s (x -> s) // fcvtas.s (s -> x) x 3",      col(h, scvtf_w,                                     fcvtas_s, fcvtas_s, fcvtas_s));
	t.put("scvtf.s (x -> s) x 2 // fcvtas.s (s -> x)",      col(h, scvtf_w, scvtf_w,                            fcvtas_s));
	t.put("scvtf.s (x -> s) x 2 // fcvtas.s (s -> x) x 2",  col(h, scvtf_w, scvtf_w,                            fcvtas_s, fcvtas_s));
	t.put("scvtf.s (x -> s) x 3 // fcvtas.s (s -> x)",      col(h, scvtf_w, scvtf_w, scvtf_w,                   fcvtas_s));
	}
	return;
}

#if 0
/* crypto */
op_col_fn(sha1c_v,             ({ g->sha1c(s->q, s->s, s->v.s); }) )
op_col_fn(sha1h_v,             ({ g->sha1h(s->s, s->s); }) )
op_col_fn(sha1m_v,             ({ g->sha1m(s->q, s->s, s->v.s); }) )
op_col_fn(sha1p_v,             ({ g->sha1p(s->q, s->s, s->v.s); }) )
op_col_fn(sha1su0_v,           ({ g->sha1su0(s->v.s, s->v.s, s->v.s); }) )
op_col_fn(sha1su1_v,           ({ g->sha1su1(s->v.s, s->v.s); }) )

op_col_fn(sha256h_v,           ({ g->sha256h(s->q, s->q, s->v.s); }) )
op_col_fn(sha256h2_v,          ({ g->sha256h2(s->q, s->q, s->v.s); }) )
op_col_fn(sha256su0_v,         ({ g->sha256su0(s->v.s, s->v.s); }) )
op_col_fn(sha256su1_v,         ({ g->sha256su1(s->v.s, s->v.s, s->v.s); }) )
#endif

static
void bench_cap_crypto(bool md, double freq) {
	table t(md, "SIMD / fp", 1, 20);
	bench_col b(freq);

	t.put("sha1c",                                          col(b, sha1c_v));
	t.put("sha1h",                                          col(b, sha1h_v));
	t.put("sha1su0",                                        col(b, sha1su0_v));
	t.put("sha256h",                                        col(b, sha256h_v));
	t.put("sha256su0",                                      col(b, sha256su0_v));

	t.put("sha1h // add.b x 2",                             col(b, sha1h_v,                                     add_v, add_v));
	t.put("sha1h // add.b x 3",                             col(b, sha1h_v,                                     add_v, add_v, add_v));
	t.put("sha1h // add.b x 4",                             col(b, sha1h_v,                                     add_v, add_v, add_v, add_v));

	/*
	t.put("sha1c // add.b x 2",                             col(b, sha1c_v,                                     add_v, add_v));
	t.put("sha1c // add.b x 3",                             col(b, sha1c_v,                                     add_v, add_v, add_v));
	t.put("sha1c // add.b x 4",                             col(b, sha1c_v,                                     add_v, add_v, add_v, add_v));
	*/

	t.put("sha1h // sha1c",                                 col(b, sha1h_v,                                     sha1c_v));
	t.put("sha1h x 2 // sha1c",                             col(b, sha1h_v, sha1h_v,                            sha1c_v));
	t.put("sha1h x 3 // sha1c",                             col(b, sha1h_v, sha1h_v, sha1h_v,                   sha1c_v));
	t.put("sha1h x 4 // sha1c",                             col(b, sha1h_v, sha1h_v, sha1h_v, sha1h_v,          sha1c_v));

	t.put("sha1h // sha256h",                               col(b, sha1h_v,                                     sha256h_v));
	t.put("sha1h x 2 // sha256h",                           col(b, sha1h_v, sha1h_v,                            sha256h_v));
	t.put("sha1h x 3 // sha256h",                           col(b, sha1h_v, sha1h_v, sha1h_v,                   sha256h_v));
	t.put("sha1h x 4 // sha256h",                           col(b, sha1h_v, sha1h_v, sha1h_v, sha1h_v,          sha256h_v));

	t.put("sha1h // sha256su0",                             col(b, sha1h_v,                                     sha256su0_v));
	t.put("sha1h x 2 // sha256su0",                         col(b, sha1h_v, sha1h_v,                            sha256su0_v));
	t.put("sha1h x 3 // sha256su0",                         col(b, sha1h_v, sha1h_v, sha1h_v,                   sha256su0_v));
	t.put("sha1h x 4 // sha256su0",                         col(b, sha1h_v, sha1h_v, sha1h_v, sha1h_v,          sha256su0_v));

	t.put("sha256su0 // sha256h",                           col(b, sha256su0_v,                                 sha256h_v));
	t.put("sha256su0 x 2 // sha256h",                       col(b, sha256su0_v, sha256su0_v,                    sha256h_v));
	t.put("sha256su0 x 3 // sha256h",                       col(b, sha256su0_v, sha256su0_v, sha256su0_v,       sha256h_v));

	t.put("sha1c // sha256h",                               col(b, sha1c_v,                                     sha256h_v));
	t.put("sha1c // sha256h x 2",                           col(b, sha1c_v,                                     sha256h_v, sha256h_v));
	t.put("sha1c // sha256h x 3",                           col(b, sha1c_v,                                     sha256h_v, sha256h_v, sha256h_v));
	t.put("sha1c // sha256h x 4",                           col(b, sha1c_v,                                     sha256h_v, sha256h_v, sha256h_v, sha256h_v));
	t.put("sha1c // sha256h x 5",                           col(b, sha1c_v,                                     sha256h_v, sha256h_v, sha256h_v, sha256h_v, sha256h_v));

	t.put("sha1c // sha256su0",                             col(b, sha1c_v,                                     sha256su0_v));
	t.put("sha1c // sha256su0 x 2",                         col(b, sha1c_v,                                     sha256su0_v, sha256su0_v));
	t.put("sha1c // sha256su0 x 3",                         col(b, sha1c_v,                                     sha256su0_v, sha256su0_v, sha256su0_v));
	t.put("sha1c // sha256su0 x 4",                         col(b, sha1c_v,                                     sha256su0_v, sha256su0_v, sha256su0_v, sha256su0_v));
	t.put("sha1c // sha256su0 x 5",                         col(b, sha1c_v,                                     sha256su0_v, sha256su0_v, sha256su0_v, sha256su0_v, sha256su0_v));

	t.put("fcmp.s // fcsel.s",                              col(b, fcmp_0,                                      fcsel_s));
	t.put("fcmp.s // fcsel.s x 2",                          col(b, fcmp_0,                                      fcsel_s, fcsel_s));

	t.put("sha1h // fcmp.s",                                col(b, sha1h_v,                                     fcmp_s));
	t.put("sha1h // fcsel.s",                               col(b, sha1h_v,                                     fcsel_s));
	t.put("sha1h // fcsel.s x 2",                           col(b, sha1h_v,                                     fcsel_s, fcsel_s));
	t.put("sha1h // fcsel.s x 3",                           col(b, sha1h_v,                                     fcsel_s, fcsel_s, fcsel_s));

	t.put("add.b // fcmp.s",                                col(b, add_v,                                       fcmp_0));
	t.put("add.b x 2 // fcmp.s",                            col(b, add_v, add_v,                                fcmp_0));
	t.put("add.b x 3 // fcmp.s",                            col(b, add_v, add_v, add_v,                         fcmp_0));
	t.put("add.b x 4 // fcmp.s",                            col(b, add_v, add_v, add_v, add_v,                  fcmp_0));

	t.put("add.b // fcsel.s",                               col(b, add_v,                                       fcsel_s));
	t.put("add.b x 2 // fcsel.s",                           col(b, add_v, add_v,                                fcsel_s));
	t.put("add.b x 3 // fcsel.s",                           col(b, add_v, add_v, add_v,                         fcsel_s));
	t.put("add.b x 4 // fcsel.s",                           col(b, add_v, add_v, add_v, add_v,                  fcsel_s));

	t.put("fjcvtzs.d (s -> x) // fcmp.s",                   col(b, fjcvtzs_d,                                   fcmp_0));
	t.put("fcvtas.s (s -> x) // fcmp.s",                    col(b, fcvtas_s,                                    fcmp_0));
	t.put("fcvtas.s (s -> x) x 2 // fcmp.s",                col(b, fcvtas_s, fcvtas_s,                          fcmp_0));

	t.put("fcvtas.s (s -> x) // fcsel.s",                   col(b, fcvtas_s,                                    fcsel_s));
	t.put("fcvtas.s (s -> x) x 2 // fcsel.s",               col(b, fcvtas_s, fcvtas_s,                          fcsel_s));
	t.put("fcvtas.s (s -> x) x 2 // fcsel.s",               col(b, fcvtas_s, fcvtas_s,                          fcsel_s));


	t.put("frecpe.s (scl) // frsqrte.s (scl)",              col(b, frecpe_s,  frsqrte_s));
	t.put("frecpe.s (scl) // fcmp.s",                       col(b, frecpe_s,  fcmp_s));
	t.put("frsqrte.s (scl) // fcmp.s",                      col(b, frsqrte_s, fcmp_s));

	t.put("frecpe.s (scl) // frsqrte.s (scl)",              col(b, frecpe_s,  frsqrte_s));
	t.put("frecpe.s (scl) // fcmp.s",                       col(b, frecpe_s,  fcmp_s));
	t.put("frsqrte.s (scl) // fcmp.s",                      col(b, frsqrte_s, fcmp_s));
	return;
}

void bench_port_cap(bool md, double freq) {
	bench_cap_base(md, freq);
	bench_cap_scalar(md, freq);
	bench_cap_crypto(md, freq);
	bench_cap_branch(md, freq);
	bench_cap_load_store(md, freq);
	bench_cap_transfer(md, freq);
	return;
}

/*
 * end of portcap.cc
 */
