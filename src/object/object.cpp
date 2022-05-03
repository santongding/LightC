//
// Created by os on 5/3/22.
//
#include "object.h"
#include "tac.hpp"

int arg_reg_num = Get_ARG_REG_NAME().size();
int caller_reg_num = Get_Caller_REG_NAME().size();
int callee_reg_nuym = Get_Callee_REG_NAME().size();
vector<AsmCode> object_generate(const TAC *tac) {
    return {};
}

void object_init(){

}

void tac_object_generate(TAC *tac) {
    int lstline = -1;
    setlineno(1);

    for (const TAC *now = tac; now; now = now->next) {
        if (now->linenum > 0) {
            setlineno(now->linenum);
        }
        if (lstline != getlineno())
            printf("line:%d\n", lstline = getlineno());
        tac_print((TAC *) now);
        for (auto &c: object_generate(now)) {
            printf("%s;\n", c.Dump().c_str());
        }
    }
}