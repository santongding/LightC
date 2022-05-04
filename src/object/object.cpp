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

        varInfo(VarPos p, AsmOpValue v, bool b, int i, int stk) : pos(p), value(v), needWriteBack(b), lstTs(i),
                                                                  stkPos({FP}, {stk * INSTRUCTION_WIDTH}) {

        }

        int getStkOffset() const {
            assert(declareInStk());
            return stkPos.second.value;
        }

        bool declareInStk() const {
            assert(stkPos.first.type == FP);
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
            needWriteBack |= isAssign;
            lstTs = clk;
            if (pos == ORIGIN) {
                assert(isAssign == false);
                assert(value.type == Zero);
            }
            return value;
        }


        vector<AsmCode> release(RegManager *ctx) {
            switch (pos) {
                case ORIGIN:
                    return {};
                    break;
                case CALLER: {

                    ctx->caller_vars[value.value] = "";

                    if (!declareInStk()) {
                        return {};
                    } else {
                        assert(stkPos.second.value == ctx->pageVarNum);
                        ctx->pageVarNum--;
                        return {{ASM_POP}};
                    }
                }
                    break;
                case CALLEE: {

                    assert(stkPos.second.value == ctx->pageVarNum);
                    ctx->pageVarNum--;
                    ctx->callee_vars[value.value] = "";
                    return {{ASM_POPR, value}};
                }
                    break;
                case STK: {

                    assert(stkPos.second.value == ctx->pageVarNum);
                    ctx->pageVarNum--;
                    return {{ASM_POP}};
                }
                    break;
                default:
                    assert(false);
            }
            return {};
        }

        vector<AsmCode> releaseCallee() {
            assert(pos == CALLEE);
            assert(declareInStk());
            assert(value.type == CalleeSaved);
            return {{ASM_LOAD, value, stkPos.first, stkPos.second}};
        }

        vector<AsmCode> persist(RegManager *ctx) {
            if (!needWriteBack) {
                if (isOrigin()) {
                    return {};
                }
                value = {};
                pos = STK;
                assert(declareInStk());
                return {};
            }
            vector<AsmCode> ans;
            switch (pos) {
                case CALLER:
                    if (!declareInStk()) {
                        stkPos = {{FP},
                                  {-(++ctx->pageVarNum * INSTRUCTION_WIDTH)}};
                        assert(needWriteBack);
                        ans.push_back({ASM_PUSH, value});

                    } else {
                        ans.push_back({ASM_STORE, value, stkPos.first, stkPos.second});
                        return ans;
                    }
                    break;
                default:
                    assert(false);
                    break;
            }
            value = {};
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
    RegManager() : pageVarNum(0), callee_vars(callee_reg_num), caller_vars(caller_reg_num + arg_reg_num) {

    }

    vector<AsmCode> formal(const string name, int num) {
        assert(name != "" && exist(name) == false);
        if (num >= arg_reg_num) {
            CheckStatus(PARAMS_NUM_OVERFLOW);
        }
        assert(caller_vars[num] == "");
        caller_vars[num] = name;
        vars[name] = varInfo{CALLER, {CallerSaved, num},
                             true, clock, ++pageVarNum};
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
        AsmOpValue op;
        if (sym->IsConst()) {
            op = {sym->GetValue()};
        } else {

            assert(exist(sym->ToStr()));
            auto r = getReg(sym->ToStr(), false);
            append(codes, r.first);
            op = r.second;
        }


        append(codes, {ASM_MOV, {CallerSaved, 0}, op});
        for (auto &s: callee_vars) {
            if (s != "") {
                append(codes, vars[s].releaseCallee());
                //s = "";
            }
        }

        return codes;
    }

    vector<AsmCode> loadArgs(const vector<SYM *> args) {
        vector<AsmCode> codes;
        int cnt = 0;
        for (auto &x: args) {
            if (x->IsConst()) {
                AsmCode c{ASM_MOV, {CallerSaved, cnt++}, x->GetValue()};
                append(codes, c);
            } else {

                auto name = x->ToStr();
                assert(exist(name));
                if (vars[name].persistInStk()) {
                    append(codes, {ASM_LOAD, {CallerSaved, cnt++}, {FP}, {vars[name].getStkOffset()}});
                } else {
                    auto r = vars[name].getReg(clock, false);

                    append(codes, {ASM_MOV, {CallerSaved, cnt++}, r});
                }
            }
        }
        return codes;
    }

    vector<AsmCode> declare(const string &name) {
        assert(name != "" && exist(name) == false);
        vars[name] = varInfo{ORIGIN, {Zero},
                             false, clock, ++pageVarNum};
        return {};
    }

    pair<vector<AsmCode>, AsmOpValue> GetOpValue(SYM *sym) {
        if (sym == nullptr)return {};
        if (sym->IsConst()) {
            return {{},
                    {sym->GetValue()}};
        } else {
            assert(exist(sym->ToStr()));
            return getReg(sym->ToStr(), false);
        }
    }

    pair<vector<AsmCode>, AsmOpValue> GetAssignValue(SYM *sym) {
        assert(!sym->IsConst());

        auto name = sym->ToStr();
        assert(exist(name));
        return getReg(name, true);

    }


    void addClock() {
        clock++;
    }

    int pageVarNum = 0;
private:


    pair<vector<AsmCode>, AsmOpValue> getReg(const string &name, bool isAssign) {
        assert(exist(name));
        auto &info = vars[name];
        if ((info.isOrigin() && isAssign) || info.persistInStk()) {
            auto ans = emptyReg(name, info.persistInStk());
            append(ans.first, info.toReg(ans.second, this));
            ans.second = info.getReg(clock, isAssign);
            return ans;
        } else {
            return {{}, info.getReg(clock, isAssign)};
        }
    }

    pair<vector<AsmCode>, AsmOpValue> emptyReg(const string &name, bool isFromStk) {
        bool isTemp = 0 == name.compare(0, TEMP_VALUE_PREFIX.length(), TEMP_VALUE_PREFIX);

        if (!isTemp && !isFromStk) {
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


    std::map<string, varInfo> vars;

};

RegManager regManager;


int pageVarSum = 0;

vector<AsmCode> begin_func(const TAC *tac) {
    regManager = RegManager();
    pageVarSum = 0;
    for (auto t = tac; t->op != TAC_ENDFUNC; t = t->next) {
        if (t->op == TAC_FORMAL || t->op == TAC_DECLARE || t->op == TAC_NEW) {
            pageVarSum++;
        }
    }
    vector<AsmCode> ans = {{ASM_PUSH, {RA}},
                           {ASM_PUSH, {FP}},
                           {ASM_MOV,  {FP}, {SP}},
                           {ASM_SUB,  {SP}, {SP}, {-pageVarSum * INSTRUCTION_WIDTH}}
    };

    int cnt = 0;
    for (const TAC *now = tac->next; now->op == TAC_FORMAL; now = now->next, cnt++) {
        append(ans, regManager.formal(now->b->ToStr(), cnt));
    }
    return ans;
}

vector<AsmCode> end_func(const TAC *tac) {
    return {};
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
    auto codes = regManager.ret(tac->a);
    append(codes, {ASM_ADD, {SP}, {SP}, {pageVarSum * INSTRUCTION_WIDTH}});
    pageVarSum = 0;
    append(codes, {ASM_POPR, {FP}});
    append(codes, {ASM_POPR, {RA}});

    codes.push_back({ASM_RET});
    return codes;
}

vector<AsmCode> func_locate(const TAC *tac) {
    auto ans = regManager.saveCaller();
    append(ans, regManager.loadArgs({tac->b, tac->a}));
    append(ans, {ASM_JR, LOCATE_FUNC});

    auto r = regManager.GetAssignValue(tac->a);

    append(ans, r.first);
    append(ans, {ASM_MOV, r.second, {CallerSaved, 0}});
    return ans;
}

vector<AsmCode> func_bind(const TAC *tac) {
    auto ans = regManager.saveCaller();
    append(ans, regManager.loadArgs({tac->a, tac->b, tac->c}));
    append(ans, {ASM_JR, BIND_FUNC});

    //auto r = regManager.GetOpValue(tac->a);

    //append(ans, r.first);
    //append(ans, {ASM_MOV, r.second, {CallerSaved, 0}});
    return ans;
}

vector<AsmCode> func_new(const TAC *tac) {
    auto ans = declare_op(tac);
    append(ans, regManager.saveCaller());
    append(ans, regManager.loadArgs({tac->a, tac->b}));
    append(ans, {ASM_JR, NEW_FUNC});

    auto r = regManager.GetAssignValue(tac->b);

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

            auto r = regManager.GetOpValue(tac->b);
            r.first.push_back({ASM_BNZ, r.second, {tac->a->ToStr()}});
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
            return {};
            break;
        case TAC_ENDBLOCK:

            return {};
            break;
        case TAC_NEW:
            return func_new(tac);
            break;
        case TAC_BIND:
            return func_bind(tac);
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
    return {};
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