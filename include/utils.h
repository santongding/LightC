//
// Created by os on 5/3/22.
//

#include "string"
using std::string;

#ifndef LIGHTC_UTILS_H
#define LIGHTC_UTILS_H
#define STATUS_TABLE() \
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
DEF_STATUS(FUN_NOT_RETURN,11)

enum STATUS {
#define DEF_STATUS(v, x) v=x,
    STATUS_TABLE()
#undef DEF_STATUS
};

extern const char *Status2Str(STATUS c) ;

void error(const char *str, int code);

void error(const string &str, int code);

void setlineno(int l);

void CheckStatus(STATUS s);

int getlineno();

#endif //LIGHTC_UTILS_H