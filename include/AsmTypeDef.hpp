//
// Created by os on 5/3/22.
//

#ifndef LIGHTC_ASMTYPEDEF_HPP
#define LIGHTC_ASMTYPEDEF_HPP
#include "string"
enum AsmValueType {
    NONE = 0,
    STRING,
    REG,
    IMM,
    REG_OR_IMM

};

struct AsmInfo {
    AsmValueType t[3];
    std::string asmcode;
};

#endif //LIGHTC_ASMTYPEDEF_HPP
