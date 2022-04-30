//
// Created by os on 4/30/22.
//
#include "tac.hpp"

TAC *mk_func(SYM *type, const string &name, TAC *tac,TAC *block) {
    tac = join_tac(mk_tac(TAC_FORMAL,new SYM(SYM_TYPE,"ref|"),new SYM(SYM_SYMBOL,"this"),NULL,false),tac);
    auto code = join_tac(mk_tac(TAC_BEGINFUNC, type, new SYM(SYM_SYMBOL, name), nullptr, false), tac);

    code = join_tac(code,block);
    return join_tac(code, mk_tac(TAC_ENDFUNC, nullptr, nullptr, nullptr, false));
}