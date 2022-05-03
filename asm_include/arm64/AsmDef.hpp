//
// Created by os on 5/3/22.
//

#ifndef LIGHTC_ASMDEF_HPP
#define LIGHTC_ASMDEF_HPP

#include "AsmTypeDef.hpp"
#include "map"
#include "vector"
#include "string"

using std::string;
using std::vector;
#define INSTRUCTION_WIDTH 8

#define TABLE() \
DEF(JR, STRING,NONE,NONE,"BL %0") \
DEF(LOAD,REG,REG,IMM,"LDRQ %0,[%1,%2]")     \
DEF(STORE,REG,REG,IMM,"STRQ %0,[%1,%2]")    \
DEF(PUSH,REG,NONE,NONE,"str {%0} [sp,-8]")        \
DEF(PUSHI,IMM,NONE,NONE,"str {%0} [sp,-8]")   \
DEF(POP,NONE,NONE,NONE,"add sp,sp,8")    \
DEF(BNZ,REG,STRING,NONE,"CMP %0 0x0\nBNE %1")  \
DEF(ADD,REG,REG,REG,"ADD %0 %1 %2")      \
DEF(ADDI,REG,REG,IMM,"ADDI %0 %1 %2")\
DEF(RET,NONE,NONE,NONE,"RET")     \
DEF(MOV,REG,REG,NONE,"MOV %0 %1")

inline string Get_FP_REG_NAME() {
    return "x29";
}




inline vector<string> Get_ARG_REG_NAME() {
    return {
            "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7"
    };
}

inline vector<string> Get_Callee_REG_NAME() {
    return {
            "x19", "x20", "x21", "x22", "x23",
            "x24", "x25", "x26", "x27", "x28"
    };
}

inline vector<string> Get_Caller_REG_NAME() {
    return {
            "x9", "x10", "x11", "x12", "x13",
            "x14", "x15"
    };
}


/********below auto generate code***********/

enum AsmType {
#define DEF(v, x, y, z, s) v,
    TABLE()
#undef DEF
};

inline std::map<AsmType, AsmInfo> GetAsmTypeMap() {
#define DEF(v, x, y, z, s) {v,{x,y,z,s}},
    return {
            TABLE()
    };
#undef DEF
}


inline const char *Status2Str(AsmType c) {
    switch (c) {
#define DEF(v, x, y, z, s) case AsmType::v: return #v;
        TABLE();
#undef DEF
        default:
            return "??";
    }
}

#undef TABLE
#endif //LIGHTC_ASMDEF_HPP
