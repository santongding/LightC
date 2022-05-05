//
// Created by os on 5/5/22.
//
#include "VarRegInfo.cpp"

extern void append(vector<AsmCode> codes);

extern void append(AsmCode codes);

const int arg_reg_num = Args_Reg_Num;
const int callee_reg_num = Get_Callee_REG_NAME().size();
const int caller_reg_num = Get_Caller_REG_NAME().size();


class RegManager {
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
            if (s != "") {
                assert(exist(s));
                vars[s].releaseCallee();
                //s = "";
            }
        }

    }

    void loadArgs(const vector<SYM *> args,int reserveArgsNum=0) {
        int cnt = reserveArgsNum;
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
                             true, clock, ++pageVarNum};
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