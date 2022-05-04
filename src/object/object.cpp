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


const int arg_reg_num = Args_Reg_Num;
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

enum VarPos {
    ORIGIN,
    CALLEE,
    CALLER,
    STK,
};
std::map<AsmOpValueType, VarPos> type2Pos = {
        {CalleeSaved, CALLEE},
        {CallerSaved, CALLER}
};

class RegManager {
private:


    class varInfo {
    public:

        varInfo() = default;

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

        int getTs() const {
            return lstTs;
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


        vector<AsmCode> release(RegManager *ctx) {
            switch (pos) {
                case ORIGIN:
                    return {};
                    break;
                case CALLER:

                    ctx->caller_vars[value.value] = "";

                    if (!declareInStk()) {
                        return {};
                    } else {
                        assert(stkPos.second.value == ctx->pageVarNum);
                        ctx->pageVarNum--;
                        return {{ASM_POP}};
                    }
                    break;
                case CALLEE:
                    assert(stkPos.second.value == ctx->pageVarNum);
                    ctx->pageVarNum--;
                    ctx->callee_vars[value.value] = "";
                    return {{ASM_POPR, value}};
                    break;
                case STK:
                    assert(stkPos.second.value == ctx->pageVarNum);
                    ctx->pageVarNum--;
                    return {{ASM_POP}};
                    break;
                default:
                    assert(false);
            }
            return {};
        }

        vector<AsmCode> persist(RegManager *ctx) {
            assert(needWriteBack);
            vector<AsmCode> ans;
            switch (pos) {
                case CALLER:
                    if (!declareInStk()) {
                        stkPos = {{FP},
                                  {++ctx->pageVarNum * INSTRUCTION_WIDTH}};
                        assert(needWriteBack);
                        ans.push_back({ASM_PUSH, value});

                    } else {
                        ans.push_back({ASM_STORE, value});
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

        vector<AsmCode> toReg(AsmOpValue &reg, RegManager *ctx) {
            assert(reg.type != value.type);
            vector<AsmCode> ans;
            assert(pos == STK || pos == ORIGIN);

            switch (reg.type) {
                case CalleeSaved:
                    if (!declareInStk()) {

                        assert(pos == ORIGIN);
                        stkPos = {{FP},
                                  {++ctx->pageVarNum * INSTRUCTION_WIDTH}};
                        ans.push_back({ASM_PUSH, value});
                        ans.push_back({ASM_MOV, reg, 0});
                    } else {
                        assert(false);
                    }
                    break;

                case CallerSaved:
                    if (!declareInStk()) {
                        assert(pos == ORIGIN);
                        ans.push_back({ASM_MOV, reg, 0});
                    } else {
                        assert(pos != ORIGIN);
                        ans.push_back({ASM_LOAD, reg, stkPos.first, stkPos.second});
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
    RegManager() : callee_vars(callee_reg_num), caller_vars(caller_reg_num + arg_reg_num) {
    }

    vector<AsmCode> formal(const string name, int num) {
        assert(name != "" && exist(name) == false);
        if (num >= arg_reg_num) {
            CheckStatus(PARAMS_NUM_OVERFLOW);
        }
        assert(caller_vars[num] == "");
        caller_vars[num] = name;
        stk.push_back(name);
        vars[name] = varInfo{CALLER, {CallerSaved, num},
                             true, clock};
        return {};
    }

    vector<AsmCode> saveCaller() {
        vector<AsmCode> codes;
        for (auto &s: caller_vars) {
            if (s != "") {
                append(codes, vars[s].persist(this));
                s = "";
            }
        }
        return codes;
    }

    vector<AsmCode> ret(SYM *sym) {
        vector<AsmCode> codes;
        for (auto &s: callee_vars) {
            if (s != "") {
                append(codes, vars[s].persist(this));
                s = "";
            }
        }
        AsmOpValue op;
        if (sym->IsConst()) {
            op = {sym->GetValue()};
        } else {
            auto r = getReg(sym->ToStr(), false);
            append(codes, r.first);
            op = r.second;
        }
        append(codes, {ASM_MOV, {CallerSaved, 0}, op});
        return codes;
    }

    vector<AsmCode> loadArgs(const vector<SYM *> args) {
        vector<AsmCode> codes;
        int cnt = 0;
        for (auto &x: args) {
            if (x->IsConst()) {
                AsmCode c{ASM_MOV, {CallerSaved, cnt++}, x->GetValue()};
                append(codes, c);
            }
            auto name = x->ToStr();
            if (vars[name].persistInStk()) {
                append(codes, {ASM_LOAD, {CallerSaved, cnt++}, {FP}, {vars[name].getStkOffset()}});
            } else {
                auto r = vars[name].getReg(clock, false);

                append(codes, {ASM_MOV, {CallerSaved, cnt++}, r});
            }
        }
        return codes;
    }
/*
    vector<AsmCode> saveRet(SYM *sym) {
        assert(!sym->IsConst());
        auto name = sym->ToStr();
        auto &info = vars[name];
        if (info.persistInStk()) {
            return {{STORE, {CallerSaved, 0}, {FP}, {info.getStkOffset()}}};
        } else {
            if (info.isOrigin()) {
                auto ans = emptyReg(name);
                append(ans.first, info.toReg(ans.second, this));
                ans.first.push_back({MOV, info.getReg(clock, true)});
                return ans.first;
            } else {
                return {{MOV, info.getReg(clock, true)}};
            }
        }
    }*/
/*
    vector<AsmCode> ret(SYM *sym) {

    }*/

    vector<AsmCode> declare(const string &name) {
        assert(name != "" && exist(name) == false);
        vars[name] = varInfo{ORIGIN, {Zero},
                             false, clock};
        stk.push_back(name);
        return {};
    }

    pair<vector<AsmCode>, AsmOpValue> GetOpValue(SYM *sym) {
        if (sym == nullptr)return {};
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
        for (int i = 1; i < caller_vars.size(); i++) {
            if (caller_vars[i].empty()) {
                caller_vars[i] = name;
                return {{},
                        {CallerSaved, i}};
            }
        }

        vector<int> id(caller_vars.size() - 1);
        for (int i = 1; i < id.size(); i++) {
            assert(vars[caller_vars[i]].persistInStk() == false);
            id[i] = i;
        }
        std::sort(id.begin(), id.end(), [&](int x, int y) {
            return vars[caller_vars[x]].getTs() < vars[caller_vars[x]].getTs();
        });
        auto &tp = vars[caller_vars[id[0]]];
        caller_vars[id[0]] = name;
        return {tp.persist(this), {CallerSaved, id[0]}};
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

RegManager regManager;

void object_init() {
    regManager = RegManager();
}


vector<AsmCode> begin_func(const TAC *tac) {
    vector<AsmCode> ans = {{ASM_PUSH, {RA}},
                           {ASM_PUSH, {FP}},
                           {ASM_MOV,  {FP, SP}}};


    regManager.beginBlock();
    int cnt = 0;
    for (const TAC *now = tac->next; now->op == TAC_FORMAL; now++, cnt++) {
        append(ans, regManager.formal(now->a->ToStr(), cnt));
    }
    return ans;
}

vector<AsmCode> end_func(const TAC *tac) {
    auto ans = regManager.endBlock();
    assert(regManager.pageVarNum == 0);
    return ans;
}

vector<AsmCode> declare_op(const TAC *tac) {

    return regManager.declare(tac->b->ToStr());
}

vector<AsmCode> func_call(const TAC *tac) {
    auto ans = regManager.saveCaller();

    vector<SYM *> actuals;
    for (auto x = tac->prev; x->op == TAC_ACTUAL; x = x->prev) {
        actuals.push_back(x->a);
    }
    std::reverse(actuals.begin(), actuals.end());
    assert(actuals.size());
    append(ans, regManager.loadArgs(actuals));
    ans.push_back({ASM_JR, {MEM_FUNC_PREFIX + "_" + tac->b->ToStr() + tac->c->ToStr()}});
    auto r = regManager.GetOpValue(tac->a);

    append(ans, r.first);
    append(ans, {ASM_MOV, r.second, {CallerSaved, 0}});
    return ans;
}

vector<AsmCode> func_ret(const TAC *tac) {

    return regManager.ret(tac->a);
}

vector<AsmCode> func_locate(const TAC *tac) {
    auto ans = regManager.saveCaller();
    append(ans, regManager.loadArgs({tac->b, tac->a}));
    append(ans, {ASM_JR, LOCATE_FUNC});

    auto r = regManager.GetOpValue(tac->a);

    append(ans, r.first);
    append(ans, {ASM_MOV, r.second, {CallerSaved, 0}});
    return ans;
}

vector<AsmCode> func_bind(const TAC *tac) {
    auto ans = regManager.saveCaller();
    append(ans, regManager.loadArgs({tac->a, tac->b, tac->c}));
    append(ans, {ASM_JR, BIND_FUNC});

    auto r = regManager.GetOpValue(tac->a);

    append(ans, r.first);
    append(ans, {ASM_MOV, r.second, {CallerSaved, 0}});
    return ans;
}

vector<AsmCode> func_new(const TAC *tac) {
    auto ans = declare_op(tac);
    append(ans, regManager.saveCaller());
    append(ans, regManager.loadArgs({tac->a, tac->b}));
    append(ans, {ASM_JR, NEW_FUNC});

    auto r = regManager.GetOpValue(tac->a);

    append(ans, r.first);
    append(ans, {ASM_MOV, r.second, {CallerSaved, 0}});
    return ans;
}

vector<AsmCode> object_generate(const TAC *tac) {
    regManager.addClock();
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
        case TAC_NEG:
        case TAC_COPY:
        case TAC_GE : {

            vector<AsmCode> codes;
            auto a = regManager.GetAssignValue(tac->a);
            auto b = regManager.GetOpValue(tac->b);
            auto c = regManager.GetOpValue(tac->c);
            append(codes, a.first);
            append(codes, b.first);
            append(codes, c.first);
            append(codes, generate_op(tac->op, a.second, b.second, c.second));
            return codes;
        }
            break;

        case TAC_GOTO:
            return {{ASM_JUMP, {tac->a->ToStr()}}};
            break;
        case TAC_IFZ: {

            auto r = regManager.GetOpValue(tac->a);
            r.first.push_back({ASM_BNZ, r.second, {tac->b->ToStr()}});
            return r.first;
            break;
        }
        case TAC_ENDFUNC:
            return end_func(tac);
            break;
        case TAC_LABEL:
            return {{ASM_LABEL, {tac->a->ToStr()}}};
            break;
        case TAC_CALL:
            return func_call(tac);
            break;
        case TAC_RETURN:
            return func_ret(tac);
            break;
        case TAC_LOCATE:
            return func_locate(tac);
            break;
        case TAC_BEGINBLOCK:
            return regManager.endBlock();
            break;
        case TAC_ENDBLOCK:
            regManager.beginBlock();
            return {};
            break;
        case TAC_NEW:
            return func_new(tac);
            break;
        case TAC_BIND:
            return func_bind(tac);
            break;
        default:
            assert(false);
            break;
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