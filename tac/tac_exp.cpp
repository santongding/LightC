//
// Created by os on 4/29/22.
//
#include "tac.hpp"
int next_tmp;
int yylineno;
TAC * tac_last;
string mk_tmp() {

    char name[12];

    sprintf(name, "t%d", next_tmp++); /* Set up text */
    return name;
}


SYM *mk_const(int n) {
    return new SYM(SYM_CONST, n);
}

SYM *mk_text(const string &s) {

}

TAC *mk_tac(int op, SYM *a, SYM *b, SYM *c) {

}

EXP *mk_exp(EXP *next, SYM *ret, TAC *code) {

}

TAC *join_tac(TAC *c1, TAC *c2) {

}

EXP *do_assign(SYM *var, EXP *exp)
{
    TAC *code;

    if(var->IsConst()) error("assignment to non-variable");

    code=mk_tac(TAC_COPY, var, exp->ret, NULL);
    code->prev=exp->tac;

    exp->ret  = var;
    exp->tac = code;
    return exp;
}

EXP *do_un(int unop, EXP *exp) {
    TAC *temp; /* TAC code for temp symbol */
    TAC *ret; /* TAC code for result */

    /* Do constant folding if possible. Calculate the constant into exp */
    if (exp->ret->IsConst()) {
        switch (unop) /* Chose the operator */
        {
            case TAC_NEG:
                exp->ret->GetValue() = -exp->ret->GetValue();
                break;
        }

        return exp; /* The new expression */
    }

    temp = mk_tac(TAC_VAR, new SYM(SYM_VAR, mk_tmp()), NULL, NULL);
    temp->prev = exp->tac;
    ret = mk_tac(unop, temp->a, exp->ret, NULL);
    ret->prev = temp;

    exp->ret = temp->a;
    exp->tac = ret;

    return exp;
}

EXP *do_bin(int binop, EXP *exp1, EXP *exp2) {
    TAC *temp; /* TAC code for temp symbol */
    TAC *ret; /* TAC code for result */

    if ((exp1->ret->IsConst()) && (exp2->ret->IsConst())) {
        int newval; /* The result of constant folding */

        switch (binop) /* Chose the operator */
        {
            case TAC_ADD:
                newval = exp1->ret->GetValue() + exp2->ret->GetValue();
                break;

            case TAC_SUB:
                newval = exp1->ret->GetValue() - exp2->ret->GetValue();
                break;

            case TAC_MUL:
                newval = exp1->ret->GetValue() * exp2->ret->GetValue();
                break;

            case TAC_DIV:
                newval = exp1->ret->GetValue() / exp2->ret->GetValue();
                break;
        }

        exp1->ret = mk_const(newval); /* New space for result */

        return exp1; /* The new expression */
    }

    temp = mk_tac(TAC_VAR, new SYM(SYM_VAR, mk_tmp()), NULL, NULL);
    temp->prev = join_tac(exp1->tac, exp2->tac);
    ret = mk_tac(binop, temp->a, exp1->ret, exp2->ret);
    ret->prev = temp;

    exp1->ret = temp->a;
    exp1->tac = ret;

    return exp1;
}

EXP *do_cmp(int binop, EXP *exp1, EXP *exp2) {
    TAC *temp; /* TAC code for temp symbol */
    TAC *ret; /* TAC code for result */

    if ((exp1->ret->IsConst()) && (exp2->ret->IsConst())) {
        int newval; /* The result of constant folding */

        switch (binop) /* Chose the operator */
        {
            case TAC_EQ:
                newval = (exp1->ret->GetValue() == exp2->ret->GetValue());
                break;

            case TAC_NE:
                newval = (exp1->ret->GetValue() != exp2->ret->GetValue());
                break;

            case TAC_LT:
                newval = (exp1->ret->GetValue() < exp2->ret->GetValue());
                break;

            case TAC_LE:
                newval = (exp1->ret->GetValue() <= exp2->ret->GetValue());
                break;

            case TAC_GT:
                newval = (exp1->ret->GetValue() > exp2->ret->GetValue());
                break;

            case TAC_GE:
                newval = (exp1->ret->GetValue() >= exp2->ret->GetValue());
                break;
        }

        exp1->ret = mk_const(newval); /* New space for result */
        return exp1; /* The new expression */
    }

    temp = mk_tac(TAC_VAR, new SYM(SYM_VAR, mk_tmp()), NULL, NULL);
    temp->prev = join_tac(exp1->tac, exp2->tac);
    ret = mk_tac(binop, temp->a, exp1->ret, exp2->ret);
    ret->prev = temp;

    exp1->ret = temp->a;
    exp1->tac = ret;

    return exp1;
}

/*
string find_first_and_truncate(string &s) {
    auto pos = s.find_first_of('.');
    auto name = s.substr(0);
    s = s.substr(pos);
    return name;
}*/

EXP *do_locate(EXP *x, EXP *y) {
    auto sx = x->ret;
    auto sy = y->ret;
    SYM *ret = new SYM(SYM_LINK, mk_tmp());
    auto code = mk_tac(TAC_LOCATE, ret, sx, sy);
    x->ret = ret;
    code->prev = x->tac;
    x->tac = join_tac(code, y->tac);
    return x;
}

EXP *do_call_ret(SYM *sym, EXP *arglist) {

    EXP *alt; /* For counting args */
    SYM *ret; /* Where function result will go */
    TAC *code; /* Resulting code */
    TAC *temp; /* Temporary for building code */


    for (alt = arglist; alt != NULL; alt = alt->next) code = join_tac(code, alt->tac);

    while (arglist != NULL) /* Generate ARG instructions */
    {
        temp = mk_tac(TAC_ACTUAL, arglist->ret, NULL, NULL);
        temp->prev = code;
        code = temp;

        alt = arglist->next;
        arglist = alt;
    };
    ret = new SYM(SYM_UNKNOWN, mk_tmp()); /* For the result */
    code = mk_tac(TAC_VAR, ret, NULL, NULL);
    temp = mk_tac(TAC_CALL, ret, sym, NULL);
    temp->prev = code;
    code = temp;

    return mk_exp(NULL, ret, code);
}
void error(char *str) {
fprintf(stderr, "error: %s\n", str);
exit(1);
}
