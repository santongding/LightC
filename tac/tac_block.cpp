//
// Created by os on 4/30/22.
//
#include "tac.hpp"

int next_label = 0;

TAC *mk_block(TAC *x) {
    auto code = mk_tac(TAC_BEGINBLOCK, nullptr, nullptr, nullptr, false);
    code = join_tac(code, x);
    auto tp = mk_tac(TAC_ENDBLOCK, nullptr, nullptr, nullptr, false);
    return join_tac(code, tp);
}

TAC *do_test(EXP *exp, TAC *stmt1, TAC *stmt2) {
    if (stmt1 == nullptr && stmt2 == nullptr) {
        return exp->tac;
    }
    if (stmt2) {

        TAC *label1 = NULL;
        label1 = mk_tac(TAC_LABEL, new SYM(SYM_LABEL, "label" + std::to_string(next_label++)), NULL, NULL, false);
        TAC *label2 = NULL;
        label2 = mk_tac(TAC_LABEL, new SYM(SYM_LABEL, "label" + std::to_string(next_label++)), NULL, NULL, false);
        TAC *code1 = mk_tac(TAC_IFZ, label1->a, exp->ret, NULL, false);
        TAC *code2 = mk_tac(TAC_GOTO, label2->a, NULL, NULL, false);

        code1->prev = exp->tac; /* Join the code */
        code1 = join_tac(code1, stmt1);
        code2->prev = code1;
        label1->prev = code2;
        label1 = join_tac(label1, stmt2);
        label2->prev = label1;
        return label2;
    } else {
        TAC *label1 = NULL;
        label1 = mk_tac(TAC_LABEL, new SYM(SYM_LABEL, "label" + std::to_string(next_label++)), NULL, NULL, false);
        TAC *code1 = mk_tac(TAC_IFZ, label1->a, exp->ret, NULL, false);

        code1->prev = exp->tac; /* Join the code */
        code1 = join_tac(code1, stmt1);
        label1->prev = code1;
        label1 = join_tac(label1, stmt2);
        return label1;
    }

}

TAC *do_while(EXP *exp, TAC *stmt) {
    TAC *label = mk_tac(TAC_LABEL, new SYM(SYM_LABEL, "label" + std::to_string(next_label++)), NULL, NULL, false);
    TAC *code = mk_tac(TAC_GOTO, label->a, NULL, NULL, false);

    code->prev = stmt; /* Bolt on the goto */

    return join_tac(label, do_test(exp, code, nullptr));
}

TAC *declare(SYM *type, SYM *name) {
    return mk_tac(TAC_DECLARE, type, name, NULL, true);
}
