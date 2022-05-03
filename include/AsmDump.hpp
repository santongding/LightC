//
// Created by os on 5/3/22.
//

#ifndef LIGHTC_ASMDUMP_HPP
#define LIGHTC_ASMDUMP_HPP
#include "AsmDef.hpp"
#include "object.h"
#include "string"
using std::string;


enum AsmValueType{
    NONE = 0,
    STRING,
    REG,
    IMM

};
struct AsmOpTypePair {
    AsmValueType t0, t1, t2;
    std::string asmcode;
};


enum AsmOpValueType {
    None = 0,
    Imm,
    FP,
    Args,
    CallerSaved,
    CalleeSaved,
    Label,
};

class AsmOpValue {
public:
    AsmOpValue() : type(None), value(0) {}

    AsmOpValue(AsmOpValueType t) : type(t), value(0) {
        assert(t == FP);
        assert(t!=Label);
    }

    AsmOpValue(AsmOpValueType t, int v) : type(t), value(v) {
        assert(!(t == FP));
        assert(t!=Label);
    }
    AsmOpValue(AsmOpValueType t, const std::string &s) : type(t), value(0),label(s) {
        assert(t==Label);
    }
    string Dump();
    AsmOpValueType type;
    int value;
    std::string label;
};

class AsmCode {
public:
    AsmCode(AsmType type) : type(type) {
    }

    AsmCode(AsmType type, AsmOpValue _v0 = AsmOpValue(), AsmOpValue _v1 = AsmOpValue(), AsmOpValue _v2 = AsmOpValue())
            : type(type), v0(_v0), v1(_v1), v2(_v2) {
    }
    string Dump();
private:
    AsmType type;
    AsmOpValue v0, v1, v2;

};


#endif //LIGHTC_ASMDUMP_HPP
