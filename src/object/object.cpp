//
// Created by os on 5/3/22.
//
#include <algorithm>
#include "object.h"
#include "tac.hpp"
#include "TypeManager.h"

vector<AsmCode> _asm_codes;

inline void append(vector<AsmCode> codes) {
    for (auto &x: codes) {
        _asm_codes.push_back(x);
    }
}

inline void append(AsmCode code) {
    _asm_codes.push_back(code);
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
            return stkPos.second.value;
        }

        bool inStk() const {
            if (pos == STK) {
                assert(value.type == None);
                return true;
            } else {
                return false;
            }
        }

        bool isOrigin() const {
            assert((!pos == ORIGIN) || needWriteBack);
            return pos == ORIGIN;
        }

        bool needWrite() const {
            return needWriteBack;
        }

        int getTs() const {
            return lstTs;
        }

        AsmOpValue getReg(int clk, bool isAssign) {
            assert(inStk() == false);
            assert(isValid());
            needWriteBack |= isAssign;
            lstTs = clk;
            return value;
        }


        void releaseCallee() {
            assert(pos == CALLEE);
            assert(value.type == CalleeSaved);
            append({{ASM_LOAD, value, stkPos.first, stkPos.second}});
        }

        void persist(RegManager *ctx) {
            assert(isValid());
            assert(!isOrigin());
            if (!needWriteBack) {
                value = {};
                pos = STK;
                return;
            }
            switch (pos) {
                case CALLER: {
                    append({ASM_STORE, value, stkPos.first, stkPos.second});
                }
                    break;
                case CALLEE: {

                    append({ASM_SWAPRM, value, stkPos.first, stkPos.second});
                }
                    break;
                default:
                    assert(false);
                    break;
            }
            value = {};
            pos = STK;

            assert(isValid());
        }

        void toReg(AsmOpValue &reg, RegManager *ctx) {
            assert(isValid());
            assert(reg.type != value.type);
            assert(pos == STK || pos == ORIGIN);

            switch (reg.type) {
                case CalleeSaved:
                    if (pos == STK) {
                        append({ASM_SWAPRM, reg, stkPos.first, stkPos.second});
                    } else {
                        append({{ASM_STORE, reg, stkPos.first, stkPos.second},
                                {ASM_MOV,   reg, 0}});
                    }
                    break;

                case CallerSaved:
                    if (pos == ORIGIN) {
                        append({ASM_MOV, reg, 0});
                    } else {
                        assert(pos != ORIGIN);
                        append({ASM_LOAD, reg, stkPos.first, stkPos.second});
                    }
                    break;
                default:
                    assert(false);
                    break;
            }
            pos = type2Pos[reg.type];
            value = reg;
            assert(isValid());
        }

    private:

        bool isValid() {
            if (stkPos.first.type != FP) {
                return false;
            }
            switch (pos) {

                case ORIGIN:
                    return value.type == Zero;
                    break;
                case CALLEE:
                    return value.type == CalleeSaved;
                    break;
                case CALLER:
                    return value.type == CallerSaved;
                    break;
                case STK:
                    return value.type == None;
                    break;
            }
        }

        VarPos pos;
        AsmOpValue value;
        bool needWriteBack;
        int lstTs;
        pair<AsmOpValue, AsmOpValue> stkPos;
    };

public:
    RegManager() : pageVarNum(0), callee_vars(callee_reg_num), caller_vars(caller_reg_num + arg_reg_num) {

    }

    void formal(const string name, int num) {
        assert(name != "" && exist(name) == false);
        if (num >= arg_reg_num) {
            CheckStatus(PARAMS_NUM_OVERFLOW);
        }
        assert(caller_vars[num] == "");
        caller_vars[num] = name;
        vars[name] = varInfo{CALLER, {CallerSaved, num},
                             true, clock, ++pageVarNum};
        return;
    }

    void saveCaller() {
        for (auto &s: caller_vars) {
            if (s != "") {
                vars[s].persist(this);
                s = "";
            }
        }
    }

    void ret(SYM *sym) {
        AsmOpValue op;
        if (sym->IsConst()) {
            op = {sym->GetValue()};
        } else {

            assert(exist(sym->ToStr()));
            op = getReg(sym->ToStr(), false);
        }


        append({ASM_MOV, {CallerSaved, 0}, op});
        for (auto &s: callee_vars) {
            assert(exist(s));
            if (s != "") {
                vars[s].releaseCallee();
                //s = "";
            }
        }

    }

    void loadArgs(const vector<SYM *> args) {
        int cnt = 0;
        for (auto &x: args) {
            if (x->IsConst()) {
                append({ASM_MOV, {CallerSaved, cnt++}, x->GetValue()});
            } else {

                auto name = x->ToStr();
                assert(exist(name));
                if (vars[name].inStk()) {
                    append({ASM_LOAD, {CallerSaved, cnt++}, {FP}, {vars[name].getStkOffset()}});
                } else {
                    auto r = vars[name].getReg(clock, false);

                    append({ASM_MOV, {CallerSaved, cnt++}, r});
                }
            }
        }
    }

    void declare(const string &name) {
        assert(name != "" && exist(name) == false);
        vars[name] = varInfo{ORIGIN, {Zero},
                             false, clock, ++pageVarNum};
    }

    AsmOpValue GetOpValue(SYM *sym) {
        if (sym == nullptr)return {};
        if (sym->IsConst()) {
            return {sym->GetValue()};
        } else {
            assert(exist(sym->ToStr()));
            return getReg(sym->ToStr(), false);
        }
    }

    AsmOpValue GetAssignValue(SYM *sym) {
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


    AsmOpValue getReg(const string &name, bool isAssign) {
        assert(exist(name));
        auto &info = vars[name];
        if ((info.isOrigin() && isAssign) || info.inStk()) {
            auto ans = emptyReg(name);
            info.toReg(ans, this);
            return info.getReg(clock, isAssign);
        } else {
            return info.getReg(clock, isAssign);
        }
    }

    AsmOpValue emptyReg(const string &name) {
        bool isTemp = 0 == name.compare(0, TEMP_VALUE_PREFIX.length(), TEMP_VALUE_PREFIX);

        if (!isTemp) {
            for (int i = 0; i < callee_vars.size(); i++) {
                if (callee_vars[i].empty()) {
                    callee_vars[i] = name;
                    return
                            {CalleeSaved, i};
                }

            }
        }
        for (int i = 1; i < caller_vars.size(); i++) {
            if (caller_vars[i].empty()) {
                caller_vars[i] = name;
                return
                        {CallerSaved, i};
            }
        }

        vector<int> id(caller_vars.size() - 1);
        for (int i = 1; i < id.size(); i++) {
            assert(vars[caller_vars[i]].inStk() == false && vars[caller_vars[i]].isOrigin() == false);
            id[i] = i;
        }
        std::sort(id.begin(), id.end(), [&](int x, int y) {
            auto &vx = vars[caller_vars[x]];
            auto &vy = vars[caller_vars[y]];
            if (vx.needWrite() != vy.needWrite()) {
                return vx.needWrite() < vy.needWrite();
            }
            return vx.getTs() < vy.getTs();
        });
        auto &tp = vars[caller_vars[id[0]]];
        caller_vars[id[0]] = name;
        tp.persist(this);
        return {CallerSaved, id[0]};
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
    append({ASM_JR, {MEM_FUNC_PREFIX + "_" + tac->b->ToStr() + tac->c->ToStr()}});
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
    regManager.loadArgs({tac->b, tac->a});
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