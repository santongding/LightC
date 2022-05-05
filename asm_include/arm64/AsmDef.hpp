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
 DEF(ASM_JR, STRING,NONE,NONE,"BL [0]") \
 DEF(ASM_JUMP, STRING,NONE,NONE,"B [0]")\
 DEF(ASM_LABEL, STRING,NONE,NONE,"[0]:") \
 DEF(ASM_LOAD,REG,REG,IMM,"LDR [0],[[1],[2]]")     \
 DEF(ASM_STORE,REG,REG,IMM,"STR [0],[[1],[2]]")    \
 DEF(ASM_SAVE,IMM,NONE,NONE,"STP x29,x30,[sp,#[0]]!\nMOV x29, sp")     \
 DEF(ASM_RESTORE,IMM,NONE,NONE,"LDP x29,x30,[sp],#[0]")       \
 DEF(ASM_BNZ,REG,STRING,NONE,"CMP xzr, [0]\nBNE [1]")         \
 DEF(ASM_RET,NONE,NONE,NONE,"RET")     \
 DEF(ASM_MOV,REG,REG_OR_IMM,NONE,"MOV [0], [1]")\
 DEF(ASM_ADD,REG,REG,REG_OR_IMM,"ADD [0], [1], [2]")  \
 DEF(ASM_SUB,REG,REG,REG_OR_IMM,"SUB [0] ,[1], [2]")  \
 DEF(ASM_MUL,REG,REG,REG_OR_IMM,"MUL [0], [1], [2]")\
 DEF(ASM_DIV,REG,REG,REG_OR_IMM,"DIV [0], [1], [2]")\
 DEF(ASM_EQ,REG,REG,REG_OR_IMM,"EQ [0], [1], [2]")    \
 DEF(ASM_NE,REG,REG,REG_OR_IMM,"NE [0], [1], [2]")    \
 DEF(ASM_LT,REG,REG,REG_OR_IMM,"LT [0], [1], [2]")    \
 DEF(ASM_LE,REG,REG,REG_OR_IMM,"LE [0], [1], [2]")    \
 DEF(ASM_GT,REG,REG,REG_OR_IMM,"GT [0], [1], [2]")\
 DEF(ASM_GE,REG,REG,REG_OR_IMM,"GE [0], [1], [2]")    \
 DEF(ASM_NEG,REG,REG_OR_IMM,NONE,"NEG [0], [1]")
//DEF(ASM_SWAPRM,REG,REG,IMM,"SWAP [0],[0],[[1],[2]]")


inline string Get_SP_REG_NAME() {
    return "sp";
}

inline string Get_RA_REG_NAME() {
    return "x30";
}

inline string Get_ZERO_REG_NAME() {
    return "xzr";
}


const int Args_Reg_Num = 8;

inline vector<string> Get_Callee_REG_NAME() {
    return {
            "x19", "x20", "x21", "x22", "x23",
            "x24", "x25", "x26", "x27", "x28"
    };
}

inline vector<string> Get_Caller_REG_NAME() {
    return {
            "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7",
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
