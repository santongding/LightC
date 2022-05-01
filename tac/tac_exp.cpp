//
// Created by os on 4/29/22.
//
#include "tac.hpp"

TAC *mk_tac(int op, SYM *a, SYM *b, SYM *c) {
    return mk_tac(op, a, b, c, true);
}

EXP *mk_exp(EXP *next, SYM *ret, TAC *code) {
    EXP *exp = new EXP;

    exp->next = next;
    exp->ret = ret;
    exp->tac = code;

    return exp;
}


EXP *do_assign(EXP *var, EXP *exp) {
    TAC *code;

    if (var->ret->IsConst()) error("assignment to non-variable");

    code = mk_tac(TAC_COPY, var->ret, exp->ret, NULL);
    code->prev = exp->tac;

    var->tac = join_tac(var->tac, code);
    return var;
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

    temp = mk_tac(TAC_DECLARE, new SYM(SYM_TYPE, "ref|"), new SYM(SYM_SYMBOL, mk_tmp()), NULL, NULL);
    temp->prev = exp->tac;
    ret = mk_tac(unop, temp->b, exp->ret, NULL);
    ret->prev = temp;

    exp->ret = temp->b;
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

    temp = mk_tac(TAC_DECLARE, new SYM(SYM_TYPE, "ref|"), new SYM(SYM_SYMBOL, mk_tmp()), NULL, NULL);
    temp->prev = join_tac(exp1->tac, exp2->tac);
    ret = mk_tac(binop, temp->b, exp1->ret, exp2->ret);
    ret->prev = temp;

    exp1->ret = temp->b;
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

    temp = mk_tac(TAC_DECLARE, new SYM(SYM_TYPE, "ref|"), new SYM(SYM_SYMBOL, mk_tmp()), NULL, NULL);
    temp->prev = join_tac(exp1->tac, exp2->tac);
    ret = mk_tac(binop, temp->b, exp1->ret, exp2->ret);
    ret->prev = temp;

    exp1->ret = temp->b;
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

EXP *do_locate(EXP *x, SYM *y) {
    auto sx = x->ret;
    SYM *ret = new SYM(SYM_SYMBOL, mk_tmp());
    auto code = mk_tac(TAC_DECLARE, new SYM(SYM_TYPE, "pointer|any|"), ret, nullptr);
    code = join_tac(code, mk_tac(TAC_LOCATE, ret, sx, y));
    x->ret = ret;

    x->tac = join_tac(x->tac, code);
    return x;
}

EXP *do_exp_list(EXP *exps) {
    TAC *code = nullptr;
    EXP *ret = exps;
    while (exps != nullptr) {
        ret->ret = exps->ret;
        code = join_tac(code, exps->tac);
        exps = exps->next;
    }
    if (exps)
        ret->tac = code;
    return ret;
}

EXP *do_call_ret(EXP *obj, SYM *func, EXP *arglist) {

    SYM *ret; /* Where function result will go */
    TAC *code = nullptr; /* Resulting code */
    TAC *temp; /* Temporary for building code */


    auto lis = arglist;

    while (arglist != NULL) /* Generate ARG instructions */
    {
        temp = mk_tac(TAC_ACTUAL, arglist->ret, NULL, NULL);
        temp->prev = code;
        code = temp;

        arglist = arglist->next;
    }
    code = join_tac(mk_tac(TAC_ACTUAL, obj->ret, NULL, NULL, false), code);

    EXP *exps = do_exp_list(lis);
    temp = join_tac(exps ? exps->tac : nullptr, code);
    ret = new SYM(SYM_UNKNOWN, mk_tmp()); /* For the result */
    code = mk_tac(TAC_DECLARE, new SYM(SYM_TYPE,"any|"),ret, NULL);
    code->prev = temp;
    temp = mk_tac(TAC_CALL, ret, obj->ret, func);

    temp->prev = code;
    code = temp;


    obj->tac = join_tac(obj->tac, code);
    obj->ret = ret;

    return obj;
}

EXP *join_exp(EXP *x, EXP *y) {
    auto tp = x;
    while (tp->next)tp = tp->next;
    tp->next = y;
    return x;
}

void error(const char *str) {
    fprintf(stderr, "line %d:  error: %s\n", getyylineno(), str);
    exit(1);
}

void error(const string &str) {
    error(str.c_str());
}