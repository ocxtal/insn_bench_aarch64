
/**
 * @file branch.cc
 * @brief measure branch latencies
 * @author Hajime Suzuki
 */
#include "utils.h"
#include <stdlib.h>

void bench_branch(bool md, double freq) {
	table t(md, "Branch");

	/* unconditional branch */ {
		bench b(freq, (size_t)0, (size_t)1);

		t.put("b (pc+4)", both(b, op(
			Label l;
			g->b(l); g->L(l);
		), 0.0, lat_patterns, lat_patterns));
		t.put("b (pc+8)", both(b, op(
			Label l;
			g->b(l); g->nop(); g->L(l);
		), 0.0, lat_patterns, lat_patterns));
		t.put("b (pc+4) // add (chain)", both(b, op(
			Label l;
			g->add(g->x0, g->x0, g->x1);
			g->b(l); g->L(l);
		), 0.0, lat_patterns, lat_patterns));
		t.put("b (pc+4) // add x 2 (chain)", both(b, op(
			Label l;
			g->add(g->x0, g->x0, g->x1);
			g->add(g->x0, g->x0, g->x1);
			g->b(l); g->L(l);
		), 0.0, lat_patterns, lat_patterns));
		t.put("adr -> br (pc+4)", both(b, op(
			g->adr(g->x0, 8); g->br(g->x0);
		), 0.0, lat_patterns, lat_patterns));
		t.put("adr -> br (pc+4) // add (chain)", both(b, op(
			g->adr(g->x0, 12);
			g->add(g->x1, g->x1, g->x2);
			g->br(g->x0);
		), 0.0, lat_patterns, lat_patterns));
		t.put("adr -> br (pc+4) // add x 2 (chain)", both(b, op(
			g->add(g->x1, g->x1, g->x2);
			g->adr(g->x0, 12);
			g->add(g->x1, g->x1, g->x2);
			g->br(g->x0);
		), 0.0, lat_patterns, lat_patterns));


		#define bl_ret(_body) both(b, \
			op( g->bl(*gl) ), op(), \
			op_init( \
				Label l; \
				               g->b(l); \
				g->align(128); g->L(*gl); _body; g->ret(); \
				g->align(128); g->L(l); \
			), \
			op_init(), \
			0.0, lat_patterns, lat_patterns \
		)
		#define blr_ret(_body) both(b, \
			op( g->blr(g->x0) ), op(), \
			op_init( \
				Label l; \
				               g->adr(g->x0, 128); g->b(l); \
				g->align(128); _body; g->ret(); \
				g->align(128); g->L(l); \
			), \
			op_init(), \
			0.0, lat_patterns, lat_patterns \
		)

		t.put("bl-ret",                     bl_ret());
		t.put("bl-ret // add (chain)",      bl_ret(  g->add(g->x1, g->x1, g->x28) ));
		t.put("bl-ret // add x 2 (chain)",  bl_ret(  g->add(g->x1, g->x1, g->x28); g->add(g->x1, g->x1, g->x28) ));
		t.put("bl-ret // add x 3 (chain)",  bl_ret(  g->add(g->x1, g->x1, g->x28); g->add(g->x1, g->x1, g->x28); g->add(g->x1, g->x1, g->x28) ));
		t.put("blr-ret",                    blr_ret());
		t.put("blr-ret // add (chain)",     blr_ret( g->add(g->x1, g->x1, g->x28) ));
		t.put("blr-ret // add x 2 (chain)", blr_ret( g->add(g->x1, g->x1, g->x28); g->add(g->x1, g->x1, g->x28) ));
		t.put("blr-ret // add x 3 (chain)", blr_ret( g->add(g->x1, g->x1, g->x28); g->add(g->x1, g->x1, g->x28); g->add(g->x1, g->x1, g->x28) ));

		#undef bl_ret
		#undef blr_ret
	}

	/* conditional branch */ {
		bench b(freq, (size_t)0, (size_t)0xffffffffffffffff);
		t.put("cbz (pc+4; taken)", both(b, op(
			Label l;
			g->cbz(g->x27, l); g->L(l);
		), 0.0, lat_patterns, lat_patterns));
		t.put("cbz (pc+4; taken) // add (chain)", both(b, op(
			Label l;
			g->cbz(g->x27, l); g->L(l); g->add(g->x0, g->x0, g->x1);
		), 0.0, lat_patterns, lat_patterns));
		t.put("cbz (pc+4; taken) // add x 2 (chain)", both(b, op(
			Label l;
			g->cbz(g->x27, l); g->L(l); g->add(g->x0, g->x0, g->x1); g->add(g->x0, g->x0, g->x1);
		), 0.0, lat_patterns, lat_patterns));
		t.put("cbz (pc+4; taken) // b (pc+4)", thr(b, op(
			Label l[2];
			g->cbz(g->x27, l[0]); g->L(l[0]); g->b(l[1]); g->L(l[1]);
		), lat_inc2_pattern));


		t.put("cbz (pc+4; not taken)", both(b, op(
			Label l;
			g->cbz(g->x28, l); g->L(l);
		), 0.0, lat_patterns, lat_patterns));
		t.put("cbz (pc+4; not taken) // add (chain)", both(b, op(
			Label l;
			g->cbz(g->x28, l); g->L(l); g->add(g->x0, g->x0, g->x1);
		), 0.0, lat_patterns, lat_patterns));
		t.put("cbz (pc+4; not taken) // add x 2 (chain)", both(b, op(
			Label l;
			g->cbz(g->x28, l); g->L(l); g->add(g->x0, g->x0, g->x1); g->add(g->x0, g->x0, g->x1);
		), 0.0, lat_patterns, lat_patterns));
		t.put("cbz (pc+4; not taken) // b (pc+4)", thr(b, op(
			Label l[2];
			g->cbz(g->x28, l[0]); g->L(l[0]); g->b(l[1]); g->L(l[1]);
		), lat_inc2_pattern));

		t.put("cbnz (pc+4; taken)", both(b, op(
			Label l;
			g->cbnz(g->x28, l); g->L(l);
		), 0.0, lat_patterns, lat_patterns));
		t.put("cbnz (pc+4; not taken)", both(b, op(
			Label l;
			g->cbnz(g->x27, l); g->L(l);
		), 0.0, lat_patterns, lat_patterns));

		t.put("tbz (pc+4; taken)", both(b, op(
			Label l;
			g->tbz(g->x27, 0x01, l); g->L(l);
		), 0.0, lat_patterns, lat_patterns));
		t.put("tbz (pc+4; not taken)", both(b, op(
			Label l;
			g->tbz(g->x28, 0x01, l); g->L(l);
		), 0.0, lat_patterns, lat_patterns));

		t.put("tbnz (pc+4; taken)", both(b, op(
			Label l;
			g->tbnz(g->x28, 0x01, l); g->L(l);
		), 0.0, lat_patterns, lat_patterns));
		t.put("tbnz (pc+4; not taken)", both(b, op(
			Label l;
			g->tbnz(g->x27, 0x01, l); g->L(l);
		), 0.0, lat_patterns, lat_patterns));

		t.put("adds -> b.eq (pc+4; taken)", both(b, op(
			Label l;
			g->adds(g->x0, g->x0, g->x1);
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
	}

	/* conditional branch (unpredictable) */ {
		memmgr m_r(mem_init( g->rnd() ), 0, 128, 42);
		bench c(freq, NULL, m_r.ptr(), 1, 1, 64, 1, 64);

		#define y(_br, _body) { \
			Label l[16]; \
			x(_br, 0, _body);  x(_br, 1, _body);  x(_br, 2, _body);  x(_br, 3, _body); \
			x(_br, 4, _body);  x(_br, 5, _body);  x(_br, 6, _body);  x(_br, 7, _body); \
			x(_br, 8, _body);  x(_br, 9, _body);  x(_br, 10, _body); x(_br, 11, _body); \
			x(_br, 12, _body); x(_br, 13, _body); x(_br, 14, _body); x(_br, 15, _body); \
			g->lsr(g->x17, g->x17, 16); \
		}
		#define z(_br, _body) both(c, \
			op( \
				g->ldr(g->x17, ptr(g->x16)); \
				y(_br, _body); y(_br, _body); y(_br, _body); y(_br, _body); \
			), \
			op(), \
			op_init(), \
			op_init( g->add(g->x28, g->x28, 0x02); g->mov(g->x16, g->x28) ), \
			0.0, lat_patterns, lat_patterns \
		)

		/*
		#define x(_br, _n, _body) { \
			g->and_;
		}
		#undef x
		*/

		/* scalar cbz */
		#define x(_br, _n, _body) { \
			g->and_(g->x##_n, g->x17, 0x01<<(_n)); g->_br(g->x##_n, l[_n]); g->L(l[_n]); _body; \
		}
		t.put("and -> cbz (pc+4; full random)",                     z(cbz,  ; ));
		t.put("and -> cbnz (pc+4; full random)",                    z(cbnz, ; ));
		t.put("and -> cbz (pc+4; full random) // add (chain)",      z(cbz,  g->add(g->x18, g->x18, g->x19); ));
		t.put("and -> cbnz (pc+4; full random) // add (chain)",     z(cbnz, g->add(g->x18, g->x18, g->x19); ));
		t.put("and -> cbz (pc+4; full random) // add x 2 (chain)",  z(cbz,  g->add(g->x18, g->x18, g->x19); g->add(g->x18, g->x18, g->x19); ));
		t.put("and -> cbnz (pc+4; full random) // add x 2 (chain)", z(cbnz, g->add(g->x18, g->x18, g->x19); g->add(g->x18, g->x18, g->x19); ));
		#undef x

		/* scalar tbz */
		#define x(_br, _n, _body) { \
			g->_br(g->x17, _n, l[_n]); g->L(l[_n]); _body; \
		}
		t.put("tbz (pc+4; full random)",                            z(tbz,  ; ));
		t.put("tbnz (pc+4; full random)",                           z(tbnz, ; ));
		t.put("tbz (pc+4; full random) // add (chain)",             z(tbz,  g->add(g->x18, g->x18, g->x19); ));
		t.put("tbnz (pc+4; full random) // add (chain)",            z(tbnz, g->add(g->x18, g->x18, g->x19); ));
		t.put("tbz (pc+4; full random) // add x 2 (chain)",         z(tbz,  g->add(g->x18, g->x18, g->x19); g->add(g->x18, g->x18, g->x19); ));
		t.put("tbnz (pc+4; full random) // add x 2 (chain)",        z(tbnz, g->add(g->x18, g->x18, g->x19); g->add(g->x18, g->x18, g->x19); ));
		#undef x
		#undef y
		#undef z

		#if 1
		/* fp fcmp */
		#define y(_body) { \
			Label l[16]; \
			x(0, _body);  x(1, _body);  x(2, _body);  x(3, _body); \
			x(4, _body);  x(5, _body);  x(6, _body);  x(7, _body); \
			x(8, _body);  x(9, _body);  x(10, _body); x(11, _body); \
			x(12, _body); x(13, _body); x(14, _body); x(15, _body); \
			g->ushr(g->d17, g->d17, 16); \
		}
		#define z(_body) both(c, \
			op( \
				g->ldr(g->d17, ptr(g->x16)); \
				y(_body); y(_body); y(_body); y(_body); \
			), \
			op(), \
			op_init( g->mov(g->x28, 1); g->fmov(g->d1, g->x28); ), \
			op_init( g->add(g->x28, g->x28, 0x02); g->mov(g->x16, g->x28); ), \
			0.0, lat_patterns, lat_patterns \
		)
		#define x(_n, _body) { \
			if((_n)) { g->ushr(g->d16, g->d17, (_n)); } \
			g->and_(g->v16.b, g->v16.b, g->v28.b); \
			g->fcmp(g->d16, 0.0); \
			g->b(Cond::EQ, l[_n]); \
			g->L(l[_n]); \
			_body; \
		}
		t.put("tbz (pc+4; full random)",                            z( ; ));
		t.put("tbnz (pc+4; full random)",                           z( ; ));
		t.put("tbz (pc+4; full random) // add (chain)",             z( g->add(g->x18, g->x18, g->x19); ));
		t.put("tbnz (pc+4; full random) // add (chain)",            z( g->add(g->x18, g->x18, g->x19); ));
		t.put("tbz (pc+4; full random) // add x 2 (chain)",         z( g->add(g->x18, g->x18, g->x19); g->add(g->x18, g->x18, g->x19); ));
		t.put("tbnz (pc+4; full random) // add x 2 (chain)",        z( g->add(g->x18, g->x18, g->x19); g->add(g->x18, g->x18, g->x19); ));
		#undef x
		#undef y
		#undef z
		#endif
	}
	return;
}

/*
 * end of branch.cc
 */
