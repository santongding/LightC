//
// Created by os on 4/30/22.
//

#include "TypeManager.h"
#include "Checker.h"
#include "string"
#include "linker.hpp"

TypeManager typeManager;

struct sym_info {
    string name;
    SYM *type_sym;

    bool operator<(const sym_info &p) const {
        return name < p.name;
    }

};

void CheckTac(const TAC *tac) {
    setlineno(1);

    typeManager.Init();
    for (const TAC *now = tac; now; now = now->next) {
        if (now->linenum > 0) {
            setlineno(now->linenum);
        }
        if (now->op == TAC_BEGINCLASS) {
            CheckStatus(typeManager.DeclareClass(now->a->ToStr()));
        }
    }
    {

        int scope = 0;//1 for class 2 for func
        string lstClass;
        setlineno(1);
        for (const TAC *now = tac; now; now = now->next) {
            if (now->linenum > 0) {
                setlineno(now->linenum);
            }
            if (now->op == TAC_BEGINCLASS) {
                assert(scope == 0);
                scope = 1;
                lstClass = now->a->ToStr();
            }
            if (now->op == TAC_BEGINFUNC) {
                assert(scope == 1);
                scope = 2;
                typeManager.DeclareFunc(lstClass, now->b->ToStr(), now);
            }
            if (now->op == TAC_DECLARE) {
                if (scope == 1)
                    typeManager.DeclareMember(lstClass, now->b->ToStr(), now->a->ToStr());
            }
            if (now->op == TAC_ENDFUNC) {
                assert(scope == 2);
                scope = 1;
            }
            if (now->op == TAC_ENDCLASS) {
                assert(scope == 1);
                scope = 0;
                lstClass = "";
            }


        }
    }
    typeManager.Print();


    dumpBuiltin(&typeManager, BUILTIN_SOURCE_NAME);

    int scope = 0;
    setlineno(1);
    int lstRetScope = 0;

    std::vector<std::set<sym_info>> sym_tables;

    TypeInfo curRetType(INVALID_V);

    auto in_sym_tables = [&sym_tables](const string &s) {
        for (auto &t: sym_tables) {
            if (t.find({s,}) != t.end())return true;
        }
        return false;
    };

    std::function<SYM *(SYM *)> get_type_sym = [&sym_tables](SYM *s) {

        if (s->IsConst()) {
            return new SYM(SYM_TYPE, "int|");
        }
        for (auto &t: sym_tables) {
            auto tp = t.find({s->ToStr(),});
            if (tp != t.end())return tp->type_sym;
        }
        CheckStatus(SYMBOL_UNDEFINED);
    };

    for (TAC *now = const_cast<TAC *>(tac); now; now = now->next) {
        if (now->linenum > 0) {
            setlineno(now->linenum);
        }
        switch (now->op) {
            case TAC_BEGINCLASS: {

                scope++;
            }
                break;
            case TAC_ENDCLASS: {
                scope--;

            }
                break;

            case TAC_NEW:
            case TAC_DECLARE:
            case TAC_FORMAL: {
                if (scope <= 1) {
                    now->prev->next = now->next;
                    now->next->prev = now->prev;
                    break;
                }

                auto name = now->b->ToStr();
                if (in_sym_tables(name)) {
                    CheckStatus(SYMBOL_REPEAT);
                }
                TypeInfo ti;
                CheckStatus(typeManager.TryDecodeType(now->a->ToStr(), ti));
                sym_tables.back().insert({name, now->a});

                if (now->op == TAC_NEW) {
                    auto s = now->a->ToStr();
                    auto pos = s.find_first_of("|");
                    s = s.substr(pos + 1);
                    now->a = new SYM(SYM_CONST, typeManager.Str2ID(s));
                }

            }
                break;
            case TAC_BEGINFUNC:
            case TAC_BEGINBLOCK: {
                if (scope == 1) {
                    lstRetScope = 0;
                    assert(curRetType.Is<INVALID_V>());
                    typeManager.TryDecodeType(now->a->ToStr(), curRetType);

                }
                assert(curRetType.Is<INVALID_V>() == false);
                assert(scope >= 1);
                scope++;
                sym_tables.emplace_back();
            }
                break;
            case TAC_ENDFUNC:
            case TAC_ENDBLOCK: {

                scope--;
                assert(scope >= 1);
                sym_tables.pop_back();
                assert(curRetType.Is<INVALID_V>() == false);
                if (scope == 1) {
                    curRetType = TypeInfo(INVALID_V);
                    if (lstRetScope != 3) {
                        CheckStatus(FUN_NOT_RETURN);
                    }
                }
            }
                break;
            case TAC_BIND:
            case TAC_LOCATE:
            case TAC_CALL: {

                auto nx = now->a->ToStr();
                auto ny = now->b->ToStr();
                if (!in_sym_tables(nx) || !in_sym_tables(ny)) {
                    CheckStatus(SYMBOL_UNDEFINED);
                }
                typeManager.CheckTac(get_type_sym(now->a), get_type_sym(now->b), const_cast<TAC *>(now), get_type_sym);

                if (now->op == TAC_CALL) {// set to func name
                    auto s = get_type_sym(now->b)->ToStr();
                    auto pos = s.find_first_of("|");
                    s = s.substr(pos + 1);
                    now->b = new SYM(SYM_SYMBOL, s);
                } else {
                    now->c = new SYM(SYM_CONST, typeManager.Str2ID(now->c->ToStr()));
                }
            }
                break;
            case TAC_COPY:
            case TAC_NEG: {
                typeManager.CastType(get_type_sym(now->a), get_type_sym(now->b), now->op == TAC_NEG);
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
                if (now->b->IsConst()) {
                    assert(!now->c->IsConst());
                    std::swap(now->b, now->c);
                }
                typeManager.CastType(get_type_sym(now->b), get_type_sym(now->c), true);
                typeManager.CastType(get_type_sym(now->a), get_type_sym(now->b), true);
            }
                break;
            case TAC_RETURN: {
                assert(scope >= 2);
                assert(curRetType.Is<INVALID_V>() == false);
                auto type = get_type_sym(now->a);
                typeManager.CastType(type, curRetType);
                lstRetScope = scope;
                if (curRetType.Is<INT_V>()) {
                    now->b = new SYM(SYM_CONST, 0);
                } else {
                    now->b = new SYM(SYM_CONST, 1);
                }
            }
                break;
            case TAC_ACTUAL:
            case TAC_LABEL:
            case TAC_IFZ:
                break;
            default:
                assert(false);
                break;
        }
    }
    typeManager.Print();
}