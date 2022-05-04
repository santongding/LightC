//
// Created by os on 5/3/22.
//

#ifndef LIGHTC_ASMDUMP_HPP
#define LIGHTC_ASMDUMP_HPP

#include "AsmDef.hpp"
#include "string"
#include "cassert"

using std::string;




enum AsmOpValueType {
    None = 0,
    Imm,
    FP,
    SP,
    RA,
    Zero,
    CallerSaved,
    CalleeSaved,
    Label,
};

class AsmOpValue {
public:
    AsmOpValue() : type(None), value(0) {}

    AsmOpValue(AsmOpValueType t) : type(t), value(0) {
        assert(t == FP || t == SP || t == RA || t == Zero);
    }

    AsmOpValue(AsmOpValueType t, int v) : type(t), value(v) {
        assert(t==CalleeSaved||t==CallerSaved);
        assert(t != Label);
    }

    AsmOpValue(const std::string &s) : value(0), label(s) {
        type = Label;
    }

    AsmOpValue(int imm) {
        value = imm;
        type = Imm;
    }

    string Dump(AsmValueType target);

    AsmOpValueType type;
    int value;
    std::string label;
};

class AsmCode {
public:
    AsmCode(AsmType type, AsmOpValue _v0 = AsmOpValue(), AsmOpValue _v1 = AsmOpValue(), AsmOpValue _v2 = AsmOpValue())
            : type(type) {
        v[0] = _v0;
        v[1] = _v1;
        v[2] = _v2;
    }

    string Dump();

private:
    AsmType type;
    AsmOpValue v[3];

};


#endif //LIGHTC_ASMDUMP_HPP
