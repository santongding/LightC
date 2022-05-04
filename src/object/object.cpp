//
// Created by os on 5/3/22.
//
#include <algorithm>
#include "object.h"
#include "tac.hpp"
#include "TypeManager.h"

template<class T>
void append(vector<T> &t, vector<T> x) {
    for (auto &v: x) {
        t.push_back(v);
    }
}

template<class T>
void append(vector<T> &t, T x) {
    t.push_back(x);
}


const int arg_reg_num = Get_ARG_REG_NAME().size();
const int callee_reg_num = Get_Callee_REG_NAME().size();
const int caller_reg_num = Get_Caller_REG_NAME().size();


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

enum VarPos {
    ORIGIN,
    ARGS,
    CALLEE,
    CALLER,
    STK,
};
std::map<AsmOpValueType, VarPos> type2Pos = {
        {Args,        ARGS},
        {CalleeSaved, CALLEE},
        {CallerSaved, CALLER}
};

class Context {
private:


    class varInfo {
    public:


        varInfo(VarPos p, AsmOpValue v, bool b, int i) : pos(p), value(v), needWriteBack(b), lstTs(i) {

        }

        int getStkOffset() const {
            assert(declareInStk());
            return stkPos.second.value;
        }

        bool declareInStk() const {
            return stkPos.first.type == FP;
        }

        bool persistInStk() const {
            if (pos == STK) {
                assert(value.type == None);
                assert(declareInStk());
                return true;
            } else {
                return false;
            }
        }

        bool isOrigin() const {
            return pos == ORIGIN;
        }

        bool needWrite() const {
            return needWriteBack;
        }


        AsmOpValue getReg(int clk, bool isAssign) {
            assert(persistInStk() == false);
            needWriteBack = isAssign;
            lstTs = clk;
            if (pos == ORIGIN) {
                assert(isAssign == false);
            }
            return value;
        }


        vector<AsmCode> release(Context *ctx) {
            switch (pos) {
                case ORIGIN:
                    return {};
                    break;
                case ARGS:
                case CALLER:
                    if (pos == ARGS) {
                        ctx->caller_vars[value.value.];
                    } else {
                        ctx->caller_vars[value.value] = "";
                    }
                    if (!declareInStk()) {
                        return {};
                    } else {
                        assert(stkPos.second.value == ctx->pageVarNum);
                        ctx->pageVarNum--;
                        return {{POP}};
                    }
                    break;
                case CALLEE:
                    assert(stkPos.second.value == ctx->pageVarNum);
                    ctx->pageVarNum--;
                    ctx->callee_vars[value.value] = "";
                    return {{POPR, value}};
                    break;
                case STK:
                    assert(stkPos.second.value == ctx->pageVarNum);
                    ctx->pageVarNum--;
                    return {{POP}};
                    break;
                default:
                    assert(false);
            }
            return {};
        }

        vector<AsmCode> persist(Context *ctx) {
            assert(needWriteBack);
            vector<AsmCode> ans;
            switch (pos) {
                case ARGS:
                case CALLER:
                    if (!declareInStk()) {
                        stkPos = {{FP},
                                  {++ctx->pageVarNum * INSTRUCTION_WIDTH}};
                        assert(needWriteBack);
                        ans.push_back({PUSH, value});

                    } else {
                        ans.push_back({STORE, value});
                        return ans;
                    }
                    break;
                default:
                    assert(false);
                    break;
            }
            value = None;
            pos = STK;
            return ans;

        }

        vector<AsmCode> toReg(AsmOpValue &reg, Context *ctx) {
            assert(reg.type != value.type);
            vector<AsmCode> ans;
            assert(pos == STK || pos == ORIGIN);

            switch (reg.type) {
                case CalleeSaved:
                    if (!declareInStk()) {

                        assert(pos == ORIGIN);
                        stkPos = {{FP},
                                  {++ctx->pageVarNum * INSTRUCTION_WIDTH}};
                        ans.push_back({PUSH, value});
                        ans.push_back({MOVI, reg, 0});
                    } else {
                        assert(false);
                    }
                    break;

                case Args:
                case CallerSaved:
                    if (!declareInStk()) {
                        assert(pos == ORIGIN);
                        ans.push_back({MOVI, reg, 0});
                    } else {
                        assert(pos != ORIGIN);
                        ans.push_back({LOAD, reg, stkPos.first, stkPos.second});
                    }
                    break;
                default:
                    assert(false);
                    break;
            }
            pos = type2Pos[reg.type];
            value = reg;
            assert(persistInStk() == false);
            return ans;
        }

    private:

        VarPos pos;
        AsmOpValue value;
        bool needWriteBack;
        int lstTs;
        pair<AsmOpValue, AsmOpValue> stkPos;
    };

public:
    Context() :callee_vars(callee_reg_num), caller_vars(caller_reg_num+arg_reg_num) {
    }

    vector<AsmCode> formal(const string name, int num) {
        assert(name != "" && exist(name) == false);
        if (num >= arg_reg_num) {
            CheckStatus(PARAMS_NUM_OVERFLOW);
        }
        assert(caller_vars[num] == "");
        caller_vars[num] = name;
        stk.push_back(name);
        vars[name] = varInfo{ARGS, {Args, num},
                             true, clock};
        return {};
    }

    vector<AsmCode> declare(const string &name) {
        assert(name != "" && exist(name) == false);
        vars[name] = varInfo{ORIGIN, {Zero},
                             false, clock};
        stk.push_back(name);
        return {};
    }

    pair<vector<AsmCode>, AsmOpValue> GetOpValue(SYM *sym) {
        assert(exist(sym->ToStr()));
        if (sym->IsConst()) {
            return {{},
                    {sym->GetValue()}};
        } else {
            return getReg(sym->ToStr(), false);
        }
    }

    pair<vector<AsmCode>, AsmOpValue> GetAssignValue(SYM *sym) {
        assert(!sym->IsConst());

        auto name = sym->ToStr();
        return getReg(name, true);

    }

    void beginBlock() {
        stk.push_back("");
    }


    vector<AsmCode> endBlock() {
        vector<AsmCode> asms;

        vector<string> releaseOrder;
        while (stk.back() != "") {
            if (!vars[stk.back()].declareInStk()) {
                append(asms, releaseVar(stk.back()));
            } else {
                releaseOrder.push_back(stk.back());
            }
            stk.pop_back();
        }
        stk.pop_back();
        std::sort(releaseOrder.begin(), releaseOrder.end(), [&](string &x, string &y) {
            return vars[x].getStkOffset() < vars[y].getStkOffset();
        });
        for (auto &s: releaseOrder) {
            append(asms, releaseVar(s));
        }
        return asms;
    }

    void addClock() {
        clock++;
    }

    int pageVarNum = 0;
private:
    pair<vector<AsmCode>, AsmOpValue> getReg(const string &name, bool isAssign) {
        auto &info = vars[name];
        if ((info.isOrigin() && isAssign) || info.persistInStk()) {
            auto ans = emptyReg(name);
            append(ans.first, info.toReg(ans.second, this));
            return ans;
        } else {
            return {{}, info.getReg(clock, isAssign)};
        }
    }

    pair<vector<AsmCode>, AsmOpValue> emptyReg(const string &name) {
        bool isTemp = 0 == name.compare(0, TEMP_VALUE_PREFIX.length(), TEMP_VALUE_PREFIX);

        if (!isTemp) {
            for (int i = 0; i < callee_vars.size(); i++) {
                if (callee_vars[i].empty()) {
                    callee_vars[i] = name;
                    return {{},
                            {CalleeSaved, i}};
                }

            }
        }
        for(int i=0;i<caller_vars.size();i++){
            if (caller_vars[i].empty()) {
                caller_vars[i] = name;
                return {{},
                        {CallerSaved, i}};
            }
        }

        vector<int>id(caller_vars.size())
    }

    vector<AsmCode> releaseVar(const string &name) {
        assert(exist(name));
        auto info = vars[name];
        vars.erase(name);
        return info.release(this);
    }


    bool exist(const string &n) {
        return vars.find(n) != vars.end();
    }

    int clock = 0;

    vector<string> callee_vars;
    vector<string> caller_vars;

    vector<string> stk;

    std::map<string, varInfo> vars;

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
    assert(ctx.pageVarNum == 0);
    return ans;
}

vector<AsmCode> declare_op(const TAC *tac) {

    return {{}};
}

vector<AsmCode> object_generate(const TAC *tac) {
    ctx.addClock();
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