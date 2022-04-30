//
// Created by os on 4/30/22.
//

#include "TypeManager.h"
#include "Checker.h"
#include "string"

TypeManager typeManager;

void CheckTac(const TAC *tac) {
    setyylineno(1);
    int scope = 0;//0 for global, 1 for class, 2 for func

    typeManager.Init();
    string lstClass;
    for (const TAC *now = tac; now; now = now->next) {
        if (now->linenum > 0) {
            setyylineno(now->linenum);
        }
        if (now->op == TAC_BEGINCLASS) {
            assert(scope == 0);
            scope = 1;
            typeManager.DeclareClass(lstClass = now->a->ToStr());

        } else if (now->op == TAC_ENDCLASS) {
            assert(scope == 1);
            scope = 0;
        } else if (now->op == TAC_BEGINFUNC) {
            assert(scope == 1);
            scope = 2;
        } else if (now->op == TAC_ENDFUNC) {
            assert(scope == 2);
            scope = 1;
        } else if (now->op == TAC_DECLARE) {
            if (scope == 1) {
                auto sts = typeManager.DeclareMember(lstClass, now->b->ToStr(), now->a->ToStr());
                if (sts != OK) {
                    error("type conficct: " + std::to_string(sts));
                }
            }
        }
    }
    typeManager.Print();
}