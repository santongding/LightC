#ifndef LIGHTC_TAC_HPP
#define LIGHTC_TAC_HPP

#include <string>
#include <cassert>
#include "def.h"
#include "utils.hpp"

#include "tac_def.h"


typedef struct exp /* Parser expression */
{
    struct exp *next; /* For argument lists */
    TAC *tac; /* The code */
    SYM *ret; /* Where the result is */
} EXP;


/* global var */
extern int next_tmp, next_label;
extern TAC *tac_first, *tac_last;

/* function */
void tac_init();

void tac_complete();

void tac_dump();

void tac_print(TAC *i);

string mk_tmp();


SYM *mk_const(int n);

TAC *mk_tac(TAC_TYPE op, SYM *a, SYM *b, SYM *c, bool lineno);

EXP *mk_exp(EXP *next, SYM *ret, TAC *code);

TAC *join_tac(TAC *c1, TAC *c2);

EXP *do_assign(EXP *var, EXP *exp);

EXP *do_un(TAC_TYPE unop, EXP *exp);

EXP *do_bin(TAC_TYPE binop, EXP *exp1, EXP *exp2);

EXP *do_cmp(TAC_TYPE binop, EXP *exp1, EXP *exp2);

EXP *do_locate(EXP *x, SYM *y);

EXP *do_call_ret(EXP *obj, SYM *func, EXP *arglist);

EXP *do_exp_list(EXP *exps, bool needFlush);

EXP *join_exp(EXP *x, EXP *y);

EXP *flush_exp(EXP *x);

TAC *mk_block(TAC *x);

TAC *do_test(EXP *exp, TAC *stmt1, TAC *stmt2);

TAC *do_while(EXP *exp, TAC *stmt);

TAC *declare(SYM *type, SYM *name);

TAC *declare_link(SYM *type, SYM *name);

TAC *declare_new(SYM *type, SYM *name);

TAC *mk_func(SYM *type, SYM *name, TAC *tac, TAC *block);

TAC *mk_class(SYM *name, TAC *tac);


#endif //LIGHTC_TAC_HPP
