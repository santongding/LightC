//
// Created by os on 4/30/22.
//
#include "tac.hpp"

TAC *mk_func(SYM *type, SYM *name, TAC *tac, TAC *block) {
    tac = join_tac(mk_tac(TAC_FORMAL, new SYM(SYM_TYPE, "ref|"), new SYM(SYM_SYMBOL, "this"), NULL, false), tac);
    assert(block->op == TAC_ENDBLOCK);
    auto bg = block;
    while (bg->prev->op != TAC_BEGINBLOCK)bg = bg->prev;
    auto code = mk_tac(TAC_BEGINFUNC, type, name, nullptr, false);
    assert(bg->prev->prev == nullptr);
    tac = join_tac(bg->prev, tac);
    tac = join_tac(code, tac);
    bg->prev = tac;


    code = block;
    return join_tac(code, mk_tac(TAC_ENDFUNC, nullptr, nullptr, nullptr, false));
}

TAC *mk_class(SYM *name, TAC *tac) {
    auto code = join_tac(mk_tac(TAC_BEGINCLASS, name, nullptr, nullptr, false), tac);

    return join_tac(code, mk_tac(TAC_ENDCLASS, nullptr, nullptr, nullptr, false));

}