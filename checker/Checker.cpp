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
    SYM *type_sym;

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
    {

        int scope = 0;//1 for class 2 for func
        string lstClass;
        setyylineno(1);
        for (const TAC *now = tac; now; now = now->next) {
            if (now->linenum > 0) {
                setyylineno(now->linenum);
            }
            if (now->op == TAC_BEGINCLASS) {
                assert(scope == 0);
                scope = 1;
                lstClass = now->a->ToStr();
            }
            if (now->op == TAC_BEGINFUNC) {
                assert(scope == 1);
                scope = 2;
                CheckStatus(typeManager.DeclareFunc(lstClass, now->b->ToStr(), now));
            }
            if (now->op == TAC_DECLARE) {
                if (scope == 1)
                    CheckStatus(typeManager.DeclareMember(lstClass, now->b->ToStr(), now->a->ToStr()));
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

    int scope = 0;
    setyylineno(1);

    std::vector<std::set<sym_info>> sym_tables;

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

    for (const TAC *now = tac; now; now = now->next) {
        if (now->linenum > 0) {
            setyylineno(now->linenum);
        }
        switch (now->op) {
            case TAC_BEGINCLASS:
            case TAC_BEGINFUNC:
                scope++;
                break;
            case TAC_ENDCLASS:
            case TAC_ENDFUNC:
                scope--;
                break;

            case TAC_NEW:
            case TAC_DECLARE:
            case TAC_FORMAL: {
                if (scope <= 2)break;

                auto name = now->b->ToStr();
                if (in_sym_tables(name)) {
                    CheckStatus(SYMBOL_REPEAT);
                }
                TypeInfo ti;
                CheckStatus(typeManager.TryDecodeType(now->a->ToStr(), ti));
                sym_tables.back().insert({name, now->a});

            }
                break;
            case TAC_BEGINBLOCK: {
                assert(scope >= 2);
                scope++;
                sym_tables.emplace_back();
            }
                break;
            case TAC_ENDBLOCK: {
                scope--;
                assert(scope >= 1);
                sym_tables.pop_back();
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

            }
                break;
            case TAC_COPY:
            case TAC_NEG: {
                /*auto nx = now->a->ToStr();
                auto ny = now->b->ToStr();
                if (!in_sym_tables(nx) || !in_sym_tables(ny)) {
                    CheckStatus(SYMBOL_UNDEFINED);
                }*/
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
                /*auto nx = now->a->ToStr();
                auto ny = now->b->ToStr();
                auto nz = now->c->ToStr();

                if (!in_sym_tables(nx) || !in_sym_tables(ny) || !in_sym_tables(nz)) {
                    CheckStatus(SYMBOL_UNDEFINED);
                }*/
                typeManager.CastType(get_type_sym(now->b), get_type_sym(now->c), true);
                typeManager.CastType(get_type_sym(now->a), get_type_sym(now->b), true);
            }
                break;

            default:
                break;
        }
    }
    typeManager.Print();
}