//
// Created by os on 4/29/22.
//
#include "tac.hpp"

int next_tmp;
TAC *tac_last, *tac_first;

void tac_init() {
    next_tmp = 0;
}

void tac_complete() {
    TAC *cur = NULL;        /* Current TAC */
    TAC *prev = tac_last;    /* Previous TAC */

    while (prev != NULL) {
        prev->next = cur;
        cur = prev;
        prev = prev->prev;
    }

    tac_first = cur;
}

int lastline = -1;

void tac_print(TAC *i) {
    if (i->linenum != lastline) {
        printf("/.............line:%04d............/\n", i->linenum);
        lastline = i->linenum;
    }
    switch (i->op) {
        case TAC_UNDEF:
            printf("undef");
            break;

        case TAC_ADD:
            printf("%s = %s + %s", i->a->ToStr().c_str(), i->b->ToStr().c_str(), i->c->ToStr().c_str());
            break;

        case TAC_SUB:
            printf("%s = %s - %s", i->a->ToStr().c_str(), i->b->ToStr().c_str(), i->c->ToStr().c_str());
            break;

        case TAC_MUL:
            printf("%s = %s * %s", i->a->ToStr().c_str(), i->b->ToStr().c_str(), i->c->ToStr().c_str());
            break;

        case TAC_DIV:
            printf("%s = %s / %s", i->a->ToStr().c_str(), i->b->ToStr().c_str(), i->c->ToStr().c_str());
            break;

        case TAC_EQ:
            printf("%s = (%s == %s)", i->a->ToStr().c_str(), i->b->ToStr().c_str(), i->c->ToStr().c_str());
            break;

        case TAC_NE:
            printf("%s = (%s != %s)", i->a->ToStr().c_str(), i->b->ToStr().c_str(), i->c->ToStr().c_str());
            break;

        case TAC_LT:
            printf("%s = (%s < %s)", i->a->ToStr().c_str(), i->b->ToStr().c_str(), i->c->ToStr().c_str());
            break;

        case TAC_LE:
            printf("%s = (%s <= %s)", i->a->ToStr().c_str(), i->b->ToStr().c_str(), i->c->ToStr().c_str());
            break;

        case TAC_GT:
            printf("%s = (%s > %s)", i->a->ToStr().c_str(), i->b->ToStr().c_str(), i->c->ToStr().c_str());
            break;

        case TAC_GE:
            printf("%s = (%s >= %s)", i->a->ToStr().c_str(), i->b->ToStr().c_str(), i->c->ToStr().c_str());
            break;

        case TAC_NEG:
            printf("%s = - %s", i->a->ToStr().c_str(), i->b->ToStr().c_str());
            break;

        case TAC_COPY:
            printf("%s = %s", i->a->ToStr().c_str(), i->b->ToStr().c_str());
            break;

        case TAC_GOTO:
            printf("goto %s", i->a->ToStr().c_str());
            break;

        case TAC_IFZ:
            printf("ifz %s goto %s", i->b->ToStr().c_str(), i->a->ToStr().c_str());
            break;

        case TAC_ACTUAL:
            printf("actual %s", i->a->ToStr().c_str());
            break;

        case TAC_FORMAL:
            printf("formal %s", i->a->ToStr().c_str());
            break;

        case TAC_CALL:
            printf("%s = call %s -> %s", i->a->ToStr().c_str(), i->b->ToStr().c_str(), i->c->ToStr().c_str());
            break;

        case TAC_RETURN:
            if (i->a != NULL)
                printf("return %s", i->a->ToStr().c_str());
            else printf("return");
            break;

        case TAC_LABEL:
            printf("label %s", i->a->ToStr().c_str());
            break;

        case TAC_VAR:
            printf("var %s", i->a->ToStr().c_str());
            break;

        case TAC_BEGINFUNC:
            printf("begin");
            break;

        case TAC_ENDFUNC:
            printf("end");
            break;

        case TAC_DECLARE_AND_LOCATE:
            printf("tmp %s = %s -> %s", i->a->ToStr().c_str(), i->b->ToStr().c_str(), i->c->ToStr().c_str());
            break;
        case TAC_TMP:
            printf("tmp %s", i->a->ToStr().c_str());
            break;

        default:
            error("unknown TAC opcode");
            break;
    }

    fflush(stdout);

}

void tac_dump() {
    TAC *cur;
    for (cur = tac_first; cur != NULL; cur = cur->next) {
        tac_print(cur);
        printf("\n");
    }
}

string mk_tmp() {

    char name[12];

    sprintf(name, "t%d", next_tmp++); /* Set up text */
    return name;
}


SYM *mk_const(int n) {
    return new SYM(SYM_CONST, n);
}

TAC *mk_tac(int op, SYM *a, SYM *b, SYM *c) {
    TAC *t = new TAC;

    t->next = NULL; /* Set these for safety */
    t->prev = NULL;
    t->op = op;
    t->a = a;
    t->b = b;
    t->c = c;
    t->linenum = getyylineno();

    return t;
}

EXP *mk_exp(EXP *next, SYM *ret, TAC *code) {
    EXP *exp = new EXP;

    exp->next = next;
    exp->ret = ret;
    exp->tac = code;

    return exp;
}

TAC *join_tac(TAC *c1, TAC *c2) {
    TAC *t;

    if (c1 == NULL) return c2;
    if (c2 == NULL) return c1;

    /* Run down c2, until we get to the beginning and then add c1 */
    t = c2;
    while (t->prev != NULL)
        t = t->prev;

    t->prev = c1;
    return c2;
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
    SYM *ret = new SYM(SYM_UNKNOWN, mk_tmp());
    auto code = mk_tac(TAC_DECLARE_AND_LOCATE, ret, sx, sy);
    x->ret = ret;
    code->prev = x->tac;
    x->tac = join_tac(code, y->tac);
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
    ret->tac = code;
    return ret;
}

EXP *do_call_ret(EXP *obj, const string &func, EXP *arglist) {

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
    };

    temp = join_tac(do_exp_list(lis)->tac, code);
    ret = new SYM(SYM_UNKNOWN, mk_tmp()); /* For the result */
    code = mk_tac(TAC_TMP, ret, NULL, NULL);
    code->prev = temp;
    temp = mk_tac(TAC_CALL, ret, obj->ret, new SYM(SYM_LINK, func));

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

void error(char *str) {
    fprintf(stderr, "error: %s\n", str);
    exit(1);
}
