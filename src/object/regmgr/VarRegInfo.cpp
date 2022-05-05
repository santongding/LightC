//
// Created by os on 5/5/22.
//

enum VarPos {
    ORIGIN,
    CALLEE,
    CALLER,
    STK,
};

class RegManager;

extern void append(vector<AsmCode> codes);

extern void append(AsmCode codes);


std::map<AsmOpValueType, VarPos> type2Pos = {
        {CalleeSaved, CALLEE},
        {CallerSaved, CALLER}
};

class varInfo {
public:

    varInfo() = default;

    varInfo(VarPos p, AsmOpValue v, bool b, int i, int stk) : pos(p), value(v), needWriteBack(b), lstTs(i),
                                                              stkPos({stk * INSTRUCTION_WIDTH}) {

    }

    int getStkOffset() const {
        return stkPos.value;
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
        assert(pos != ORIGIN || needWriteBack);
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
        append({{ASM_LOAD, value, {FP}, stkPos}});
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
                append({ASM_STORE, value, {FP}, stkPos});
            }
                break;
            case CALLEE: {

                append({ASM_SWAPRM, value, {FP}, stkPos});
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
                    append({ASM_SWAPRM, reg, {FP}, stkPos});
                } else {
                    append({{ASM_STORE, reg, {FP}, stkPos},
                            {ASM_MOV,   reg, 0}});
                }
                break;

            case CallerSaved:
                if (pos == ORIGIN) {
                    append({ASM_MOV, reg, 0});
                } else {
                    assert(pos != ORIGIN);
                    append({ASM_LOAD, reg, {FP}, stkPos});
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
        if (stkPos.type != Imm) {
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
    AsmOpValue stkPos;
};