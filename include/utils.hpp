//
// Created by os on 5/3/22.
//
#ifndef LIGHTC_UTILS_HPP
#define LIGHTC_UTILS_HPP

#include "string"
using std::string;

#define TABLE() \
DEF_STATUS(UNEXPECTED_ERROR,-1)         \
DEF_STATUS(OK,0) \
DEF_STATUS(SYMBOL_REPEAT,1) \
DEF_STATUS(SYMBOL_UNDEFINED,2) \
DEF_STATUS(TYPE_CONFLICT,3) \
DEF_STATUS(TYPE_UNKNOWN ,4) \
DEF_STATUS(TYPE_NEED_INT, 5) \
DEF_STATUS(TYPE_NEED_CLASS,6)  \
DEF_STATUS(CLASS_MEMBER_NOT_EXIST, 7) \
DEF_STATUS(FUNC_PARAM_NOT_MATCH , 8)  \
DEF_STATUS(BAD_SYNTAX , 9) \
DEF_STATUS(ASSIGN_TO_CONST ,10)         \
DEF_STATUS(FUN_NOT_RETURN,11)           \
DEF_STATUS(PARAMS_NUM_OVERFLOW,12)      \
DEF_STATUS(MAIN_CLASS_NOT_DEFINE,13)
enum STATUS {
#define DEF_STATUS(v, x) v=x,
    TABLE()
#undef DEF_STATUS
};

inline const char *Status2Str(STATUS c) {
    switch (c) {
#define DEF_STATUS(v, x) case STATUS::v: return #v;
        TABLE()
#undef DEF_STATUS
        default:
            return "??";
    }
}
#undef TABLE

void error(const char *str, int code);

void error(const string &str, int code);

void setlineno(int l);

void CheckStatus(STATUS s);

int getlineno();

#endif //LIGHTC_UTILS_HPP
