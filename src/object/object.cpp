//
// Created by os on 5/3/22.
//
#include <algorithm>
#include "object.h"
#include "tac.hpp"
#include "regmgr/RegManager.cpp"

vector<AsmCode> _asm_codes;

inline void append(vector<AsmCode> codes) {
    for (auto &x: codes) {
        _asm_codes.push_back(x);
    }
}

inline void append(AsmCode code) {
    _asm_codes.push_back(code);
}


std::map<TAC_TYPE, AsmType> tac2asm{


        {TAC_ADD,  ASM_ADD},
        {TAC_SUB,  ASM_SUB},
        {TAC_MUL,  ASM_MUL},
        {TAC_DIV,  ASM_DIV},
        {TAC_EQ,   ASM_EQ},
        {TAC_NE,   ASM_NE},
        {TAC_LT,   ASM_LT},
        {TAC_GT,   ASM_GT},
        {TAC_NEG,  ASM_NEG},
        {TAC_COPY, ASM_MOV},
        {TAC_GE,   ASM_GE}
};

vector<AsmCode> generate_op(TAC_TYPE type, AsmOpValue a, AsmOpValue b, AsmOpValue c) {
    return {{tac2asm[type], a, b, c}};
}


RegManager regManager;


int pageVarSum = 0;

void begin_func(const TAC *tac) {
    regManager = RegManager();
    pageVarSum = 0;
    for (auto t = tac; t->op != TAC_ENDFUNC; t = t->next) {
        if (t->op == TAC_FORMAL || t->op == TAC_DECLARE || t->op == TAC_NEW) {
            pageVarSum++;
        }
    }
    append({{ASM_PUSH, {RA}},
            {ASM_PUSH, {FP}},
            {ASM_MOV,  {FP}, {SP}},
            {ASM_SUB,  {SP}, {SP}, {-pageVarSum * INSTRUCTION_WIDTH}}
           });

    int cnt = 0;
    for (const TAC *now = tac->next; now->op == TAC_FORMAL; now = now->next, cnt++) {
        regManager.formal(now->b->ToStr(), cnt);
    }
}

void end_func(const TAC *tac) {
}

void declare_op(const TAC *tac) {

    return regManager.declare(tac->b->ToStr());
}

void func_call(const TAC *tac) {
    regManager.saveCaller();

    vector<SYM *> actuals;
    for (auto x = tac->prev; x->op == TAC_ACTUAL; x = x->prev) {
        actuals.push_back(x->a);
    }
    std::reverse(actuals.begin(), actuals.end());
    assert(actuals.size());
    regManager.loadArgs(actuals);
    append({ASM_JR, {MEM_FUNC_PREFIX + tac->b->ToStr() + "_" + tac->c->ToStr()}});
    auto r = regManager.GetOpValue(tac->a);

    append({ASM_MOV, r, {CallerSaved, 0}});

}


void func_ret(const TAC *tac) {
    regManager.ret(tac->a);
    append({ASM_ADD, {SP}, {SP}, {pageVarSum * INSTRUCTION_WIDTH}});
    append({ASM_POPR, {FP}});
    append({ASM_POPR, {RA}});

    append(AsmCode(ASM_RET));
}

void func_locate(const TAC *tac) {
    regManager.saveCaller();
    regManager.loadArgs({ tac->b, tac->c});
    append({ASM_JR, LOCATE_FUNC});

    auto r = regManager.GetAssignValue(tac->a);

    append({ASM_MOV, r, {CallerSaved, 0}});
}

void func_bind(const TAC *tac) {
    regManager.saveCaller();
    regManager.loadArgs({tac->a, tac->b, tac->c});
    append({ASM_JR, BIND_FUNC});
}

void func_new(const TAC *tac) {
    declare_op(tac);
    regManager.saveCaller();
    regManager.loadArgs({tac->a, tac->b});
    append({ASM_JR, NEW_FUNC});

    auto r = regManager.GetAssignValue(tac->b);

    append({ASM_MOV, r, {CallerSaved, 0}});
}

vector<AsmCode> object_generate(const TAC *tac) {
    regManager.addClock();
    _asm_codes.clear();
    switch (tac->op) {
        case TAC_BEGINFUNC:
            begin_func(tac);
            break;
        case TAC_DECLARE:
            declare_op(tac);
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
        case TAC_NEG:
        case TAC_COPY:
        case TAC_GE : {

            auto a = regManager.GetAssignValue(tac->a);
            auto b = regManager.GetOpValue(tac->b);
            auto c = regManager.GetOpValue(tac->c);
            append(generate_op(tac->op, a, b, c));

        }
            break;

        case TAC_GOTO:
            return {{ASM_JUMP, {tac->a->ToStr()}}};
            break;
        case TAC_IFZ: {

            auto r = regManager.GetOpValue(tac->b);
            append({ASM_BNZ, r, {tac->a->ToStr()}});
            break;
        }
        case TAC_ENDFUNC:
            end_func(tac);
            break;
        case TAC_LABEL:
            append({{ASM_LABEL, {tac->a->ToStr()}}});
            break;
        case TAC_CALL:
            func_call(tac);
            break;
        case TAC_RETURN:
            func_ret(tac);
            break;
        case TAC_LOCATE:
            func_locate(tac);
            break;
        case TAC_BEGINBLOCK:
            break;
        case TAC_ENDBLOCK:
            break;
        case TAC_NEW:
            func_new(tac);
            break;
        case TAC_BIND:
            func_bind(tac);
            break;
        case TAC_UNDEF:
            break;
        case TAC_FORMAL:
            break;
        case TAC_ACTUAL:
            break;
        case TAC_BEGINCLASS:
            break;
        case TAC_ENDCLASS:
            break;
        default:
            assert(false);
            break;
    }
    return _asm_codes;
}

void tac_object_generate(TAC *tac) {
    int lstline = -1;
    setlineno(1);

    for (const TAC *now = tac; now; now = now->next) {
        if (now->linenum > 0) {
            setlineno(now->linenum);
        }
        //if (lstline != getlineno())
        //  printf("line:%d\n", lstline = getlineno());
        tac_print((TAC *) now);
        puts("");
        auto asms = object_generate(now);
        for (auto &c: asms) {
            printf("\t%s;\n", c.Dump().c_str());
        }
    }
}