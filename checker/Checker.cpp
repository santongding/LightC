//
// Created by os on 4/30/22.
//

#include "TypeManager.h"
#include "Checker.h"
#include "string"
#include "unordered_set"

TypeManager typeManager;

struct sym_info {
    string name;
    string type_string;

    bool operator<(const sym_info &p) const {
        return name < p.name;
    }

};

void CheckTac(const TAC *tac) {
    setyylineno(1);

    typeManager.Init();
    for (const TAC *now = tac; now; now = now->next) {
        if (now->linenum > 0) {
            setyylineno(now->linenum);
        }
        if (now->op == TAC_BEGINCLASS) {
            CheckStatus(typeManager.DeclareClass(now->a->ToStr()));
        }
    }


    int scope = 0;//0 for global, 1 for class, 2 for func,3 and bigger for block
    string lstClass;
    setyylineno(1);

    std::vector<std::set<sym_info>> sym_tables;

    auto in_sym_tables = [&sym_tables](const string &s) {
        for (auto &t: sym_tables) {
            if (t.find({s,}) != t.end())return true;
        }
        return false;
    };

    for (const TAC *now = tac; now; now = now->next) {
        if (now->linenum > 0) {
            setyylineno(now->linenum);
        }
        switch (now->op) {
            case TAC_BEGINCLASS : {
                assert(scope == 0);
                scope = 1;
                lstClass = now->a->ToStr();
            }
                break;
            case TAC_ENDCLASS: {
                assert(scope == 1);
                lstClass = "";
                scope = 0;
            }
                break;
            case TAC_BEGINFUNC : {
                assert(scope == 1);
                assert(sym_tables.size() == 0);
                sym_tables.push_back(std::set<sym_info>());
                scope = 2;
                CheckStatus(typeManager.DeclareFunc(lstClass, now->b->ToStr(), now));
            }
                break;
            case TAC_ENDFUNC: {
                assert(scope == 2);
                sym_tables.pop_back();
                scope = 1;
            }
                break;
            case TAC_DECLARE:
            case TAC_FORMAL: {
                if (scope == 1) {
                    assert(now->op == TAC_DECLARE);
                    CheckStatus(typeManager.DeclareMember(lstClass, now->b->ToStr(), now->a->ToStr()));
                } else {
                    auto name = now->b->ToStr();
                    if (in_sym_tables(name)) {
                        CheckStatus(SYMBOL_REPEAT);
                    }
                    sym_tables.back().insert({name, now->a->ToStr()});
                }
            }
                break;
            case TAC_BEGINBLOCK: {
                assert(scope >= 2);
                scope++;
                sym_tables.push_back(std::set<sym_info>());
            }
                break;
            case TAC_ENDBLOCK: {
                scope--;
                assert(scope >= 2);
                sym_tables.pop_back();
            }
                break;
            case TAC_LOCATE:
            case TAC_CALL: {

                auto nx = now->a->ToStr();
                auto ny = now->b->ToStr();
                if (!in_sym_tables(nx) || !in_sym_tables(ny)) {
                    CheckStatus(SYMBOL_UNDEFINED);
                }

            }
                break;
            case TAC_COPY:
            case TAC_NEG: {
                auto nx = now->a->ToStr();
                auto ny = now->b->ToStr();
                if (!in_sym_tables(nx) || !in_sym_tables(ny)) {
                    CheckStatus(SYMBOL_UNDEFINED);
                }
            }
                break;
            case TAC_ADD:
            case TAC_SUB:
            case TAC_MUL:
            case TAC_DIV:
            case TAC_EQ:
            case TAC_NE:
            case TAC_LT:
            case TAC_LE:
            case TAC_GT:
            case TAC_GE : {
                auto nx = now->a->ToStr();
                auto ny = now->b->ToStr();
                auto nz = now->c->ToStr();

                if (!in_sym_tables(nx) || !in_sym_tables(ny) || !in_sym_tables(nz)) {
                    CheckStatus(SYMBOL_UNDEFINED);
                }
            }
                break;

            default:
                break;
        }
    }
    typeManager.Print();
}