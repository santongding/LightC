//
// Created by os on 4/30/22.
//
#include "tac.hpp"

int next_tmp = 0;
TAC *tac_last, *tac_first;

void tac_init() {
    next_tmp = 0;
    next_label = 0;
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
    if (i->linenum < 0) {
        if (lastline < 0)i->linenum = 1;
        else
            i->linenum = lastline;
    }
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
            printf("formal %s %s", i->a->ToStr().c_str(), i->b->ToStr().c_str());
            break;

        case TAC_CALL:
            printf("%s = call %s -> %s", i->a->ToStr().c_str(), i->b->ToStr().c_str(), i->c->ToStr().c_str());
            break;

        case TAC_RETURN:
           // if (i->a != NULL)
                printf("return %s", i->a->ToStr().c_str());
           // else printf("return");
            break;

        case TAC_LABEL:
            printf("label %s", i->a->ToStr().c_str());
            break;

        case TAC_DECLARE:
            printf("declare %s %s", i->a->ToStr().c_str(), i->b->ToStr().c_str());
            break;

        case TAC_BEGINFUNC:
            printf("begin func.cpp: %s %s", i->a->ToStr().c_str(), i->b->ToStr().c_str());
            break;

        case TAC_ENDFUNC:
            printf("end func.cpp");
            break;

        case TAC_BEGINCLASS:
            printf("begin class: %s", i->a->ToStr().c_str());
            break;

        case TAC_ENDCLASS:
            printf("end class");
            break;
        case TAC_LOCATE:
            printf("%s = %s -> %s", i->a->ToStr().c_str(), i->b->ToStr().c_str(), i->c->ToStr().c_str());
            break;
        case TAC_BEGINBLOCK:
            printf("block begin:");
            break;
        case TAC_ENDBLOCK:
            printf("block end");
            break;
        default:

            error(("unknown TAC opcode: " + std::to_string(i->op)).c_str());
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

TAC *mk_tac(int op, SYM *a, SYM *b, SYM *c, bool lineno) {
    TAC *t = new TAC;

    t->next = NULL; /* Set these for safety */
    t->prev = NULL;
    t->op = op;
    t->a = a;
    t->b = b;
    t->c = c;
    if (lineno)
        t->linenum = getyylineno();
    else t->linenum = -1;

    return t;
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
