/* Lab3 Code generator. with virtual machine */

enum { IFETCH, ISTORE, IPUSH, IPOP, IADD, ISUB, ILT, JZ, JNZ, JMP, HALT };

typedef char code;
code object[1000], *here = object;

void g(code c) { *here++ = c; } 
code *hole() { return here++; }
void fix(code *src, code *dst) { *src = dst - src; } 

void c(node *x)
{
	code *p1, *p2;
	switch (x->kind)
	{
	case VAR: g(IFETCH); g(x->val); break;
	case CST: g(IPUSH); g(x->val); break;
	case ADD: c(x->o1); c(x->o2); g(IADD); break;
	case SUB: c(x->o1); c(x->o2); g(ISUB); break;
	case LT: c(x->o1); c(x->o2); g(ILT); break;
	case SET: c(x->o2); g(ISTORE); g(x->o1->val); break;
	case IF1: c(x->o1); g(JZ); p1 = hole(); c(x->o2); fix(p1, here); break;
	case IF2: c(x->o1); g(JZ); p1 = hole(); c(x->o2); g(JMP); p2 = hole();
		fix(p1, here); c(x->o3); fix(p2, here); break;
	case WHILE: p1 = here; c(x->o1); g(JZ); p2 = hole(); c(x->o2);
		g(JMP); fix(hole(), p1); fix(p2, here); break;
	case DO: p1 = here; c(x->o1); c(x->o2); g(JNZ); fix(hole(), p1); break;
	case EMPTY: break;
	case SEQ: c(x->o1); c(x->o2); break;
	case EXPR: c(x->o1); g(IPOP); break;
	case PROG: c(x->o1); g(HALT); break;
	}
}


/* Virtual machine. */

int globals[26];

void run()
{
	int stack[1000], *sp = stack;
	code *pc = object;
again: switch (*pc++)
{
case IFETCH: *sp++ = globals[*pc++];               goto again;
case ISTORE: globals[*pc++] = sp[-1];              goto again;
case IPUSH: *sp++ = *pc++;                        goto again;
case IPOP: --sp;                                 goto again;
case IADD: sp[-2] = sp[-2] + sp[-1]; --sp;       goto again;
case ISUB: sp[-2] = sp[-2] - sp[-1]; --sp;       goto again;
case ILT: sp[-2] = sp[-2] < sp[-1]; --sp;       goto again;
case JMP: pc += *pc;                            goto again;
case JZ: if (*--sp == 0) pc += *pc; else pc++; goto again;
case JNZ: if (*--sp != 0) pc += *pc; else pc++; goto again;
}
}