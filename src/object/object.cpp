//
// Created by os on 5/3/22.
//
#include "object.h"
#include "tac.hpp"
#include "TypeManager.h"

template<class T>
void append(vector<T> &t, vector<T> x) {
    t.push_back(x.begin(), x.end());
}

template<class T>
void append(vector<T> &t, T x) {
    t.push_back(x);
}


const int arg_reg_num = Get_ARG_REG_NAME().size();

pair<AsmOpValue, vector<AsmCode>> get_val(SYM *sym) {
    if (sym == nullptr) {
        return {AsmOpValue(None), {}};
    } else if (sym->IsConst()) {
        return {AsmOpValue(Imm, sym->GetValue()), {}};
    } else {

    }

}

vector<AsmCode> generate_op(TAC_TYPE type) {

}


class Context {
public:
    Context(){



    }
    AsmCode formal(const string name, int num) {
        assert(name != "" && exist(name) == false);
        if (!num < arg_reg_num) {
            CheckStatus(PARAMS_NUM_OVERFLOW);
        }
        declaredNum++;
        varPos[name] = {FP, -(declaredNum) * INSTRUCTION_WIDTH};
        stk.push_back(name);
        return {PUSH, {Args, num}};
    }

    AsmCode declare(const string name) {
        assert(name != "" && exist(name) == false);
        declaredNum++;
        varPos[name] = {AsmOpValue(FP), -(declaredNum) * INSTRUCTION_WIDTH};
        stk.push_back(name);
        return {PUSHI, {Imm, 0}};
    }

    pair<vector<AsmCode>, AsmOpValue> GetOpValue(SYM *sym) {
        if (sym->IsConst()) {
            return {{},
                    {sym->GetValue()}};
        } else {
            auto name = sym->ToStr();
        }
    }

    void beginBlock() {
        stk.push_back("");
    }

    vector<AsmCode> endBlock() {
        vector<AsmCode> asms;
        while (stk.back() != "") {
            asms.push_back({POP});
            stk.pop_back();
        }
        stk.pop_back();
        return asms;
    }

    int declaredNum = 0;
private:
    bool exist(const string &n) {
        return varPos.find(n) != varPos.end();
    }

    class regInfo {
    public:
        AsmOpValue value;
        string curVar;//"" for no use;
        int lstTs;
        bool isModified;
    };

    vector<string> stk;
    std::map<string, pair<AsmOpValue, AsmOpValue>> varPos;
};

Context ctx;

void object_init() {
    ctx = Context();
}


vector<AsmCode> begin_func(const TAC *tac) {
    vector<AsmCode> ans = {{PUSH, {RA}},
                           {PUSH, {FP}},
                           {MOV,  {FP, SP}}};


    ctx.beginBlock();
    int cnt = 0;
    for (const TAC *now = tac->next; now->op == TAC_FORMAL; now++, cnt++) {
        append(ans, ctx.formal(now->a->ToStr(), cnt));
    }
    return ans;
}

vector<AsmCode> end_func(const TAC *tac) {
    auto ans = ctx.endBlock();
    assert(ctx.declaredNum == 0);
    return ans;
}

vector<AsmCode> declare_op(const TAC *tac) {

    return {{}};
}

vector<AsmCode> object_generate(const TAC *tac) {
    switch (tac->op) {
        case TAC_BEGINFUNC:
            return begin_func(tac);
            break;
        case TAC_DECLARE:
            return declare_op(tac);
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
            return generate_op(tac->op);
        }
    }
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